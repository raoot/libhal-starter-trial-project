#include <libhal-actuator/rc_servo.hpp>
#include <libhal-micromod/micromod.hpp>
#include <libhal-sensor/imu/mpu6050.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/pwm.hpp>
#include "../include/resource_list.hpp"

void application(resource_list p_resources) {
    auto& console = *p_resources.console.value();
    auto& i2c = *p_resources.i2c.value();
    auto& clock = *p_resources.clock.value();
    // auto& pwm = *p_resources.pwm.value(); // might not need
    using namespace std::chrono_literals;

    hal::sensor::mpu6050 mpu(i2c); // mpu6050

    hal::actuator::rc_servo::settings rc_servo_settings = { // settings (servo)
    .frequency = 50,
    .min_angle = 0, // -90?
    .max_angle = 90,
    .min_microseconds = 1000,
    .max_microseconds = 2000,
    };
    hal::actuator::rc_servo servo(*p_resources.pwm.value(), rc_servo_settings); // servo

    while(true) { // loop
        auto read = mpu.read();
        hal::print<1024>(console, "x: %fg, y: %fg, z: %fg", read.x, read.y, read.z);
        hal::delay(clock, 100ms);
    }
}