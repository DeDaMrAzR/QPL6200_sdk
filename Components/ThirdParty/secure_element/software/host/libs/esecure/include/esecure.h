/**
 * @brief eSecure Library
 * @copyright Copyright (c) 2017-2021 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESECURE_HEADER
#define ESECURE_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_config.h"
#include "esec_keys_basics.h"
#include "esec_hash.h"
#include "esec_symm.h"
#include "esec_aes.h"
#include "esec_chachapoly.h"
#include "esec_3des.h"
#include "esec_sm4.h"
#include "esec_jpake.h"
#include "esec_ed25519.h"
#include "esec_ecc.h"
#include "esec_rsa.h"
#include "esec_rsa_basics.h"
#include "esec_dsa.h"
#include "esec_dh.h"
#include "esec_srp.h"
#include "esec_cert.h"
#include "esec_shake.h"
#include "esec_sm2.h"
#include "esec_aria.h"
#include "esec_hmac.h"
#include "esec_qorvo.h"

/** Notify host firmware boot success to eSecure.
 *
 * This command confirms that the host firmware has successfully started. Once
 * this command has been issued, it will not be allowed to revert to any older
 * version of the host firmware.
 */
uint32_t esec_mark_boot_success(void);

/** A chip or system manufacturer certificate.
 *
 * See esec_revoke_system_boot_key(), esec_revoke_chip_boot_key().
 *
 * The structure layout is provided for reference only.
 **/
struct esec_manufacturer_cert {
    /** Magic number which should be 0xE5ECCE01 **/
    uint32_t magic;
    /** Bitfield of debug authorizations. **/
    uint8_t debug_grants[DBG_GRANT_SIZE];
#if(DBG_GRANT_SIZE % 4)
    uint8_t reserved0[4 - (DBG_GRANT_SIZE % 4)];
#endif

    /** Bitfield of authorizations associated to this certificate.
     *
     * bit 0: eSecure firmware can be signed
     * bit 1: host firmware can be signed
     * bit 2: manufacturer key can be revoked
     * bit 3: eSecure can be decommissioned
     * the other bits are reserved and should be 0.
     **/
    uint8_t authorizations0;
    /** the index of the key in security configuration to be used to verify **/
    uint8_t key_index;
    /** 2 bytes reserved for more authorizations **/
    uint8_t authorizations1;
    uint8_t authorizations2;
    /** Bitfield of tampers which can be disabled **/
    uint32_t tamper_authorizations;
    /** Certification (Virtual) Expiration Time */
    uint32_t expiration_time_val;
    /** The target serial numbers after masking with 'serial_mask' **/
    uint8_t serial_number[16];
    /** The bits of the serial_number which are compared **/
    uint8_t serial_mask[16];
    /** Developper or user public key **/
    uint8_t pubkey[ESEC_MFCT_PUBKEY_SZ];
    /** Manufacturer public key **/
    uint8_t manpubkey[ESEC_MFCT_PUBKEY_SZ];

    /** Certificate signature **/
    uint8_t signature[ESEC_MFCT_SIG_SZ];
} __attribute__((packed));

/** Revoke the chip boot key at key_index **/
uint32_t esec_revoke_chip_boot_key(uint32_t key_index, struct esec_manufacturer_cert* cert,
                                   uint8_t signature[ESEC_MFCT_SIG_SZ]);

/** Revoke the system manufacturer boot key at key_index **/
uint32_t esec_revoke_system_boot_key(uint32_t key_index, struct esec_manufacturer_cert* cert,
                                     uint8_t signature[ESEC_MFCT_SIG_SZ]);

/** Read the chip public boot key **/
uint32_t esec_read_chip_boot_key(uint32_t key_index, uint8_t pubkey[ESEC_PUBKEY_DIGESTSIZE]);

/** Read the system public boot key **/
uint32_t esec_read_system_boot_key(uint32_t key_index, uint8_t pubkey[ESEC_PUBKEY_DIGESTSIZE]);

