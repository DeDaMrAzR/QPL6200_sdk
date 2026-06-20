/**
 * @brief eSecure Library: Platform Layer Interface for PSA API Implementation
 *
 *        This implementation needs to be implemented for the platform.
 *        Please Implement
 *          - psa_plat_ps_XXX() : to provide a protected storage on the host side.
 *              (Isolated and/or encrypted storage)
 *          - psa_plat_crypto_XXX_key() : to provide persistent storage to keep
 *              key instance for device resets.
 *
 *
 * @copyright Copyright (c) 2022 Silex Insight. All Rights reserved
 */

#ifndef __PSA_ESEC_PLATFROM_H
#define __PSA_ESEC_PLATFROM_H

#include "esecure.h"
#include "psa/crypto.h"
#include "psa/protected_storage.h"
#include "psa_crypto_internal.h"

/* Symbol definition to mark as private */
#define PRIVATE static
/* Symbol definition to mark as public */
#define PUBLIC
/* Symbol definition to mark as public */
#define INTERNAL

/* The number of Key Slots to keep Key Instances */
#define NUMBER_OF_KEY_SLOTS                     2

/*
 * @brief Flushes a key to the platform storage. Persistent keys
 *        may need to be stored somewhere in platform (file, flash etc)
 *
 * @param key Key to flush
 */
psa_status_t psa_plat_crypto_flush_key(const psa_esec_key* key);

/*
 * @brief Deletes a key from the platform storage. Persistent keys
 *        may need to be removed from the persintent storage
 *
 * @param key Key to delete
 */
psa_status_t psa_plat_crypto_delete_key(const psa_esec_key* key);

/*
 * @brief Opens a existing persistent key and fills into the key instance.
 *
 * @param id Key ID to open
 * @param[out] key Key Instance to fill
 */
psa_status_t psa_plat_crypto_open_key(psa_key_id_t id, psa_esec_key* key);

/*
 * @brief Allocates a memory from the platform
 *
 * @param size to be allocated memory size
 */
void* psa_plat_mem_alloc(size_t size);

/*
 * @brief Frees a memory
 *
 * @param data data to free
 */
void psa_plat_mem_free(void* data);

/*
 * @brief Gets Protected Storage(PS) record details
 *
 * @param uid PS record id to get
 * @param[out] item PS record info
 */
psa_status_t psa_plat_ps_get_info(psa_storage_uid_t uid, struct psa_storage_info_t* item);


/*
 * @brief Sets a Protected Storage(PS) record
 *
 * @param uid PS record id to set
 * @param item PS record details
 * @param data to be stored data
 *
 */
psa_status_t psa_plat_ps_set(psa_storage_uid_t uid, struct psa_storage_info_t* item, const uint8_t* data);

/*
 * @brief Gets the Protected Storage(PS) Record
 *
 * @param uid PS record id to get
 * @param data_offset Offset to get the protected data
 * @param data_size Size to read
 * @param[out] p_data Buffer to read data into
 * @param[out] p_data_length Buffer to read data into
 */
psa_status_t psa_plat_ps_get(psa_storage_uid_t uid, size_t data_offset, size_t data_size, void* p_data, size_t* p_data_length);

/*
 * @brief Removes a record from the Protected Storage(PS)
 *
 * @param uid Record uid to remove
 */
psa_status_t psa_plat_ps_remove(psa_storage_uid_t uid);

psa_status_t psa_plat_ps_set_extended(psa_storage_uid_t uid, size_t data_offset, size_t data_length, const void* p_data);

#endif /* __PSA_PLATFROM_H */
