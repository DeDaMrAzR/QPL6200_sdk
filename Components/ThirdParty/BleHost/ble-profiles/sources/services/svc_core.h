/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example GATT and GAP service implementations.
 *
 *  Copyright (c) 2009-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Copyright (c) 2021, Qorvo Inc
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
 */
/*************************************************************************************************/

#ifndef SVC_CORE_H
#define SVC_CORE_H


#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GATT_AND_GAP_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/
/** \name GAP Service Handles
 * \note GAP -- RPAO characterstic added only when DM Privacy enabled
 */
/**@{*/
#define GAP_START_HDL               0x01               /*!< GAP start handle */
#define GAP_END_HDL                 (GAP_MAX_HDL - 3)  /*!< GAP end handle */
/**@}*/

/** \name GATT Service Handles
 *
 */
/**@{*/
#define GATT_START_HDL              0x10                /*!< GATT start handle */
#define GATT_END_HDL                (GATT_MAX_HDL - 1)  /*!< GATT end handle */
/**@}*/

/**************************************************************************************************
 Handles
**************************************************************************************************/

/** \name GAP Service Handles
 *
 */
/**@{*/
/*! \brief GAP service handle */
enum
{
  GAP_SVC_HDL = GAP_START_HDL,      /*!< GAP service declaration */
  GAP_DN_CH_HDL,                    /*!< Device name characteristic */
  GAP_DN_HDL,                       /*!< Device name */
  GAP_AP_CH_HDL,                    /*!< Appearance characteristic */
  GAP_AP_HDL,                       /*!< Appearance */
  GAP_CAR_CH_HDL,                   /*!< Central address resolution characteristic */
  GAP_CAR_HDL,                      /*!< Central address resolution */
  GAP_RPAO_CH_HDL,                  /*!< Resolvable private address only characteristic */
  GAP_RPAO_HDL,                     /*!< Resolvable private address only */
  GAP_MAX_HDL                       /*!< GAP maximum handle */
};
/**@}*/

/** \name GATT Service Handles
 *
 */
/**@{*/
/*! \brief GATT service handles */
enum
{
  GATT_SVC_HDL = GATT_START_HDL,    /*!< GATT service declaration */
  GATT_SC_CH_HDL,                   /*!< Service changed characteristic */
  GATT_SC_HDL,                      /*!< Service changed */
  GATT_SC_CH_CCC_HDL,               /*!< Service changed client characteristic configuration descriptor */
  GATT_CSF_CH_HDL,                  /*!< Client supported features characteristic */
  GATT_CSF_HDL,                     /*!< Client supported features */
  GATT_DBH_CH_HDL,                  /*!< Database hash characteristic */
  GATT_DBH_HDL,                     /*!< Database hash */
  GATT_SSF_CH_HDL,                  /*!< Server supported features characteristic */
  GATT_SSF_HDL,                     /*!< Server supported features */
  GATT_MAX_HDL                      /*!< GATT maximum handle */
};
/**@}*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGattCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*************************************************************************************************/
/*!
 *  \brief  Update the central address resolution attribute value.
 *
 *  \param  value   New value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapCentAddrResUpdate(bool_t value);

/*************************************************************************************************/
/*!
 *  \brief  Add the Resolvable Private Address Only (RPAO) characteristic to the GAP service.
 *          The RPAO characteristic should be added only when DM Privacy is enabled.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapAddRpaoCh(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the Server Supported Features (SSF) bitmask.
 *
 *  \param  value   New value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGattSetSsf(uint8_t value);

/*************************************************************************************************/
/*!
 *  \brief  Update the Device Name attribute value.
 *
 *  \param  name   Pointer to NULL terminated char string with device name
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapDeviceNameUpdate(const char *name);

/*************************************************************************************************/
/*!
 *  \brief  Update the Appearance attribute value.
 *
 *  \param  appearance  Value of Appearance attribute (@see svc_ch.c)
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapAppearanceUpdate(uint16_t appearance);

/*! \} */    /* GATT_AND_GAP_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_CORE_H */

