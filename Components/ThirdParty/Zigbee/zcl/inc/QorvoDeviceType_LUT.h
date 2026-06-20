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

/** @file "QorvoDeviceType_LUT.h"
 *
 * Device Type LUT of the device types according to Device Type Library Specification - 19-02016 Revision 8
 */

#ifndef _DEVICETYPE_LUT_H_
#define _DEVICETYPE_LUT_H_

#include "global.h"
#include "QorvoDeviceType_defs.h"
#include "QorvoCluster_defs.h"

typedef struct devType_s {
    UInt16 devId;
    UInt8 mandatoryServerClusterSize;
    const UInt16* mandatoryServerClusters;
    UInt8 optionalServerClusterSize;
    const UInt16* optionalServerClusters;
} devType_t;

// On/Off switch
static const UInt16 mandatoryClustersOnoffSwitch[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };
static const UInt16 optionalClustersOnoffSwitch[] =
    {
        ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID
    };

// Level Control Switch
static const UInt16 mandatoryClustersLvlControlSwitch[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };
static const UInt16 optionalClustersLvlControlSwitch[] = 
    {
        ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID
    };

// On/Off output
static const UInt16 mandatoryClustersOnOffOutput[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID
    };

// Level controllable output
static const UInt16 mandatoryClustersLevelControllableOutput[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID
    };

// Scene selector
static const UInt16 mandatoryClustersSceneSelector[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };

// Configuration Tool 
static const UInt16 mandatoryClustersConfigurationTool[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };

// Remote control
static const UInt16 mandatoryClustersRemoteControl[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };

// Combined interface
static const UInt16 mandatoryClustersCombinedInterface[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };
static const UInt16 optionalClustersCombinedInterface[] = 
    {
        ZCL_OTA_UPGRADE_CLUSTER_ID
    };

// Range Extender
static const UInt16 mandatoryClustersRangeExtender[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };

// Mains Power outlet 
static const UInt16 mandatoryClustersMainsPowerOutlet[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID
    };

// Door Lock
static const UInt16 mandatoryClustersDoorLock[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_DOOR_LOCK_CLUSTER_ID,
    };
static const UInt16 optionalClustersDoorLock[] = 
    {
        ZCL_POWER_CONFIG_CLUSTER_ID,
        ZCL_ALARM_CLUSTER_ID,
        ZCL_POLL_CONTROL_CLUSTER_ID
    };

// Lock controller
static const UInt16 mandatoryClustersLockController[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };

// Simple sensor
static const UInt16 mandatoryClustersSimpleSensor[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_BINARY_INPUT_BASIC_CLUSTER_ID
    };

// Consumption awareness
static const UInt16 mandatoryClustersConsumptionAwareness[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_METERING_CLUSTER_ID
    };

// Home gateway
static const UInt16 mandatoryClustersHomeGateway[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_TIME_CLUSTER_ID
    };
static const UInt16 optionalClustersHomeGateway[] = 
    {
        ZCL_METERING_CLUSTER_ID,
        ZCL_OTA_UPGRADE_CLUSTER_ID 
    };

// Smart plug
static const UInt16 mandatoryClustersSmartPlug[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_METERING_CLUSTER_ID
    };
static const UInt16 optionalClustersSmartPlug[] = 
    {
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID 
    };

// White goods
static const UInt16 mandatoryClustersWhiteGoods[] = 
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_POWER_PROFILE_CLUSTER_ID,
        ZCL_APPLICIANCE_CONTROL_CLUSTER_ID,
        ZCL_APPLICIANCE_IDENTIFICATIONL_CLUSTER_ID,
        ZCL_APPLICIANCE_EVENTS_AND_ALERTS_CLUSTER_ID
    };

static const UInt16 optionalClustersWhiteGoods[] =
    {
        ZCL_APPLICIANCE_STATISTICS_CLUSTER_ID,
        ZCL_METERING_CLUSTER_ID
    };

// Meter inteferance
static const UInt16 mandatoryClustersMeterInterferance[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_METERING_CLUSTER_ID,
        ZCL_METER_IDENTIFICATION_CLUSTER_ID
    };
static const UInt16 optionalClustersMeterInterferance[] =
    {
        ZCL_PRICE_CLUSTER_ID
    };

// On/off light
static const UInt16 mandatoryClustersOnOffLight[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID
    };
static const UInt16 optionalClustersOnOffLight[] =
    {
        ZCL_LEVEL_CONTROL_CLUSTER_ID,
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID
    };

// Dimmable light
static const UInt16 mandatoryClustersDimmableLight[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID
    };
static const UInt16 optionalClustersDimmableLight[] =
    {
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID
    };

// Color dimmable light
static const UInt16 mandatoryClustersColorDimmableLight[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID,
        ZCL_COLOR_CONTROL_CLUSTER_ID
    };
static const UInt16 optionalClustersColorDimmableLight[] =
    {
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID
    };

// On/off light switch

static const UInt16 mandatoryClustersOnOffLightSwitch[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };
static const UInt16 optionalClustersOnOffLightSwitch[] =
    {
        ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID
    };

// Dimmer switch

