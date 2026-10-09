#include "motor.hpp"
#include <cassert>
int main() {
 Inputs safe{false,true,true,false};
 MotorController motor;
 motor.commandStart(safe,0xfffffff0U);
 assert(motor.scan(safe,1484)==MotorState::Running);
 motor.commandStop(1485);
 assert(motor.scan(safe,100)==MotorState::Stopping);
 assert(motor.scan(safe,900)==MotorState::Stopped);
}
