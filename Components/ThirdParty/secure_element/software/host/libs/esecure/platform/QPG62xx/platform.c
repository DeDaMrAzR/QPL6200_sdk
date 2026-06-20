/**
 * @brief platform specific functions used by eSecure library
 * @copyright Copyright (c) 2024, Qorvo Inc
 */

#include <stdint.h>
#include "iomem_layout.h"
#include "../QPG62xx/platform.h" /* make sure the include doesn't accidentally take the platform.h from mbedtls */

#include "global.h"
#include "hal.h"

// xp4001 backwards compatibility

void esec_wait_cmd_response(void)
{
    while(GP_WB_READ_INT_CTRL_MASKED_APPUC_SECURE_ELEMENT_MAILBOX_RX_INTERRUPT() == 0)
        ;
}

bool esec_has_cmd_response(void)
{
    return (GP_WB_READ_ESEC_MB_STATUS_RX_REM_MASK() & 0xFFFF) != 0;
}

/** Convert a host address for DMA from the host to eSecure. **/
static inline dmaaddr_t esec_map_to_esec(chunkaddr_t addr)
{
    return (addr);
}

/** Convert a host address for DMA from eSecure to the host. **/
static inline dmaaddr_t esec_map_from_esec(chunkaddr_t addr)
{
    return (addr);
}

#define ESEC_DMA_END ((dmaaddr_t)1)

/* This implemenation maps the descriptors in place. The list of chunks
 * is replaced by the descriptors. This reduces the memory usage and code size.
 */
void esec_map_desclists(esec_chunk* inputs, esec_chunk* outputs, esec_chunk* end, dmaaddr_t* mapped_in,
                        dmaaddr_t* mapped_out)
{
    dmaaddr_t next_desc;
    esec_chunk* chunk;

    /* create list of input descriptors
     * esec_map_to_esec converts the data adresses to DMA address space
     * mapped_in returns the address of the first input descriptor or 0 if there
     * is no input data for the DMA to fetch
     */
    next_desc = esec_map_to_esec((chunkaddr_t)inputs);
    *mapped_in = (inputs == outputs) ? 0 : next_desc;
    for(chunk = inputs; chunk < outputs; chunk++)
    {
        next_desc += sizeof(esec_chunk);
        chunk->addr = esec_map_to_esec(chunk->addr);
        chunk->next = (chunk + 1 < outputs) ? next_desc : ESEC_DMA_END;
    }

    /* create list of output descriptors
     * esec_map_to_esec converts the data adresses to DMA address space
     * mapped_out returns the address of the first output descriptor or 0 if
     * there is no output data for the DMA to push
     */
    *mapped_out = (outputs == end) ? 0 : next_desc;
    for(chunk = outputs; chunk < end; chunk++)
    {
        next_desc += sizeof(esec_chunk);
        chunk->addr = esec_map_to_esec(chunk->addr);
        chunk->next = (chunk + 1 < end) ? next_desc : ESEC_DMA_END;
    }
}
