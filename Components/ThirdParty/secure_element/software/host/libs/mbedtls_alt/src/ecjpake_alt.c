/*
 *  Elliptic curve J-PAKE
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
 * References in the code are to the Thread v1.0 Specification,
 * available to members of the Thread Group http://threadgroup.org/
 */

#include "common.h"

#if defined(MBEDTLS_ECJPAKE_C)

#include "mbedtls/ecjpake.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"
/* We use MD first if it's available (for compatibility reasons)
 * and "fall back" to PSA otherwise (which needs psa_crypto_init()). */
#if !defined(MBEDTLS_MD_C)
#include "psa/crypto.h"
#include "mbedtls/psa_util.h"
#endif /* !MBEDTLS_MD_C */

#include "hash_info.h"
#include "esecure.h"
#include "hal_SEMailbox.h"

#if defined(MBEDTLS_ECJPAKE_ALT)

#define ECP256_ELEMENT_SIZE  32
#define ECP256_POINT_SIZE  (ECP256_ELEMENT_SIZE * 2)
#define ECP256_ZKP_SIZE  (ECP256_ELEMENT_SIZE * 3)
/*
 * Convert a mbedtls_ecjpake_role to identifier string
 */
static const char* const ecjpake_id[] = {"client", "server"};

#define ID_MINE (ecjpake_id[ctx->role])
#define ID_PEER (ecjpake_id[1 - ctx->role])

/*
 * Initialize context
 */
void mbedtls_ecjpake_init(mbedtls_ecjpake_context* ctx)
{
    ctx->md_type = MBEDTLS_MD_NONE;
    mbedtls_ecp_group_init(&ctx->grp);
    ctx->point_format = MBEDTLS_ECP_PF_UNCOMPRESSED;

    mbedtls_ecp_point_init(&ctx->Xm1);
    mbedtls_ecp_point_init(&ctx->Xm2);
    mbedtls_ecp_point_init(&ctx->Xp1);
    mbedtls_ecp_point_init(&ctx->Xp2);
    mbedtls_ecp_point_init(&ctx->Xp);

    mbedtls_mpi_init(&ctx->xm1);
    mbedtls_mpi_init(&ctx->xm2);
    mbedtls_mpi_init(&ctx->s);
}

/*
 * Free context
 */
void mbedtls_ecjpake_free(mbedtls_ecjpake_context* ctx)
{
    if(ctx == NULL)
        return;

    ctx->md_type = MBEDTLS_MD_NONE;
    mbedtls_ecp_group_free(&ctx->grp);

    mbedtls_ecp_point_free(&ctx->Xm1);
    mbedtls_ecp_point_free(&ctx->Xm2);
    mbedtls_ecp_point_free(&ctx->Xp1);
    mbedtls_ecp_point_free(&ctx->Xp2);
    mbedtls_ecp_point_free(&ctx->Xp);

    mbedtls_mpi_free(&ctx->xm1);
    mbedtls_mpi_free(&ctx->xm2);
    mbedtls_mpi_free(&ctx->s);
}

/*
 * Setup context
 */
int mbedtls_ecjpake_setup(mbedtls_ecjpake_context* ctx, mbedtls_ecjpake_role role, mbedtls_md_type_t hash,
                          mbedtls_ecp_group_id curve, const unsigned char* secret, size_t len)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if(role != MBEDTLS_ECJPAKE_CLIENT && role != MBEDTLS_ECJPAKE_SERVER)
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    if(hash != MBEDTLS_MD_SHA256 || curve != MBEDTLS_ECP_DP_SECP256R1)
    {
        // ALT implementation only supports SHA256 and SECP256R1
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

    ctx->role = role;

#if defined(MBEDTLS_MD_C)
    if((mbedtls_md_info_from_type(hash)) == NULL)
        return (MBEDTLS_ERR_MD_FEATURE_UNAVAILABLE);
#else
    if(mbedtls_psa_translate_md(hash) == MBEDTLS_MD_NONE)
        return (MBEDTLS_ERR_MD_FEATURE_UNAVAILABLE);
#endif

    ctx->md_type = hash;

    MBEDTLS_MPI_CHK(mbedtls_ecp_group_load(&ctx->grp, curve));

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->s, secret, len));

cleanup:
    if(ret != 0)
        mbedtls_ecjpake_free(ctx);

    return (ret);
}

/*
 * Check if context is ready for use
 */
