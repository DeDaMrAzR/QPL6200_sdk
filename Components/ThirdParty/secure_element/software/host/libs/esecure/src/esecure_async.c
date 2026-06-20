/**
 * @brief Asynchronous eSecure library
 * @copyright Copyright (c) 2019-2021 Silex Insight. All Rights reserved
 * @file
 */

#include <stdbool.h>
#include "../include/esecure_async.h"
#include "mb_commands.h"
#include "mb_regs.h"

#define ESEC_DMA_REALIGN_DATA 0x20000000

#define ESEC_RESTR_LOCKED     (1 << 24)
#define ESEC_RESTR_INTERNAL   (2 << 24)
#define ESEC_RESTR_RESTRICTED (3 << 24)
#define KEY_MODE_OFFSET       (26)

#define ESEC_CMD_CHECK 0xf0000000

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4
#include "hal.h"

extern HAL_CRITICAL_SECTION_DEF(esec_mailbox_mutex);

static void esec_mb_push_header(uint32_t word)
{
    esec_wrreg(ESEC_MB_REG_TX_HEADER, word);
}

static void esec_mb_push(uint32_t word)
{
    esec_wrreg(ESEC_MB_REG_FIFO_DATA, word);
}

void esec_acquire_mutex(void)
{
    if(HAL_VALID_MUTEX(esec_mailbox_mutex))
    {
        HAL_ACQUIRE_MUTEX(esec_mailbox_mutex);
    }
}

void esec_release_mutex(void)
{
    if(HAL_VALID_MUTEX(esec_mailbox_mutex))
    {
        HAL_RELEASE_MUTEX(esec_mailbox_mutex);
    }
}

/* We rely on the mailbox TX fifo status before pushing a command. When bit 20
 * is up, we are sure at least 16 words may be safely pushed in the fifo.
 *
 * On 32bit systems, 2 words are reserved for addresses, 2 for the header and
 * command resulting in a maximum allowed size of 12 words for \c entries.
 * On 64bit systems, 4 words are reserved for addresses, 2 for the header and
 * command resulting in a maximum allowed size of 10 words for \c entries.
 */
static bool esec_mb_push_command(struct esec_mb* mb, uint32_t command, int entries)
{
    /* there's always a header, a command, inputs and outputs */
    entries += 2 + (2 * sizeof(dmaaddr_t) / sizeof(uint32_t));

    if(entries > 16)
        return false;

    if(HAL_VALID_MUTEX(esec_mailbox_mutex))
    {
        if(command != ESEC_CMD_CUSTOM_GO_TO_SLEEP)
        {
            HAL_ACQUIRE_MUTEX(esec_mailbox_mutex);
        }
    }
    if(!(esec_rdreg(ESEC_MB_REG_TX_STATUS) & (1 << 20)))
    {
        if(HAL_VALID_MUTEX(esec_mailbox_mutex))
        {
            if(command != ESEC_CMD_CUSTOM_GO_TO_SLEEP)
            {
                HAL_RELEASE_MUTEX(esec_mailbox_mutex);
            }
        }
        return false;
    }

    esec_mb_push_header(entries * 4);
    esec_mb_push(command);
    mb->pos = mb->descs;
    mb->output = mb->descs;

    return true;
}

static void esec_mb_push_param(uint32_t param)
{
    esec_mb_push(param);
}

static void esec_mb_push_addr(dmaaddr_t addr)
{
    for(uint32_t i = 0; i < sizeof(dmaaddr_t) / sizeof(uint32_t); i++)
    {
        esec_mb_push(((uint32_t*)&addr)[i]);
    }
}

static void esec_mb_push_out_desc(struct esec_mb* mb, uint8_t* dest, uint32_t len)
{
    if(!len)
        return;
    mb->pos->addr = (chunkaddr_t)dest;
    mb->pos->len = len | ESEC_DMA_REALIGN_DATA;
    mb->pos++;
}

static void esec_mb_push_in_chunk(struct esec_mb* mb, const uint8_t* src, uint32_t len)
{
    if(!len)
        return;
    mb->pos->addr = (chunkaddr_t)src;
    mb->pos->len = len;
    mb->pos++;
    mb->output++;
}

static void esec_mb_push_in_buf(struct esec_mb* mb, const uint8_t* src, uint32_t len)
{
    if(!len)
        return;
    mb->pos->addr = (chunkaddr_t)src;
    mb->pos->len = len | ESEC_DMA_REALIGN_DATA;
    mb->pos++;
    mb->output++;
}

static void esec_mb_finish_outputs(struct esec_mb* mb)
{
    dmaaddr_t dma_desc;
    dmaaddr_t output_desc;

    esec_map_desclists(mb->descs, mb->output, mb->pos, &dma_desc, &output_desc);
    esec_mb_push_addr(dma_desc);
    esec_mb_push_addr(output_desc);
}

static uint32_t esec_mb_pop_header(void)
{
    return esec_rdreg(ESEC_MB_REG_RX_HEADER);
}

int esec_pop_cmd_response(void)
{
    uint32_t rsp_header = 0;

    esec_wait_cmd_response();
    rsp_header = esec_mb_pop_header();
    while((rsp_header & 0xFFFF) == 0)
    {
        esec_wait_cmd_response();
        rsp_header = esec_mb_pop_header();
    }
    if(HAL_VALID_MUTEX(esec_mailbox_mutex))
    {
        HAL_RELEASE_MUTEX(esec_mailbox_mutex);
    }
    if((rsp_header & 0xFFFF) != 0x0004)
    {
        return ESEC_INVALID_RESPONSE;
    }
    return (rsp_header >> 16) & 0xF;
}

bool esec_pop_cmd_response_if_any(uint32_t* response)
{
    if(!esec_has_cmd_response())
        return false;

    uint32_t rsp_header = esec_mb_pop_header();
    if((rsp_header & 0xFFFF) != 0x0004)
        *response = ESEC_INVALID_RESPONSE;
    else
        *response = (rsp_header >> 16) & 0xF;

    return true;
}

static void esec_mb_push_in_domain(struct esec_mb* mb, const uint8_t* domain, uint32_t domainlen)
{
    esec_mb_push_in_buf(mb, domain, domainlen);
}

static uint32_t mb_storage_flags(struct esec_key_storage* storage)
{
    return storage->cfg;
}

/** Symmetric key of a given length (Bits[13:0]). **/
#define ESEC_KEY_SYM(len) (ESEC_KEY_TYPE_RAW | (len & 0x3FFF))

static void esec_mb_push_param_skey(struct esec_skey* skey)
{
    esec_mb_push_param(skey->keyspec | mb_storage_flags(&skey->storage));
}

static int esec_is_protected(struct esec_key_storage* storage)
{
    return ((storage->cfg >> 26) & 3) != ESEC_STORAGE_FLAG_HOST;
}

static void esec_mb_push_key_auth_if_any(struct esec_mb* mb, struct esec_key_storage* storage)
{
#if ESEC_STOR_AUTH_SIZE > 0
    // If in host (== not protected), storage->auth doesn't contain data, it's
    // a null pointer. And so, it doesn't make sense to push it to mailbox
    if(esec_is_protected(storage))
        esec_mb_push_in_buf(mb, storage->auth, ESEC_STOR_AUTH_SIZE);
#endif
}

static void esec_mb_push_key_material(struct esec_mb* mb, struct esec_key_storage* storage, size_t full_size)
{
    switch((storage->cfg >> 26) & 0x3)
    {
        case ESEC_STORAGE_FLAG_HOST:
            esec_mb_push_in_buf(mb, storage->material, full_size);
            break;
        case ESEC_STORAGE_FLAG_VOLATILE:
            esec_mb_push_key_auth_if_any(mb, storage);
            break;
        case ESEC_STORAGE_FLAG_WRAPPED:
        case ESEC_STORAGE_FLAG_WRAPPED_AR:
            esec_mb_push_key_auth_if_any(mb, storage);
            esec_mb_push_in_buf(mb, storage->material, full_size + ESEC_STOR_PROT_SIZE);
            break;
        default:
            break;
    }
}

static void esec_mb_push_key_material_out(struct esec_mb* mb, struct esec_key_storage* storage, size_t full_size)
{
    switch((storage->cfg >> 26) & 0x3)
    {
        case ESEC_STORAGE_FLAG_HOST:
            esec_mb_push_out_desc(mb, storage->material, full_size);
            break;
        case ESEC_STORAGE_FLAG_VOLATILE:
            break;
        case ESEC_STORAGE_FLAG_WRAPPED:
        case ESEC_STORAGE_FLAG_WRAPPED_AR:
            esec_mb_push_out_desc(mb, storage->material, full_size + ESEC_STOR_PROT_SIZE);
            break;
        default:
            break;
    }
}

static void esec_mb_push_in_skey(struct esec_mb* mb, struct esec_skey* skey)
{
    esec_mb_push_key_material(mb, &skey->storage, ESEC_KEY_SYM(skey->keyspec));
}

static uint32_t esec_crypt_aes_ecb_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                      uint8_t* ciphertext, size_t len, uint32_t cmd)
{
    if(!esec_mb_push_command(mb, cmd, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aes_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_ecb_go(mb, skey, plaintext, ciphertext, len, ESEC_CMD_ENCRYPT_ECB);
}

uint32_t esec_aes_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_ecb_go(mb, skey, ciphertext, plaintext, len, ESEC_CMD_DECRYPT_ECB);
}

static uint32_t esec_crypt_aes_go(struct esec_mb* mb, uint32_t cmd, struct esec_skey skey, const uint8_t* data_in,
                                  const uint8_t context_in[16], uint8_t* data_out, size_t len, uint8_t context_out[16])
{
    if(!esec_mb_push_command(mb, cmd, 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t context_switching = cmd & ESEC_CONTEXT_SWITCHING_MASK;
    bool has_context_out =
        context_switching == ESEC_CONTEXT_SWITCHING_START || context_switching == ESEC_CONTEXT_SWITCHING_MIDDLE;

    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, context_in, 16);
    esec_mb_push_in_buf(mb, data_in, len);
    esec_mb_push_out_desc(mb, data_out, len);
    if(has_context_out)
    {
        esec_mb_push_out_desc(mb, context_out, 16);
    }
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aes_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                               enum esec_aes_mode mode, const uint8_t iv[16], const uint8_t* plaintext,
                               uint8_t* ciphertext, size_t len)
{
    /* Store the eSecure command to be used. */
    state->command = ESEC_CMD_ENCRYPT_AES((uint32_t)mode);
    /* Store the key to be used. */
    state->skey = skey;

    return esec_crypt_aes_go(mb, state->command | ESEC_CONTEXT_SWITCHING_START, state->skey, plaintext, iv, ciphertext,
                             len, state->context);
}

uint32_t esec_aes_cbc_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start_go(mb, state, skey, ESEC_AES_CBC, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_ctr_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start_go(mb, state, skey, ESEC_AES_CTR, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_cfb_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start_go(mb, state, skey, ESEC_AES_CFB, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_ofb_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start_go(mb, state, skey, ESEC_AES_OFB, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_xts_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aes_enc_start_go(mb, state, skey, ESEC_AES_XTS, iv, plaintext, ciphertext, len);
}

uint32_t esec_aes_enc_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* plaintext,
                         uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, state->command | ESEC_CONTEXT_SWITCHING_MIDDLE, state->skey, plaintext, state->context,
                             ciphertext, len, state->context);
}

uint32_t esec_aes_enc_finish_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* plaintext,
                                uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, state->command | ESEC_CONTEXT_SWITCHING_END, state->skey, plaintext, state->context,
                             ciphertext, len, NULL);
}

uint32_t esec_aes_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                               enum esec_aes_mode mode, const uint8_t iv[16], const uint8_t* ciphertext,
                               uint8_t* plaintext, size_t len)
{
    /* Store the eSecure command to be used. */
    state->command = ESEC_CMD_DECRYPT_AES((uint32_t)mode);
    /* Store the key to be used. */
    state->skey = skey;

    return esec_crypt_aes_go(mb, state->command | ESEC_CONTEXT_SWITCHING_START, state->skey, ciphertext, iv, plaintext,
                             len, state->context);
}

uint32_t esec_aes_cbc_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start_go(mb, state, skey, ESEC_AES_CBC, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_ctr_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start_go(mb, state, skey, ESEC_AES_CTR, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_cfb_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start_go(mb, state, skey, ESEC_AES_CFB, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_ofb_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start_go(mb, state, skey, ESEC_AES_OFB, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_xts_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aes_dec_start_go(mb, state, skey, ESEC_AES_XTS, iv, ciphertext, plaintext, len);
}

uint32_t esec_aes_dec_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* ciphertext,
                         uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, state->command | ESEC_CONTEXT_SWITCHING_MIDDLE, state->skey, ciphertext,
                             state->context, plaintext, len, state->context);
}

uint32_t esec_aes_dec_finish_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* ciphertext,
                                uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, state->command | ESEC_CONTEXT_SWITCHING_END, state->skey, ciphertext, state->context,
                             plaintext, len, NULL);
}

uint32_t esec_aes_ctr_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_ENCRYPT_CTR, skey, plaintext, iv, ciphertext, len, NULL);
}

uint32_t esec_aes_ctr_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_DECRYPT_CTR, skey, ciphertext, iv, plaintext, len, NULL);
}

uint32_t esec_aes_xts_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_ENCRYPT_XTS, skey, plaintext, iv, ciphertext, len, NULL);
}

uint32_t esec_aes_xts_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_DECRYPT_XTS, skey, ciphertext, iv, plaintext, len, NULL);
}

uint32_t esec_aes_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_ENCRYPT_CBC, skey, plaintext, iv, ciphertext, len, NULL);
}

uint32_t esec_aes_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_DECRYPT_CBC, skey, ciphertext, iv, plaintext, len, NULL);
}

uint32_t esec_aes_cfb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_ENCRYPT_CFB, skey, plaintext, iv, ciphertext, len, NULL);
}

uint32_t esec_aes_cfb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_DECRYPT_CFB, skey, ciphertext, iv, plaintext, len, NULL);
}

uint32_t esec_aes_ofb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_ENCRYPT_OFB, skey, plaintext, iv, ciphertext, len, NULL);
}

uint32_t esec_aes_ofb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aes_go(mb, ESEC_CMD_DECRYPT_OFB, skey, ciphertext, iv, plaintext, len, NULL);
}

static uint32_t esec_import_stored_key_go(struct esec_mb* mb, uint32_t keyspec, uint16_t klen, const uint8_t* material,
                                          struct esec_key_storage* storage)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_WRAP_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, material, klen);
    esec_mb_push_key_auth_if_any(mb, storage);
    esec_mb_push_key_material_out(mb, storage, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(keyspec | mb_storage_flags(storage));

    return ESEC_OKAY;
}

