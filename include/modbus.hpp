#pragma once
#include "motor.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>

class ModbusProcessor {
  MotorController &motor_;
  std::uint8_t unit_;
  static std::uint16_t word(const std::vector<std::uint8_t> &frame,
                            std::size_t offset) {
    return std::uint16_t((frame.at(offset) << 8) | frame.at(offset + 1));
  }
  static void append(std::vector<std::uint8_t> &frame, std::uint16_t value) {
    frame.push_back(value >> 8);
    frame.push_back(value & 255);
  }
  std::vector<std::uint8_t> response(const std::vector<std::uint8_t> &request,
                                     const std::vector<std::uint8_t> &pdu) {
    std::vector<std::uint8_t> result{request[0], request[1], 0, 0};
    append(result, std::uint16_t(pdu.size() + 1));
    result.push_back(unit_);
    result.insert(result.end(), pdu.begin(), pdu.end());
    return result;
  }

public:
  explicit ModbusProcessor(MotorController &motor, std::uint8_t unit = 1)
      : motor_(motor), unit_(unit) {
    if (!unit)
      throw std::invalid_argument("unit zero unsupported");
  }
  std::vector<std::uint8_t> process(const std::vector<std::uint8_t> &request,
                                    const Inputs &inputs, std::uint32_t now) {
    if (request.size() < 8 || request.size() > 260 || word(request, 2) != 0 ||
        word(request, 4) + 6 != request.size() || request[6] != unit_)
      throw std::invalid_argument("invalid MBAP frame");
    std::uint8_t function = request[7];
    auto fail = [&](std::uint8_t code) {
      return response(request, {std::uint8_t(function | 0x80), code});
    };
    if (request.size() != 12)
      return fail(3);
    std::uint16_t address = word(request, 8), value = word(request, 10);
    if (function == 3) {
      if (!value || value > 8)
        return fail(3);
      if (address > 7 || address + value > 8)
        return fail(2);
      motor_.scan(inputs, now);
      std::uint32_t sequence = motor_.sequence();
      std::uint16_t flags = std::uint16_t(inputs.emergencyStop) |
                            (std::uint16_t(inputs.guardClosed) << 1) |
                            (std::uint16_t(inputs.pressureHealthy) << 2) |
                            (std::uint16_t(inputs.overload) << 3);
      std::uint16_t registers[] = {std::uint16_t(motor_.state()),
                                   std::uint16_t(sequence >> 16),
                                   std::uint16_t(sequence),
                                   flags,
                                   0,
                                   0,
                                   0,
                                   0};
      std::vector<std::uint8_t> pdu{3, std::uint8_t(value * 2)};
      for (std::uint16_t i = 0; i < value; i++)
        append(pdu, registers[address + i]);
      return response(request, pdu);
    }
    if (function == 6) {
      if (address != 4)
        return fail(2);
      try {
        if (value == 1)
          motor_.commandStart(inputs, now);
        else if (value == 2)
          motor_.commandStop(now);
        else if (value == 3)
          motor_.reset(inputs, now);
        else
          return fail(3);
      } catch (const std::logic_error &) {
        return fail(4);
      }
      return request;
    }
    return fail(1);
  }
};
