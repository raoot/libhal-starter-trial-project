// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <optional>

#include <libhal/functional.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/serial.hpp>
#include <libhal/steady_clock.hpp>
#include <libhal/i2c.hpp>
#include <libhal/accelerometer.hpp>
#include <libhal-actuator/rc_servo.hpp>
#include <libhal-expander/pca9685.hpp>

struct resource_list
{
  /// initialized 1st
  hal::callback<void()> reset = +[] {
    while (true) {
      continue;
    }
  };
  // both status_led and clock are required in order to generate the terminate
  // blink pattern
  std::optional<hal::output_pin*> status_led;
  std::optional<hal::steady_clock*> clock;
  // initialize 3rd to support logging error messages
  std::optional<hal::serial*> console;
  std::optional<hal::i2c*> i2c;
  std::optional<hal::pwm*> pwm;

  std::optional<hal::output_pin*> G0_SDA;
  std::optional<hal::output_pin*> G1_SCL;
};  

/**
 * @brief Initializes the target platform and the resource list
 *
 * @param p_list - the list of resources to initialize for the application to
 * run properly. The initialize platform library should initialize each resource
 * in the resoure list.
 */
 
 void initialize_platform(resource_list& p_list);
 void pca_setup(resource_list& p_list);