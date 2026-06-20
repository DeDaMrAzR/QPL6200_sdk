
#if defined(GP_BSP_ANTENNATUNECONFIG_10DBM_SINGLE_ENDED)
#define GP_BSP_ANTENNATUNECONFIG_INIT() \
    do                                  \
    {                                   \
    } while(0)
#elif defined(GP_BSP_ANTENNATUNECONFIG_10DBM_DIFFERENTIAL)
#define GP_BSP_ANTENNATUNECONFIG_INIT() \
    do                                  \
    {                                   \
    } while(0)
#elif defined(GP_BSP_ANTENNATUNECONFIG_20DBM_SINGLE_ENDED)
#define GP_BSP_ANTENNATUNECONFIG_INIT() \
    do                                  \
    {                                   \
    } while(0)
#elif defined(GP_BSP_ANTENNATUNECONFIG_20DBM_DIFFERENTIAL)
#define GP_BSP_ANTENNATUNECONFIG_INIT() \
    do                                  \
    {                                   \
    } while(0)
#else
#error "define one of the antenna tune modes for your design"
#endif
