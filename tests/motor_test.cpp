#include "motor.hpp"

#include <cassert>
#include <stdexcept>

int main() {
  MotorController motor;
  Inputs unsafe{false, false, true, false};
  bool blocked = false;
  try {
    motor.commandStart(unsafe, 10);
  } catch (const std::logic_error &) {
    blocked = true;
  }
  assert(blocked);

  Inputs safe{false, true, true, false};
  assert(motor.commandStart(safe, 100) == MotorState::Starting);
  assert(motor.scan(safe, 1599) == MotorState::Starting);
  assert(motor.scan(safe, 1600) == MotorState::Running);
  assert(motor.scan({false, true, true, true}, 1700) == MotorState::Tripped);
  assert(motor.reset(safe, 1800) == MotorState::Stopped);
  assert(motor.sequence() == 4);
}
