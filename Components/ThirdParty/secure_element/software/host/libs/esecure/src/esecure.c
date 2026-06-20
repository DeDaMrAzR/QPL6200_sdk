/**
 * @brief eSecure library
 * @copyright Copyright (c) 2019-2021 Silex Insight. All Rights reserved
 */

#include <stdbool.h>
#include "../include/esecure.h"
#include "../include/esecure_async.h"
#include "mb_commands.h"
#include "mb_regs.h"
#include "../QPG62xx/platform.h" /* make sure the include doesn't accidentally take the platform.h from mbedtls */
#include "esec_mailbox_basics.h"
#include "q_certificate_definition.h"

#define ESEC_DMA_REALIGN_DATA 0x20000000

#define ESEC_RESTR_LOCKED     (1 << 24)
#define ESEC_RESTR_INTERNAL   (2 << 24)
#define ESEC_RESTR_RESTRICTED (3 << 24)
#define KEY_MODE_OFFSET       (26)

#define ESEC_CMD_CHECK 0xf0000000

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4
#include "hal.h"

HAL_CRITICAL_SECTION_DEF(esec_mailbox_mutex);

static void esec_mb_push_header(uint32_t word)
{
    esec_wrreg(ESEC_MB_REG_TX_HEADER, word);
}

static void esec_mb_push(uint32_t word)
{
    esec_wrreg(ESEC_MB_REG_FIFO_DATA, word);
}

/** Enable mailbox RX interrupts */
void esec_enable_interrupts(void)
{
    esec_wrreg(ESEC_MB_REG_CONFIGURATION, ESEC_MB_CONFIG_RXINTEN);
}

/** Initialize the mutex */
void esec_init_mutex(void)
{
    // esec_wrreg(ESEC_MB_REG_CONFIGURATION, ESEC_MB_CONFIG_RXINTEN);
    HAL_CREATE_MUTEX(&esec_mailbox_mutex);
}

/** Disable mailbox RX interrupts */
void esec_disable_interrupts(void)
{
    esec_wrreg(ESEC_MB_REG_CONFIGURATION, 0);
}

void esec_mb_push_command(struct esec_mb* mb, uint32_t command, int entries)
{
    if(HAL_VALID_MUTEX(esec_mailbox_mutex))
    {
        if(command != ESEC_CMD_CUSTOM_GO_TO_SLEEP)
        {
            HAL_ACQUIRE_MUTEX(esec_mailbox_mutex);
        }
    }
    /* there's always a header, a command, inputs and outputs */
    entries += 2 + (2 * sizeof(dmaaddr_t) / sizeof(uint32_t));
    esec_mb_push_header(entries * 4);
    esec_mb_push(command);
    mb->pos = mb->descs;
    mb->output = mb->descs;
}

void esec_mb_push_param(uint32_t param)
{
    esec_mb_push(param);
}

void esec_mb_push_addr(const dmaaddr_t addr)
{
    for(uint32_t i = 0; i < sizeof(dmaaddr_t) / sizeof(uint32_t); i++)
    {
        esec_mb_push(((uint32_t*)&addr)[i]);
    }
}

void esec_mb_push_out_desc(struct esec_mb* mb, uint8_t* dest, uint32_t len)
{
    if(!len)
        return;
    mb->pos->addr = (chunkaddr_t)dest;
    mb->pos->len = len | ESEC_DMA_REALIGN_DATA;
    mb->pos++;
}

void esec_mb_push_in_buf(struct esec_mb* mb, const uint8_t* src, uint32_t len)
{
    if(!len)
        return;
    mb->pos->addr = (chunkaddr_t)src;
    mb->pos->len = len | ESEC_DMA_REALIGN_DATA;
    mb->pos++;
    mb->output++;
}

void esec_mb_finish_outputs(struct esec_mb* mb)
{
    dmaaddr_t dma_desc;
    dmaaddr_t output_desc;

    esec_map_desclists(mb->descs, mb->output, mb->pos, &dma_desc, &output_desc);
    esec_mb_push_addr(dma_desc);
    esec_mb_push_addr(output_desc);
}

/**
 * Test secure element's handling of DMA descriptor overflow conditions
 *
 * This function tests that the secure element correctly detects attempts to use
 * more DMA descriptors than allowed, verifying proper bounds checking in both
 * input (fetcher) and output (pusher) descriptors.
 *
 * @return ESEC_OKAY on successful test completion, ESEC_DMA_ERROR on failure
 */

struct esec_mb mb;

uint32_t test_esec_dma_descriptors_overflow(void)
{
    // Initialize test variables
    memset(&mb, 0, sizeof(mb));

    // Test 1: Input descriptor overflow

    // Push a command that we'll use to test input descriptor overflow
    esec_mb_push_command(&mb, ESEC_CMD_WRAP_KEY, 0);

    // Fill input descriptors beyond ESEC_MAX_DESCS limit
    for(int i = 0; i < ESEC_MAX_DESCS + 5; i++)
    {
        esec_mb_push_in_buf(&mb, (void*)NULL, UINT32_MAX);
    }

    // Finish and check response
    esec_mb_finish_outputs(&mb);

    if(esec_pop_cmd_response() != ESEC_INVALID_PARAMETER)
    {
        return ESEC_DMA_ERROR;
    }

    // Test 2: Output descriptor overflow

    memset(&mb, 0, sizeof(mb));
    // Push a command that we'll use to test output descriptor overflow
    esec_mb_push_command(&mb, ESEC_CMD_WRAP_KEY, 0);

    // Add a single valid input descriptor
    esec_mb_push_in_buf(&mb, (void*)NULL, UINT32_MAX);

    // Fill output descriptors beyond ESEC_MAX_DESCS limit
    for(int i = 0; i < ESEC_MAX_DESCS + 5; i++)
    {
        esec_mb_push_out_desc(&mb, (void*)NULL, UINT32_MAX);
    }

    // Finish and check response
    esec_mb_finish_outputs(&mb);

    if(esec_pop_cmd_response() != ESEC_INVALID_PARAMETER)
    {
        return ESEC_DMA_ERROR;
    }

    return ESEC_OKAY;
}

