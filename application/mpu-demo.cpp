#include <libhal-sensor/imu/mpu6050.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include "../include/resource_list.hpp"

void application(resource_list p_resources) {
    auto& console = *p_resources.console.value();
    auto& i2c = *p_resources.i2c.value();
    auto& clock = *p_resources.clock.value();
    using namespace std::chrono_literals;
    hal::sensor::mpu6050 mpu(i2c);

    while(true) {
        auto read = mpu.read();
        hal::print<1024>(console, "x: %fg, y: %fg, z: %fg", read.x, read.y, read.z);
        hal::delay(clock, 100ms);
    }
}