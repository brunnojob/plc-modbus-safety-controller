#include "modbus.hpp"
#include <cassert>
int main() {
  MotorController motor;
  ModbusProcessor p(motor);
  Inputs safe{false, true, true, false};
  std::vector<std::uint8_t> read{0, 1, 0, 0, 0, 6, 1, 3, 0, 0, 0, 4};
  auto result = p.process(read, safe, 100);
  assert(result.size() == 17 && result[0] == 0 && result[1] == 1 &&
         result[8] == 8);
  std::vector<std::uint8_t> start{0, 2, 0, 0, 0, 6, 1, 6, 0, 4, 0, 1};
  assert(p.process(start, safe, 100) == start);
  assert(motor.state() == MotorState::Starting);
  auto fail = p.process(start, safe, 200);
  assert(fail[7] == 0x86 && fail[8] == 4);
  auto bad = read;
  bad[5] = 255;
  bool rejected = false;
  try {
    p.process(bad, safe, 200);
  } catch (const std::invalid_argument &) {
    rejected = true;
  }
  assert(rejected);
}