uint32_t esec_aes_gcm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[12], uint8_t* ciphertext, size_t len, const uint8_t* header,
                                 size_t headerlen, uint8_t* tag, size_t tag_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_MAC_GCM | (((16 - tag_len) & 0xF) << 8), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 12);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tag_len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aes_gcm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[12], uint8_t* plaintext, size_t len, const uint8_t* header,
                                 size_t headerlen, const uint8_t* tag, size_t tag_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_MAC_GCM | (((16 - tag_len) & 0xF) << 8), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 12);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, tag_len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aes_ccm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen,
                                 const uint8_t* nonce, size_t noncelen, uint8_t* tag, size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_MAC_CCM, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, noncelen);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tagsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aes_ccm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 uint8_t* plaintext, size_t len, const uint8_t* header, size_t headerlen,
                                 const uint8_t* nonce, size_t noncelen, const uint8_t* tag, size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_MAC_CCM, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, noncelen);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, tagsize);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_3des_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext, size_t len,
                                  uint8_t* ciphertext)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_3DES_ENCRYPT_ECB, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_3des_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext, size_t len,
                                  uint8_t* plaintext)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_3DES_DECRYPT_ECB, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_3des_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t iv[8],
                                  const uint8_t* plaintext, size_t len, uint8_t* ciphertext)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_3DES_ENCRYPT_CBC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 8);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_3des_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t iv[8],
                                  const uint8_t* ciphertext, size_t len, uint8_t* plaintext)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_3DES_DECRYPT_CBC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 8);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_3des_cbc_mac_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t iv[8], const uint8_t* message,
                              size_t len, uint8_t mac[8])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_3DES_CBC_MAC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 8);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, mac, 8);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_chachapoly_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                    const uint8_t nonce[12], uint8_t* ciphertext, size_t len, const uint8_t* header,
                                    size_t headerlen, uint8_t tag[16])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_CHACHAPOL_ENC_MAC, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, 16);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_chachapoly_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                    const uint8_t nonce[12], uint8_t* plaintext, size_t len, const uint8_t* header,
                                    size_t headerlen, const uint8_t tag[16])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_CHACHAPOL_DEC_MAC, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, 16);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_chacha_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                const uint8_t iv[4], const uint8_t nonce[12], size_t len, uint8_t* ciphertext)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_CHACHAPOL_ENCRYPT, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 4);
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_chacha_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                const uint8_t iv[4], const uint8_t nonce[12], size_t len, uint8_t* plaintext)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_CHACHAPOL_DECRYPT, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 4);
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

static uint32_t esec_export_stored_key_go(struct esec_mb* mb, struct esec_key_storage* storage, uint32_t klen,
                                          uint32_t keyspec, uint8_t* material)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_UNWRAP_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, storage, klen);
    esec_mb_push_out_desc(mb, material, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(keyspec | mb_storage_flags(storage));

    return ESEC_OKAY;
}

static uint32_t esec_transfer_key_storage_go(struct esec_mb* mb, struct esec_key_storage* src, uint32_t keyspec,
                                             size_t klen, uint32_t busprotbits, struct esec_key_storage* dst)
{
    uint32_t new_index;

    new_index = (dst->cfg >> 16) & 0xFF;
    if(!esec_mb_push_command(mb, ESEC_CMD_TRANSFER_KEY, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, src, klen);
    esec_mb_push_key_auth_if_any(mb, dst);
    esec_mb_push_key_material_out(mb, dst, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(keyspec | src->cfg);
    esec_mb_push_param(new_index);
    esec_mb_push_param(busprotbits);

    return ESEC_OKAY;
}

static uint32_t esec_delete_stored_key_go(struct esec_mb* mb, struct esec_key_storage* storage, uint32_t klen,
                                          uint32_t keyspec)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DELETE_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, storage, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(keyspec | mb_storage_flags(storage));

    return ESEC_OKAY;
}

static uint32_t esec_create_stored_key_go(struct esec_mb* mb, uint32_t keyspec, uint16_t klen,
                                          struct esec_key_storage* storage)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_CREATE_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_auth_if_any(mb, storage);
    esec_mb_push_key_material_out(mb, storage, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(keyspec | mb_storage_flags(storage));

    return ESEC_OKAY;
}

static uint32_t hash_digest_len(const uint16_t algo)
{
    uint32_t sizes[] = {ESEC_HASH_SIZE_SHA1,        ESEC_HASH_SIZE_SHA224,      ESEC_HASH_SIZE_SHA256,
                        ESEC_HASH_SIZE_SHA384,      ESEC_HASH_SIZE_SHA512,      ESEC_SHA3_HASH_SIZE_SHA224,
                        ESEC_SHA3_HASH_SIZE_SHA256, ESEC_SHA3_HASH_SIZE_SHA384, ESEC_SHA3_HASH_SIZE_SHA512,
                        ESEC_HASH_SIZE_SM3};

    if(algo > ESEC_HASH_ALGO_SM3 || algo < ESEC_HASH_ALGO_SHA1)
        return ESEC_SHA3_HASH_SIZE_SHA512;

    return sizes[algo - ESEC_HASH_ALGO_SHA1];
}

uint32_t esec_hash_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* message, size_t len, uint8_t* digest)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HASH(algo), 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, digest, hash_digest_len(algo));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

const uint8_t esec_sha1_initial_state[ESEC_HASH_STATE_SIZE_SHA1] =
    "\x67\x45\x23\x01\xEF\xCD\xAB\x89\x98\xBA\xDC\xFE\x10\x32\x54\x76"
    "\xC3\xD2\xE1\xF0";

const uint8_t esec_sha224_initial_state[ESEC_HASH_STATE_SIZE_SHA224] =
    "\xc1\x05\x9e\xd8\x36\x7c\xd5\x07\x30\x70\xdd\x17\xf7\x0e\x59\x39"
    "\xff\xc0\x0b\x31\x68\x58\x15\x11\x64\xf9\x8f\xa7\xbe\xfa\x4f\xa4";

const uint8_t esec_sha256_initial_state[ESEC_HASH_STATE_SIZE_SHA256] =
    "\x6a\x09\xe6\x67\xbb\x67\xae\x85\x3c\x6e\xf3\x72\xa5\x4f\xf5\x3a"
    "\x51\x0e\x52\x7f\x9b\x05\x68\x8c\x1f\x83\xd9\xab\x5b\xe0\xcd\x19";

const uint8_t esec_sha384_initial_state[ESEC_HASH_STATE_SIZE_SHA384] =
    "\xcb\xbb\x9d\x5d\xc1\x05\x9e\xd8\x62\x9a\x29\x2a\x36\x7c\xd5\x07"
    "\x91\x59\x01\x5a\x30\x70\xdd\x17\x15\x2f\xec\xd8\xf7\x0e\x59\x39"
    "\x67\x33\x26\x67\xff\xc0\x0b\x31\x8e\xb4\x4a\x87\x68\x58\x15\x11"
    "\xdb\x0c\x2e\x0d\x64\xf9\x8f\xa7\x47\xb5\x48\x1d\xbe\xfa\x4f\xa4";

const uint8_t esec_sha512_initial_state[ESEC_HASH_STATE_SIZE_SHA512] =
    "\x6a\x09\xe6\x67\xf3\xbc\xc9\x08\xbb\x67\xae\x85\x84\xca\xa7\x3b"
    "\x3c\x6e\xf3\x72\xfe\x94\xf8\x2b\xa5\x4f\xf5\x3a\x5f\x1d\x36\xf1"
    "\x51\x0e\x52\x7f\xad\xe6\x82\xd1\x9b\x05\x68\x8c\x2b\x3e\x6c\x1f"
    "\x1f\x83\xd9\xab\xfb\x41\xbd\x6b\x5b\xe0\xcd\x19\x13\x7e\x21\x79";

/*
 * https://tools.ietf.org/id/draft-sca-cfrg-sm3-01.html
 *
 */
const uint8_t esec_sm3_hash_initial_state[ESEC_HASH_STATE_SIZE_SM3] =
    "\x73\x80\x16\x6f\x49\x14\xb2\xb9\x17\x24\x42\xd7\xda\x8a\x06\x00"
    "\xa9\x6f\x30\xbc\x16\x31\x38\xaa\xe3\x8d\xee\x4d\xb0\xfb\x0e\x4e";

static uint32_t hash_state_size(int algo)
{
    uint32_t size;
    switch(algo)
    {
        case ESEC_HASH_ALGO_SHA1:
            size = 20;
            break;
        case ESEC_HASH_ALGO_SHA224:
            size = 32;
            break;
        case ESEC_HASH_ALGO_SHA256:
            size = 32;
            break;
        case ESEC_HASH_ALGO_SHA384:
            size = 64;
            break;
        case ESEC_HASH_ALGO_SHA512:
            size = 64;
            break;
        case ESEC_HASH_ALGO_SM3:
            size = 32;
            break;
        case ESEC_HASH_ALGO_SHA3_224:
        case ESEC_HASH_ALGO_SHA3_256:
        case ESEC_HASH_ALGO_SHA3_384:
        case ESEC_HASH_ALGO_SHA3_512:
            size = ESEC_SHA3_HASH_STATE_SIZE;
            break;
        default:
            size = 0;
    }
    return size;
}

static uint32_t hmac_state_size(int algo)
{
    uint32_t size;
    switch(algo)
    {
        case ESEC_HASH_ALGO_SHA1:
            size = 28;
            break;
        case ESEC_HASH_ALGO_SHA224:
            size = 40;
            break;
        case ESEC_HASH_ALGO_SHA256:
            size = 40;
            break;
        case ESEC_HASH_ALGO_SHA384:
            size = 72;
            break;
        case ESEC_HASH_ALGO_SHA512:
            size = 72;
            break;
        case ESEC_HASH_ALGO_SHA3_224:
        case ESEC_HASH_ALGO_SHA3_256:
        case ESEC_HASH_ALGO_SHA3_384:
        case ESEC_HASH_ALGO_SHA3_512:
            size = 208;
            break;
        default:
            size = 0;
    }
    return size;
}

static uint32_t hash_output_size(int algo)
{
    uint32_t size;
    switch(algo)
    {
        case ESEC_HASH_ALGO_SHA1:
            size = ESEC_HASH_SIZE_SHA1;
            break;
        case ESEC_HASH_ALGO_SHA224:
            size = ESEC_HASH_SIZE_SHA224;
            break;
        case ESEC_HASH_ALGO_SHA256:
            size = ESEC_HASH_SIZE_SHA256;
            break;
        case ESEC_HASH_ALGO_SHA384:
            size = ESEC_HASH_SIZE_SHA384;
            break;
        case ESEC_HASH_ALGO_SHA512:
            size = ESEC_HASH_SIZE_SHA512;
            break;
        case ESEC_HASH_ALGO_SM3:
            size = ESEC_HASH_SIZE_SM3;
            break;
        case ESEC_HASH_ALGO_SHA3_224:
            size = ESEC_SHA3_HASH_SIZE_SHA224;
            break;
        case ESEC_HASH_ALGO_SHA3_256:
            size = ESEC_SHA3_HASH_SIZE_SHA256;
            break;
        case ESEC_HASH_ALGO_SHA3_384:
            size = ESEC_SHA3_HASH_SIZE_SHA384;
            break;
        case ESEC_HASH_ALGO_SHA3_512:
            size = ESEC_SHA3_HASH_SIZE_SHA512;
            break;
        default:
            size = 0;
    }
    return size;
}

uint32_t esec_hash_update_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* state, const uint8_t* message,
                             size_t len, uint8_t* new_state)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HASH_UPD(algo), 1))
        return ESEC_TX_FIFO_FULL;
    uint32_t state_size = hash_state_size(algo);
    esec_mb_push_in_buf(mb, state, state_size);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, new_state, state_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_hash_finish_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* state, const uint8_t* message,
                             size_t len, size_t total_len, uint8_t* raw_digest)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HASH_FINISH(algo), 2))
        return ESEC_TX_FIFO_FULL;
    uint32_t state_size = hash_state_size(algo);
    uint32_t digest_size = hash_output_size(algo);
    esec_mb_push_in_buf(mb, state, state_size);
    esec_mb_push_in_buf(mb, message, len);

    esec_mb_push_out_desc(mb, raw_digest, digest_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);
    esec_mb_push_param(total_len);

    return ESEC_OKAY;
}

uint32_t esec_hmac_go(struct esec_mb* mb, const uint16_t algo, struct esec_skey skey, const uint8_t* message,
                      size_t len, uint8_t* digest)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HMAC(algo), 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, digest, hash_digest_len(algo));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_hmac_begin_go(struct esec_mb* mb, const uint16_t algo, struct esec_skey skey, const uint8_t* message,
                            size_t len, uint8_t* state)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HMAC_BEGIN(algo), 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, state, hmac_state_size(algo));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_hmac_update_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* message, size_t len,
                             const uint8_t* state, uint8_t* state_new)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HMAC_UPDATE(algo), 1))
        return ESEC_TX_FIFO_FULL;
    size_t state_sz = hmac_state_size(algo);
    esec_mb_push_in_buf(mb, state, state_sz);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, state_new, state_sz);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_hmac_finish_go(struct esec_mb* mb, const uint16_t algo, struct esec_skey skey, const uint8_t* message,
                             size_t len, const uint8_t* state, uint8_t* digest)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HMAC_FINISH(algo), 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, state, hmac_state_size(algo));
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, digest, hash_digest_len(algo));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_cmac_generate_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                               uint8_t digest[ESEC_TAG_SIZE_AES_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_MAC_CMAC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, digest, ESEC_TAG_SIZE_AES_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_cmac_generate_begin_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                     uint8_t* context_out)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_MAC_CMAC | ESEC_CONTEXT_SWITCHING_START, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, context_out, ESEC_AES_BLOCK_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_cmac_generate_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                      const uint8_t* context_in, uint8_t* context_out)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_MAC_CMAC | ESEC_CONTEXT_SWITCHING_MIDDLE, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, context_in, ESEC_AES_BLOCK_SIZE);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, context_out, ESEC_AES_BLOCK_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_cmac_generate_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                      const uint8_t* context_in, uint8_t digest[ESEC_TAG_SIZE_AES_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_MAC_CMAC | ESEC_CONTEXT_SWITCHING_END, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, context_in, ESEC_AES_BLOCK_SIZE);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, digest, ESEC_TAG_SIZE_AES_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_cmac_verify_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                             const uint8_t digest[ESEC_TAG_SIZE_AES_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_MAC_CMAC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, digest, ESEC_TAG_SIZE_AES_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_shake_hash_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len, uint8_t* digest,
                            size_t digest_len)
{
    if(algo != ESEC_HASH_ALGO_SHAKE_128 && algo != ESEC_HASH_ALGO_SHAKE_256)
        return ESEC_INVALID_COMMAND;

    if(!esec_mb_push_command(mb, ESEC_CMD_SHAKE_HASH(algo), 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_buf(mb, msg, len);
    esec_mb_push_out_desc(mb, digest, digest_len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);
    esec_mb_push_param(digest_len);
    return ESEC_OKAY;
}

uint32_t esec_shake_hash_begin_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len, uint8_t* state_out)
{
    if(algo != ESEC_HASH_ALGO_SHAKE_128 && algo != ESEC_HASH_ALGO_SHAKE_256)
        return ESEC_INVALID_COMMAND;

    if(!esec_mb_push_command(mb, ESEC_CMD_SHAKE_HASH_BEGIN(algo), 1))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_buf(mb, msg, len);
    esec_mb_push_out_desc(mb, state_out, ESEC_SHA3_HASH_STATE_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);
    return ESEC_OKAY;
}

uint32_t esec_shake_hash_update_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len,
                                   const uint8_t* state_in, uint8_t* state_out)
{
    if(algo != ESEC_HASH_ALGO_SHAKE_128 && algo != ESEC_HASH_ALGO_SHAKE_256)
        return ESEC_INVALID_COMMAND;

    if(!esec_mb_push_command(mb, ESEC_CMD_SHAKE_HASH_UPDATE(algo), 1))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_buf(mb, state_in, ESEC_SHA3_HASH_STATE_SIZE);
    esec_mb_push_in_buf(mb, msg, len);
    esec_mb_push_out_desc(mb, state_out, ESEC_SHA3_HASH_STATE_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);
    return ESEC_OKAY;
}

