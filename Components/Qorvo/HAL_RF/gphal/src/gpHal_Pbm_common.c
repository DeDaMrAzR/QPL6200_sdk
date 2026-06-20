#include "gpHal_Pbm.h"

#pragma weak gpHal_GetTxAckRSSI

Int8 gpHal_GetTxAckRSSI(UInt8 PBMentry)
{
    NOT_USED(PBMentry); // Suppress unused parameter warning
    return 0;           // Default weak implementation returning RSSI of 0
}
