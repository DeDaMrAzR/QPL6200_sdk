#include "hal.h"
#include "hal_defs.h"
#include "rap/rap_appuc.h"

/** Wait a number of us.
*/
void hal_Waitus(UInt16 us)
{
        rap_appuc_lpwait_us((UInt32)us);
}

/** Wait a number of ms.
 */
void hal_Waitms(UInt16 ms)
{
    while(ms--)
    {
        hal_Waitus(1000);
    }
}
