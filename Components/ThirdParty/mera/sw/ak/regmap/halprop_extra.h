#ifndef HALPROP_EXTRA_H
#define HALPROP_EXTRA_H

//This file is also included on rombl *.S files, so keep it simple

#define GP_WB_ADDRESS_U1(bit)           \
    (GP_MM_WISHB_BITBAND_START + (GP_WB_##bit##_ADDRESS - GP_MM_AK_WISHB_START) * 32 + GP_WB_##bit##_LSB * 4)

#define GP_WB_WRITE(size, prop, val)    \
    GP_WB_WRITE_##size(GP_WB_##prop##_ADDRESS + GP_WB_##prop##_LSB / 8, (val))
#define GP_WB_READ(size, prop)          \
    GP_WB_READ_##size(GP_WB_##prop##_ADDRESS + GP_WB_##prop##_LSB / 8)

#define WRITE_IOB_GPIO_N_PULL_MODE_INNER(num, type)              GP_WB_WRITE_IOB_GPIO_## num ##_PULL_MODE(GP_WB_ENUM_GPIO_PULL_MODE_## type)
#define GP_WB_WRITE_IOB_GPIO_N_PULL_MODE(num, type)              WRITE_IOB_GPIO_N_PULL_MODE_INNER(num, type)

#define WRITE_IOB_GPIO_N_PULL_ENABLE_INNER(num, type)            GP_WB_WRITE_IOB_GPIO_## num ##_PULL_ENABLE(GP_WB_ENUM_GPIO_PULL_ENABLE_## type)
#define GP_WB_WRITE_IOB_GPIO_N_PULL_ENABLE(num, type)            WRITE_IOB_GPIO_N_PULL_ENABLE_INNER(num, type)

#define WRITE_IOB_GPIO_N_INPUT_INNER(num, val)                  GP_WB_WRITE_IOB_GPIO_## num ##_INPUT_BUFFER_ENABLE(val)
#define GP_WB_WRITE_IOB_GPIO_N_INPUT_BUFFER_ENABLE(num, val)    WRITE_IOB_GPIO_N_INPUT_INNER(num, val)

#define READ_AK_GPIO_N_INPUT_INNER(num)                            GP_WB_READ_AK_GPIO_GPIO## num ##_INPUT_VALUE()
#define GP_WB_READ_AK_GPIO_GPION_INPUT_VALUE(num)                  READ_GPIO_N_INPUT_INNER(num)

#define GP_WB_READ_AK_GPIO_GPIOX_INPUT_VALUE(x)                    (*(UInt32*) GP_WB_AK_GPIO_GPIO0_INPUT_VALUE_ADDRESS >> (x) & 1)

#endif //HALPROP_EXTRA_H

