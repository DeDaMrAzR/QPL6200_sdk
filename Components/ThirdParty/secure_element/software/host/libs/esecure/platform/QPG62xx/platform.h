/**
 * @brief platform specific functions used by eSecure library
 * @copyright Copyright (c) 2024, Qorvo Inc
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "iomem_layout.h"
#include "esec_mailbox_basics.h"
#include <stdbool.h>

/** Address type to be used in esec_chunk
 * On QPG62xx, the host memory can be directly accessed by the DMA. The content
 * of esec_chunk is used directly as DMA descriptor. Therefore the size of an
 * address in esec_chunk must match the DMA address width.
 * The host CPU is 32 bit based, while the DMA address width can be 32 or 64 bit.
 * So if a conversion is needed, it will always be to a larger container, which is
 * never a problem.
 **/
typedef dmaaddr_t chunkaddr_t;

/** Data descriptor in host memory */
typedef struct esec_chunk {
   chunkaddr_t addr;
   chunkaddr_t next;
   uint32_t len;
} esec_chunk;

/** Write to a register */
static inline void esec_wrreg(uint32_t address, uint32_t value)
{
   *(volatile uint32_t*)address = value;
}

/** Read from a register */
static inline uint32_t esec_rdreg(uint32_t address)
{
   return *(volatile uint32_t*)address;
}

/** Wait until a response becomes available on the mailbox RX FIFO
 *
 * Implementations can implement architecture and platform for optimal
 * power and CPU usage efficiency.
 **/
extern void esec_wait_cmd_response(void);

/** Check if a response is available on the mailbox RX FIFO
 *
 * @return true if a response is available
 **/
extern bool esec_has_cmd_response(void);

/** Map list of input and output chunks to descriptors for DMA
 *
 * This function will convert the previously created data descriptors from host
 * memory space to DMA address space, link them together in 2 descriptors lists
 * (one for input, one for output) and return the addresses of the first input
 * descriptor (mapped_in) and the first output descriptor (mapped_out). If there
 * is no input and/or output to be given, an address of zero is returned.
 **/
void esec_map_desclists(esec_chunk *inputs, esec_chunk *outputs,
   esec_chunk *end, dmaaddr_t *mapped_in, dmaaddr_t *mapped_out);


#endif