static const UInt16 mandatoryClustersDimmerSwitch[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };
static const UInt16 optionalClustersDimmerSwitch[] =
    {
        ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID
    };

// Color dimmer switch
static const UInt16 mandatoryClustersColorDimmerSwitch[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID
    };
static const UInt16 optionalClustersColorDimmerSwitch[] =
    {
        ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID
    };

// Light Sensor
static const UInt16 mandatoryClustersLightSensor[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_ILLUMINANCE_MEASUREMENT_CLUSTER_ID
    };
// Occupancy Sensor
static const UInt16 mandatoryClustersOccupancySensor[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_OCCUPANCY_SENSING_CLUSTER_ID
    };
 
// On/off ballast
static const UInt16 mandatoryClustersOnOffBalast[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_POWER_CONFIG_CLUSTER_ID,
        ZCL_DEVICE_TEMP_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_BALLAST_CONFIGURATION_CLUSTER_ID
    };
static const UInt16 optionalClustersOnOffBalast[] =
    {
        ZCL_LEVEL_CONTROL_CLUSTER_ID,
        ZCL_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ID,
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID   
    };

// Dimmable balast
static const UInt16 mandatoryClustersDimmableBalast[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_POWER_CONFIG_CLUSTER_ID,
        ZCL_DEVICE_TEMP_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID,
        ZCL_BALLAST_CONFIGURATION_CLUSTER_ID
    };
static const UInt16 optionalClustersDimmableBalast[] =
    {
        ZCL_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ID,
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID
    };

// On/off plug-in unit
static const UInt16 mandatoryClustersOnOffPlugInUnit[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID
    };
static const UInt16 optionalClustersOnOffPlugInUnit[] =
    {
        ZCL_LEVEL_CONTROL_CLUSTER_ID
    };
// Dimmable plug-in unit 
static const UInt16 mandatoryClustersDimmablePlugInUnit[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID
    };

// Color temperature light
static const UInt16 mandatoryClustersColorTemperatureLight[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID,
        ZCL_COLOR_CONTROL_CLUSTER_ID
    };
static const UInt16 optionalClustersColorTemperatureLight[] =
    {
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID
        
    };

// Extended color light
static const UInt16 mandatoryClustersExtendedColorLight[] =
    {
        ZCL_BASIC_CLUSTER_ID,
        ZCL_IDENTIFY_CLUSTER_ID,
        ZCL_GROUPS_CLUSTER_ID,
        ZCL_SCENES_CLUSTER_ID,
        ZCL_ON_OFF_CLUSTER_ID,
        ZCL_LEVEL_CONTROL_CLUSTER_ID,
        ZCL_COLOR_CONTROL_CLUSTER_ID
    };
static const UInt16 optionalClustersExtendedColorLight[] =
    {
        ZCL_TOUCHLINK_COMMISSIONING_CLUSTER_ID
    };