uint32_t esec_aes_ecb_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ecb_encrypt_go(&mb, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ecb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ecb_decrypt_go(&mb, skey, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_enc_start(struct esec_aes_state* state, struct esec_skey skey, enum esec_aes_mode mode,
                            const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_enc_start_go(&mb, state, skey, mode, iv, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_dec_start(struct esec_aes_state* state, struct esec_skey skey, enum esec_aes_mode mode,
                            const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_dec_start_go(&mb, state, skey, mode, iv, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_cbc_enc_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start(state, skey, ESEC_AES_CBC, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_cbc_dec_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start(state, skey, ESEC_AES_CBC, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_ctr_enc_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start(state, skey, ESEC_AES_CTR, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_ctr_dec_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start(state, skey, ESEC_AES_CTR, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_cfb_enc_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start(state, skey, ESEC_AES_CFB, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_cfb_dec_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start(state, skey, ESEC_AES_CFB, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_ofb_enc_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start(state, skey, ESEC_AES_OFB, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_ofb_dec_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start(state, skey, ESEC_AES_OFB, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_xts_enc_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start(state, skey, ESEC_AES_XTS, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_xts_dec_start(struct esec_aes_state* state, struct esec_skey skey, const uint8_t iv[16],
                                const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start(state, skey, ESEC_AES_XTS, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_enc(struct esec_aes_state* state, const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_enc_go(&mb, state, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_dec(struct esec_aes_state* state, const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_dec_go(&mb, state, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_enc_finish(struct esec_aes_state* state, const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_enc_finish_go(&mb, state, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_dec_finish(struct esec_aes_state* state, const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_dec_finish_go(&mb, state, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ctr_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[16],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ctr_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ctr_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[16],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ctr_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_xts_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[16],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_xts_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_xts_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[16],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_xts_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_cbc_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[16],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_cbc_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_cbc_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[16],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_cbc_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_cfb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[16],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_cfb_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_cfb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[16],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_cfb_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ofb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[16],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ofb_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ofb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[16],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ofb_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_gcm_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[12],
                              uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen, uint8_t* tag,
                              size_t tag_len)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aes_gcm_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len, header, headerlen, tag, tag_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_gcm_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[12],
                              uint8_t* plaintext, size_t len, const uint8_t* header, size_t headerlen,
                              const uint8_t* tag, size_t tag_len)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aes_gcm_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len, header, headerlen, tag, tag_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ccm_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                              const uint8_t* header, size_t headerlen, const uint8_t* nonce, size_t noncelen,
                              uint8_t* tag, size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ccm_encrypt_go(&mb, skey, plaintext, ciphertext, len, header, headerlen, nonce, noncelen,
                                              tag, tagsize);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aes_ccm_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len,
                              const uint8_t* header, size_t headerlen, const uint8_t* nonce, size_t noncelen,
                              const uint8_t* tag, size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_ccm_decrypt_go(&mb, skey, ciphertext, plaintext, len, header, headerlen, nonce, noncelen,
                                              tag, tagsize);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_enc_start_go(&mb, state, skey, iv, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_enc_start_go(&mb, state, skey, iv, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_enc_start_go(&mb, state, skey, iv, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_enc_start_go(&mb, state, skey, iv, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_dec_start_go(&mb, state, skey, iv, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_dec_start_go(&mb, state, skey, iv, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_dec_start_go(&mb, state, skey, iv, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_dec_start_go(&mb, state, skey, iv, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_enc_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_enc_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_enc_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_enc_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_dec_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_dec_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_dec_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_dec_update_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_enc_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_enc_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_enc_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_enc_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_dec_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_dec_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_dec_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_dec_finish_go(&mb, state, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_encrypt_start(struct esec_skey skey, const uint8_t* plaintext,
                                     const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                     const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_gcm_encrypt_start_go(&mb, skey, plaintext, iv, ciphertext, len, header, headerlen, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_encrypt_update(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_gcm_encrypt_update_go(&mb, skey, plaintext, ciphertext, len, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_encrypt_finish(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tag_len,
                                      uint8_t lenAlenC[16])
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aria_gcm_encrypt_finish_go(&mb, skey, plaintext, ciphertext, len, ctx, tag, tag_len, lenAlenC);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_decrypt_start(struct esec_skey skey, uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE],
                                     const uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen,
                                     uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_gcm_decrypt_start_go(&mb, skey, plaintext, iv, ciphertext, len, header, headerlen, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_decrypt_update(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_gcm_decrypt_update_go(&mb, skey, plaintext, ciphertext, len, ctx);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_decrypt_finish(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tag_len,
                                      uint8_t lenAlenC[16])
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aria_gcm_decrypt_finish_go(&mb, skey, plaintext, ciphertext, len, ctx, tag, tag_len, lenAlenC);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_encrypt_start(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                     const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                                     uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize, size_t total_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ccm_encrypt_start_go(&mb, skey, plaintext, ciphertext, len, nonce, noncelen, header,
                                                     headerlen, ctx, tagsize, total_len);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_encrypt_update(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ccm_encrypt_update_go(&mb, skey, plaintext, ciphertext, len, noncelen, ctx, tagsize);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_encrypt_finish(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag,
                                      size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aria_ccm_encrypt_finish_go(&mb, skey, plaintext, ciphertext, len, noncelen, ctx, tag, tagsize);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_decrypt_start(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                     const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                                     uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize, size_t total_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ccm_decrypt_start_go(&mb, skey, plaintext, ciphertext, len, nonce, noncelen, header,
                                                     headerlen, ctx, tagsize, total_len);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_decrypt_update(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ccm_decrypt_update_go(&mb, skey, plaintext, ciphertext, len, noncelen, ctx, tagsize);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_decrypt_finish(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag,
                                      size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aria_ccm_decrypt_finish_go(&mb, skey, plaintext, ciphertext, len, noncelen, ctx, tag, tagsize);
    if(status != ESEC_OKAY)
        return status;
    return esec_pop_cmd_response();
}

uint32_t esec_3des_ecb_encrypt(struct esec_skey skey, const uint8_t* plaintext, size_t len, uint8_t* ciphertext)
{
    struct esec_mb mb = {};
    uint32_t status = esec_3des_ecb_encrypt_go(&mb, skey, plaintext, len, ciphertext);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_3des_ecb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, size_t len, uint8_t* plaintext)
{
    struct esec_mb mb = {};
    uint32_t status = esec_3des_ecb_decrypt_go(&mb, skey, ciphertext, len, plaintext);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_3des_cbc_encrypt(struct esec_skey skey, const uint8_t iv[8], const uint8_t* plaintext, size_t len,
                               uint8_t* ciphertext)
{
    struct esec_mb mb = {};
    uint32_t status = esec_3des_cbc_encrypt_go(&mb, skey, iv, plaintext, len, ciphertext);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_3des_cbc_decrypt(struct esec_skey skey, const uint8_t iv[8], const uint8_t* ciphertext, size_t len,
                               uint8_t* plaintext)
{
    struct esec_mb mb = {};
    uint32_t status = esec_3des_cbc_decrypt_go(&mb, skey, iv, ciphertext, len, plaintext);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_3des_cbc_mac(struct esec_skey skey, const uint8_t iv[8], const uint8_t* message, size_t len,
                           uint8_t mac[8])
{
    struct esec_mb mb = {};
    uint32_t status = esec_3des_cbc_mac_go(&mb, skey, iv, message, len, mac);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_chachapoly_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t nonce[12],
                                 uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen,
                                 uint8_t tag[16])
{
    struct esec_mb mb = {};
    uint32_t status = esec_chachapoly_encrypt_go(&mb, skey, plaintext, nonce, ciphertext, len, header, headerlen, tag);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_chachapoly_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t nonce[12],
                                 uint8_t* plaintext, size_t len, const uint8_t* header, size_t headerlen,
                                 const uint8_t tag[16])
{
    struct esec_mb mb = {};
    uint32_t status = esec_chachapoly_decrypt_go(&mb, skey, ciphertext, nonce, plaintext, len, header, headerlen, tag);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_chacha_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[4],
                             const uint8_t nonce[12], size_t len, uint8_t* ciphertext)
{
    struct esec_mb mb = {};
    uint32_t status = esec_chacha_encrypt_go(&mb, skey, plaintext, iv, nonce, len, ciphertext);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_chacha_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[4],
                             const uint8_t nonce[12], size_t len, uint8_t* plaintext)
{
    struct esec_mb mb = {};
    uint32_t status = esec_chacha_decrypt_go(&mb, skey, ciphertext, iv, nonce, len, plaintext);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hash(const uint16_t algo, const uint8_t* message, size_t len, uint8_t* digest)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hash_go(&mb, algo, message, len, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hash_update(const uint16_t algo, const uint8_t* state, const uint8_t* message, size_t len,
                          uint8_t* new_state)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hash_update_go(&mb, algo, state, message, len, new_state);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hash_finish(const uint16_t algo, const uint8_t* state, const uint8_t* message, size_t len,
                          size_t total_len, uint8_t* raw_digest)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hash_finish_go(&mb, algo, state, message, len, total_len, raw_digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hmac(const uint16_t algo, struct esec_skey skey, const uint8_t* message, size_t len, uint8_t* digest)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hmac_go(&mb, algo, skey, message, len, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hmac_begin(const uint16_t algo, struct esec_skey skey, const uint8_t* message, size_t len, uint8_t* state)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hmac_begin_go(&mb, algo, skey, message, len, state);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hmac_update(const uint16_t algo, const uint8_t* message, size_t len, const uint8_t* state,
                          uint8_t* state_new)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hmac_update_go(&mb, algo, message, len, state, state_new);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hmac_finish(const uint16_t algo, struct esec_skey skey, const uint8_t* message, size_t len,
                          const uint8_t* state, uint8_t* digest)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hmac_finish_go(&mb, algo, skey, message, len, state, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_cmac_generate(struct esec_skey skey, const uint8_t* message, size_t len,
                            uint8_t digest[ESEC_TAG_SIZE_AES_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_cmac_generate_go(&mb, skey, message, len, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_cmac_generate_begin(struct esec_skey skey, const uint8_t* message, size_t len, uint8_t* context_out)
{
    struct esec_mb mb = {};
    uint32_t status = esec_cmac_generate_begin_go(&mb, skey, message, len, context_out);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_cmac_generate_update(struct esec_skey skey, const uint8_t* message, size_t len, const uint8_t* context_in,
                                   uint8_t* context_out)
{
    struct esec_mb mb = {};
    uint32_t status = esec_cmac_generate_update_go(&mb, skey, message, len, context_in, context_out);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_cmac_generate_finish(struct esec_skey skey, const uint8_t* message, size_t len, const uint8_t* context_in,
                                   uint8_t digest[ESEC_TAG_SIZE_AES_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_cmac_generate_finish_go(&mb, skey, message, len, context_in, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

/** Verify a message integrity against the passed CMAC signature. */
uint32_t esec_cmac_verify(struct esec_skey skey, const uint8_t* message, size_t len,
                          const uint8_t digest[ESEC_TAG_SIZE_AES_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_cmac_verify_go(&mb, skey, message, len, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_shake_hash(uint16_t algo, const uint8_t* msg, size_t len, uint8_t* digest, size_t digest_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_shake_hash_go(&mb, algo, msg, len, digest, digest_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_shake_hash_begin(uint16_t algo, const uint8_t* msg, size_t len, uint8_t* state_out)
{
    struct esec_mb mb = {};
    uint32_t status = esec_shake_hash_begin_go(&mb, algo, msg, len, state_out);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_shake_hash_update(uint16_t algo, const uint8_t* msg, size_t len, const uint8_t* state_in,
                                uint8_t* state_out)
{
    struct esec_mb mb = {};
    uint32_t status = esec_shake_hash_update_go(&mb, algo, msg, len, state_in, state_out);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_shake_hash_finish(uint16_t algo, const uint8_t* msg, size_t len, const uint8_t* state_in, uint8_t* digest,
                                size_t digest_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_shake_hash_finish_go(&mb, algo, msg, len, state_in, digest, digest_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_create_skey(uint16_t len, struct esec_key_storage storage, int flags, struct esec_skey* skey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_create_skey_go(&mb, len, storage, flags, skey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_skey_from_seed(const uint8_t* salt, size_t salt_sz, uint16_t len, struct esec_key_storage storage,
                                    int flags, struct esec_skey* skey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_skey_from_seed_go(&mb, salt, salt_sz, len, storage, flags, skey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_hkdf_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* salt, size_t salt_sz,
                               const uint8_t* info, size_t info_sz, struct esec_key_storage storage, uint32_t len,
                               struct esec_skey* okm)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_hkdf_skey_go(&mb, ikm, hash_alg, salt, salt_sz, info, info_sz, storage, len, okm);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_pbkdf2_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* salt, size_t salt_sz,
                                 uint32_t iterations, struct esec_key_storage storage, uint32_t len,
                                 struct esec_skey* okm)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_pbkdf2_skey_go(&mb, ikm, hash_alg, salt, salt_sz, iterations, storage, len, okm);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_kdf2_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* param, size_t param_sz,
                               struct esec_key_storage storage, uint32_t len, struct esec_skey* okm)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_kdf2_skey_go(&mb, ikm, hash_alg, param, param_sz, storage, len, okm);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_nist_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* param, size_t param_sz,
                               struct esec_key_storage storage, uint32_t len, struct esec_skey* okm)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_nist_skey_go(&mb, ikm, hash_alg, param, param_sz, storage, len, okm);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_import_skey(const uint8_t* material, uint16_t klen, struct esec_key_storage storage, int flags,
                          struct esec_skey* skey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_import_skey_go(&mb, material, klen, storage, flags, skey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_export_skey(struct esec_skey skey, uint8_t* material)
{
    struct esec_mb mb = {};
    uint32_t status = esec_export_skey_go(&mb, skey, material);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_hw_export_skey(struct esec_skey skey, uint32_t offset)
{
    struct esec_mb mb = {};
    uint32_t status = esec_hw_export_skey_go(&mb, skey, offset);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_transfer_skey(struct esec_skey src, struct esec_key_storage storage, uint32_t busprotbits,
                            struct esec_skey* dst)
{
    struct esec_mb mb = {};
    uint32_t status = esec_transfer_skey_go(&mb, src, storage, busprotbits, dst);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_delete_skey(struct esec_skey key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_delete_skey_go(&mb, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecdsa_sign(const uint16_t hash_alg, struct esec_ecc_key key, const uint8_t* message, size_t len,
                         uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecdsa_sign_go(&mb, hash_alg, key, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecdsa_verify(const uint16_t hash_alg, struct esec_ecc_key key, const uint8_t* message, size_t len,
                           const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecdsa_verify_go(&mb, hash_alg, key, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecdsa_sign_digest(struct esec_ecc_key key, const uint8_t* digest, size_t len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecdsa_sign_digest_go(&mb, key, digest, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecdsa_verify_digest(struct esec_ecc_key key, const uint8_t* digest, size_t len, const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecdsa_verify_digest_go(&mb, key, digest, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_create_key(uint32_t domainspec, const uint8_t* domain, struct esec_key_storage storage, int flags,
                             struct esec_ecc_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecc_create_key_go(&mb, domainspec, domain, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_ecc_key_from_seed(const uint8_t* salt, size_t salt_sz, uint32_t domainspec, const uint8_t* domain,
                                       struct esec_key_storage storage, int flags, struct esec_ecc_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_ecc_key_from_seed_go(&mb, salt, salt_sz, domainspec, domain, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_import_key(uint32_t keyspec, const uint8_t* material, struct esec_key_storage storage, int flags,
                             struct esec_ecc_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecc_import_key_go(&mb, keyspec, material, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_export_key(struct esec_ecc_key key, uint8_t* material)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecc_export_key_go(&mb, key, material);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_read_pub_key(struct esec_ecc_key key, uint8_t* pubkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecc_read_pub_key_go(&mb, key, pubkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_transfer_key(struct esec_ecc_key src, struct esec_key_storage storage, uint32_t busprotbits,
                               struct esec_ecc_key* dst)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecc_transfer_key_go(&mb, src, storage, busprotbits, dst);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_delete_key(struct esec_ecc_key key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecc_delete_key_go(&mb, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_genkey_ecdh(struct esec_ecc_key key1, const uint8_t* key2, struct esec_key_storage storage, int flags,
                          struct esec_skey* sharedkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_genkey_ecdh_go(&mb, key1, key2, storage, flags, sharedkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_eddsa_create_key(uint32_t domainspec, struct esec_key_storage storage, int flags,
                               struct esec_eddsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_eddsa_create_key_go(&mb, domainspec, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_eddsa_import_key(uint32_t domainspec, uint32_t pubpriv, const uint8_t* material,
                               struct esec_key_storage storage, int flags, struct esec_eddsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_eddsa_import_key_go(&mb, domainspec, pubpriv, material, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_eddsa_export_key(struct esec_eddsa_key key, uint8_t* material)
{
    struct esec_mb mb = {};
    uint32_t status = esec_eddsa_export_key_go(&mb, key, material);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_eddsa_read_pub_key(struct esec_eddsa_key key, uint8_t* pubkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_eddsa_read_pub_key_go(&mb, key, pubkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_eddsa_transfer_key(struct esec_eddsa_key src, struct esec_key_storage storage, uint32_t busprotbits,
                                 struct esec_eddsa_key* dst)
{
    struct esec_mb mb = {};
    uint32_t status = esec_eddsa_transfer_key_go(&mb, src, storage, busprotbits, dst);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_eddsa_delete_key(struct esec_eddsa_key key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_eddsa_delete_key_go(&mb, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ed448_sign(struct esec_eddsa_key key, const uint8_t* ctx, size_t ctx_len, const uint8_t* message,
                         size_t len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ed448_sign_go(&mb, key, ctx, ctx_len, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ed25519_sign(struct esec_eddsa_key key, const uint8_t* message, size_t len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ed25519_sign_go(&mb, key, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ed25519_verify(struct esec_eddsa_key key, const uint8_t* message, size_t len, const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ed25519_verify_go(&mb, key, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ed448_verify(struct esec_eddsa_key key, const uint8_t* ctx, size_t ctx_len, const uint8_t* message,
                           size_t len, const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ed448_verify_go(&mb, key, ctx, ctx_len, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecjpake_generate_r1(const struct esec_jpake_cfg* cfg, uint8_t* x2, uint8_t* X1, uint8_t* zkp1,
                                  uint8_t* X2, uint8_t* zkp2)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecjpake_generate_r1_go(&mb, cfg, x2, X1, zkp1, X2, zkp2);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecjpake_verify_r1(const struct esec_jpake_cfg* cfg, const uint8_t* X3, const uint8_t* zkp3,
                                const uint8_t* X4, const uint8_t* zkp4)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecjpake_verify_r1_go(&mb, cfg, X3, zkp3, X4, zkp4);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecjpake_generate_r2(const struct esec_jpake_cfg* cfg, const uint8_t* pwd, size_t pwdlen,
                                  const uint8_t* x2, const uint8_t* X1, const uint8_t* X3, const uint8_t* X4,
                                  uint8_t* A, uint8_t* zkpA)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecjpake_generate_r2_go(&mb, cfg, pwd, pwdlen, x2, X1, X3, X4, A, zkpA);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecjpake_verify_r2(const struct esec_jpake_cfg* cfg, uint8_t* x2, const uint8_t* X1, const uint8_t* X2,
                                const uint8_t* X3, const uint8_t* B, const uint8_t* zkpB)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecjpake_verify_r2_go(&mb, cfg, x2, X1, X2, X3, B, zkpB);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ecjpake_generate_sessionkey(const struct esec_jpake_cfg* cfg, const uint8_t* pwd, size_t pwdlen,
                                          const uint8_t* x2, const uint8_t* X4, const uint8_t* B, uint8_t* session_key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecjpake_generate_sessionkey_go(&mb, cfg, pwd, pwdlen, x2, X4, B, session_key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_mark_boot_success(void)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, ESEC_CMD_BOOT_SUCCESS, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

static uint32_t esec_revoke_boot_key(uint32_t key_index, cr_key_t cr_key, struct esec_manufacturer_cert* cert,
                                     uint8_t signature[ESEC_MFCT_SIG_SZ], uint32_t cmd)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, cmd, 1);
    esec_mb_push_in_buf(&mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_push_in_buf(&mb, signature, ESEC_MFCT_SIG_SZ);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(cr_key);
    esec_mb_push_param(key_index);

    return esec_pop_cmd_response();
}

uint32_t esec_revoke_chip_boot_key(uint32_t key_index, struct esec_manufacturer_cert* cert,
                                   uint8_t signature[ESEC_MFCT_SIG_SZ])
{
    return esec_revoke_boot_key(key_index, CR_KEY_CM_FW, cert, signature, ESEC_CMD_REVOKE_PUB_KEY_CHIP);
}

uint32_t esec_revoke_system_boot_key(uint32_t key_index, struct esec_manufacturer_cert* cert,
                                     uint8_t signature[ESEC_MFCT_SIG_SZ])
{
    return esec_revoke_boot_key(key_index, CR_KEY_SM_FW, cert, signature, ESEC_CMD_REVOKE_PUB_KEY_SYSTEM);
}

static uint32_t esec_read_boot_pub_key(uint32_t key_index, uint8_t pubkey[ESEC_PUBKEY_DIGESTSIZE], uint32_t cmd)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, cmd, 1);
    esec_mb_push_out_desc(&mb, pubkey, ESEC_PUBKEY_DIGESTSIZE);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(key_index);

    return esec_pop_cmd_response();
}

uint32_t esec_read_chip_boot_key(uint32_t key_index, uint8_t pubkey[ESEC_PUBKEY_DIGESTSIZE])
{
    return esec_read_boot_pub_key(key_index, pubkey, ESEC_CMD_READ_CHIP_BOOT_PUBKEY);
}

uint32_t esec_read_system_boot_key(uint32_t key_index, uint8_t pubkey[ESEC_PUBKEY_DIGESTSIZE])
{
    return esec_read_boot_pub_key(key_index, pubkey, ESEC_CMD_READ_SYS_BOOT_PUBKEY);
}

uint32_t esec_get_random_bytes(uint8_t* bytes, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_get_random_bytes_go(&mb, bytes, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_read_clock(uint64_t* time)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, ESEC_CMD_READ_CLOCK, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)time, sizeof(*time));
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_generate_prime(uint8_t* prime, uint32_t bits, uint32_t rounds)
{
    struct esec_mb mb = {};
    uint32_t status = esec_generate_prime_go(&mb, prime, bits, rounds);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_read_serial_number(uint8_t sn[16])
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_READ_SERIAL_NUMBER, 0);
    esec_mb_push_out_desc(&mb, sn, 16);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

static uint32_t esec_read_tamperboot_status(struct esec_tamper* tamper, struct esec_boot* boot)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_GET_STATUS, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)tamper, 16);
    esec_mb_push_out_desc(&mb, (uint8_t*)boot, 12);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_read_tamper_status(struct esec_tamper* status)
{
    struct esec_boot garbage;

    return esec_read_tamperboot_status(status, &garbage);
}

uint32_t esec_read_boot_status(struct esec_boot* status)
{
    struct esec_tamper garbage;

    return esec_read_tamperboot_status(&garbage, status);
}

uint32_t esec_certify_eddsa_key(esec_certifier_key signer, const uint8_t nonce[12], struct esec_eddsa_key target_key,
                                struct esec_cert_ed25519_key* cert)
{
    struct esec_mb mb = {};
    uint32_t status = esec_certify_eddsa_key_go(&mb, signer, nonce, target_key, cert);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_certify_ecc_key(esec_certifier_key signer, const uint8_t nonce[12], struct esec_ecc_key target_key,
                              uint8_t* cert)
{
    struct esec_mb mb = {};
    uint32_t status = esec_certify_ecc_key_go(&mb, signer, nonce, target_key, cert);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_certify_time(esec_certifier_key signer, const uint8_t nonce[12], struct esec_time_certificate* cert)
{
    struct esec_mb mb = {};
    uint32_t status = esec_certify_time_go(&mb, signer, nonce, cert);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_read_chip_certificate(struct esec_chip_certificate* cert)
{
    struct esec_mb mb = {};
    uint32_t status = esec_read_chip_certificate_go(&mb, cert);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_get_attestation_token(uint8_t* challenge, uint32_t challenge_size, uint8_t* token, uint32_t token_size,
                                    uint32_t* token_len, int32_t* attest_status)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_get_attestation_token_go(&mb, challenge, challenge_size, token, token_size, token_len, attest_status);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_get_attestation_token_len(uint32_t challenge_size, uint32_t* token_len, int32_t* attest_status)
{
    struct esec_mb mb = {};
    uint32_t status = esec_get_attestation_token_len_go(&mb, challenge_size, token_len, attest_status);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_write_otp(uint32_t offset, const uint8_t* data, size_t len)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, ESEC_CMD_WRITE_OTP, 3);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(offset);
    esec_mb_push_param((uint32_t)data);
    esec_mb_push_param(len);

    return esec_pop_cmd_response();
}

uint32_t esec_read_otp(uint32_t offset, uint8_t* data, size_t len)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, ESEC_CMD_READ_OTP, 2);
    esec_mb_push_out_desc(&mb, data, len);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(offset);
    esec_mb_push_param(len);

    return esec_pop_cmd_response();
}

static uint32_t esec_get_maintenance_challenge(uint8_t* challenge)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_GET_MAINTENANCE_CHALLENGE, 0);
    esec_mb_push_out_desc(&mb, challenge, 16);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_create_tampering_request(uint32_t tampers, struct esec_tamper_maintenance_request* request)
{
    request->cmd = ESEC_CMD_DISABLE_TAMPER;
    request->access_bits = tampers;

    return esec_get_maintenance_challenge(request->challenge);
}

uint32_t esec_create_debuging_request(uint8_t* debug_bits, struct esec_debug_maintenance_request* request)
{
    size_t i;

    request->cmd = ESEC_CMD_DEBUG_ACCESS;
    for(i = 0; i < DBG_GRANT_SIZE; i++)
        request->access_bits[i] = debug_bits[i];
#if(DBG_GRANT_SIZE % 4)
    for(i = 0; i < 4 - (DBG_GRANT_SIZE % 4); i++)
        request->reserved[i] = 0;
#endif

    return esec_get_maintenance_challenge(request->challenge);
}

uint32_t esec_create_decommission_request(struct esec_decommission_request* request)
{
    request->cmd = ESEC_CMD_DECOMMISSION;
    return esec_get_maintenance_challenge(request->challenge);
}

uint32_t esec_create_set_virtual_time_request(uint32_t virtual_time, struct esec_set_virtual_time_request* request)
{
    request->cmd = ESEC_CMD_SET_VIRTUAL_TIME;
    request->virtual_time = virtual_time;

    return ESEC_OKAY;
}

uint32_t esec_apply_debug_maintenance(struct esec_manufacturer_cert* cert,
                                      struct esec_debug_maintenance_request* request,
                                      uint8_t signature[ESEC_MFCT_SIG_SZ])
{
    struct esec_mb mb = {};

    if(request->cmd != ESEC_CMD_DEBUG_ACCESS)
        return ESEC_INVALID_PARAMETER;
    esec_mb_push_command(&mb, request->cmd, 0);
    esec_mb_push_in_buf(&mb, request->access_bits, (DBG_GRANT_SIZE + 3) / 4 * 4);
    esec_mb_push_in_buf(&mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_push_in_buf(&mb, signature, ESEC_MFCT_SIG_SZ);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_apply_tamper_maintenance(struct esec_manufacturer_cert* cert,
                                       struct esec_tamper_maintenance_request* request,
                                       uint8_t signature[ESEC_MFCT_SIG_SZ])
{
    struct esec_mb mb = {};

    if(request->cmd != ESEC_CMD_DISABLE_TAMPER)
    {
        return ESEC_INVALID_PARAMETER;
    }
    esec_mb_push_command(&mb, request->cmd, 1);
    esec_mb_push_in_buf(&mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_push_in_buf(&mb, signature, ESEC_MFCT_SIG_SZ);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(request->access_bits);

    return esec_pop_cmd_response();
}

uint32_t esec_apply_decommission(struct esec_manufacturer_cert* cert, struct esec_decommission_request* request,
                                 uint8_t signature[ESEC_MFCT_SIG_SZ])
{
    struct esec_mb mb = {};

    if(request->cmd != ESEC_CMD_DECOMMISSION)
    {
        return ESEC_INVALID_PARAMETER;
    }
    esec_mb_push_command(&mb, request->cmd, 0);
    esec_mb_push_in_buf(&mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_push_in_buf(&mb, signature, ESEC_MFCT_SIG_SZ);
    esec_mb_finish_outputs(&mb);

    return ESEC_OKAY;
}

uint32_t esec_apply_set_virtual_time_maintenance(struct esec_manufacturer_cert* cert,
                                                 struct esec_set_virtual_time_request* request,
                                                 uint8_t signature[ESEC_MFCT_SIG_SZ])
{
    struct esec_mb mb = {};

    if(request->cmd != ESEC_CMD_SET_VIRTUAL_TIME) // Only SM can set virtual time
        return ESEC_INVALID_PARAMETER;

    esec_mb_push_command(&mb, request->cmd, 1);
    esec_mb_push_in_buf(&mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_push_in_buf(&mb, signature, ESEC_MFCT_SIG_SZ);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(request->virtual_time);

    return esec_pop_cmd_response();
}

uint32_t esec_get_virtual_time_maintenance(uint32_t* virtual_time)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_GET_VIRTUAL_TIME, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)virtual_time, sizeof(uint32_t));
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_integrity_add_region(const void* address, size_t size)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_INTEGRITY_UPDATE, 1 + (sizeof(dmaaddr_t) / sizeof(uint32_t)));
    esec_mb_finish_outputs(&mb);
    esec_mb_push_addr((const dmaaddr_t)address);
    esec_mb_push_param(size);
    return esec_pop_cmd_response();
}

uint32_t esec_integrity_verify(void)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_INTEGRITY_VERIFY, 0);
    esec_mb_finish_outputs(&mb);
    return esec_pop_cmd_response();
}

uint32_t esec_integrity_configure(uint32_t period)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_INTEGRITY_CONFIGURE, 1);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(period);
    return esec_pop_cmd_response();
}

uint32_t esec_rsa_create_key(uint32_t domainspec, uint8_t* pub_key, struct esec_key_storage storage, int flags,
                             struct esec_rsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_create_key_go(&mb, domainspec, pub_key, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_derive_rsa_key_from_seed(const uint8_t* salt, size_t salt_sz, uint32_t domainspec, uint8_t* pub_key,
                                       struct esec_key_storage storage, int flags, struct esec_rsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_derive_rsa_key_from_seed_go(&mb, salt, salt_sz, domainspec, pub_key, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_import_key(uint32_t keyspec, const uint8_t* material, struct esec_key_storage storage, int flags,
                             struct esec_rsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_import_key_go(&mb, keyspec, material, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_export_key(struct esec_rsa_key key, uint8_t* material)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_export_key_go(&mb, key, material);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_read_pub_key(struct esec_rsa_key key, uint8_t* pubkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_read_pub_key_go(&mb, key, pubkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_transfer_key(struct esec_rsa_key src, struct esec_key_storage storage, uint32_t busprotbits,
                               struct esec_rsa_key* dst)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_transfer_key_go(&mb, src, storage, busprotbits, dst);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_delete_key(struct esec_rsa_key key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_delete_key_go(&mb, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_sign(uint8_t padding, uint8_t hash_alg, struct esec_rsa_key key, const uint8_t* message, size_t len,
                       uint8_t* signature, size_t salt_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_sign_go(&mb, padding, hash_alg, key, message, len, signature, salt_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_verify(uint8_t padding, uint8_t hash_alg, struct esec_rsa_key key, const uint8_t* message, size_t len,
                         const uint8_t* signature, size_t salt_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_verify_go(&mb, padding, hash_alg, key, message, len, signature, salt_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_sign_digest(struct esec_rsa_key key, const uint8_t* digest, size_t len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_sign_digest_go(&mb, key, digest, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_verify_digest(struct esec_rsa_key key, const uint8_t* digest, size_t len, const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_verify_digest_go(&mb, key, digest, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_encrypt(uint8_t padding, struct esec_rsa_key key, const uint8_t* plaintext, size_t len,
                          uint8_t* ciphertext, uint8_t hash_algo)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_encrypt_go(&mb, padding, key, plaintext, len, ciphertext, hash_algo);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_decrypt(uint8_t padding, struct esec_rsa_key key, const uint8_t* ciphertext, uint8_t* plaintext,
                          uint32_t* decrypted_len, uint8_t hash_algo)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_decrypt_go(&mb, padding, key, ciphertext, plaintext, decrypted_len, hash_algo);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_key_wrap(struct esec_skey target_key, struct esec_rsa_key wrapping_key, uint32_t tmp_aes_key_len,
                           uint8_t* output, uint32_t out_size)
{
    struct esec_mb mb = {};
    uint32_t status = esec_rsa_key_wrap_go(&mb, target_key, wrapping_key, tmp_aes_key_len, output, out_size);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_rsa_key_unwrap(struct esec_rsa_key wrapping_key, uint8_t* wrapped_key, uint32_t wrapped_key_len,
                             uint32_t flags, uint16_t klen, struct esec_key_storage storage, struct esec_skey* key)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_rsa_key_unwrap_go(&mb, wrapping_key, wrapped_key, wrapped_key_len, flags, klen, storage, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_create_key(uint32_t domainspec, uint8_t* domain, struct esec_key_storage storage, int flags,
                             struct esec_dsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_create_key_go(&mb, domainspec, domain, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_import_key(uint32_t keyspec, const uint8_t* material, struct esec_key_storage storage, int flags,
                             struct esec_dsa_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_import_key_go(&mb, keyspec, material, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_export_key(struct esec_dsa_key key, uint8_t* material)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_export_key_go(&mb, key, material);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_read_pub_key(struct esec_dsa_key key, uint8_t* pubkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_read_pub_key_go(&mb, key, pubkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_transfer_key(struct esec_dsa_key src, struct esec_key_storage storage, uint32_t busprotbits,
                               struct esec_dsa_key* dst)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_transfer_key_go(&mb, src, storage, busprotbits, dst);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_delete_key(struct esec_dsa_key key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_delete_key_go(&mb, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_sign(const uint16_t hash_alg, struct esec_dsa_key key, const uint8_t* message, size_t len,
                       uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_sign_go(&mb, hash_alg, key, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_verify(const uint16_t hash_alg, struct esec_dsa_key key, const uint8_t* message, size_t len,
                         const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_verify_go(&mb, hash_alg, key, message, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_sign_digest(struct esec_dsa_key key, const uint8_t* digest, size_t len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_sign_digest_go(&mb, key, digest, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dsa_verify_digest(struct esec_dsa_key key, const uint8_t* digest, size_t len, const uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dsa_verify_digest_go(&mb, key, digest, len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_create_key(uint32_t domainspec, uint8_t* domain, struct esec_key_storage storage, int flags,
                            struct esec_dh_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_create_key_go(&mb, domainspec, domain, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_import_key(uint32_t keyspec, const uint8_t* material, struct esec_key_storage storage, int flags,
                            struct esec_dh_key* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_import_key_go(&mb, keyspec, material, storage, flags, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_export_key(struct esec_dh_key key, uint8_t* material)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_export_key_go(&mb, key, material);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_read_pub_key(struct esec_dh_key key, uint8_t* pubkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_read_pub_key_go(&mb, key, pubkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_transfer_key(struct esec_dh_key src, struct esec_key_storage storage, uint32_t busprotbits,
                              struct esec_dh_key* dst)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_transfer_key_go(&mb, src, storage, busprotbits, dst);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_delete_key(struct esec_dh_key key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_delete_key_go(&mb, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_dh_genkey(struct esec_dh_key key, uint8_t* remote_pubkey, struct esec_key_storage storage, int flags,
                        struct esec_skey* sharedkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_dh_genkey_go(&mb, key, remote_pubkey, storage, flags, sharedkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_srp_gen_verifier(struct esec_srp_config* cfg, struct esec_srp_user_credentials* creds, uint8_t* salt,
                               size_t salt_len, uint8_t* verifier)
{
    struct esec_mb mb = {};
    uint32_t status = esec_srp_gen_verifier_go(&mb, cfg, creds, salt, salt_len, verifier);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_srp_user_gen_public(struct esec_srp_config* cfg, struct esec_srp_user_session_state* session)
{
    struct esec_mb mb = {};
    uint32_t status = esec_srp_user_gen_public_go(&mb, cfg, session);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_srp_server_gen_public(struct esec_srp_config* cfg, uint8_t* verifier,
                                    struct esec_srp_server_session_state* session)
{
    struct esec_mb mb = {};
    uint32_t status = esec_srp_server_gen_public_go(&mb, cfg, verifier, session);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_srp_user_gen_key(struct esec_srp_config* cfg, struct esec_srp_user_credentials* creds, uint8_t* B,
                               uint8_t* salt, size_t salt_len, struct esec_srp_user_session_state* session,
                               uint8_t* sharedkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_srp_user_gen_key_go(&mb, cfg, creds, B, salt, salt_len, session, sharedkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_srp_server_gen_key(struct esec_srp_config* cfg, uint8_t* A, uint8_t* verifier,
                                 struct esec_srp_server_session_state* session, uint8_t* sharedkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_srp_server_gen_key_go(&mb, cfg, A, verifier, session, sharedkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_ar_load_table(struct esec_ar_entry* artable, uint32_t entries)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_LOAD_AR_TABLE, 1);
    esec_mb_push_in_buf(&mb, (uint8_t*)artable, entries * sizeof(struct esec_ar_entry));
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(entries);

    return esec_pop_cmd_response();
}

uint32_t esec_ar_read_table(struct esec_ar_entry* artable, uint32_t entries)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_READ_AR_TABLE, 1);
    esec_mb_push_out_desc(&mb, (uint8_t*)artable, entries * sizeof(struct esec_ar_entry));
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(entries);

    return esec_pop_cmd_response();
}

uint32_t esec_ar_commit_table(void)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_COMMIT_AR_TABLE, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_sm2_sign(struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len, const uint8_t* message,
                       size_t message_len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm2_sign_go(&mb, key, id_signer, id_signer_len, message, message_len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm2_verify(struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len,
                         const uint8_t* message, size_t message_len, uint8_t* signature)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm2_verify_go(&mb, key, id_signer, id_signer_len, message, message_len, signature);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm2_key_exch_rand(struct esec_ecc_key key, uint8_t* random_number, uint8_t* random_point)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm2_key_exch_rand_go(&mb, key, random_number, random_point);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm2_key_exch_gen(struct esec_ecc_key key_private, const uint8_t* key_public_rcv, const uint8_t* cofactor,
                               uint8_t* random_point_rcv, uint8_t* random_point_cal, uint8_t* random_number_cal,
                               struct esec_key_storage storage, int flags, struct esec_skey* sharedkey)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm2_key_exch_gen_go(&mb, key_private, key_public_rcv, cofactor, random_point_rcv,
                                               random_point_cal, random_number_cal, storage, flags, sharedkey);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_ecb_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_ecb_encrypt_go(&mb, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_ecb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_ecb_decrypt_go(&mb, skey, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_cbc_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_cbc_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_cbc_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_cbc_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_ctr_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_ctr_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_ctr_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_ctr_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_cfb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_cfb_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_cfb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_cfb_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_ofb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_ofb_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_ofb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_ofb_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_gcm_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_GCM_IV_SIZE],
                              uint8_t* ciphertext, size_t len, const uint8_t* aad, size_t aadlen, uint8_t* tag,
                              size_t tag_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_gcm_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len, aad, aadlen, tag, tag_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_sm4_gcm_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_GCM_IV_SIZE],
                              uint8_t* plaintext, size_t len, const uint8_t* aad, size_t aadlen, const uint8_t* tag,
                              size_t tag_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_sm4_gcm_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len, aad, aadlen, tag, tag_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ecb_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ecb_encrypt_go(&mb, skey, plaintext, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ecb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ecb_decrypt_go(&mb, skey, ciphertext, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cbc_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cbc_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ctr_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ctr_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cfb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cfb_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ofb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ofb_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE],
                               uint8_t* ciphertext, size_t len, const uint8_t* aad, size_t aadlen, uint8_t* tag,
                               size_t tag_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_gcm_encrypt_go(&mb, skey, plaintext, iv, ciphertext, len, aad, aadlen, tag, tag_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_gcm_decrypt(struct esec_skey skey, const uint8_t* ciphertext,
                               const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* plaintext, size_t len,
                               const uint8_t* aad, size_t aadlen, const uint8_t* tag, size_t tag_len)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_gcm_decrypt_go(&mb, skey, ciphertext, iv, plaintext, len, aad, aadlen, tag, tag_len);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                               const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                               uint8_t* tag, size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ccm_encrypt_go(&mb, skey, plaintext, ciphertext, len, nonce, noncelen, header,
                                               headerlen, tag, tagsize);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_ccm_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len,
                               const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                               const uint8_t* tag, size_t tagsize)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_ccm_decrypt_go(&mb, skey, ciphertext, plaintext, len, nonce, noncelen, header,
                                               headerlen, tag, tagsize);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_generate(struct esec_skey skey, const uint8_t* message, size_t len,
                                 uint8_t digest[ESEC_TAG_SIZE_ARIA_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_generate_go(&mb, skey, message, len, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

/** Verify a message integrity against the passed CMAC signature. */
uint32_t esec_aria_cmac_verify(struct esec_skey skey, const uint8_t* message, size_t len,
                               const uint8_t digest[ESEC_TAG_SIZE_ARIA_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_verify_go(&mb, skey, message, len, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_generate_start(struct esec_skey skey, const uint8_t* message, size_t len,
                                       uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_generate_init_go(&mb, skey, message, len, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_generate_update(struct esec_skey skey, const uint8_t* message, size_t len,
                                        uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_generate_update_go(&mb, skey, message, len, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_generate_finish(struct esec_skey skey, const uint8_t* message, size_t len,
                                        uint8_t ctx[ESEC_ARIA_IV_SIZE], uint8_t digest[ESEC_TAG_SIZE_ARIA_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_generate_finish_go(&mb, skey, message, len, ctx, digest);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_verify_start(struct esec_skey skey, const uint8_t* message, size_t len,
                                     uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_verify_init_go(&mb, skey, message, len, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_verify_update(struct esec_skey skey, const uint8_t* message, size_t len,
                                      uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_verify_update_go(&mb, skey, message, len, ctx);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_aria_cmac_verify_finish(struct esec_skey skey, const uint8_t* message, size_t len,
                                      uint8_t ctx[ESEC_ARIA_IV_SIZE], uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC])
{
    struct esec_mb mb = {};
    uint32_t status = esec_aria_cmac_verify_finish_go(&mb, skey, message, len, ctx, tag);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

/** Wrap a key(or data) using AES. */
uint32_t esec_aes_key_wrap(struct esec_skey target_key, struct esec_skey wrapping_key, uint8_t* output,
                           uint32_t output_size)
{
    struct esec_mb mb = {};
    uint32_t status = esec_aes_key_wrap_go(&mb, target_key, wrapping_key, output, output_size);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

/** Unwrap a key(or data) using AES. */
uint32_t esec_aes_key_unwrap(struct esec_skey wrapping_key, uint8_t* wrapped_key, uint32_t wrapped_key_len,
                             uint32_t flags, uint16_t klen, struct esec_key_storage storage, struct esec_skey* key)
{
    struct esec_mb mb = {};
    uint32_t status =
        esec_aes_key_unwrap_go(&mb, wrapping_key, wrapped_key, wrapped_key_len, flags, klen, storage, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

/** Wrap a key using ECDH */
uint32_t esec_ecdh_key_wrap(struct esec_skey target_key, struct esec_ecc_key wrapping_key, uint8_t* shared_data,
                            uint32_t shared_data_len, uint32_t tmp_aes_key_len, uint8_t* output, uint32_t out_size)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecdh_key_wrap_go(&mb, target_key, wrapping_key, shared_data, shared_data_len,
                                            tmp_aes_key_len, output, out_size);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

/** Unwrap a key using ECDH */
uint32_t esec_ecdh_key_unwrap(struct esec_ecc_key wrapping_key, uint8_t* shared_data, uint32_t shared_data_len,
                              uint32_t tmp_aes_key_len, uint8_t* wrapped_key, uint32_t wrapped_key_len, uint32_t flags,
                              uint16_t klen, struct esec_key_storage storage, struct esec_skey* key)
{
    struct esec_mb mb = {};
    uint32_t status = esec_ecdh_key_unwrap_go(&mb, wrapping_key, shared_data, shared_data_len, tmp_aes_key_len,
                                              wrapped_key, wrapped_key_len, flags, klen, storage, key);
    if(status != ESEC_OKAY)
        return status;

    return esec_pop_cmd_response();
}

uint32_t esec_get_firmware_version(uint32_t* version)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_VERSION, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)version, sizeof(*version));
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_nvm_write(uint32_t destination_address, uint32_t source_address, uint32_t length)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_NVM_WRITE, 3);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(destination_address);
    esec_mb_push_param(source_address);
    esec_mb_push_param(length);

    GP_WB_WRITE_APPUC_CACHE_FLUSH(1);

    return esec_pop_cmd_response();
}

uint32_t esec_nvm_write_with_error_check(uint32_t destination_address, uint32_t source_address)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_NVM_WRITE_WITH_ERROR_CHECK, 2);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(destination_address);
    esec_mb_push_param(source_address);

    GP_WB_WRITE_APPUC_CACHE_FLUSH(1);

    return esec_pop_cmd_response();
}

uint32_t esec_nvm_erase(uint32_t address, uint32_t length)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_NVM_ERASE, 2);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(address);
    esec_mb_push_param(length);

    return esec_pop_cmd_response();
}

uint32_t esec_reset_request(uint8_t soft_por_trigger)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, (ESEC_CMD_CUSTOM_RESET_REQUEST | ((uint32_t)soft_por_trigger << 8)), 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_set_clk_freq(uint8_t clk_freq_enum, core_enum_t core_enum)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_SET_CLK_FREQ | (clk_freq_enum << 8) | (core_enum), 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_xt32k_power_up(bool power_up)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_XT32K_POWER_UP | (power_up << 8), 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_get_sm_qorvo_config(uint32_t* qorvo_config)
{
    struct esec_mb mb = {};
    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_GET_SM_QORVO_CONFIG, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)qorvo_config, sizeof(*qorvo_config));
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_secp256r1_point_mul(const uint8_t* scalar, const uint8_t* point, uint8_t* result)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_ECC_SECP256R1_POINT_MUL, 0);
    esec_mb_push_in_buf(&mb, scalar, 32);
    esec_mb_push_in_buf(&mb, point, 64);
    esec_mb_push_out_desc(&mb, result, 64);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_ecc_secp256r1_point_muladd(const uint8_t* scalar_m, const uint8_t* point1, const uint8_t* scalar_n,
                                         const uint8_t* point2, uint8_t* result)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_ECC_SECP256R1_POINT_MULADD, 0);
    esec_mb_push_in_buf(&mb, scalar_m, 32);
    esec_mb_push_in_buf(&mb, point1, 64);
    esec_mb_push_in_buf(&mb, scalar_n, 32);
    esec_mb_push_in_buf(&mb, point2, 64);
    esec_mb_push_out_desc(&mb, result, 64);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

void esec_go_to_sleep(void)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_GO_TO_SLEEP, 0);
    esec_mb_finish_outputs(&mb);
}

uint32_t esec_get_seucfw_version(uint8_t* version)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_GET_SEUCFW_VERSION, 0);
    esec_mb_push_out_desc(&mb, version, 128);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_validate_certificate(certificate_t* certificate, uint32_t auth_misc, cr_key_t cr_key)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_VALIDATE_CERTIFICATE, 2);
    esec_mb_push_in_buf(&mb, (const uint8_t*)certificate, sizeof(certificate_t));
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(auth_misc);
    esec_mb_push_param(cr_key);

    return esec_pop_cmd_response();
}

uint32_t esec_finalize_wakeup(void)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_FINALIZE_WAKEUP, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_config_gpio_wakeup(uint32_t wakeup_pin_modes_0, uint32_t wakeup_pin_modes_1)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_CONFIG_GPIO_WAKEUP, 2);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(wakeup_pin_modes_0);
    esec_mb_push_param(wakeup_pin_modes_1);

    return esec_pop_cmd_response();
}

uint32_t esec_config_appuc_ram_retain(uint16_t ram_retain)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_CONFIG_APPUC_RAM_RETAIN + ram_retain, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_change_temperature()
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, EXEC_CMD_CUSTOM_CHANGE_TEMPERATURE, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_write_rc64k_use_vcore(uint8_t value)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_WRITE_RC64K_USE_VCORE + value, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_change_voltage()
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, EXEC_CMD_CUSTOM_CHANGE_BATTERY_VOLTAGE, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_set_power_mode(const power_mode_t power_mode)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_SET_POWER_MODE | power_mode, 0);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_get_power_mode(power_mode_t* power_mode)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_GET_POWER_MODE, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)power_mode, sizeof(*power_mode));
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_prepare_sleep(power_mode_t* power_mode)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_PREPARE_SLEEP, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)power_mode, sizeof(*power_mode));
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_standby_reset_write(uint8_t reset_index, uint32_t reset_value)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_STANDBY_RESET_WRITE | (reset_index << 8), 1);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(reset_value);

    return esec_pop_cmd_response();
}

uint32_t esec_get_platform_state(uint64_t nonce, platform_state_t* platform_state)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_GET_PLATFORM_STATE, 2);
    esec_mb_push_out_desc(&mb, (uint8_t*)platform_state, sizeof(*platform_state));
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(nonce & UINT32_MAX);
    esec_mb_push_param((nonce >> 32) & UINT32_MAX);

    return esec_pop_cmd_response();
}

uint32_t esec_start_upgrade(bool sefw_or_appuc, uint32_t offset)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, ESEC_CMD_CUSTOM_START_FW_UPGRADE | (sefw_or_appuc << 8), 1);
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(offset);

    return esec_pop_cmd_response();
}

uint32_t esec_get_tamper_levels(uint32_t* tamper_levels, uint8_t number_of_words)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, EXEC_CMD_CUSTOM_GET_TAMPER_LEVELS, 0);
    esec_mb_push_out_desc(&mb, (uint8_t*)tamper_levels, sizeof(*tamper_levels) * number_of_words);
    esec_mb_finish_outputs(&mb);

    return esec_pop_cmd_response();
}

uint32_t esec_apply_sm_bundle(uint32_t bundle_address, uint32_t* result)
{
    struct esec_mb mb = {};

    esec_mb_push_command(&mb, EXEC_CMD_CUSTOM_SM_PROVISIONING, 1);
    esec_mb_push_out_desc(&mb, (uint8_t*)result, sizeof(*result));
    esec_mb_finish_outputs(&mb);
    esec_mb_push_param(bundle_address);

    return esec_pop_cmd_response();
}