uint32_t esec_shake_hash_finish_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len,
                                   const uint8_t* state_in, uint8_t* digest, size_t digest_len)
{
    if(algo != ESEC_HASH_ALGO_SHAKE_128 && algo != ESEC_HASH_ALGO_SHAKE_256)
        return ESEC_INVALID_COMMAND;

    if(!esec_mb_push_command(mb, ESEC_CMD_SHAKE_HASH_FINISH(algo), 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_buf(mb, state_in, ESEC_SHA3_HASH_STATE_SIZE);
    esec_mb_push_in_buf(mb, msg, len);
    esec_mb_push_out_desc(mb, digest, digest_len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);
    esec_mb_push_param(digest_len);
    return ESEC_OKAY;
}

uint32_t esec_create_skey_go(struct esec_mb* mb, uint16_t len, struct esec_key_storage storage, int flags,
                             struct esec_skey* skey)
{
    if(flags == ESEC_KEY_ONLY_ESECURE)
        return ESEC_INVALID_KEY;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        storage.cfg |= ESEC_RESTR_INTERNAL;
    skey->keyspec = len;
    skey->storage = storage;

    return esec_create_stored_key_go(mb, len, len, &skey->storage);
}

uint32_t esec_derive_skey_from_seed_go(struct esec_mb* mb, const uint8_t* salt, size_t salt_sz, uint16_t len,
                                       struct esec_key_storage storage, int flags, struct esec_skey* skey)
{
    if(flags == ESEC_KEY_ONLY_ESECURE)
        return ESEC_INVALID_KEY;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        storage.cfg |= ESEC_RESTR_INTERNAL;

    if(!esec_mb_push_command(mb, ESEC_CMD_DERIV_KEY_FROM_SEED, 2))
        return ESEC_TX_FIFO_FULL;
    skey->keyspec = len;
    skey->storage = storage;
    esec_mb_push_in_buf(mb, salt, salt_sz);
    esec_mb_push_key_auth_if_any(mb, &skey->storage);
    esec_mb_push_key_material_out(mb, &skey->storage, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(salt_sz);
    esec_mb_push_param_skey(skey);

    return ESEC_OKAY;
}

uint32_t esec_import_skey_go(struct esec_mb* mb, const uint8_t* material, uint16_t klen,
                             struct esec_key_storage storage, int flags, struct esec_skey* skey)
{
    if(flags & ~ESEC_KEY_EXPORTABLE)
        return ESEC_INVALID_PARAMETER;

    skey->keyspec = klen;
    skey->storage = storage;
    if(!esec_is_protected(&storage))
    {
        /* The key is already unprotected in the memory of the host. */
        return ESEC_INVALID_PARAMETER;
    }
    if(!(flags & ESEC_KEY_EXPORTABLE))
        skey->storage.cfg |= ESEC_RESTR_LOCKED;

    return esec_import_stored_key_go(mb, klen, klen, material, &skey->storage);
}

uint32_t esec_export_skey_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* material)
{
    if(!esec_is_protected(&skey.storage))
        return ESEC_INVALID_PARAMETER;

    return esec_export_stored_key_go(mb, &skey.storage, skey.keyspec, skey.keyspec, material);
}

uint32_t esec_hw_export_skey_go(struct esec_mb* mb, struct esec_skey skey, uint32_t offset)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_HW_EXPORT_KEY, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_key_material(mb, &skey.storage, skey.keyspec);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(skey.keyspec | mb_storage_flags(&skey.storage));
    esec_mb_push_param(offset);

    return ESEC_OKAY;
}

uint32_t esec_transfer_skey_go(struct esec_mb* mb, struct esec_skey src, struct esec_key_storage storage,
                               uint32_t busprotbits, struct esec_skey* dst)
{
    if(src.storage.cfg >> KEY_MODE_OFFSET != storage.cfg >> KEY_MODE_OFFSET)
        return ESEC_INVALID_PARAMETER;

    dst->keyspec = src.keyspec;
    dst->storage = storage;

    return esec_transfer_key_storage_go(mb, &src.storage, src.keyspec, src.keyspec, busprotbits, &dst->storage);
}

uint32_t esec_delete_skey_go(struct esec_mb* mb, struct esec_skey key)
{
    return esec_delete_stored_key_go(mb, &key.storage, key.keyspec, key.keyspec);
}

static uint32_t key_base_size(uint32_t keyspec)
{
    return ((keyspec & 0x7F) + 1);
}

static uint32_t compute_ecc_size(uint32_t keyflags, uint32_t kbaselen)
{
    uint32_t klen = 0;
    uint32_t pub_factor = 1;
    uint32_t domain_factor = 1;

    switch(keyflags & ESEC_KEY_TYPE_MASK)
    {
        case ESEC_KEY_TYPE_ECC_W:
            pub_factor = 2;
            domain_factor = 6;
            break;
        case ESEC_KEY_TYPE_ECKCDSA:
            pub_factor = 2;
            domain_factor = 6;
            break;
        case ESEC_KEY_TYPE_ECC_MG:
            pub_factor = 1;
            domain_factor = 3;
            break;
        default:
            break;
    }

    if(keyflags & ESEC_KEY_PUB)
        klen += kbaselen * pub_factor;
    if(keyflags & ESEC_KEY_PRIV)
        klen += kbaselen;
    if(keyflags & ESEC_KEY_DOMAIN)
    {
        klen += kbaselen * domain_factor;
    }
    return klen;
}

static void esec_mb_push_in_ecckey(struct esec_mb* mb, struct esec_ecc_key* key)
{
    uint32_t kbaselen = key_base_size(key->keyspec);
    uint32_t klen;

    klen = compute_ecc_size(key->keyspec, kbaselen);
    esec_mb_push_key_material(mb, &key->storage, klen);
}

static int ecdsa_check_command(uint32_t keyspec)
{
    int status = 0;

    switch(keyspec & ESEC_CMD_CHECK)
    {
        case ESEC_KEY_TYPE_ECC_W:
            status = ESEC_OKAY;
            break;
        case ESEC_KEY_TYPE_ECKCDSA:
            status = ESEC_OKAY;
            break;
        default:
            status = ESEC_INVALID_PARAMETER;
            break;
    }

    return status;
}

uint32_t esec_ecdsa_sign_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_ecc_key key,
                            const uint8_t* message, size_t len, uint8_t* signature)
{
    if((key.keyspec & ESEC_KEY_PRIV) != ESEC_KEY_PRIV)
        return ESEC_INVALID_KEY;
    if(ecdsa_check_command(key.keyspec) != ESEC_OKAY)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_SIGN(hash_alg), 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, signature, 2 * kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ecdsa_verify_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_ecc_key key,
                              const uint8_t* message, size_t len, const uint8_t* signature)
{
    if((key.keyspec & ESEC_KEY_PUB) != ESEC_KEY_PUB)
        return ESEC_INVALID_KEY;
    if(ecdsa_check_command(key.keyspec) != ESEC_OKAY)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY(hash_alg), 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, signature, kbaselen * 2);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ecdsa_sign_digest_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* digest, size_t len,
                                   uint8_t* signature)
{
    if((key.keyspec & ESEC_KEY_PRIV) != ESEC_KEY_PRIV)
        return ESEC_INVALID_KEY;
    if(ecdsa_check_command(key.keyspec) != ESEC_OKAY)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_SIGN_DIGEST, 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_in_buf(mb, digest, len);
    esec_mb_push_out_desc(mb, signature, 2 * kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ecdsa_verify_digest_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* digest, size_t len,
                                     const uint8_t* signature)
{
    if((key.keyspec & ESEC_KEY_PUB) != ESEC_KEY_PUB)
        return ESEC_INVALID_KEY;
    if(ecdsa_check_command(key.keyspec) != ESEC_OKAY)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_DIGEST, 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_in_buf(mb, digest, len);
    esec_mb_push_in_buf(mb, signature, kbaselen * 2);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ecc_create_key_go(struct esec_mb* mb, uint32_t domainspec, const uint8_t* domain,
                                struct esec_key_storage storage, int flags, struct esec_ecc_key* key)
{
    if((domainspec & ESEC_KEY_TYPE_MASK) == ESEC_KEY_TYPE_ECC_MG &&
       (flags & (ESEC_KEY_ONLY_ESECURE | ESEC_KEY_PERMS_SIGN)))
        /* Montgommery curves can only be used for ECDH key exchange.
         * They cannot be used for signatures. */
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_CREATE_KEY, 1))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(domainspec);
    uint32_t domainlen = compute_ecc_size(domainspec & (ESEC_KEY_TYPE_MASK | ESEC_KEY_DOMAIN), kbaselen);
    key->storage = storage;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    key->storage.cfg |= flags & ESEC_KEY_PERMS_SIGN;

    key->keyspec = domainspec;
    key->keyspec |= ESEC_KEY_PRIV | ESEC_KEY_PUB;
    esec_mb_push_in_domain(mb, domain, domainlen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &key->storage, compute_ecc_size(key->keyspec, kbaselen));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key->keyspec | mb_storage_flags(&key->storage));

    return ESEC_OKAY;
}

uint32_t esec_derive_ecc_key_from_seed_go(struct esec_mb* mb, const uint8_t* salt, size_t salt_sz, uint32_t domainspec,
                                          const uint8_t* domain, struct esec_key_storage storage, int flags,
                                          struct esec_ecc_key* key)
{
    if((domainspec & ESEC_KEY_TYPE_MASK) == ESEC_KEY_TYPE_ECC_MG &&
       (flags & (ESEC_KEY_ONLY_ESECURE | ESEC_KEY_PERMS_SIGN)))
        /* Montgommery curves can only be used for ECDH key exchange.
         * They cannot be used for signatures. */
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_DERIV_KEY_FROM_SEED, 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(domainspec);
    uint32_t domainlen = compute_ecc_size(domainspec & (ESEC_KEY_TYPE_MASK | ESEC_KEY_DOMAIN), kbaselen);
    key->storage = storage;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    key->storage.cfg |= flags & ESEC_KEY_PERMS_SIGN;

    key->keyspec = domainspec;
    key->keyspec |= ESEC_KEY_PRIV | ESEC_KEY_PUB;
    esec_mb_push_in_buf(mb, salt, salt_sz);
    esec_mb_push_in_domain(mb, domain, domainlen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &key->storage, compute_ecc_size(key->keyspec, kbaselen));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(salt_sz);
    esec_mb_push_param(key->keyspec | mb_storage_flags(&key->storage));

    return ESEC_OKAY;
}

uint32_t esec_ecc_import_key_go(struct esec_mb* mb, uint32_t keyspec, const uint8_t* material,
                                struct esec_key_storage storage, int flags, struct esec_ecc_key* key)
{
    uint32_t kbaselen = key_base_size(keyspec);
    uint32_t klen = compute_ecc_size(keyspec, kbaselen);

    if(!(keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&storage))
    {
        return ESEC_INVALID_PARAMETER;
    }
    if(flags & ~(ESEC_KEY_EXPORTABLE | ESEC_KEY_ONLY_ESECURE | ESEC_KEY_PERMS_SIGN))
        return ESEC_INVALID_PARAMETER;
    if((keyspec & ESEC_KEY_TYPE_MASK) == ESEC_KEY_TYPE_ECC_MG && (flags & ESEC_KEY_PERMS_SIGN))
        /* Montgommery curves can only be used for ECDH key exchange.
         * They cannot be used for signatures. */
        return ESEC_INVALID_PARAMETER;

    key->keyspec = keyspec;
    key->storage = storage;
    if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_LOCKED;

    return esec_import_stored_key_go(mb, key->keyspec, klen, material, &key->storage);
}

uint32_t esec_ecc_export_key_go(struct esec_mb* mb, struct esec_ecc_key key, uint8_t* material)
{
    uint32_t kbaselen = key_base_size(key.keyspec);
    uint32_t klen = compute_ecc_size(key.keyspec, kbaselen);

    if(!(key.keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&key.storage))
    {
        return ESEC_INVALID_PARAMETER;
    }

    return esec_export_stored_key_go(mb, &key.storage, klen, key.keyspec, material);
}

uint32_t esec_ecc_read_pub_key_go(struct esec_mb* mb, struct esec_ecc_key key, uint8_t* pubkey)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_READ_PUB_KEY, 1))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    uint32_t klen = compute_ecc_size(key.keyspec, kbaselen);
    uint32_t publen = compute_ecc_size((key.keyspec & ~ESEC_KEY_PRIV) | ESEC_KEY_PUB, kbaselen);
    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_out_desc(mb, pubkey, publen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));

    return ESEC_OKAY;
}

uint32_t esec_ecc_transfer_key_go(struct esec_mb* mb, struct esec_ecc_key src, struct esec_key_storage storage,
                                  uint32_t busprotbits, struct esec_ecc_key* dst)
{
    if(src.storage.cfg >> KEY_MODE_OFFSET != storage.cfg >> KEY_MODE_OFFSET)
        return ESEC_INVALID_PARAMETER;
    size_t kbaselen = key_base_size(src.keyspec);
    size_t klen = compute_ecc_size(src.keyspec, kbaselen);

    dst->keyspec = src.keyspec;
    dst->storage = storage;

    return esec_transfer_key_storage_go(mb, &src.storage, src.keyspec, klen, busprotbits, &dst->storage);
}

uint32_t esec_ecc_delete_key_go(struct esec_mb* mb, struct esec_ecc_key key)
{
    uint32_t kbaselen = key_base_size(key.keyspec);
    uint32_t klen = compute_ecc_size(key.keyspec, kbaselen);

    return esec_delete_stored_key_go(mb, &key.storage, klen, key.keyspec);
}

bool esec_ecc_is_domain_included(const struct esec_ecc_key* key)
{
    return key->keyspec & ESEC_KEY_DOMAIN;
}

