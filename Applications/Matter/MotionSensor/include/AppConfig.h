/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Enable/Disable Motion Status LED
#define CONFIG_ENABLE_MOTION_SENSOR_STATUS_LED 1

// Update Light Sensor MeasuredValue Interval
#define LIGHT_SENSOR_MEASURE_INTERVAL_SECONDS (30)

// This defines activates configuration of Identify cluster on endpoint 2 in base AppTask.cpp
#define ACTIVATE_IDENTIFY_ON_EP2

#endif // APP_CONFIG_H
