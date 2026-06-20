//Auto generated file
#ifndef SYS_AHB_BUS_IDS_H
#define SYS_AHB_BUS_IDS_H

//------------------------------------------------------------------------------
//-- AHB AUSER (Address phase user bits) info
//------------------------------------------------------------------------------
#define AHB_AUSER_MASTER_ID_BIT_LSB 1
#define AHB_AUSER_MASTER_ID_BIT_MSB 4


//------------------------------------------------------------------------------
//-- AHB Master IDs (same value as priority, lower = higher priority on sys ahb bus)
//------------------------------------------------------------------------------
#define AHB_MASTER_ID_MSI_MAIN              0
#define AHB_MASTER_ID_DMA_SCB_M_AHB         1
#define AHB_MASTER_ID_SECURE_ELEMENT_PERIP  2
#define AHB_MASTER_ID_APPUC_SYS_AHB         3
#define AHB_MASTER_ID_AKUMA_M_AHB           4
#define AHB_MASTER_ID_IR_MAIN               5
#define AHB_MASTER_ID_USB_MAIN              6
#define AHB_MASTER_ID_SSP_MAIN              7
#define AHB_MASTER_ID_ES_ESSET              8
#define AHB_MASTER_ID_ES_ESGET              9
#define AHB_MASTER_ID_SECURE_ELEMENT_DMA    10
#define AHB_MASTER_ID_DMA_DCB_M_AHB         11

#endif //SYS_AHB_BUS_IDS_H
