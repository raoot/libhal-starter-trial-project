#include "../include/resource_list.hpp"
#include <libhal-actuator/rc_servo.hpp>
#include <libhal-micromod/micromod.hpp>
#include <libhal-sensor/imu/mpu6050.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/pwm.hpp>
// #include <libhal-pca/pca9685.hpp>
#include <libhal-expander/pca9685.hpp>
#include <libhal/units.hpp>

/* void pca_setup(resource_list& p_resources){

    using namespace std::chrono_literals;
    using namespace hal::literals;

    auto& clock = *p_resources.clock.value();
    auto& console = *p_resources.console.value();
    auto& i2c = *p_resources.i2c.value();
    
    hal::print<1024>(console, "pca setting...");

    // hal::expander::pca9685 pca9685(i2c, 0b100'0000); } */

void application(resource_list p_resources){

    using namespace std::chrono_literals;
    auto& console = *p_resources.console.value();
    auto& i2c = *p_resources.i2c.value();
    auto& clock = *p_resources.clock.value();
    auto& led = *p_resources.status_led.value();
    
    auto& pwm = *p_resources.pwm.value();
    hal::print<1024>(console, "Initializing PWM...");
    
    for (int i = 0; i < 0xFF; i++) {
        auto res = hal::probe(i2c, i);
        if (res) {
            hal::print<1024>(console, "\n Res: %d \n", i);
        }
    }
    hal::sensor::mpu6050 mpu(i2c, 0x68); 
    hal::print<1024>(console, "Initializing MPU...");

    // pca_setup(p_resources);

    hal::actuator::rc_servo::settings rc_servo_settings{
        .frequency = 50,
        // total 180 deg, change for your use case
        .min_angle = -90,
        .max_angle = 90,
        // change to 500us and 2500us if RC Servo
        // supports those pulse widths
        .min_microseconds = 500,
        .max_microseconds = 2400,
      };
    hal::actuator::rc_servo servo(pwm, rc_servo_settings);
    hal::print<1024>(console, "Initializing Servo... \n");

    /* hal::expander::pca9685 pca(i2c, 0x40); // expander pca9685

    // pca.get_pwm_channel<1>();

    auto pwm0 = pca.get_pwm_channel<0>(); // allows you to get the duty cycle?
    pwm0.frequency(50); // 50 Hz
    // set servo to middle position
    pwm0.duty_cycle(0.075f); // 1.5ms duty cycle */

    hal::degrees degrees = -90;
    hal::delay(clock, 200ms);

    while(true) { // loop
        mpu.configure_full_scale(hal::sensor::mpu6050::max_acceleration::g2); 
        hal::print<1024>(console, "MPU Configured. \n");

        if (degrees > 90) {
            degrees = -90;
        }
        else {
            degrees += 20;    
        }
        
        servo.position(degrees);
        hal::delay(clock, 1000ms);

        hal::print<1024>(console, "Set Servo Position. \n");

        // flash
        auto read = mpu.read();
        hal::print<1024>(console, "x: %fg, y: %fg, z: %fg \n", read.x, read.y, read.z);
        led.level(true);
        hal::delay(clock, 100ms);
        led.level(false);
        hal::delay(clock, 100ms);
    }
}