/** Generate random bytes. */
uint32_t esec_get_random_bytes(uint8_t* bytes, size_t len);

/** Representation of RTC time in eSecure. */
struct esec_clock {
    /** fractions of seconds. */
    uint32_t frac;
    /** elapsed seconds. */
    uint32_t seconds;
};

/** Read time from built-in RTC clock.
 *
 * The time from the RTC is transmitted as one 64-bit value with fixed point.
 * The 32 most significant bits represent the seconds and the 32 least
 * significant bits represent the fractions of seconds.
 *
 * With a 32 kHz RTC clock, the 17 least significant bits are always zeroes.
 **/
uint32_t esec_read_clock(uint64_t* time);

/** Generate a random prime number.
 *
 * The argument 'bits' gives the length in bits in the prime. The most
 * significant will be one. The 'rounds' argument specifies the number
 * of Miller-Rabin rounds to generate the prime number.
 *
 * The prime buffer size must be rounded up to a multiple of 32 bits.
 * Zero-padding is performed before the value.
 **/
uint32_t esec_generate_prime(uint8_t* prime, uint32_t bits, uint32_t rounds);

/** Enable mailbox RX interupts */
void esec_enable_interrupts(void);

/** Disable mailbox RX interupts */
void esec_disable_interrupts(void);

/** Initialize the mutex */
void esec_init_mutex(void);

/** Read the device 128 bits serial number from eSecure */
uint32_t esec_read_serial_number(uint8_t sn[16]);

/** Tampers reported by eSecure
 *
 * See esec_read_tamper_status().
 **/
struct esec_tamper {
    /** Status at last tamper */
    uint32_t last_tamper_status;
    /** Timestamp of last tamper */
    uint32_t last_tamper_time;
    /** Current raw tamper status */
    uint32_t current_tamper_status;
    /** Current time */
    uint32_t current_time;
};

#define ESEC_HOST_BOOT_FLAG_UPGRADE     (1 << 12)
#define ESEC_HOST_BOOT_FLAG_IMAGE       (1 << 20)
#define ESEC_FIRMWARE_BOOT_FLAG_UPGRADE (1 << 8)
#define ESEC_FIRMWARE_BOOT_FLAG_IMAGE   (1 << 16)

/** Boot status and versions reported by eSecure.
 *
 * See esec_read_boot_status().
 **/
struct esec_boot {
    /** Boot status bits
     *
     * Bit 8: eSecure upgrade flag
     * Bit 12: host upgrade flag
     * Bit 16: eSecure image (0: image A, 1: image B)
     * Bit 20: host image (0: image A, 1: image B)
     *
     * See also ESEC_HOST_BOOT_FLAG_UPGRADE, ESEC_HOST_BOOT_FLAG_IMAGE,
     * ESEC_FIRMWARE_BOOT_FLAG_UPGRADE, ESEC_FIRMWARE_BOOT_FLAG_IMAGE
     **/
    uint32_t status;
    /** eSecure firmware version */
    uint32_t esec_fw_version;
    /** host software image version */
    uint32_t host_image_version;
};

/** Read the current tamper status of eSecure */
uint32_t esec_read_tamper_status(struct esec_tamper* status);

/** Read the boot status from eSecure */
uint32_t esec_read_boot_status(struct esec_boot* status);

/** Write data at an offset into the host area of the OTP. */
uint32_t esec_write_otp(uint32_t offset, const uint8_t* data, size_t len);

/** Read data at an offset from the host area of the OTP. */
uint32_t esec_read_otp(uint32_t offset, uint8_t* data, size_t len);

/** A request to be signed to enable tamper maintenance operations.
 *
 * Created by esec_create_tampers_maintenance_request(). After creation,
 * the request can be signed by a key certified by the system manufacturer.
 * The signed request enables the desired maintenance mode with
 * esec_apply_tamper_maintenance().
 *
 * The user should handle this structure as an opaque object.
 **/