uint32_t esec_genkey_ecdh_go(struct esec_mb* mb, struct esec_ecc_key key1, const uint8_t* key2,
                             struct esec_key_storage storage, int flags, struct esec_skey* sharedkey)
{
    uint32_t kbaselen = key_base_size(key1.keyspec);
    uint32_t pubklen = compute_ecc_size((key1.keyspec & ~(ESEC_KEY_PRIV | ESEC_KEY_DOMAIN)) | ESEC_KEY_PUB, kbaselen);

    if(!(key1.keyspec & ESEC_KEY_PRIV))
        return ESEC_INVALID_KEY;

    if(flags & ~ESEC_KEY_EXPORTABLE)
        return ESEC_INVALID_PARAMETER;

    sharedkey->keyspec = pubklen;
    sharedkey->storage = storage;
    if(!esec_mb_push_command(mb, ESEC_CMD_DIFFIE_HELLMAN, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_ecckey(mb, &key1);
    esec_mb_push_in_buf(mb, key2, pubklen);
    esec_mb_push_key_material_out(mb, &sharedkey->storage, sharedkey->keyspec);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key1.keyspec | mb_storage_flags(&key1.storage));
    esec_mb_push_param_skey(sharedkey);

    return ESEC_OKAY;
}

static inline uint32_t eddsa_size(uint32_t keyspec)
{
    return (keyspec & 0x7F) + 1;
}

static inline uint32_t eddsa_klen(uint32_t keyspec)
{
    /* The key size depends only on the presence of public or private key:
     *      bits 14  13
     *  none      0   0  => 0
     *  pub       0   1  => size
     *  priv      1   0  => size
     *  pair      1   1  => 2 * size
     *
     * That's the same as
     *    (((keyspec >> 13) + 1) >> 1) * size
     */
    return (((keyspec >> 13) + 1) >> 1) * eddsa_size(keyspec);
}

static void esec_mb_push_eddsa_spec(struct esec_eddsa_key* key)
{
    esec_mb_push_param(ESEC_KEY_TYPE_EDDSA | key->spec | mb_storage_flags(&key->storage));
}

uint32_t esec_eddsa_create_key_go(struct esec_mb* mb, uint32_t domainspec, struct esec_key_storage storage, int flags,
                                  struct esec_eddsa_key* key)
{
    key->spec = ESEC_KEY_PRIV | ESEC_KEY_PUB | domainspec;
    key->storage = storage;

    const uint32_t klen = eddsa_klen(key->spec);

    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    return esec_create_stored_key_go(mb, ESEC_KEY_TYPE_EDDSA | key->spec, klen, &key->storage);
}

uint32_t esec_eddsa_import_key_go(struct esec_mb* mb, uint32_t domainspec, uint32_t pubpriv, const uint8_t* material,
                                  struct esec_key_storage storage, int flags, struct esec_eddsa_key* key)
{
    key->spec = domainspec;

    if(!esec_is_protected(&storage))
    {
        return ESEC_INVALID_PARAMETER;
    }
    if(flags & ~ESEC_KEY_EXPORTABLE)
        return ESEC_INVALID_PARAMETER;
    key->spec |= pubpriv;
    key->storage = storage;

    if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_LOCKED;

    uint32_t klen = eddsa_klen(key->spec);
    if(!klen)
    {
        return ESEC_INVALID_KEY;
    }

    return esec_import_stored_key_go(mb, ESEC_KEY_TYPE_EDDSA | key->spec, klen, material, &key->storage);
}

uint32_t esec_eddsa_export_key_go(struct esec_mb* mb, struct esec_eddsa_key key, uint8_t* material)
{
    uint32_t klen = eddsa_klen(key.spec);

    if(!klen)
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&key.storage))
    {
        return ESEC_INVALID_PARAMETER;
    }
    return esec_export_stored_key_go(mb, &key.storage, klen, ESEC_KEY_TYPE_EDDSA | key.spec, material);
}

uint32_t esec_eddsa_read_pub_key_go(struct esec_mb* mb, struct esec_eddsa_key key, uint8_t* pubkey)
{
    uint32_t klen = eddsa_klen(key.spec);
    uint32_t size = eddsa_size(key.spec);

    if(!klen)
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, ESEC_CMD_READ_PUB_KEY, 1))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_out_desc(mb, pubkey, size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_eddsa_spec(&key);

    return ESEC_OKAY;
}

uint32_t esec_eddsa_transfer_key_go(struct esec_mb* mb, struct esec_eddsa_key src, struct esec_key_storage storage,
                                    uint32_t busprotbits, struct esec_eddsa_key* dst)
{
    if(src.storage.cfg >> KEY_MODE_OFFSET != storage.cfg >> KEY_MODE_OFFSET)
        return ESEC_INVALID_PARAMETER;
    size_t klen = eddsa_klen(src.spec);

    dst->spec = src.spec;
    dst->storage = storage;

    return esec_transfer_key_storage_go(mb, &src.storage, ESEC_KEY_TYPE_EDDSA | src.spec, klen, busprotbits,
                                        &dst->storage);
}

uint32_t esec_eddsa_delete_key_go(struct esec_mb* mb, struct esec_eddsa_key key)
{
    uint32_t klen = eddsa_klen(key.spec);

    return esec_delete_stored_key_go(mb, &key.storage, klen, ESEC_KEY_TYPE_EDDSA | key.spec);
}

uint32_t esec_ed448_sign_go(struct esec_mb* mb, struct esec_eddsa_key key, const uint8_t* ctx, size_t ctx_len,
                            const uint8_t* message, size_t len, uint8_t* signature)
{
    uint32_t klen = eddsa_klen(key.spec);
    uint32_t size = eddsa_size(key.spec);
    if(size != ESEC_KEY_ECC_EDDSA_ED448_BYTESIZE)
        return ESEC_INVALID_PARAMETER;
    if(!(key.spec & ESEC_KEY_PRIV))
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, ESEC_CMD_SIGN_EDDSA, 3))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_in_buf(mb, ctx, ctx_len);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, signature, 2 * size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_eddsa_spec(&key);
    esec_mb_push_param(ctx_len);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ed25519_sign_go(struct esec_mb* mb, struct esec_eddsa_key key, const uint8_t* message, size_t len,
                              uint8_t* signature)
{
    uint32_t klen = eddsa_klen(key.spec);
    uint32_t size = eddsa_size(key.spec);

    if(size != ESEC_KEY_ECC_EDDSA_ED25519_BYTESIZE)
        return ESEC_INVALID_PARAMETER;
    if(!(key.spec & ESEC_KEY_PRIV))
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, ESEC_CMD_SIGN_EDDSA, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, signature, 2 * size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_eddsa_spec(&key);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ed25519_verify_go(struct esec_mb* mb, struct esec_eddsa_key key, const uint8_t* message, size_t len,
                                const uint8_t* signature)
{
    uint32_t klen = eddsa_klen(key.spec);
    uint32_t size = eddsa_size(key.spec);

    if(size != ESEC_KEY_ECC_EDDSA_ED25519_BYTESIZE)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_EDDSA, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_in_buf(mb, signature, 2 * size);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_eddsa_spec(&key);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_ed448_verify_go(struct esec_mb* mb, struct esec_eddsa_key key, const uint8_t* ctx, size_t ctx_len,
                              const uint8_t* message, size_t len, const uint8_t* signature)
{
    uint32_t klen = eddsa_klen(key.spec);
    uint32_t size = eddsa_size(key.spec);

    if(size != ESEC_KEY_ECC_EDDSA_ED448_BYTESIZE)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_EDDSA, 3))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_in_buf(mb, signature, 2 * size);
    esec_mb_push_in_buf(mb, ctx, ctx_len);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_eddsa_spec(&key);
    esec_mb_push_param(ctx_len);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

/* The eSecure implementation only supports NIST P-256 curve */
#define ESEC_JPAKE_ELEM_SIZE (32)

uint32_t esec_ecjpake_generate_r1_go(struct esec_mb* mb, const struct esec_jpake_cfg* cfg, uint8_t* x2, uint8_t* X1,
                                     uint8_t* zkp1, uint8_t* X2, uint8_t* zkp2)
{
    uint32_t kbaselen = ESEC_JPAKE_ELEM_SIZE;

    if(!esec_mb_push_command(mb, ESEC_CMD_JPAKE_ROUND1_GEN, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->user_id1, cfg->idlen1);
    esec_mb_push_out_desc(mb, x2, kbaselen);
    esec_mb_push_out_desc(mb, X1, kbaselen * 2);
    esec_mb_push_out_desc(mb, zkp1, kbaselen * 3);
    esec_mb_push_out_desc(mb, X2, kbaselen * 2);
    esec_mb_push_out_desc(mb, zkp2, kbaselen * 3);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(cfg->idlen1);

    return ESEC_OKAY;
}

uint32_t esec_ecjpake_verify_r1_go(struct esec_mb* mb, const struct esec_jpake_cfg* cfg, const uint8_t* X3,
                                   const uint8_t* zkp3, const uint8_t* X4, const uint8_t* zkp4)
{
    uint32_t kbaselen = ESEC_JPAKE_ELEM_SIZE;

    if(!esec_mb_push_command(mb, ESEC_CMD_JPAKE_ROUND1_VER, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->user_id1, cfg->idlen1);
    esec_mb_push_in_buf(mb, cfg->user_id2, cfg->idlen2);
    esec_mb_push_in_buf(mb, X3, kbaselen * 2);
    esec_mb_push_in_buf(mb, zkp3, kbaselen * 3);
    esec_mb_push_in_buf(mb, X4, kbaselen * 2);
    esec_mb_push_in_buf(mb, zkp4, kbaselen * 3);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(cfg->idlen1);
    esec_mb_push_param(cfg->idlen2);

    return ESEC_OKAY;
}

uint32_t esec_ecjpake_generate_r2_go(struct esec_mb* mb, const struct esec_jpake_cfg* cfg, const uint8_t* pwd,
                                     size_t pwdlen, const uint8_t* x2, const uint8_t* X1, const uint8_t* X3,
                                     const uint8_t* X4, uint8_t* A, uint8_t* zkpA)
{
    uint32_t kbaselen = ESEC_JPAKE_ELEM_SIZE;

    if(!esec_mb_push_command(mb, ESEC_CMD_JPAKE_ROUND2_GEN, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, pwd, pwdlen);
    esec_mb_push_in_buf(mb, cfg->user_id1, cfg->idlen1);
    esec_mb_push_in_buf(mb, x2, kbaselen);
    esec_mb_push_in_buf(mb, X1, kbaselen * 2);
    esec_mb_push_in_buf(mb, X3, kbaselen * 2);
    esec_mb_push_in_buf(mb, X4, kbaselen * 2);
    esec_mb_push_out_desc(mb, A, kbaselen * 2);
    esec_mb_push_out_desc(mb, zkpA, kbaselen * 3);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(pwdlen);
    esec_mb_push_param(cfg->idlen1);

    return ESEC_OKAY;
}

uint32_t esec_ecjpake_verify_r2_go(struct esec_mb* mb, const struct esec_jpake_cfg* cfg, uint8_t* x2, const uint8_t* X1,
                                   const uint8_t* X2, const uint8_t* X3, const uint8_t* B, const uint8_t* zkpB)
{
    UNUSED(x2);
    uint32_t kbaselen = ESEC_JPAKE_ELEM_SIZE;

    if(!esec_mb_push_command(mb, ESEC_CMD_JPAKE_ROUND2_VER, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->user_id2, cfg->idlen2);
    esec_mb_push_in_buf(mb, X1, kbaselen * 2);
    esec_mb_push_in_buf(mb, X2, kbaselen * 2);
    esec_mb_push_in_buf(mb, X3, kbaselen * 2);
    esec_mb_push_in_buf(mb, B, kbaselen * 2);
    esec_mb_push_in_buf(mb, zkpB, kbaselen * 3);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(cfg->idlen2);

    return ESEC_OKAY;
}

uint32_t esec_ecjpake_generate_sessionkey_go(struct esec_mb* mb, const struct esec_jpake_cfg* cfg, const uint8_t* pwd,
                                             size_t pwdlen, const uint8_t* x2, const uint8_t* X4, const uint8_t* B,
                                             uint8_t* session_key)
{
    UNUSED(cfg);
    uint32_t kbaselen = ESEC_JPAKE_ELEM_SIZE;

    if(!esec_mb_push_command(mb, ESEC_CMD_JPAKE_SESSION_KEY(ESEC_HASH_ALGO_SHA256), 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, pwd, pwdlen);
    esec_mb_push_in_buf(mb, x2, kbaselen);
    esec_mb_push_in_buf(mb, X4, kbaselen * 2);
    esec_mb_push_in_buf(mb, B, kbaselen * 2);
    esec_mb_push_out_desc(mb, session_key, ESEC_HASH_SIZE_SHA256);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(pwdlen);
    esec_mb_push_param(ESEC_KEY_SYM(ESEC_HASH_SIZE_SHA256));

    return ESEC_OKAY;
}

uint32_t esec_derive_hkdf_skey_go(struct esec_mb* mb, struct esec_skey ikm, uint32_t hash_alg, const uint8_t* salt,
                                  size_t salt_sz, const uint8_t* info, size_t info_sz, struct esec_key_storage storage,
                                  uint32_t len, struct esec_skey* okm)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DERIVE_KEY_HKDF(hash_alg), 4))
        return ESEC_TX_FIFO_FULL;
    okm->keyspec = len;
    okm->storage = storage;
    esec_mb_push_in_skey(mb, &ikm);
    esec_mb_push_in_buf(mb, salt, salt_sz);
    esec_mb_push_in_buf(mb, info, info_sz);
    esec_mb_push_key_auth_if_any(mb, &okm->storage);
    esec_mb_push_key_material_out(mb, &okm->storage, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&ikm);
    esec_mb_push_param(salt_sz);
    esec_mb_push_param(info_sz);
    esec_mb_push_param_skey(okm);

    return ESEC_OKAY;
}

uint32_t esec_derive_pbkdf2_skey_go(struct esec_mb* mb, struct esec_skey ikm, uint32_t hash_alg, const uint8_t* salt,
                                    size_t salt_sz, uint32_t iterations, struct esec_key_storage storage, uint32_t len,
                                    struct esec_skey* okm)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DERIVE_KEY_PBKDF2(hash_alg), 4))
        return ESEC_TX_FIFO_FULL;
    okm->keyspec = len;
    okm->storage = storage;
    esec_mb_push_in_skey(mb, &ikm);
    esec_mb_push_in_buf(mb, salt, salt_sz);
    esec_mb_push_key_auth_if_any(mb, &okm->storage);
    esec_mb_push_key_material_out(mb, &okm->storage, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&ikm);
    esec_mb_push_param(salt_sz);
    esec_mb_push_param(iterations);
    esec_mb_push_param_skey(okm);

    return ESEC_OKAY;
}

