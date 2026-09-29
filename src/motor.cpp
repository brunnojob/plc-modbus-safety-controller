#include "motor.hpp"

#include <stdexcept>

MotorState MotorController::commandStart(const Inputs& inputs, std::uint32_t nowMs) {
    if (state_ != MotorState::Stopped)
        throw std::logic_error("start_requires_stopped");
    if (unsafe(inputs))
        throw std::logic_error("start_interlock_open");
    transition(MotorState::Starting, nowMs);
    return state_;
}

MotorState MotorController::commandStop(std::uint32_t nowMs) {
    if (state_ == MotorState::Running || state_ == MotorState::Starting)
        transition(MotorState::Stopping, nowMs);
    return state_;
}

MotorState MotorController::reset(const Inputs& inputs, std::uint32_t nowMs) {
    if (state_ != MotorState::Tripped || unsafe(inputs))
        throw std::logic_error("reset_requires_safe_trip");
    transition(MotorState::Stopped, nowMs);
    return state_;
}

MotorState MotorController::scan(const Inputs& inputs, std::uint32_t nowMs) {
    if (unsafe(inputs)) {
        transition(MotorState::Tripped, nowMs);
        return state_;
    }
    const auto elapsed = nowMs - stateSinceMs_;
    if (state_ == MotorState::Starting && elapsed >= 1500)
        transition(MotorState::Running, nowMs);
    else if (state_ == MotorState::Stopping && elapsed >= 800)
        transition(MotorState::Stopped, nowMs);
    return state_;
}

MotorState MotorController::state() const {
    return state_;
}

std::uint32_t MotorController::sequence() const {
    return sequence_;
}

void MotorController::transition(MotorState next, std::uint32_t nowMs) {
    if (state_ == next)
        return;
    state_ = next;
    stateSinceMs_ = nowMs;
    ++sequence_;
}

bool MotorController::unsafe(const Inputs& inputs) {
    return inputs.emergencyStop || !inputs.guardClosed || !inputs.pressureHealthy || inputs.overload;
}