static const devType_t deviceTypeLookUpTable[] = 
{
    {
        //Device ID
        DEVICE_ID_ONOFF_SWITCH,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOnoffSwitch)/sizeof(mandatoryClustersOnoffSwitch),
        //Mandatory Cluster array    
        mandatoryClustersOnoffSwitch,
        //Optional Cluster array size
        sizeof(optionalClustersOnoffSwitch)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersOnoffSwitch
    },
    {
        //Device ID
        DEVICE_ID_LEVEL_CONTROL_SWITCH,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersLvlControlSwitch)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersLvlControlSwitch,
        //Optional Cluster array size
        sizeof(optionalClustersLvlControlSwitch)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersLvlControlSwitch
    },
    {
        //Device ID
        DEVICE_ID_ON_OFF_OUTPUT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOnOffOutput)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersOnOffOutput,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL

    },
    {
        //Device ID
        DEVICE_ID_LEVEL_CONTROLLABLE_OUTPUT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersLevelControllableOutput)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersLevelControllableOutput,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_SCENE_SELECTOR,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersSceneSelector)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersSceneSelector,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_CONFIGURATION_TOOL,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersConfigurationTool)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersConfigurationTool,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_REMOTE_CONTROL,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersRemoteControl)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersRemoteControl,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_COMBINED_INTERFACE,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersCombinedInterface)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersCombinedInterface,
        //Optional Cluster array size
        sizeof(optionalClustersCombinedInterface)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersCombinedInterface
      },
    {
        //Device ID
        DEVICE_ID_RANGE_EXTENDER,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersRangeExtender)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersRangeExtender,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_MAINS_POWER_OUTLET,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersMainsPowerOutlet)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersMainsPowerOutlet,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_DOOR_LOCK,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersDoorLock)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersDoorLock,
        //Optional Cluster array size
        sizeof(optionalClustersDoorLock)/sizeof(UInt16),        
        //Optional Cluster array
        optionalClustersDoorLock
      },
    {
        //Device ID
        DEVICE_ID_DOOR_LOCK_CONTROLLER,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersLockController)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersLockController,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_SIMPLE_SENSOR,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersSimpleSensor)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersSimpleSensor,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_CONSUMPTION_AWARENESS,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersConsumptionAwareness)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersConsumptionAwareness,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_HOME_GATEWAY,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersHomeGateway)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersHomeGateway,
        //Optional Cluster array size
        sizeof(optionalClustersHomeGateway)/sizeof(UInt16),  
        //Optional Cluster array
        optionalClustersHomeGateway
      },
    {
        //Device ID
        DEVICE_ID_SMART_PLUG,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersSmartPlug)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersSmartPlug,
        //Optional Cluster array size
        sizeof(optionalClustersSmartPlug)/sizeof(UInt16),        
        //Optional Cluster array
        optionalClustersSmartPlug
      },
    {
        //Device ID
        DEVICE_ID_WHITE_GOODS,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersWhiteGoods)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersWhiteGoods,
        //Optional Cluster array size
        sizeof(optionalClustersWhiteGoods)/sizeof(UInt16),        
        //Optional Cluster array
        optionalClustersWhiteGoods
      },
    {
        //Device ID
        DEVICE_ID_METER_INTERFACE,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersMeterInterferance)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersMeterInterferance,
        //Optional Cluster array size
        sizeof(optionalClustersMeterInterferance)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersMeterInterferance
      },
    {
        //Device ID
        DEVICE_ID_ON_OFF_LIGHT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOnOffLight)/sizeof(UInt16),        
        //Mandatory Cluster array    
        mandatoryClustersOnOffLight,
        //Optional Cluster array size
        sizeof(optionalClustersOnOffLight)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersOnOffLight
      },
    {
        //Device ID
        DEVICE_ID_DIMMABLE_LIGHT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersDimmableLight)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersDimmableLight,
        //Optional Cluster array size
        sizeof(optionalClustersDimmableLight)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersDimmableLight
      },
    {
        //Device ID
        DEVICE_ID_COLOR_DIMMABLE_LIGHT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersColorDimmableLight)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersColorDimmableLight,
        //Optional Cluster array size
        sizeof(optionalClustersColorDimmableLight)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersColorDimmableLight
    }, 
    {
        //Device ID
        DEVICE_ID_ON_OFF_LIGHT_SWITCH,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOnOffLightSwitch)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersOnOffLightSwitch,
        //Optional Cluster array size
        sizeof(optionalClustersOnOffLightSwitch)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersOnOffLightSwitch
    },
    {
        //Device ID
        DEVICE_ID_DIMMER_SWITCH,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersDimmerSwitch)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersDimmerSwitch,
        //Optional Cluster array size
        sizeof(optionalClustersDimmerSwitch)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersDimmerSwitch
    },
    {
        //Device ID
        DEVICE_ID_COLOR_DIMMER_SWITCH,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersColorDimmerSwitch)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersColorDimmerSwitch,
        //Optional Cluster array size
        sizeof(optionalClustersColorDimmerSwitch)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersColorDimmerSwitch
    },  
    {
        //Device ID
        DEVICE_ID_LIGHT_SENSOR,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersLightSensor)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersLightSensor,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },  
    {
        //Device ID
        DEVICE_ID_OCCUPANCY_SENSOR,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOccupancySensor)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersOccupancySensor,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
      },
    {
        //Device ID
        DEVICE_ID_ON_OFF_BALLAST,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOnOffBalast)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersOnOffBalast,
        //Optional Cluster array size
        sizeof(optionalClustersOnOffBalast)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersOnOffBalast  
    }, 
    {
        //Device ID
        DEVICE_ID_DIMMABLE_BALLAST,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersDimmableBalast)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersDimmableBalast,
        //Optional Cluster array size
        sizeof(optionalClustersDimmableBalast)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersDimmableBalast  
    }, 
    {
        //Device ID
        DEVICE_ID_ON_OFF_PLUG_IN_UNIT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersOnOffPlugInUnit)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersOnOffPlugInUnit,
        //Optional Cluster array size
        sizeof(optionalClustersOnOffPlugInUnit)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersOnOffPlugInUnit  
    }, 
    {
        //Device ID
        DEVICE_ID_DIMMABLE_PLUG_IN_UNIT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersDimmablePlugInUnit)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersDimmablePlugInUnit,
        //Optional Cluster array size
        0,
        //Optional Cluster array
        NULL
    }, 
    {
        //Device ID
        DEVICE_ID_COLOR_TEMPERATURE_LIGHT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersColorTemperatureLight)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersColorTemperatureLight,
        //Optional Cluster array size
        sizeof(optionalClustersColorTemperatureLight)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersColorTemperatureLight
    },
    {
        //Device ID
        DEVICE_ID_EXTENDED_COLOR_LIGHT,
        //Mandatory Cluster array size
        sizeof(mandatoryClustersExtendedColorLight)/sizeof(UInt16),
        //Mandatory Cluster array    
        mandatoryClustersExtendedColorLight,
        //Optional Cluster array size
        sizeof(optionalClustersExtendedColorLight)/sizeof(UInt16),
        //Optional Cluster array
        optionalClustersExtendedColorLight
    }
};


devType_t* DeviceType_LUT_getDeviceTypebyId(UInt16 deviceTypeId);

#endif // _DEVICETYPE_LUT_H_