int mbedtls_ecjpake_check(const mbedtls_ecjpake_context* ctx)
{
    if(ctx->md_type == MBEDTLS_MD_NONE || ctx->grp.id == MBEDTLS_ECP_DP_NONE || ctx->s.p == NULL)
    {
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

    return (0);
}

void set_esec_jpake_cfg(const mbedtls_ecjpake_context* ctx, struct esec_jpake_cfg* jpake_cfg)
{
    jpake_cfg->user_id1 = (const uint8_t*)ID_MINE;
    jpake_cfg->idlen1 = strlen(ID_MINE);
    jpake_cfg->user_id2 = (const uint8_t*)ID_PEER;
    jpake_cfg->idlen2 = strlen(ID_PEER);
}

int unpack_esec_ecp_pt_buf_to_mbedtls_ecp_pt(mbedtls_ecp_point* P, const uint8_t* esec_buf)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&P->X, esec_buf, ECP256_ELEMENT_SIZE));
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&P->Y, esec_buf + ECP256_ELEMENT_SIZE, ECP256_ELEMENT_SIZE));
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&P->Z, 1));

cleanup:
    return ret;
}

int pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(const mbedtls_ecp_point* P, uint8_t* esec_buf)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&P->X, esec_buf, ECP256_ELEMENT_SIZE));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&P->Y, esec_buf + ECP256_ELEMENT_SIZE, ECP256_ELEMENT_SIZE));
cleanup:
    return ret;
}

/* zkp = (V,r) where v is a point on curve and r is an integer */
/* This function extracts V and r given esecure zkp buffer */
static int unpack_V_r_from_esec_zkp(mbedtls_ecp_point* V, mbedtls_mpi* r, uint8_t *esec_zkp)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK(unpack_esec_ecp_pt_buf_to_mbedtls_ecp_pt(V, esec_zkp));
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(r, esec_zkp + ECP256_POINT_SIZE, ECP256_ELEMENT_SIZE));
cleanup:
    return ret;
}

static int pack_V_r_into_esec_zkp(mbedtls_ecp_point* V, mbedtls_mpi* r, uint8_t *esec_zkp)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(V, esec_zkp));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(r, esec_zkp + ECP256_POINT_SIZE, ECP256_ELEMENT_SIZE));
cleanup:
    return ret;
}

static int pack_X_V_r_to_mbedtls_buffer(const mbedtls_ecp_group* grp, const int pf, const mbedtls_ecp_point* X,
                                       const mbedtls_ecp_point* V, const mbedtls_mpi* r, uint8_t** p,
                                       const uint8_t* end)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t wlen;

    /* Use mbedtls APIs to write to buffer */
    /* store X into buffer */
    MBEDTLS_MPI_CHK(mbedtls_ecp_tls_write_point(grp, X, pf, &wlen, *p, end - *p));
    *p += wlen;

    /* store V into buffer */
    MBEDTLS_MPI_CHK(mbedtls_ecp_tls_write_point(grp, V, pf, &wlen, *p, end - *p));
    *p += wlen;

    /* store r into buffer */
    *(*p) = ECP256_ELEMENT_SIZE;
    *p += 1;
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(r, *p, ECP256_ELEMENT_SIZE));
    (*p) += ECP256_ELEMENT_SIZE;

cleanup:
    return ret;
}

static int unpack_X_V_r_from_mbedtls_buffer(const mbedtls_ecp_group* grp, mbedtls_ecp_point* X,
                                         mbedtls_ecp_point* V, mbedtls_mpi* r, const uint8_t** p,
                                         const uint8_t* end)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t rlen;
    MBEDTLS_MPI_CHK(mbedtls_ecp_tls_read_point(grp, X, p, end - *p));
    MBEDTLS_MPI_CHK(mbedtls_ecp_tls_read_point(grp, V, p, end - *p));
    rlen = *(*p); (*p)++;
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(r, *p, rlen));
    *p += rlen;
cleanup:
    return ret;
}
/*
 * Generate and write the first round message
 */
int mbedtls_ecjpake_write_round_one( mbedtls_ecjpake_context *ctx,
                            unsigned char *buf, size_t len, size_t *olen,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    uint32_t status;
    uint8_t x2[ECP256_ELEMENT_SIZE];
    uint8_t X1[ECP256_POINT_SIZE];
    uint8_t X2[ECP256_POINT_SIZE];
    uint8_t zkp_a[ECP256_ZKP_SIZE];
    uint8_t zkp_b[ECP256_ZKP_SIZE];
    struct esec_jpake_cfg jpake_cfg;
    /* zkp = (V,r) where v is a point on curve and r is an integer */
    mbedtls_mpi r;
    mbedtls_ecp_point V;

    mbedtls_mpi_init(&r);
    mbedtls_ecp_point_init(&V);

    unsigned char* p = buf;
    const unsigned char* end = buf + len;

    set_esec_jpake_cfg(ctx, &jpake_cfg);

    status = esec_ecjpake_generate_r1(&jpake_cfg, x2, X1, zkp_a, X2, zkp_b);
    if(status != ESEC_OKAY)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }

    /* Set mbetls ecjpake context variables */
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->xm2, x2, sizeof(x2)));
    MBEDTLS_MPI_CHK(unpack_esec_ecp_pt_buf_to_mbedtls_ecp_pt(&ctx->Xm1, X1));
    MBEDTLS_MPI_CHK(unpack_esec_ecp_pt_buf_to_mbedtls_ecp_pt(&ctx->Xm2, X2));

    /* Pack buffer */
    MBEDTLS_MPI_CHK(unpack_V_r_from_esec_zkp(&V, &r, zkp_a));
    MBEDTLS_MPI_CHK(pack_X_V_r_to_mbedtls_buffer(&ctx->grp, ctx->point_format, &ctx->Xm1, &V, &r, &p, end));
    MBEDTLS_MPI_CHK(unpack_V_r_from_esec_zkp(&V, &r, zkp_b));
    MBEDTLS_MPI_CHK(pack_X_V_r_to_mbedtls_buffer(&ctx->grp, ctx->point_format, &ctx->Xm2, &V, &r, &p, end));
    *olen = p - buf;

