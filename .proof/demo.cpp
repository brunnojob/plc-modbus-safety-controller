#include "motor.hpp"
#include <cassert>
#include <iostream>
int main() {
    MotorController motor;
    Inputs healthy{false, true, true, false};
    auto start = motor.commandStart(healthy, 0);
    assert(start == MotorState::Starting);
    auto run = motor.scan(healthy, 5000);
    assert(run == MotorState::Running);
    auto trip = motor.scan({true, true, true, false}, 5001);
    assert(trip == MotorState::Tripped);
    auto denied = motor.reset({true, true, true, false}, 5002);
    assert(denied == MotorState::Tripped);
    auto reset = motor.reset(healthy, 5003);
    assert(reset == MotorState::Stopped);
    std::cout << "{\"starting\":true,\"running\":true,\"emergency_stop_trip\":true,\"unsafe_reset_denied\":true,\"healthy_reset\":true}\n";
}