struct esec_tamper_maintenance_request {
    uint32_t cmd;
    uint32_t access_bits;
    uint8_t challenge[16];
};

/** Number of words required for the DBG_GRANT mask. */
#define DBG_GRANT_WORD_SIZE ((DBG_GRANT_SIZE + 3) / 4)

/** A request to be signed to enable debug maintenance operations.
 *
 * Created by esec_create_debug_maintenance_request(). After creation,
 * the request can be signed by a key certified by the system manufacturer.
 * The signed request enables the desired maintenance mode with esec_apply_debug_maintenance().
 *
 * The user should handle this structure as an opaque object.
 **/
struct esec_debug_maintenance_request {
    uint32_t cmd;
    uint8_t access_bits[DBG_GRANT_SIZE];
#if(DBG_GRANT_SIZE % 4)
    uint8_t reserved[DBG_GRANT_WORD_SIZE * 4 - DBG_GRANT_SIZE];
#endif
    uint8_t challenge[16];
};

/** A request to be signed to decommission eSecure.
 *
 * Created by esec_create_decommission_request(). After creation,
 * the request can be signed by a key certified by the system manufacturer.
 * The signed request decommissions eSecure with esec_apply_decommission().
 *
 * The user should handle this structure as an opaque object.
 **/
struct esec_decommission_request {
    uint32_t cmd;
    uint8_t challenge[16];
};

/** A request to be signed to set virtual time operations.
 *
 * Created by esec_create_set_virtual_time_request(). After creation,
 * the request can be signed by a key certified by the system manufacturer.
 * The signed request sets the virtual time.
 *
 * The user should handle this structure as an opaque object.
 **/
struct esec_set_virtual_time_request {
    uint32_t cmd;
    uint32_t virtual_time;
};

/** Create a maintenance request to disable/enable some tamper sources.
 *
 * After signing the generated request, disable/enable the tamper sources by
 * calling esec_apply_tamper_maintenance().
 **/
uint32_t esec_create_tampering_request(uint32_t tampers, struct esec_tamper_maintenance_request* request);

/** Create a maintenance request to grant or remove debug authorizations.
 *
 * After signing the generated request, grant the debug accesses by
 * calling esec_apply_debug_maintenance().
 **/
uint32_t esec_create_debuging_request(uint8_t* debug_bits, struct esec_debug_maintenance_request* request);

/** Create a request to decommission eSecure.
 *
 * After signing the generated request, decommission eSecure by calling
 * esec_apply_decommission().
 **/
uint32_t esec_create_decommission_request(struct esec_decommission_request* request);

/** Create a request to set virtual time.
 *
 * After signing the generated request, set virtual time by calling
 * esec_apply_set_virtual_time_maintenance().
 **/
uint32_t esec_create_set_virtual_time_request(uint32_t virtual_time, struct esec_set_virtual_time_request* request);

/** Apply a tamper sources maintenance request.
 **/
uint32_t esec_apply_tamper_maintenance(struct esec_manufacturer_cert* cert,
                                       struct esec_tamper_maintenance_request* request,
                                       uint8_t signature[ESEC_MFCT_SIG_SZ]);

/** Apply a debug maintenance request.
 **/
uint32_t esec_apply_debug_maintenance(struct esec_manufacturer_cert* cert,
                                      struct esec_debug_maintenance_request* request,
                                      uint8_t signature[ESEC_MFCT_SIG_SZ]);

/** Apply an eSecure decommission request.
 **/
uint32_t esec_apply_decommission(struct esec_manufacturer_cert* cert, struct esec_decommission_request* request,
                                 uint8_t signature[ESEC_MFCT_SIG_SZ]);

/** Apply a set virtual time request.
 **/
