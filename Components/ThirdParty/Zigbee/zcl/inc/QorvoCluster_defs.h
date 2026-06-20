/*
 * Copyright (c) 2024, Qorvo Inc
 *
 *
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 *
 */

/** @file "QorvoDeviceType_defs.h"
 *
 * Definition of the Clusters according to Device Type Library Specification - 19-02016 Revision 8
 */

#ifndef ZCL_CLUSTER_IDS_H
#define ZCL_CLUSTER_IDS_H
 
// General Cluster IDs
#define ZCL_BASIC_CLUSTER_ID               0x0000
#define ZCL_POWER_CONFIG_CLUSTER_ID        0x0001
#define ZCL_DEVICE_TEMP_CLUSTER_ID         0x0002
#define ZCL_IDENTIFY_CLUSTER_ID            0x0003
#define ZCL_GROUPS_CLUSTER_ID              0x0004
#define ZCL_SCENES_CLUSTER_ID              0x0005
#define ZCL_ON_OFF_CLUSTER_ID              0x0006
#define ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID 0x0007
#define ZCL_LEVEL_CONTROL_CLUSTER_ID       0x0008
#define ZCL_ALARM_CLUSTER_ID               0x0009
#define ZCL_TIME_CLUSTER_ID                0x000A
#define ZCL_BINARY_INPUT_BASIC_CLUSTER_ID  0x000F
#define ZCL_OTA_UPGRADE_CLUSTER_ID         0x0019
#define ZCL_POWER_PROFILE_CLUSTER_ID       0x001A
#define ZCL_POLL_CONTROL_CLUSTER_ID        0x0020
 
// Closures Cluster IDs
#define ZCL_DOOR_LOCK_CLUSTER_ID           0x0101
#define ZCL_WINDOW_COVERING_CLUSTER_ID     0x0102
 
// HVAC Cluster IDs
#define ZCL_THERMOSTAT_CLUSTER_ID          0x0201
#define ZCL_FAN_CONTROL_CLUSTER_ID         0x0202
 
// Lighting Cluster IDs
#define ZCL_COLOR_CONTROL_CLUSTER_ID       0x0300
#define ZCL_BALLAST_CONFIGURATION_CLUSTER_ID 0x0301

// Illuminance Cluster IDs
#define ZCL_ILLUMINANCE_MEASUREMENT_CLUSTER_ID 0x0400
#define ZCL_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ID 0x0401

// Occupancy Cluster IDs
#define ZCL_OCCUPANCY_SENSING_CLUSTER_ID   0x0406
 
// Security and Safety Cluster IDs
#define ZCL_IAS_ZONE_CLUSTER_ID            0x0500
#define ZCL_IAS_ACE_CLUSTER_ID             0x0501
#define ZCL_IAS_WD_CLUSTER_ID              0x0502
 
// Smart Energy Cluster IDs
#define ZCL_PRICE_CLUSTER_ID               0x0700
#define ZCL_METERING_CLUSTER_ID            0x0702
#define ZCL_DEMAND_RESPONSE_CLUSTER_ID     0x0703
#define ZCL_METER_IDENTIFICATION_CLUSTER_ID 0x0B01

// EN50523 Cluster IDs
#define ZCL_APPLICIANCE_CONTROL_CLUSTER_ID 0x001B
#define ZCL_APPLICIANCE_IDENTIFICATIONL_CLUSTER_ID 0x0B00
#define ZCL_APPLICIANCE_EVENTS_AND_ALERTS_CLUSTER_ID 0x0B02
#define ZCL_APPLICIANCE_STATISTICS_CLUSTER_ID 0x0B03

// Electrical Measurement Cluster IDs
#define ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID 0x0B04

// Touchlink Cluster IDs
#define ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID 0x1000
 
// Manufacturer Specific Cluster Range
#define ZCL_MANUFACTURER_SPECIFIC_CLUSTER_ID_START 0xFC00
#define ZCL_MANUFACTURER_SPECIFIC_CLUSTER_ID_END   0xFFFF
 
#endif // ZCL_CLUSTER_IDS_H