cleanup:
    mbedtls_mpi_free(&r);
    mbedtls_ecp_point_free(&V);
    return ret;
}

/*
 * Read and process the first round message
 */
int mbedtls_ecjpake_read_round_one( mbedtls_ecjpake_context *ctx,
                                    const unsigned char *buf,
                                    size_t len )
{
    int ret;
    uint32_t status;

    uint8_t esec_zkp_a[ECP256_POINT_SIZE + ECP256_ELEMENT_SIZE];
    uint8_t esec_zkp_b[ECP256_POINT_SIZE + ECP256_ELEMENT_SIZE];
    uint8_t X3[ECP256_POINT_SIZE];
    uint8_t X4[ECP256_POINT_SIZE];

    mbedtls_ecp_point V;
    mbedtls_mpi r;
    mbedtls_ecp_point_init(&V);
    mbedtls_mpi_init(&r);

    const unsigned char *p = buf;
    const unsigned char *end = buf + len;

    struct esec_jpake_cfg jpake_cfg;
    set_esec_jpake_cfg(ctx, &jpake_cfg);

    MBEDTLS_MPI_CHK(unpack_X_V_r_from_mbedtls_buffer(&ctx->grp, &ctx->Xp1, &V, &r, &p, end));
    MBEDTLS_MPI_CHK(pack_V_r_into_esec_zkp(&V, &r, esec_zkp_a));
    MBEDTLS_MPI_CHK(unpack_X_V_r_from_mbedtls_buffer(&ctx->grp, &ctx->Xp2, &V, &r, &p, end));
    MBEDTLS_MPI_CHK(pack_V_r_into_esec_zkp(&V, &r, esec_zkp_b));

    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp1, X3));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp2, X4));

    status = esec_ecjpake_verify_r1(&jpake_cfg, X3, esec_zkp_a, X4, esec_zkp_b);
    if(status != ESEC_OKAY)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free(&V);
    mbedtls_mpi_free(&r);
    return ret;
}


/*
 * Generate and write the second round message (S: 7.4.2.5, C: 7.4.2.6)
 */
int mbedtls_ecjpake_write_round_two( mbedtls_ecjpake_context *ctx,
                            unsigned char *buf, size_t len, size_t *olen,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng )
{
    (void)f_rng;
    (void)p_rng;
    int ret;
    uint32_t status;
    uint8_t x2[ECP256_ELEMENT_SIZE];
    uint8_t X1[ECP256_POINT_SIZE];
    uint8_t X3[ECP256_POINT_SIZE];
    uint8_t X4[ECP256_POINT_SIZE];
    uint8_t A[ECP256_POINT_SIZE];
    uint8_t zkp_a[ECP256_ZKP_SIZE];
    uint8_t pwd[ECP256_ELEMENT_SIZE];
    size_t ec_len;
    struct esec_jpake_cfg jpake_cfg;
    mbedtls_ecp_point Xm;
    mbedtls_mpi r;
    mbedtls_ecp_point V;

    mbedtls_ecp_point_init(&Xm);
    mbedtls_mpi_init(&r);
    mbedtls_ecp_point_init(&V);

    set_esec_jpake_cfg(ctx, &jpake_cfg);

    /* map mbedtls ctx variables to esec mailbox input */
    mbedtls_mpi_write_binary(&ctx->s, pwd, sizeof(pwd));
    mbedtls_mpi_write_binary(&ctx->xm2, x2, sizeof(x2));
    pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xm1, X1);
    pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp1, X3);
    pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp2, X4);

    status = esec_ecjpake_generate_r2(&jpake_cfg,
        pwd, sizeof(pwd),
        x2, X1, X3, X4,
        A, zkp_a);

    if(status != ESEC_OKAY)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }

    unsigned char* p = buf;
    const unsigned char* end = buf + len;

    /* write it out */
    if(ctx->role == MBEDTLS_ECJPAKE_SERVER)
    {
        if(end < p)
        {
            ret = MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
            goto cleanup;
        }
        MBEDTLS_MPI_CHK(mbedtls_ecp_tls_write_group(&ctx->grp, &ec_len, p, end - p));
        p += ec_len;
    }

    if(end < p)
    {
        ret = MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
        goto cleanup;
    }

    MBEDTLS_MPI_CHK(unpack_esec_ecp_pt_buf_to_mbedtls_ecp_pt(&Xm, A));
    MBEDTLS_MPI_CHK(unpack_V_r_from_esec_zkp(&V, &r, zkp_a));
    MBEDTLS_MPI_CHK(pack_X_V_r_to_mbedtls_buffer(&ctx->grp, ctx->point_format, &Xm, &V, &r, &p, end));

    *olen = p - buf;