uint32_t esec_derive_kdf2_skey_go(struct esec_mb* mb, struct esec_skey ikm, uint32_t hash_alg, const uint8_t* param,
                                  size_t param_sz, struct esec_key_storage storage, uint32_t len, struct esec_skey* okm)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DERIVE_KEY_KDF2(hash_alg), 3))
        return ESEC_TX_FIFO_FULL;
    okm->keyspec = len;
    okm->storage = storage;
    esec_mb_push_in_skey(mb, &ikm);
    esec_mb_push_in_buf(mb, param, param_sz);
    esec_mb_push_key_auth_if_any(mb, &okm->storage);
    esec_mb_push_key_material_out(mb, &okm->storage, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&ikm);
    esec_mb_push_param(param_sz);
    esec_mb_push_param_skey(okm);

    return ESEC_OKAY;
}

uint32_t esec_derive_nist_skey_go(struct esec_mb* mb, struct esec_skey ikm, uint32_t hash_alg, const uint8_t* param,
                                  size_t param_sz, struct esec_key_storage storage, uint32_t len, struct esec_skey* okm)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DERIVE_KEY_NIST(hash_alg), 3))
        return ESEC_TX_FIFO_FULL;
    okm->keyspec = len;
    okm->storage = storage;
    esec_mb_push_in_skey(mb, &ikm);
    esec_mb_push_in_buf(mb, param, param_sz);
    esec_mb_push_key_auth_if_any(mb, &okm->storage);
    esec_mb_push_key_material_out(mb, &okm->storage, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&ikm);
    esec_mb_push_param(param_sz);
    esec_mb_push_param_skey(okm);

    return ESEC_OKAY;
}

static uint32_t compute_rsa_size(uint32_t keyspec, uint32_t kbaselen)
{
    uint32_t len = 0;

    if(keyspec & ESEC_KEY_DOMAIN)
    {
        len += kbaselen;
    }
    if(keyspec & ESEC_KEY_PUB)
    {
        len += (keyspec & ESEC_KEY_SHORT_EXP) ? 4 : kbaselen;
    }
    if(keyspec & ESEC_KEY_PRIV)
    {
        /* TODO countermeasures */
        len += kbaselen * 2;
        if(keyspec & ESEC_KEY_PRIV_CRT)
        {
            len += kbaselen / 2;
        }
    }
    return len;
}

static uint32_t rsa_key_base_size(uint32_t domainspec)
{
    return (domainspec & 0xFF) * 4 + 4;
}

uint32_t esec_rsa_create_key_go(struct esec_mb* mb, uint32_t domainspec, uint8_t* pub_key,
                                struct esec_key_storage storage, int flags, struct esec_rsa_key* key)
{
    uint32_t kbaselen = rsa_key_base_size(domainspec);
    uint32_t pubklen = (domainspec & ESEC_KEY_SHORT_EXP) ? 4 : kbaselen;

    key->storage = storage;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    key->storage.cfg |= flags & ESEC_KEY_PERMS_SIGN;

    key->keyspec = domainspec;
    key->keyspec |= ESEC_KEY_PRIV | ESEC_KEY_PUB;
    if(!esec_mb_push_command(mb, ESEC_CMD_CREATE_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, pub_key, pubklen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &key->storage, compute_rsa_size(key->keyspec, kbaselen));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key->keyspec | mb_storage_flags(&key->storage));

    return ESEC_OKAY;
}

uint32_t esec_derive_rsa_key_from_seed_go(struct esec_mb* mb, const uint8_t* salt, size_t salt_sz, uint32_t domainspec,
                                          uint8_t* pub_key, struct esec_key_storage storage, int flags,
                                          struct esec_rsa_key* key)
{
    uint32_t kbaselen = rsa_key_base_size(domainspec);
    uint32_t pubklen = (domainspec & ESEC_KEY_SHORT_EXP) ? 4 : kbaselen;

    key->storage = storage;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    key->storage.cfg |= flags & ESEC_KEY_PERMS_SIGN;

    key->keyspec = domainspec;
    key->keyspec |= ESEC_KEY_PRIV | ESEC_KEY_PUB;

    if(!esec_mb_push_command(mb, ESEC_CMD_DERIV_KEY_FROM_SEED, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_buf(mb, salt, salt_sz);
    esec_mb_push_in_buf(mb, pub_key, pubklen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &key->storage, compute_rsa_size(key->keyspec, kbaselen));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(salt_sz);
    esec_mb_push_param(key->keyspec | mb_storage_flags(&key->storage));

    return ESEC_OKAY;
}

uint32_t esec_rsa_import_key_go(struct esec_mb* mb, uint32_t keyspec, const uint8_t* material,
                                struct esec_key_storage storage, int flags, struct esec_rsa_key* key)
{
    uint32_t kbaselen = rsa_key_base_size(keyspec);
    uint32_t klen = compute_rsa_size(keyspec, kbaselen);

    if(!(keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&storage))
    {
        return ESEC_INVALID_PARAMETER;
    }
    if(flags & ~(ESEC_KEY_EXPORTABLE | ESEC_KEY_ONLY_ESECURE | ESEC_KEY_PERMS_SIGN))
        return ESEC_INVALID_PARAMETER;

    key->keyspec = keyspec;
    key->storage = storage;
    if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_LOCKED;

    return esec_import_stored_key_go(mb, key->keyspec, klen, material, &key->storage);
}

uint32_t esec_rsa_export_key_go(struct esec_mb* mb, struct esec_rsa_key key, uint8_t* material)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);
    uint32_t klen = compute_rsa_size(key.keyspec, kbaselen);

    if(!(key.keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&key.storage))
    {
        return ESEC_INVALID_PARAMETER;
    }

    return esec_export_stored_key_go(mb, &key.storage, klen, key.keyspec, material);
}

uint32_t esec_rsa_read_pub_key_go(struct esec_mb* mb, struct esec_rsa_key key, uint8_t* pubkey)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);
    uint32_t klen = compute_rsa_size(key.keyspec, kbaselen);
    uint32_t publen = compute_rsa_size((key.keyspec & ~ESEC_KEY_PRIV) | ESEC_KEY_PUB, kbaselen);

    if(!esec_mb_push_command(mb, ESEC_CMD_READ_PUB_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_out_desc(mb, pubkey, publen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));

    return ESEC_OKAY;
}

uint32_t esec_rsa_transfer_key_go(struct esec_mb* mb, struct esec_rsa_key src, struct esec_key_storage storage,
                                  uint32_t busprotbits, struct esec_rsa_key* dst)
{
    if(src.storage.cfg >> KEY_MODE_OFFSET != storage.cfg >> KEY_MODE_OFFSET)
        return ESEC_INVALID_PARAMETER;
    size_t kbaselen = rsa_key_base_size(src.keyspec);
    size_t klen = compute_rsa_size(src.keyspec, kbaselen);

    dst->keyspec = src.keyspec;
    dst->storage = storage;

    return esec_transfer_key_storage_go(mb, &src.storage, src.keyspec, klen, busprotbits, &dst->storage);
}

uint32_t esec_rsa_delete_key_go(struct esec_mb* mb, struct esec_rsa_key key)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);
    uint32_t klen = compute_rsa_size(key.keyspec, kbaselen);

    return esec_delete_stored_key_go(mb, &key.storage, klen, key.keyspec);
}

static void esec_mb_push_in_rsakey(struct esec_mb* mb, struct esec_rsa_key* key)
{
    uint32_t kbaselen = rsa_key_base_size(key->keyspec);
    uint32_t klen;

    klen = compute_rsa_size(key->keyspec, kbaselen);
    esec_mb_push_key_material(mb, &key->storage, klen);
}

uint32_t esec_rsa_sign_go(struct esec_mb* mb, uint8_t padding, uint8_t hash_alg, struct esec_rsa_key key,
                          const uint8_t* message, size_t len, uint8_t* signature, size_t salt_len)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);

    if((key.keyspec & ESEC_KEY_PRIV) != ESEC_KEY_PRIV)
        return ESEC_INVALID_KEY;

    bool ok;
    if(padding == ESEC_RSA_PADDING_PSS)
        ok = esec_mb_push_command(mb, ESEC_CMD_SIGN(hash_alg) | padding, 3);
    else
        ok = esec_mb_push_command(mb, ESEC_CMD_SIGN(hash_alg) | padding, 2);
    if(!ok)
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_rsakey(mb, &key);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, signature, kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);
    if(padding == ESEC_RSA_PADDING_PSS)
        esec_mb_push_param(salt_len);

    return ESEC_OKAY;
}

uint32_t esec_rsa_verify_go(struct esec_mb* mb, uint8_t padding, uint8_t hash_alg, struct esec_rsa_key key,
                            const uint8_t* message, size_t len, const uint8_t* signature, size_t salt_len)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);

    if((key.keyspec & ESEC_KEY_PUB) != ESEC_KEY_PUB)
        return ESEC_INVALID_KEY;

    bool ok;
    if(padding == ESEC_RSA_PADDING_PSS)
        ok = esec_mb_push_command(mb, ESEC_CMD_VERIFY(hash_alg) | padding, 3);
    else
        ok = esec_mb_push_command(mb, ESEC_CMD_VERIFY(hash_alg) | padding, 2);
    if(!ok)
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_rsakey(mb, &key);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, signature, kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);
    if(padding == ESEC_RSA_PADDING_PSS)
        esec_mb_push_param(salt_len);

    return ESEC_OKAY;
}

uint32_t esec_rsa_sign_digest_go(struct esec_mb* mb, struct esec_rsa_key key, const uint8_t* digest, size_t len,
                                 uint8_t* signature)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);

    if((key.keyspec & ESEC_KEY_PRIV) != ESEC_KEY_PRIV)
        return ESEC_INVALID_KEY;

    bool ok;
    ok = esec_mb_push_command(mb, ESEC_CMD_SIGN_DIGEST, 2);
    if(!ok)
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_rsakey(mb, &key);
    esec_mb_push_in_buf(mb, digest, len);
    esec_mb_push_out_desc(mb, signature, kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_rsa_verify_digest_go(struct esec_mb* mb, struct esec_rsa_key key, const uint8_t* digest, size_t len,
                                   const uint8_t* signature)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);

    if((key.keyspec & ESEC_KEY_PUB) != ESEC_KEY_PUB)
        return ESEC_INVALID_KEY;

    bool ok;
    ok = esec_mb_push_command(mb, ESEC_CMD_VERIFY_DIGEST, 2);
    if(!ok)
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_rsakey(mb, &key);
    esec_mb_push_in_buf(mb, digest, len);
    esec_mb_push_in_buf(mb, signature, kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_rsa_encrypt_go(struct esec_mb* mb, uint8_t padding, struct esec_rsa_key key, const uint8_t* plaintext,
                             size_t len, uint8_t* ciphertext, uint8_t hash_algo)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);

    if(!(key.keyspec & ESEC_KEY_PUB))
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, (ESEC_CMD_RSA_ENCRYPT | hash_algo << 8) | padding, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_rsakey(mb, &key);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_rsa_decrypt_go(struct esec_mb* mb, uint8_t padding, struct esec_rsa_key key, const uint8_t* ciphertext,
                             uint8_t* plaintext, uint32_t* decrypted_len, uint8_t hash_algo)
{
    uint32_t kbaselen = rsa_key_base_size(key.keyspec);

    if(!(key.keyspec & ESEC_KEY_PRIV))
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, (ESEC_CMD_RSA_DECRYPT | hash_algo << 8) | padding, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_rsakey(mb, &key);
    esec_mb_push_in_buf(mb, ciphertext, kbaselen);
    esec_mb_push_out_desc(mb, plaintext, kbaselen);
    esec_mb_push_out_desc(mb, (uint8_t*)decrypted_len, sizeof(uint32_t));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));

    return ESEC_OKAY;
}

uint32_t esec_rsa_key_wrap_go(struct esec_mb* mb, struct esec_skey target_key, struct esec_rsa_key wrapping_key,
                              uint32_t tmp_aes_key_len, uint8_t* output, uint32_t out_size)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_KEYWRAP_RSA, 4))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &target_key);
    esec_mb_push_in_rsakey(mb, &wrapping_key);
    esec_mb_push_out_desc(mb, output, out_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&target_key);
    esec_mb_push_param(wrapping_key.keyspec | mb_storage_flags(&wrapping_key.storage));
    esec_mb_push_param(tmp_aes_key_len);
    esec_mb_push_param(out_size);

    return ESEC_OKAY;
}

uint32_t esec_rsa_key_unwrap_go(struct esec_mb* mb, struct esec_rsa_key wrapping_key, uint8_t* wrapped_key,
                                uint32_t wrapped_key_len, uint32_t flags, uint16_t klen,
                                struct esec_key_storage storage, struct esec_skey* target_key)
{
    if((flags & ESEC_KEY_EXPORTABLE) || (flags & ESEC_KEY_ONLY_ESECURE))
        return ESEC_INVALID_PARAMETER;

    target_key->keyspec = klen;
    storage.cfg &= ~ESEC_RESTR_RESTRICTED;
    storage.cfg |= ESEC_RESTR_LOCKED;
    target_key->storage = storage;

    if(!esec_mb_push_command(mb, ESEC_CMD_KEYUNWRAP_RSA, 3))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_rsakey(mb, &wrapping_key);
    esec_mb_push_in_buf(mb, wrapped_key, wrapped_key_len);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &storage, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(wrapping_key.keyspec | mb_storage_flags(&wrapping_key.storage));
    esec_mb_push_param(wrapped_key_len);
    esec_mb_push_param(klen | mb_storage_flags(&storage));

    return ESEC_OKAY;
}

uint32_t esec_ecdh_key_wrap_go(struct esec_mb* mb, struct esec_skey target_key, struct esec_ecc_key wrapping_key,
                               uint8_t* shared_data, uint32_t shared_data_len, uint32_t tmp_aes_key_len,
                               uint8_t* output, uint32_t out_size)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_KEYWRAP_ECDH, 5))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &target_key);
    esec_mb_push_in_ecckey(mb, &wrapping_key);
    esec_mb_push_in_buf(mb, shared_data, shared_data_len);
    esec_mb_push_out_desc(mb, output, out_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&target_key);
    esec_mb_push_param(wrapping_key.keyspec | mb_storage_flags(&wrapping_key.storage));
    esec_mb_push_param(shared_data_len);
    esec_mb_push_param(tmp_aes_key_len);
    esec_mb_push_param(out_size);

    return ESEC_OKAY;
}

uint32_t esec_ecdh_key_unwrap_go(struct esec_mb* mb, struct esec_ecc_key wrapping_key, uint8_t* shared_data,
                                 uint32_t shared_data_len, uint32_t tmp_aes_key_len, uint8_t* wrapped_key,
                                 uint32_t wrapped_key_len, uint32_t flags, uint16_t klen,
                                 struct esec_key_storage storage, struct esec_skey* target_key)
{
    if((flags & ESEC_KEY_EXPORTABLE) || (flags & ESEC_KEY_ONLY_ESECURE))
        return ESEC_INVALID_PARAMETER;

