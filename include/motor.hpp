#pragma once

#include <cstdint>

enum class MotorState { Stopped, Starting, Running, Stopping, Tripped };

struct Inputs {
    bool emergencyStop;
    bool guardClosed;
    bool pressureHealthy;
    bool overload;
};

class MotorController {
public:
    MotorState commandStart(const Inputs& inputs, std::uint32_t nowMs);
    MotorState commandStop(std::uint32_t nowMs);
    MotorState reset(const Inputs& inputs, std::uint32_t nowMs);
    MotorState scan(const Inputs& inputs, std::uint32_t nowMs);
    MotorState state() const;
    std::uint32_t sequence() const;
private:
    void transition(MotorState next, std::uint32_t nowMs);
    static bool unsafe(const Inputs& inputs);
    MotorState state_ = MotorState::Stopped;
    std::uint32_t stateSinceMs_ = 0;
    std::uint32_t sequence_ = 0;
};