cleanup:
    mbedtls_ecp_point_free(&Xm);
    mbedtls_ecp_point_free(&V);
    mbedtls_mpi_free(&r);
    return ret;
}

/*
 * Read and process second round message (C: 7.4.2.5, S: 7.4.2.6)
 */
int mbedtls_ecjpake_read_round_two(mbedtls_ecjpake_context* ctx, const unsigned char* buf, size_t len)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const unsigned char* p = buf;
    const unsigned char* end = buf + len;
    uint32_t status;
    mbedtls_ecp_group grp;
    mbedtls_ecp_point V;
    mbedtls_mpi r;

    uint8_t x2[ECP256_ELEMENT_SIZE];
    uint8_t X1[ECP256_POINT_SIZE];
    uint8_t X2[ECP256_POINT_SIZE];
    uint8_t X3[ECP256_POINT_SIZE];
    uint8_t B[ECP256_POINT_SIZE];
    uint8_t zkp_b[ECP256_ZKP_SIZE];

    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&V);
    mbedtls_mpi_init(&r);

    struct esec_jpake_cfg jpake_cfg;
    set_esec_jpake_cfg(ctx, &jpake_cfg);

    if(ctx->role == MBEDTLS_ECJPAKE_CLIENT)
    {
        MBEDTLS_MPI_CHK(mbedtls_ecp_tls_read_group(&grp, &p, len));
        if(grp.id != ctx->grp.id)
        {
            ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
            goto cleanup;
        }
    }

    MBEDTLS_MPI_CHK(unpack_X_V_r_from_mbedtls_buffer(&ctx->grp, &ctx->Xp, &V, &r, &p, end));
    if(p != end)
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    /* populate input buffers to esec mailbox call */
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->xm2, x2, ECP256_ELEMENT_SIZE));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xm1, X1));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xm2, X2));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp1, X3));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp, B));
    MBEDTLS_MPI_CHK(pack_V_r_into_esec_zkp(&V, &r, zkp_b));

    status = esec_ecjpake_verify_r2(&jpake_cfg, x2, X1, X2, X3, B, zkp_b);
    if(status != ESEC_OKAY)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }
cleanup:
    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&V);
    mbedtls_mpi_free(&r);
    return ret;
}

int mbedtls_ecjpake_derive_secret( mbedtls_ecjpake_context *ctx,
                            unsigned char *buf, size_t len, size_t *olen,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng )
{
    uint32_t status;
    struct esec_jpake_cfg jpake_cfg;
    uint8_t A[ECP256_POINT_SIZE];
    uint8_t pwd[ECP256_ELEMENT_SIZE];
    uint8_t x2[ECP256_ELEMENT_SIZE];
    uint8_t X2[ECP256_POINT_SIZE];
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    *olen = mbedtls_hash_info_get_size( ctx->md_type );
    if( len < *olen )
        return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

    set_esec_jpake_cfg(ctx, &jpake_cfg);
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->s, pwd, sizeof(pwd)));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->xm2, x2, sizeof(x2)));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp2, X2));
    MBEDTLS_MPI_CHK(pack_mbedtls_ecp_pt_to_esec_ecp_pt_buf(&ctx->Xp, A));

    status = esec_ecjpake_generate_sessionkey(&jpake_cfg,
      pwd, sizeof(pwd),
      x2, X2, A, buf);

    if(status != ESEC_OKAY)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }
cleanup:
    return (ret);
}

int mbedtls_ecjpake_set_point_format(mbedtls_ecjpake_context* ctx, int point_format)
{
    // Currently only MBEDTLS_ECP_PF_UNCOMPRESSED supported
    return point_format == MBEDTLS_ECP_PF_UNCOMPRESSED ? 0 : MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
}

#undef ID_MINE
#undef ID_PEER

#endif /* ! MBEDTLS_ECJPAKE_ALT */

#endif /* MBEDTLS_ECJPAKE_C */
