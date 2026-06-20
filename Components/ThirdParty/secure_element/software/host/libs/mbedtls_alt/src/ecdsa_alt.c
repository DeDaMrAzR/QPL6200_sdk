/*
 *  Elliptic curves over GF(p): generic functions
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 * GECC = Guide to Elliptic Curve Cryptography - Hankerson, Menezes, Vanstone
 * FIPS 186-3 http://csrc.nist.gov/publications/fips/fips186-3/fips_186-3.pdf
 * RFC 4492 for the related TLS structures and constants
 * RFC 7748 for the Curve448 and Curve25519 curve definitions
 *
 * [Curve25519] http://cr.yp.to/ecdh/curve25519-20060209.pdf
 *
 * [2] CORON, Jean-S'ebastien. Resistance against differential power analysis
 *     for elliptic curve cryptosystems. In : Cryptographic Hardware and
 *     Embedded Systems. Springer Berlin Heidelberg, 1999. p. 292-302.
 *     <http://link.springer.com/chapter/10.1007/3-540-48059-5_25>
 *
 * [3] HEDABOU, Mustapha, PINEL, Pierre, et B'EN'ETEAU, Lucien. A comb method to
 *     render ECC resistant against Side Channel Attacks. IACR Cryptology
 *     ePrint Archive, 2004, vol. 2004, p. 342.
 *     <http://eprint.iacr.org/2004/342.pdf>
 */

#include "common.h"

/**
 * \brief Function level alternative implementation.
 *
 * The MBEDTLS_ECP_INTERNAL_ALT macro enables alternative implementations to
 * replace certain functions in this module. The alternative implementations are
 * typically hardware accelerators and need to activate the hardware before the
 * computation starts and deactivate it after it finishes. The
 * mbedtls_internal_ecp_init() and mbedtls_internal_ecp_free() functions serve
 * this purpose.
 *
 * To preserve the correct functionality the following conditions must hold:
 *
 * - The alternative implementation must be activated by
 *   mbedtls_internal_ecp_init() before any of the replaceable functions is
 *   called.
 * - mbedtls_internal_ecp_free() must \b only be called when the alternative
 *   implementation is activated.
 * - mbedtls_internal_ecp_init() must \b not be called when the alternative
 *   implementation is activated.
 * - Public functions must not return while the alternative implementation is
 *   activated.
 * - Replaceable functions are guarded by \c MBEDTLS_ECP_XXX_ALT macros and
 *   before calling them an \code if( mbedtls_internal_ecp_grp_capable( grp ) )
 *   \endcode ensures that the alternative implementation supports the current
 *   group.
 */

#if defined(MBEDTLS_ECDSA_C)

#include "mbedtls/ecdsa.h"
#include "mbedtls/threading.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#include "mbedtls/platform.h"

#include "esecure.h"

#define ECC_MAX_OPERAND_SZ 32

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify(mbedtls_ecp_group* grp, const unsigned char* buf, size_t blen, const mbedtls_ecp_point* Q,
                         const mbedtls_mpi* r, const mbedtls_mpi* s)
{
    int ret;
    uint8_t signature[ECC_MAX_OPERAND_SZ * 2];
    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if(grp->N.MBEDTLS_PRIVATE(p) == NULL)
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    if(grp->id != MBEDTLS_ECP_DP_SECP256R1)
    {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    /* make sure r and s are in range 1..n-1 */
    if(mbedtls_mpi_cmp_int(r, 1) < 0 || mbedtls_mpi_cmp_mpi(r, &grp->N) >= 0 || mbedtls_mpi_cmp_int(s, 1) < 0 ||
       mbedtls_mpi_cmp_mpi(s, &grp->N) >= 0)
    {
        return MBEDTLS_ERR_ECP_VERIFY_FAILED;
    }
    uint8_t key[ECC_MAX_OPERAND_SZ * 2 + 1];
    uint8_t aligned_key[ECC_MAX_OPERAND_SZ * 2] __attribute__((aligned));
    size_t olen;
    MBEDTLS_MPI_CHK(mbedtls_ecp_point_write_binary(grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, key, sizeof(key)));
    memcpy(aligned_key, &key[1], sizeof(aligned_key));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(r, signature, ECC_MAX_OPERAND_SZ));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(s, signature + ECC_MAX_OPERAND_SZ, ECC_MAX_OPERAND_SZ));

    struct esec_ecc_key pkey = {.keyspec = ESEC_KEY_ECC_P256 | ESEC_KEY_PUB | ESEC_KEY_PERMS_SIGN,
                                .storage = ESEC_STORAGE_HOST(aligned_key)};

    ret = esec_ecdsa_verify_digest(pkey, buf, blen, signature);

    if(ret == ESEC_INVALID_SIGNATURE)
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        // delete key
        goto cleanup;
    }
    else if(ret != 0)
    {
        ret = MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }
cleanup:
    return ret;
}

#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#if defined(MBEDTLS_ECDSA_SIGN_ALT)

#define CRYPTOLIB_ECDSA_RETRY 9999

int mbedtls_ecdsa_sign(mbedtls_ecp_group* grp, mbedtls_mpi* r, mbedtls_mpi* s, const mbedtls_mpi* d,
                       const unsigned char* buf, size_t blen, int (*f_rng)(void*, unsigned char*, size_t), void* p_rng)
{
    int ret;
    uint8_t signature[ECC_MAX_OPERAND_SZ * 2];
    uint8_t key[ECC_MAX_OPERAND_SZ];

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if(grp->N.MBEDTLS_PRIVATE(p) == NULL)
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    if(grp->id != MBEDTLS_ECP_DP_SECP256R1)
    {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    /* Make sure d is in range 1..n-1 */
    if(mbedtls_mpi_cmp_int(d, 1) < 0 || mbedtls_mpi_cmp_mpi(d, &grp->N) >= 0)
        return (MBEDTLS_ERR_ECP_INVALID_KEY);

    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(d, key, ECC_MAX_OPERAND_SZ));

    struct esec_ecc_key pkey = {.keyspec = ESEC_KEY_ECC_P256 | ESEC_KEY_PRIV | ESEC_KEY_PERMS_SIGN,
                                .storage = ESEC_STORAGE_HOST(key)};

    ret = esec_ecdsa_sign_digest(pkey, buf, blen, signature);
    if(ret)
    {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(r, signature, ECC_MAX_OPERAND_SZ));
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(s, signature + ECC_MAX_OPERAND_SZ, ECC_MAX_OPERAND_SZ));

cleanup:
    return (ret);
}

#endif

#endif /* MBEDTLS_ECDSA_C */