uint32_t esec_apply_set_virtual_time_maintenance(struct esec_manufacturer_cert* cert,
                                                 struct esec_set_virtual_time_request* request,
                                                 uint8_t signature[ESEC_MFCT_SIG_SZ]);

/** Get the Virtual Time.
 **/
uint32_t esec_get_virtual_time_maintenance(uint32_t* virtual_time);

/** Configure periodic firmware integrity monitoring.
 *
 * Initialize the reference internal integrity MAC with the host memory
 * regions MAC added via esec_integrity_add_region() and the eSecure embedded
 * firmware MAC.
 *
 * Set the maximum period at which the integrity verifications must be
 * requested by the host software with esec_integrity_verify(). This should be
 * run as soon as possible after boot. The period is given in seconds.
 *
 * After starting integrity monitoring, the host has to run all integrity
 * checks each time before the configured time period elapses. Failing
 * an integrity test or not running all integrity tests in the configured
 * time period causes eSecure to trigger a tamper.
 *
 * This function may be called only once.
 **/
uint32_t esec_integrity_configure(uint32_t period);

/** Update internal MAC based on content of additional host memory region.
 *
 * After adding a region, it may not be modified anymore. The function is used
 * as well at start-up to create the reference MAC and periodically to verify
 * the integrity of the memory regions. The memory regions must always be
 * checked in the same order as at the first time.
 *
 * The address of the memory region is an address on the bus which eSecure
 * can directly read via DMA.
 **/
uint32_t esec_integrity_add_region(const void* address, size_t size);

/** Verify integrity by comparing computed MAC with reference MAC.
 *
 * The reference MAC has been set when calling esec_integrity_configure().
 * The MAC is updated with the host memory regions by
 * esec_integrity_add_region(). The function automatically updates the
 * computed MAC with the eSecure firmware content.
 *
 * esec_integrity_configure() must have been called before calling this
 * function.
 **/
uint32_t esec_integrity_verify(void);

/** Anti-replay element for one anti-replay protected key.
 *
 * Each key protected with anti-replay has such an entry in the anti-replay
 * table. The index in the table is the same as the key index.
 *
 * The first ESEC_STOR_PROT_IV_SIZE bytes of the wrapped protected key form the
 * anti-replay IV element for that key.
 */
struct esec_ar_entry {
    uint8_t prot_iv[ESEC_STOR_PROT_IV_SIZE];
};

/** Load the Anti-replay table into eSecure.
 *
 * Must be called before using keys with anti-replay protection. Entries for
 * unused key entries should be initialized to 0. The entries argument
 * gives the number of elements in artable. It should be equal or bigger than
 * the maximum anti-replay protected key index in use.
 *
 * Returns ESEC_AUTHORIZATION_ERROR when the table does not match with the MAC
 * stored persistently in eSecure.
 */
uint32_t esec_ar_load_table(struct esec_ar_entry* artable, uint32_t entries);

/** Read the anti-replay table from eSecure.
 *
 * Read the current anti-replay table from eSecure. This operation is useally
 * not needed as the table can be reconstructed directly from the wrapped
 * keys with anti-replay protection stored in host memory.
 */
uint32_t esec_ar_read_table(struct esec_ar_entry* artable, uint32_t entries);

/** Commit changes to the anti-replay keys.
 *
 * After a commit, an anti-replay table loaded after boot must match the keys
 * at commit time.
 *
 * The host should commit the table after keys anti-replay protected keys are
 * changed or added and effectively written into memory.
 */
uint32_t esec_ar_commit_table(void);

/**
 * @brief Tests for overflow in the DMA descriptors of the secure element.
 *
 * This function checks if there is an overflow condition in the number of DMA descriptors
 *  parsed by the secure element.
 *
 * @return uint32_t ESEC_DMA_ERROR if the overflow did NOT trigger, ESEC_OKAY if it did.
 */
uint32_t test_esec_dma_descriptors_overflow(void);
#endif