    target_key->keyspec = klen;
    storage.cfg &= ~ESEC_RESTR_RESTRICTED;
    storage.cfg |= ESEC_RESTR_LOCKED;
    target_key->storage = storage;

    if(!esec_mb_push_command(mb, ESEC_CMD_KEYUNWRAP_ECDH, 5))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_ecckey(mb, &wrapping_key);
    esec_mb_push_in_buf(mb, wrapped_key, wrapped_key_len);
    esec_mb_push_in_buf(mb, shared_data, shared_data_len);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &storage, klen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(wrapping_key.keyspec | mb_storage_flags(&wrapping_key.storage));
    esec_mb_push_param(wrapped_key_len);
    esec_mb_push_param(shared_data_len);
    esec_mb_push_param(tmp_aes_key_len);
    esec_mb_push_param(klen | mb_storage_flags(&storage));

    return ESEC_OKAY;
}

static uint32_t dsa_qsize(uint32_t domainspec)
{
    /* bits [10:7], add 1 and multiply by 4. This multiplication can be
     * simplified: ">> 7 << 2" into ">> 5". */
    return ((domainspec & 0x780) >> 5) + 4;
}

static uint32_t dsa_psize(uint32_t domainspec)
{
    return ((domainspec & 0x7F) + 1) << 2;
}

static uint32_t compute_dsa_size(uint32_t keyspec)
{
    uint32_t len = 0;
    uint32_t psize = dsa_psize(keyspec);
    uint32_t qsize = dsa_qsize(keyspec);

    /* domain is always included */
    len += psize + qsize;
    if(keyspec & ESEC_KEY_SHORT_GENERATOR)
    {
        len += 4;
    }
    else
    {
        len += psize;
    }
    if(keyspec & ESEC_KEY_PUB)
    {
        len += psize;
    }
    if(keyspec & ESEC_KEY_PRIV)
    {
        len += qsize;
    }

    return len;
}

uint32_t esec_dsa_create_key_go(struct esec_mb* mb, uint32_t domainspec, uint8_t* domain,
                                struct esec_key_storage storage, int flags, struct esec_dsa_key* key)
{
    uint32_t domainlen = compute_dsa_size(domainspec);

    key->storage = storage;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    key->storage.cfg |= flags & ESEC_KEY_PERMS_SIGN;

    key->keyspec = domainspec;
    key->keyspec |= ESEC_KEY_PRIV | ESEC_KEY_PUB;
    if(!esec_mb_push_command(mb, ESEC_CMD_CREATE_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_domain(mb, domain, domainlen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &key->storage, compute_dsa_size(key->keyspec));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key->keyspec | mb_storage_flags(&key->storage));

    return ESEC_OKAY;
}

uint32_t esec_dsa_import_key_go(struct esec_mb* mb, uint32_t keyspec, const uint8_t* material,
                                struct esec_key_storage storage, int flags, struct esec_dsa_key* key)
{
    uint32_t klen = compute_dsa_size(keyspec);

    if(!(keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&storage))
    {
        return ESEC_INVALID_PARAMETER;
    }
    if(flags & ~(ESEC_KEY_EXPORTABLE | ESEC_KEY_ONLY_ESECURE))
        return ESEC_INVALID_PARAMETER;

    key->keyspec = keyspec;
    key->storage = storage;
    if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_LOCKED;

