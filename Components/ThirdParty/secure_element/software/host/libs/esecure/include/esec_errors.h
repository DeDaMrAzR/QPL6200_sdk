/**
 * @brief eSecure Library: status/error codes
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ERRORS_HEADER
#define ESEC_ERRORS_HEADER

#if defined(SEUC_HARDEN_MB_RETURN_CODES)
/** The function or command completed without any error. **/
#define ESEC_OKAY 1

/** Invalid or unknown eSecure command **/
#define ESEC_INVALID_COMMAND 2

/** Failed authorization to access a protected key or protected command.
 *
 * This can happen when providing the incorrect authorization value for
 * protected keys. Some protected commands can also trigger this error.
 **/
#define ESEC_AUTHORIZATION_ERROR 4

/** Signature or authentication tag verification failed.
 *
 * The signature or authentication tag computed for a message did not match
 * the given one. See also esec_aes_decrypt_gcm(), esec_aes_decrypt_ccm() and
 * esec_chachapoly_decrypt().
 **/
#define ESEC_INVALID_SIGNATURE 7

/** eSecure encountered a DMA error while fetching or pushing data.
 *
 * This can happen when passing invalid addresses or incorrect buffer sizes.
 **/
#define ESEC_DMA_ERROR 8

/** Error encountered during cryptographic operations by eSecure. **/
#define ESEC_CRYPTO_ERROR 13

/** Invalid argument or content provided to the function. **/
#define ESEC_INVALID_PARAMETER 14

#else /* SEUC_HARDEN_MB_RETURN_CODES */

/** The function or command completed without any error. **/
#define ESEC_OKAY                0

/** Invalid or unknown eSecure command **/
#define ESEC_INVALID_COMMAND     1

/** Failed authorization to access a protected key or protected command.
 *
 * This can happen when providing the incorrect authorization value for
 * protected keys. Some protected commands can also trigger this error.
 **/
#define ESEC_AUTHORIZATION_ERROR 2

/** Signature or authentication tag verification failed.
 *
 * The signature or authentication tag computed for a message did not match
 * the given one. See also esec_aes_decrypt_gcm(), esec_aes_decrypt_ccm() and
 * esec_chachapoly_decrypt().
 **/
#define ESEC_INVALID_SIGNATURE   3

/** eSecure encountered a DMA error while fetching or pushing data.
 *
 * This can happen when passing invalid addresses or incorrect buffer sizes.
 **/
#define ESEC_DMA_ERROR           4

/** Error encountered during cryptographic operations by eSecure. **/
#define ESEC_CRYPTO_ERROR        6

/** Invalid argument or content provided to the function. **/
#define ESEC_INVALID_PARAMETER   7
#endif /*SEUC_HARDEN_MB_RETURN_CODES */

/** Esecure mailbox fifo is full, one can't push command anymore **/
#define ESEC_TX_FIFO_FULL 100

/** Invalid buffer length **/
#define ESEC_INVALID_LENGTH 200

/** Invalid key for this function or command **/
#define ESEC_INVALID_KEY 201

/** Invalid response received from eSecure mailbox.
 *
 * Communication with eSecure failed. Next accesses could fail badly.
 *
 * This can be caused for example by non-protected concurrent access to eSecure.
 **/
#define ESEC_INVALID_RESPONSE 0xFF

#endif /*ESEC_ERRORS_HEADER*/