    return esec_import_stored_key_go(mb, key->keyspec, klen, material, &key->storage);
}

uint32_t esec_dsa_export_key_go(struct esec_mb* mb, struct esec_dsa_key key, uint8_t* material)
{
    uint32_t klen = compute_dsa_size(key.keyspec);

    if(!(key.keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&key.storage))
    {
        return ESEC_INVALID_PARAMETER;
    }

    return esec_export_stored_key_go(mb, &key.storage, klen, key.keyspec, material);
}

uint32_t esec_dsa_read_pub_key_go(struct esec_mb* mb, struct esec_dsa_key key, uint8_t* pubkey)
{
    uint32_t klen = compute_dsa_size(key.keyspec);
    uint32_t publen = compute_dsa_size((key.keyspec & ~ESEC_KEY_PRIV) | ESEC_KEY_PUB);

    if(!esec_mb_push_command(mb, ESEC_CMD_READ_PUB_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_out_desc(mb, pubkey, publen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));

    return ESEC_OKAY;
}

uint32_t esec_dsa_transfer_key_go(struct esec_mb* mb, struct esec_dsa_key src, struct esec_key_storage storage,
                                  uint32_t busprotbits, struct esec_dsa_key* dst)
{
    if(src.storage.cfg >> KEY_MODE_OFFSET != storage.cfg >> KEY_MODE_OFFSET)
        return ESEC_INVALID_PARAMETER;
    size_t klen = compute_dsa_size(src.keyspec);

    dst->keyspec = src.keyspec;
    dst->storage = storage;

    return esec_transfer_key_storage_go(mb, &src.storage, src.keyspec, klen, busprotbits, &dst->storage);
}

uint32_t esec_dsa_delete_key_go(struct esec_mb* mb, struct esec_dsa_key key)
{
    uint32_t klen = compute_dsa_size(key.keyspec);

    return esec_delete_stored_key_go(mb, &key.storage, klen, key.keyspec);
}

static void esec_mb_push_in_dsakey(struct esec_mb* mb, struct esec_dsa_key* key)
{
    uint32_t klen;

    klen = compute_dsa_size(key->keyspec);
    esec_mb_push_key_material(mb, &key->storage, klen);
}

uint32_t esec_dsa_sign_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_dsa_key key, const uint8_t* message,
                          size_t len, uint8_t* signature)
{
    uint32_t qsize = dsa_qsize(key.keyspec);

    if((key.keyspec & ESEC_KEY_PRIV) != ESEC_KEY_PRIV)
        return ESEC_INVALID_KEY;
    if((key.keyspec & ESEC_CMD_CHECK) != ESEC_KEY_TYPE_DSA)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_SIGN(hash_alg), 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_dsakey(mb, &key);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, signature, 2 * qsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_dsa_verify_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_dsa_key key,
                            const uint8_t* message, size_t len, const uint8_t* signature)
{
    uint32_t qsize = dsa_qsize(key.keyspec);

    if((key.keyspec & ESEC_KEY_PUB) != ESEC_KEY_PUB)
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY(hash_alg), 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_dsakey(mb, &key);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, signature, 2 * qsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_dsa_sign_digest_go(struct esec_mb* mb, struct esec_dsa_key key, const uint8_t* digest, size_t len,
                                 uint8_t* signature)
{
    uint32_t qsize = dsa_qsize(key.keyspec);

    if((key.keyspec & ESEC_KEY_PRIV) != ESEC_KEY_PRIV)
        return ESEC_INVALID_KEY;
    if((key.keyspec & ESEC_CMD_CHECK) != ESEC_KEY_TYPE_DSA)
        return ESEC_INVALID_PARAMETER;
    if(!esec_mb_push_command(mb, ESEC_CMD_SIGN_DIGEST, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_dsakey(mb, &key);
    esec_mb_push_in_buf(mb, digest, len);
    esec_mb_push_out_desc(mb, signature, 2 * qsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_dsa_verify_digest_go(struct esec_mb* mb, struct esec_dsa_key key, const uint8_t* digest, size_t len,
                                   const uint8_t* signature)
{
    uint32_t qsize = dsa_qsize(key.keyspec);

    if((key.keyspec & ESEC_KEY_PUB) != ESEC_KEY_PUB)
        return ESEC_INVALID_KEY;
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_DIGEST, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_dsakey(mb, &key);
    esec_mb_push_in_buf(mb, digest, len);
    esec_mb_push_in_buf(mb, signature, 2 * qsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

static uint32_t compute_dh_kbaselen(uint32_t keyspec)
{
    return ((keyspec & 0x7F) << 2) + 4;
}

static uint32_t compute_dh_size(uint32_t keyspec)
{
    uint32_t kbaselen = compute_dh_kbaselen(keyspec);
    uint32_t len = kbaselen;

    len += (keyspec & ESEC_KEY_SHORT_GENERATOR) ? 4 : kbaselen;
    if(keyspec & ESEC_KEY_PRIV)
    {
        len += kbaselen;
    }
    if(keyspec & ESEC_KEY_PUB)
    {
        len += kbaselen;
    }

    return len;
}

uint32_t esec_dh_create_key_go(struct esec_mb* mb, uint32_t domainspec, uint8_t* domain,
                               struct esec_key_storage storage, int flags, struct esec_dh_key* key)
{
    uint32_t domainlen = compute_dh_size(domainspec);

    key->storage = storage;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        key->storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_INTERNAL;
    key->storage.cfg |= flags & ESEC_KEY_PERMS_SIGN;

    key->keyspec = domainspec;
    key->keyspec |= ESEC_KEY_PRIV | ESEC_KEY_PUB;
    if(!esec_mb_push_command(mb, ESEC_CMD_CREATE_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_domain(mb, domain, domainlen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &key->storage, compute_dh_size(key->keyspec));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key->keyspec | mb_storage_flags(&key->storage));

    return ESEC_OKAY;
}

uint32_t esec_dh_import_key_go(struct esec_mb* mb, uint32_t keyspec, const uint8_t* material,
                               struct esec_key_storage storage, int flags, struct esec_dh_key* key)
{
    uint32_t klen = compute_dh_size(keyspec);

    if(!(keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&storage))
    {
        return ESEC_INVALID_PARAMETER;
    }
    if(flags & ~(ESEC_KEY_EXPORTABLE | ESEC_KEY_ONLY_ESECURE))
        return ESEC_INVALID_PARAMETER;

    key->keyspec = keyspec;
    key->storage = storage;
    if(!(flags & ESEC_KEY_EXPORTABLE))
        key->storage.cfg |= ESEC_RESTR_LOCKED;

    return esec_import_stored_key_go(mb, key->keyspec, klen, material, &key->storage);
}

uint32_t esec_dh_export_key_go(struct esec_mb* mb, struct esec_dh_key key, uint8_t* material)
{
    uint32_t klen = compute_dh_size(key.keyspec);

    if(!(key.keyspec & ESEC_KEY_PAIR))
    {
        return ESEC_INVALID_KEY;
    }
    if(!esec_is_protected(&key.storage))
    {
        return ESEC_INVALID_PARAMETER;
    }

    return esec_export_stored_key_go(mb, &key.storage, klen, key.keyspec, material);
}

uint32_t esec_dh_read_pub_key_go(struct esec_mb* mb, struct esec_dh_key key, uint8_t* pubkey)
{
    uint32_t klen = compute_dh_size(key.keyspec);
    uint32_t publen = compute_dh_size((key.keyspec & ~ESEC_KEY_PRIV) | ESEC_KEY_PUB);

    if(!esec_mb_push_command(mb, ESEC_CMD_READ_PUB_KEY, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_out_desc(mb, pubkey, publen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));

    return ESEC_OKAY;
}

uint32_t esec_dh_transfer_key_go(struct esec_mb* mb, struct esec_dh_key src, struct esec_key_storage storage,
                                 uint32_t busprotbits, struct esec_dh_key* dst)
{
    if(src.storage.cfg >> KEY_MODE_OFFSET != storage.cfg >> KEY_MODE_OFFSET)
        return ESEC_INVALID_PARAMETER;
    size_t klen = compute_dh_size(src.keyspec);

    dst->keyspec = src.keyspec;
    dst->storage = storage;

    return esec_transfer_key_storage_go(mb, &src.storage, src.keyspec, klen, busprotbits, &dst->storage);
}

uint32_t esec_dh_delete_key_go(struct esec_mb* mb, struct esec_dh_key key)
{
    uint32_t klen = compute_dh_size(key.keyspec);

    return esec_delete_stored_key_go(mb, &key.storage, klen, key.keyspec);
}

uint32_t esec_dh_genkey_go(struct esec_mb* mb, struct esec_dh_key key, uint8_t* remote_pubkey,
                           struct esec_key_storage storage, int flags, struct esec_skey* sharedkey)
{
    UNUSED(flags);
    uint32_t kbaselen = compute_dh_kbaselen(key.keyspec);
    uint32_t pubklen = kbaselen;
    uint32_t klen = compute_dh_size(key.keyspec);

    if(!(key.keyspec & ESEC_KEY_PRIV))
        return ESEC_INVALID_KEY;

    sharedkey->keyspec = pubklen;
    sharedkey->storage = storage;
    if(!esec_mb_push_command(mb, ESEC_CMD_DIFFIE_HELLMAN, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_key_material(mb, &key.storage, klen);
    esec_mb_push_in_buf(mb, remote_pubkey, pubklen);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &sharedkey->storage, sharedkey->keyspec);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param_skey(sharedkey);

    return ESEC_OKAY;
}

static bool is_valid_srp_config(struct esec_srp_config* cfg)
{
    if(!cfg || !cfg->modulus || !cfg->generator)
        return false;

    return cfg->hash_algo >= ESEC_HASH_ALGO_SHA1 && cfg->hash_algo <= ESEC_HASH_ALGO_SHA512 &&
           cfg->num_size <= ESEC_SRP_MAX_NUM_SIZE && cfg->version <= ESEC_SRP_VERSION_SRP6A;
}

static uint8_t srp_credentials_seperator[1] = {':'};

uint32_t esec_srp_gen_verifier_go(struct esec_mb* mb, struct esec_srp_config* cfg,
                                  struct esec_srp_user_credentials* creds, uint8_t* salt, size_t salt_len,
                                  uint8_t* verifier)
{
    if(!is_valid_srp_config(cfg))
        return ESEC_INVALID_PARAMETER;

    if(!esec_mb_push_command(mb, ESEC_CMD_SRP_GEN_VERIFIER(cfg->hash_algo, cfg->version), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->modulus, cfg->num_size);
    esec_mb_push_in_buf(mb, cfg->generator, cfg->num_size);
    esec_mb_push_in_chunk(mb, creds->username, creds->username_len);
    esec_mb_push_in_chunk(mb, srp_credentials_seperator, 1);
    esec_mb_push_in_buf(mb, creds->password, creds->password_len);
    esec_mb_push_in_buf(mb, salt, salt_len);
    esec_mb_push_out_desc(mb, verifier, cfg->num_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(ESEC_KEY_DH(cfg->num_size));
    esec_mb_push_param(creds->username_len + creds->password_len + 1);
    esec_mb_push_param(salt_len);

    return ESEC_OKAY;
}

uint32_t esec_srp_user_gen_public_go(struct esec_mb* mb, struct esec_srp_config* cfg,
                                     struct esec_srp_user_session_state* session)
{
    if(!is_valid_srp_config(cfg))
        return ESEC_INVALID_PARAMETER;

    if(!esec_mb_push_command(mb, ESEC_CMD_SRP_USER_GEN_PUB(cfg->hash_algo, cfg->version), 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->modulus, cfg->num_size);
    esec_mb_push_in_buf(mb, cfg->generator, cfg->num_size);
    esec_mb_push_out_desc(mb, session->a, cfg->num_size);
    esec_mb_push_out_desc(mb, session->A, cfg->num_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(ESEC_KEY_DH(cfg->num_size));

    return ESEC_OKAY;
}

uint32_t esec_srp_server_gen_public_go(struct esec_mb* mb, struct esec_srp_config* cfg, uint8_t* verifier,
                                       struct esec_srp_server_session_state* session)
{
    if(!is_valid_srp_config(cfg))
        return ESEC_INVALID_PARAMETER;

    if(!esec_mb_push_command(mb, ESEC_CMD_SRP_HOST_GEN_PUB(cfg->hash_algo, cfg->version), 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->modulus, cfg->num_size);
    esec_mb_push_in_buf(mb, cfg->generator, cfg->num_size);
    esec_mb_push_in_buf(mb, verifier, cfg->num_size);
    esec_mb_push_out_desc(mb, session->b, cfg->num_size);
    esec_mb_push_out_desc(mb, session->B, cfg->num_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(ESEC_KEY_DH(cfg->num_size));

    return ESEC_OKAY;
}

uint32_t esec_srp_user_gen_key_go(struct esec_mb* mb, struct esec_srp_config* cfg,
                                  struct esec_srp_user_credentials* creds, uint8_t* B, uint8_t* salt, size_t salt_len,
                                  struct esec_srp_user_session_state* session, uint8_t* sharedkey)
{
    if(!is_valid_srp_config(cfg))
        return ESEC_INVALID_PARAMETER;

    if(!esec_mb_push_command(mb, ESEC_CMD_SRP_USER_GEN_KEY(cfg->hash_algo, cfg->version), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->modulus, cfg->num_size);
    esec_mb_push_in_buf(mb, cfg->generator, cfg->num_size);
    esec_mb_push_in_buf(mb, session->A, cfg->num_size);
    esec_mb_push_in_buf(mb, B, cfg->num_size);
    esec_mb_push_in_chunk(mb, creds->username, creds->username_len);
    esec_mb_push_in_chunk(mb, srp_credentials_seperator, 1);
    esec_mb_push_in_buf(mb, creds->password, creds->password_len);
    esec_mb_push_in_buf(mb, salt, salt_len);
    esec_mb_push_in_buf(mb, session->a, cfg->num_size);
    esec_mb_push_out_desc(mb, sharedkey, hash_digest_len(cfg->hash_algo));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(ESEC_KEY_DH(cfg->num_size));
    esec_mb_push_param(creds->username_len + creds->password_len + 1);
    esec_mb_push_param(salt_len);

    return ESEC_OKAY;
}

uint32_t esec_srp_server_gen_key_go(struct esec_mb* mb, struct esec_srp_config* cfg, uint8_t* A, uint8_t* verifier,
                                    struct esec_srp_server_session_state* session, uint8_t* sharedkey)
{
    if(!is_valid_srp_config(cfg))
        return ESEC_INVALID_PARAMETER;

    if(!esec_mb_push_command(mb, ESEC_CMD_SRP_HOST_GEN_KEY(cfg->hash_algo, cfg->version), 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, cfg->modulus, cfg->num_size);
    esec_mb_push_in_buf(mb, cfg->generator, cfg->num_size);
    esec_mb_push_in_buf(mb, A, cfg->num_size);
    esec_mb_push_in_buf(mb, session->B, cfg->num_size);
    esec_mb_push_in_buf(mb, verifier, cfg->num_size);
    esec_mb_push_in_buf(mb, session->b, cfg->num_size);
    esec_mb_push_out_desc(mb, sharedkey, hash_digest_len(cfg->hash_algo));
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(ESEC_KEY_DH(cfg->num_size));

    return ESEC_OKAY;
}

uint32_t esec_get_random_bytes_go(struct esec_mb* mb, uint8_t* bytes, size_t len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GET_RANDOM, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_out_desc(mb, bytes, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_generate_prime_go(struct esec_mb* mb, uint8_t* prime, uint32_t bits, uint32_t rounds)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GEN_PRIME, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_out_desc(mb, prime, ((bits + 31) >> 3) & ~3);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(bits);
    esec_mb_push_param(rounds);

    return ESEC_OKAY;
}

static void esec_mb_push_certifier_material(struct esec_mb* mb, esec_certifier_key* certifier)
{
#if ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ED25519
    uint32_t klen = (certifier->spec & ESEC_KEY_PUB) ? 64 : 32;
#elif(ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ECDSA_P256) || (ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ECDSA_P384) ||                 \
    (ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ECDSA_P521)
    uint32_t kbaselen = key_base_size(certifier->keyspec);
    uint32_t klen = compute_ecc_size(certifier->keyspec, kbaselen);
#elif(ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_RSA_4K)
    uint32_t kbaselen = rsa_key_base_size(certifier->keyspec);
    uint32_t klen = compute_rsa_size(certifier->keyspec, kbaselen);
#endif
    esec_mb_push_key_material(mb, &certifier->storage, klen);
}

static void esec_mb_push_certifier_spec(esec_certifier_key* certifier)
{
#if ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ED25519
    esec_mb_push_eddsa_spec(certifier);
#elif(ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ECDSA_P256) || (ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ECDSA_P384) ||                 \
    (ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_ECDSA_P521) || (ESEC_AUTH_ALGO == ESEC_AUTH_ALGO_RSA_4K)
    esec_mb_push_param(certifier->keyspec | mb_storage_flags(&certifier->storage));
#endif
}

uint32_t esec_certify_eddsa_key_go(struct esec_mb* mb, esec_certifier_key signer, const uint8_t nonce[12],
                                   struct esec_eddsa_key target_key, struct esec_cert_ed25519_key* cert)
{
    uint32_t klen = eddsa_klen(target_key.spec);

    if(!esec_mb_push_command(mb, ESEC_CMD_CERTIFY_KEY, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_key_material(mb, &target_key.storage, klen);
    esec_mb_push_certifier_material(mb, &signer);
    esec_mb_push_out_desc(mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_finish_outputs(mb);
    esec_mb_push_certifier_spec(&signer);
    esec_mb_push_eddsa_spec(&target_key);

    return ESEC_OKAY;
}

uint32_t esec_certify_ecc_key_go(struct esec_mb* mb, esec_certifier_key signer, const uint8_t nonce[12],
                                 struct esec_ecc_key target_key, uint8_t* cert)
{
    uint32_t kbaselen = key_base_size(target_key.keyspec);
    uint32_t klen = compute_ecc_size(target_key.keyspec, kbaselen);
    uint32_t kpublen = compute_ecc_size((target_key.keyspec & ~ESEC_KEY_PRIV) | ESEC_KEY_PUB, kbaselen);
    uint32_t cert_size = ESEC_CERT_KEY_BASE_SIZE + kpublen;

    if(!esec_mb_push_command(mb, ESEC_CMD_CERTIFY_KEY, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_key_material(mb, &target_key.storage, klen);
    esec_mb_push_certifier_material(mb, &signer);
    esec_mb_push_out_desc(mb, (uint8_t*)cert, cert_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_certifier_spec(&signer);
    esec_mb_push_param(target_key.keyspec | mb_storage_flags(&target_key.storage));

    return ESEC_OKAY;
}

uint32_t esec_certify_time_go(struct esec_mb* mb, esec_certifier_key signer, const uint8_t nonce[12],
                              struct esec_time_certificate* cert)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_CERTIFY_TIME, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_buf(mb, nonce, 12);
    esec_mb_push_certifier_material(mb, &signer);
    esec_mb_push_out_desc(mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_finish_outputs(mb);
    esec_mb_push_certifier_spec(&signer);

    return ESEC_OKAY;
}

uint32_t esec_read_chip_certificate_go(struct esec_mb* mb, struct esec_chip_certificate* cert)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_READ_PUB_EK, 0))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_out_desc(mb, (uint8_t*)cert, sizeof(*cert));
    esec_mb_finish_outputs(mb);

    return ESEC_OKAY;
}

uint32_t esec_get_attestation_token_go(struct esec_mb* mb, uint8_t* challenge, uint32_t challenge_size, uint8_t* token,
                                       uint32_t token_size, uint32_t* token_len, int32_t* attest_status)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GET_ATTESTATION_TOKEN, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_buf(mb, challenge, challenge_size);
    esec_mb_push_out_desc(mb, (uint8_t*)attest_status, sizeof(int32_t));
    esec_mb_push_out_desc(mb, (uint8_t*)token, token_size);
    esec_mb_push_out_desc(mb, (uint8_t*)token_len, sizeof(uint32_t));
    esec_mb_finish_outputs(mb);

    esec_mb_push_param(challenge_size);
    esec_mb_push_param(token_size);

    return ESEC_OKAY;
}

uint32_t esec_get_attestation_token_len_go(struct esec_mb* mb, uint32_t challenge_size, uint32_t* token_size,
                                           int32_t* attest_status)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GET_ATTESTATION_TOKEN_LEN, 1))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_out_desc(mb, (uint8_t*)attest_status, sizeof(int32_t));
    esec_mb_push_out_desc(mb, (uint8_t*)token_size, sizeof(uint32_t));
    esec_mb_finish_outputs(mb);

    esec_mb_push_param(challenge_size);

    return ESEC_OKAY;
}

uint32_t esec_sm2_sign_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len,
                          const uint8_t* message, size_t message_len, uint8_t* signature)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_SM2_SIGN, 3))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_in_buf(mb, id_signer, id_signer_len);
    esec_mb_push_in_buf(mb, message, message_len);
    esec_mb_push_out_desc(mb, signature, 2 * kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(message_len);
    esec_mb_push_param(id_signer_len);

    return ESEC_OKAY;
}

uint32_t esec_sm2_verify_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len,
                            const uint8_t* message, size_t message_len, uint8_t* signature)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_SM2_VERIFY, 3))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_in_buf(mb, id_signer, id_signer_len);
    esec_mb_push_in_buf(mb, message, message_len);
    esec_mb_push_in_buf(mb, signature, 2 * kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));
    esec_mb_push_param(message_len);
    esec_mb_push_param(id_signer_len);

    return ESEC_OKAY;
}

uint32_t esec_sm2_key_exch_rand_go(struct esec_mb* mb, struct esec_ecc_key key, uint8_t* random_number,
                                   uint8_t* random_point)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_SM2_KEY_EXCH_RAND, 1))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key.keyspec);
    esec_mb_push_in_ecckey(mb, &key);
    esec_mb_push_out_desc(mb, random_number, kbaselen);
    esec_mb_push_out_desc(mb, random_point, 2 * kbaselen);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key.keyspec | mb_storage_flags(&key.storage));

    return ESEC_OKAY;
}

uint32_t esec_sm2_key_exch_gen_go(struct esec_mb* mb, struct esec_ecc_key key_private, const uint8_t* key_public_rcv,
                                  const uint8_t* cofactor, uint8_t* random_point_rcv, uint8_t* random_point_cal,
                                  uint8_t* random_number_cal, struct esec_key_storage storage, int flags,
                                  struct esec_skey* sharedkey)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_SM2_KEY_EXCH_GEN, 2))
        return ESEC_TX_FIFO_FULL;

    uint32_t kbaselen = key_base_size(key_private.keyspec);
    uint32_t pubklen =
        compute_ecc_size((key_private.keyspec & ~(ESEC_KEY_PRIV | ESEC_KEY_DOMAIN)) | ESEC_KEY_PUB, kbaselen);

    if(flags & ~ESEC_KEY_EXPORTABLE)
        return ESEC_INVALID_PARAMETER;

    sharedkey->keyspec = pubklen;
    sharedkey->storage = storage;

    esec_mb_push_in_ecckey(mb, &key_private);
    esec_mb_push_in_buf(mb, key_public_rcv, 2 * kbaselen);
    esec_mb_push_in_buf(mb, cofactor, kbaselen);
    esec_mb_push_in_buf(mb, random_point_rcv, 2 * kbaselen);
    esec_mb_push_in_buf(mb, random_point_cal, 2 * kbaselen);
    esec_mb_push_in_buf(mb, random_number_cal, kbaselen);
    esec_mb_push_key_material_out(mb, &sharedkey->storage, sharedkey->keyspec);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param(key_private.keyspec | mb_storage_flags(&key_private.storage));
    esec_mb_push_param_skey(sharedkey);

    return ESEC_OKAY;
}

static uint32_t esec_crypt_sm4_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* data_in,
                                  const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* data_out, size_t len, uint32_t cmd)
{
    if(!esec_mb_push_command(mb, cmd, 2))
        return ESEC_TX_FIFO_FULL;

    bool has_iv = (cmd != ESEC_CMD_ENCRYPT_SM4_ECB) && (cmd != ESEC_CMD_DECRYPT_SM4_ECB);

    esec_mb_push_in_skey(mb, &skey);
    if(has_iv)
        esec_mb_push_in_buf(mb, iv, 16);

    esec_mb_push_in_buf(mb, data_in, len);
    esec_mb_push_out_desc(mb, data_out, len);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_sm4_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 uint8_t* ciphertext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, plaintext, NULL, ciphertext, len, ESEC_CMD_ENCRYPT_SM4_ECB);
}

uint32_t esec_sm4_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 uint8_t* plaintext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, ciphertext, NULL, plaintext, len, ESEC_CMD_DECRYPT_SM4_ECB);
}

uint32_t esec_sm4_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_SM4_CBC);
}

uint32_t esec_sm4_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_SM4_CBC);
}

uint32_t esec_sm4_ctr_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_SM4_CTR);
}

uint32_t esec_sm4_ctr_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_SM4_CTR);
}

uint32_t esec_sm4_cfb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_SM4_CFB);
}

uint32_t esec_sm4_cfb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_SM4_CFB);
}

uint32_t esec_sm4_ofb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_SM4_OFB);
}

uint32_t esec_sm4_ofb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_sm4_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_SM4_OFB);
}

uint32_t esec_sm4_gcm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                 const uint8_t* header, size_t headerlen, uint8_t* tag, size_t tag_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_SM4_GCM | (((16 - tag_len) & 0xF) << 8), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, ESEC_SM4_GCM_IV_SIZE);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tag_len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_sm4_gcm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_GCM_IV_SIZE], uint8_t* plaintext, size_t len,
                                 const uint8_t* header, size_t headerlen, const uint8_t* tag, size_t tag_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_SM4_GCM | (((16 - tag_len) & 0xF) << 8), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, ESEC_SM4_GCM_IV_SIZE);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, tag_len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

static uint32_t esec_crypt_aria_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* data_in,
                                   const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* data_out, size_t len, uint32_t cmd)
{
    if(!esec_mb_push_command(mb, cmd, 2))
        return ESEC_TX_FIFO_FULL;

    bool has_iv = (cmd != ESEC_CMD_ENCRYPT_ARIA_ECB) && (cmd != ESEC_CMD_DECRYPT_ARIA_ECB);

    esec_mb_push_in_skey(mb, &skey);
    if(has_iv)
        esec_mb_push_in_buf(mb, iv, 16);

    esec_mb_push_in_buf(mb, data_in, len);
    esec_mb_push_out_desc(mb, data_out, len);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, plaintext, NULL, ciphertext, len, ESEC_CMD_ENCRYPT_ARIA_ECB);
}

uint32_t esec_aria_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  uint8_t* plaintext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, ciphertext, NULL, plaintext, len, ESEC_CMD_DECRYPT_ARIA_ECB);
}

uint32_t esec_aria_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_ARIA_CBC);
}

uint32_t esec_aria_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_ARIA_CBC);
}

uint32_t esec_aria_ctr_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_ARIA_CTR);
}

uint32_t esec_aria_ctr_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_ARIA_CTR);
}

uint32_t esec_aria_cfb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_ARIA_CFB);
}

uint32_t esec_aria_cfb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_ARIA_CFB);
}

uint32_t esec_aria_ofb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, plaintext, iv, ciphertext, len, ESEC_CMD_ENCRYPT_ARIA_OFB);
}

uint32_t esec_aria_ofb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len)
{
    return esec_crypt_aria_go(mb, skey, ciphertext, iv, plaintext, len, ESEC_CMD_DECRYPT_ARIA_OFB);
}

static uint32_t esec_aria_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                       enum esec_aria_mode mode, const uint8_t iv[16], const uint8_t* plaintext,
                                       uint8_t* ciphertext, size_t len)
{
    state->command = ESEC_CMD_ENCRYPT_ARIA((uint32_t)mode) | ESEC_CONTEXT_SWITCHING_START;
    state->skey = skey;

    if(!esec_mb_push_command(mb, state->command, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 16);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, state->context, 16);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cbc_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_start_go(mb, state, skey, ESEC_ARIA_CBC, iv, plaintext, ciphertext, len);
}

uint32_t esec_aria_ctr_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_start_go(mb, state, skey, ESEC_ARIA_CTR, iv, plaintext, ciphertext, len);
}

uint32_t esec_aria_cfb_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_start_go(mb, state, skey, ESEC_ARIA_CFB, iv, plaintext, ciphertext, len);
}

uint32_t esec_aria_ofb_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_start_go(mb, state, skey, ESEC_ARIA_OFB, iv, plaintext, ciphertext, len);
}

static uint32_t esec_aria_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                       enum esec_aria_mode mode, const uint8_t iv[16], const uint8_t* ciphertext,
                                       uint8_t* plaintext, size_t len)
{
    state->command = ESEC_CMD_DECRYPT_ARIA((uint32_t)mode) | ESEC_CONTEXT_SWITCHING_START;
    state->skey = skey;

    if(!esec_mb_push_command(mb, state->command, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, 16);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_push_out_desc(mb, state->context, 16);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cbc_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aria_dec_start_go(mb, state, skey, ESEC_ARIA_CBC, iv, ciphertext, plaintext, len);
}

uint32_t esec_aria_ctr_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aria_dec_start_go(mb, state, skey, ESEC_ARIA_CTR, iv, ciphertext, plaintext, len);
}

uint32_t esec_aria_cfb_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aria_dec_start_go(mb, state, skey, ESEC_ARIA_CFB, iv, ciphertext, plaintext, len);
}

uint32_t esec_aria_ofb_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len)
{
    return esec_aria_dec_start_go(mb, state, skey, ESEC_ARIA_OFB, iv, ciphertext, plaintext, len);
}

static uint32_t esec_aria_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                        enum esec_aria_mode mode, const uint8_t* plaintext, uint8_t* ciphertext,
                                        size_t len)
{
    state->command = ESEC_CMD_ENCRYPT_ARIA((uint32_t)mode) | ESEC_CONTEXT_SWITCHING_MIDDLE;
    state->skey = skey;

    if(!esec_mb_push_command(mb, state->command, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, state->context, 16);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, state->context, 16);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cbc_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_update_go(mb, state, skey, ESEC_ARIA_CBC, plaintext, ciphertext, len);
}

uint32_t esec_aria_ctr_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_update_go(mb, state, skey, ESEC_ARIA_CTR, plaintext, ciphertext, len);
}

uint32_t esec_aria_cfb_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_update_go(mb, state, skey, ESEC_ARIA_CFB, plaintext, ciphertext, len);
}

uint32_t esec_aria_ofb_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_update_go(mb, state, skey, ESEC_ARIA_OFB, plaintext, ciphertext, len);
}

static uint32_t esec_aria_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                        enum esec_aria_mode mode, uint8_t* plaintext, const uint8_t* ciphertext,
                                        size_t len)
{
    state->command = ESEC_CMD_DECRYPT_ARIA((uint32_t)mode) | ESEC_CONTEXT_SWITCHING_MIDDLE;
    state->skey = skey;

    if(!esec_mb_push_command(mb, state->command, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, state->context, 16);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_push_out_desc(mb, state->context, 16);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cbc_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_update_go(mb, state, skey, ESEC_ARIA_CBC, plaintext, ciphertext, len);
}

uint32_t esec_aria_ctr_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_update_go(mb, state, skey, ESEC_ARIA_CTR, plaintext, ciphertext, len);
}

uint32_t esec_aria_cfb_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_update_go(mb, state, skey, ESEC_ARIA_CFB, plaintext, ciphertext, len);
}

uint32_t esec_aria_ofb_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_update_go(mb, state, skey, ESEC_ARIA_OFB, plaintext, ciphertext, len);
}

static uint32_t esec_aria_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                        enum esec_aria_mode mode, const uint8_t* plaintext, uint8_t* ciphertext,
                                        size_t len)
{
    state->command = ESEC_CMD_ENCRYPT_ARIA((uint32_t)mode) | ESEC_CONTEXT_SWITCHING_END;
    state->skey = skey;

    if(!esec_mb_push_command(mb, state->command, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &state->skey);
    esec_mb_push_in_buf(mb, state->context, 16);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cbc_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_finish_go(mb, state, skey, ESEC_ARIA_CBC, plaintext, ciphertext, len);
}

uint32_t esec_aria_ctr_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_finish_go(mb, state, skey, ESEC_ARIA_CTR, plaintext, ciphertext, len);
}

uint32_t esec_aria_cfb_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_finish_go(mb, state, skey, ESEC_ARIA_CFB, plaintext, ciphertext, len);
}

uint32_t esec_aria_ofb_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len)
{
    return esec_aria_enc_finish_go(mb, state, skey, ESEC_ARIA_OFB, plaintext, ciphertext, len);
}

static uint32_t esec_aria_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                        enum esec_aria_mode mode, uint8_t* plaintext, const uint8_t* ciphertext,
                                        size_t len)
{
    state->command = ESEC_CMD_DECRYPT_ARIA((uint32_t)mode) | ESEC_CONTEXT_SWITCHING_END;
    state->skey = skey;

    if(!esec_mb_push_command(mb, state->command, 2))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &state->skey);
    esec_mb_push_in_buf(mb, state->context, 16);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);

    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cbc_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_finish_go(mb, state, skey, ESEC_ARIA_CBC, plaintext, ciphertext, len);
}

uint32_t esec_aria_ctr_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_finish_go(mb, state, skey, ESEC_ARIA_CTR, plaintext, ciphertext, len);
}

uint32_t esec_aria_cfb_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_finish_go(mb, state, skey, ESEC_ARIA_CFB, plaintext, ciphertext, len);
}

uint32_t esec_aria_ofb_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len)
{
    return esec_aria_dec_finish_go(mb, state, skey, ESEC_ARIA_OFB, plaintext, ciphertext, len);
}

uint32_t esec_aria_gcm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                  const uint8_t* header, size_t headerlen, uint8_t* tag, size_t tag_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_GCM | (((16 - tag_len) & 0xF) << 8), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, ESEC_ARIA_GCM_IV_SIZE);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tag_len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_encrypt_start_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                        const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_GCM | ESEC_CONTEXT_SWITCHING_START, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, ESEC_ARIA_GCM_IV_SIZE);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_encrypt_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_GCM | ESEC_CONTEXT_SWITCHING_MIDDLE, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_encrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                         uint8_t* tag, size_t tag_len, uint8_t lenAlenC[16])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_GCM | ESEC_CONTEXT_SWITCHING_END | (((16 - tag_len) & 0xF) << 8),
                             3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_in_buf(mb, lenAlenC, 16);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tag_len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_decrypt_start_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                        const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], const uint8_t* ciphertext, size_t len,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_GCM | ESEC_CONTEXT_SWITCHING_START, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, ESEC_ARIA_GCM_IV_SIZE);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_decrypt_update_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_GCM | ESEC_CONTEXT_SWITCHING_MIDDLE, 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_decrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                         uint8_t* tag, size_t tag_len, uint8_t lenAlenC[16])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_GCM | ESEC_CONTEXT_SWITCHING_END | (((16 - tag_len) & 0xF) << 8),
                             3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, lenAlenC, 16);
    esec_mb_push_in_buf(mb, tag, tag_len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_gcm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* plaintext, size_t len,
                                  const uint8_t* header, size_t headerlen, const uint8_t* tag, size_t tag_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_GCM | (((16 - tag_len) & 0xF) << 8), 3))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, iv, ESEC_ARIA_GCM_IV_SIZE);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, tag_len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len, const uint8_t* nonce, size_t noncelen,
                                  const uint8_t* header, size_t headerlen, uint8_t* tag, size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_CCM, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, noncelen);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tagsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_encrypt_start_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                        uint8_t* ciphertext, size_t len, const uint8_t* nonce, size_t noncelen,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                        size_t tagsize, size_t total_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_CCM | ESEC_CONTEXT_SWITCHING_START, 5))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, noncelen);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);
    esec_mb_push_param(total_len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_encrypt_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_CCM | ESEC_CONTEXT_SWITCHING_MIDDLE, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_encrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_ENCRYPT_ARIA_CCM | ESEC_CONTEXT_SWITCHING_END, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, tag, tagsize);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_decrypt_start_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                        const uint8_t* ciphertext, size_t len, const uint8_t* nonce, size_t noncelen,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                        size_t tagsize, size_t total_len)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_CCM | ESEC_CONTEXT_SWITCHING_START, 5))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, noncelen);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);
    esec_mb_push_param(total_len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_decrypt_update_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_CCM | ESEC_CONTEXT_SWITCHING_MIDDLE, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_decrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_CCM | ESEC_CONTEXT_SWITCHING_END, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_CTX_xCM_SIZE);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, NULL, 0);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, tagsize);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(0);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_ccm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  uint8_t* plaintext, size_t len, const uint8_t* nonce, size_t noncelen,
                                  const uint8_t* header, size_t headerlen, const uint8_t* tag, size_t tagsize)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_DECRYPT_ARIA_CCM, 4))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, nonce, noncelen);
    esec_mb_push_in_buf(mb, header, headerlen);
    esec_mb_push_in_buf(mb, ciphertext, len);
    esec_mb_push_in_buf(mb, tag, tagsize);
    esec_mb_push_out_desc(mb, plaintext, len);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param((noncelen << 16) | tagsize);
    esec_mb_push_param(headerlen);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_generate_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                    uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_ARIA_CMAC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, tag, ESEC_TAG_SIZE_ARIA_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_generate_init_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                         uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_ARIA_CMAC | ESEC_CONTEXT_SWITCHING_START, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_generate_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message,
                                           size_t len, uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_ARIA_CMAC | ESEC_CONTEXT_SWITCHING_MIDDLE, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_generate_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message,
                                           size_t len, uint8_t ctx[ESEC_ARIA_IV_SIZE],
                                           uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_GENERATE_ARIA_CMAC | ESEC_CONTEXT_SWITCHING_END, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, tag, ESEC_TAG_SIZE_ARIA_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_verify_init_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                       uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_ARIA_CMAC | ESEC_CONTEXT_SWITCHING_START, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_verify_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                         uint8_t ctx[ESEC_ARIA_IV_SIZE])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_ARIA_CMAC | ESEC_CONTEXT_SWITCHING_MIDDLE, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_out_desc(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_verify_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                         uint8_t ctx[ESEC_ARIA_IV_SIZE], uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_ARIA_CMAC | ESEC_CONTEXT_SWITCHING_END, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, ctx, ESEC_ARIA_IV_SIZE);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, tag, ESEC_TAG_SIZE_ARIA_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aria_cmac_verify_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                  const uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC])
{
    if(!esec_mb_push_command(mb, ESEC_CMD_VERIFY_ARIA_CMAC, 2))
        return ESEC_TX_FIFO_FULL;
    esec_mb_push_in_skey(mb, &skey);
    esec_mb_push_in_buf(mb, message, len);
    esec_mb_push_in_buf(mb, tag, ESEC_TAG_SIZE_ARIA_CMAC);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&skey);
    esec_mb_push_param(len);

    return ESEC_OKAY;
}

uint32_t esec_aes_key_wrap_go(struct esec_mb* mb, struct esec_skey target_key, struct esec_skey wrapping_key,
                              uint8_t* output, uint32_t output_size)
{
    if(!esec_mb_push_command(mb, ESEC_CMD_KEYWRAP_AES, 3))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &target_key);
    esec_mb_push_in_skey(mb, &wrapping_key);
    esec_mb_push_out_desc(mb, output, output_size);
    esec_mb_finish_outputs(mb);
    esec_mb_push_param_skey(&target_key);
    esec_mb_push_param_skey(&wrapping_key);
    esec_mb_push_param(output_size);

    return ESEC_OKAY;
}

uint32_t esec_aes_key_unwrap_go(struct esec_mb* mb, struct esec_skey wrapping_key, uint8_t* wrapped_key,
                                uint32_t wrapped_key_len, uint32_t flags, uint16_t klen,
                                struct esec_key_storage storage, struct esec_skey* target_key)
{
    target_key->keyspec = klen;
    if(flags & ESEC_KEY_ONLY_ESECURE)
        storage.cfg |= ESEC_RESTR_RESTRICTED;
    else if(!(flags & ESEC_KEY_EXPORTABLE))
        storage.cfg |= ESEC_RESTR_LOCKED;
    target_key->storage = storage;

    if(!esec_mb_push_command(mb, ESEC_CMD_KEYUNWRAP_AES, 3))
        return ESEC_TX_FIFO_FULL;

    esec_mb_push_in_skey(mb, &wrapping_key);
    esec_mb_push_in_buf(mb, wrapped_key, wrapped_key_len);
    esec_mb_push_key_auth_if_any(mb, &storage);
    esec_mb_push_key_material_out(mb, &storage, klen);
    esec_mb_finish_outputs(mb);

    esec_mb_push_param_skey(&wrapping_key);
    esec_mb_push_param(wrapped_key_len);
    esec_mb_push_param(klen | mb_storage_flags(&storage));

    return ESEC_OKAY;
}
