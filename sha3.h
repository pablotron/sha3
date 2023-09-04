/**
 * C11 implementations of SHA-3 algorithms from FIPS 202 and NIST SP
 * 800-185.
 */
#ifndef SHA3_H
#define SHA3_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

// sha3 state
typedef union {
  uint8_t u8[200];
  uint64_t u64[25];
} sha3_state_t;

// XOF state
typedef struct {
  size_t num_bytes;
  sha3_state_t a;
  _Bool squeezing;
} sha3_xof_t;

/**
 * SHA3-224, as specified in FIPS 202, section 6.1.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 28 bytes in length.
 */
void sha3_224(const uint8_t *m, size_t m_len, uint8_t dst[static 28]);

/**
 * SHA3-256, as specified in FIPS 202, section 6.1.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 32 bytes in length.
 */
void sha3_256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);

/**
 * SHA3-384, as specified in FIPS 202, section 6.1.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 48 bytes in length.
 */
void sha3_384(const uint8_t *m, size_t m_len, uint8_t dst[static 48]);

/**
 * SHA3-512, as specified in FIPS 202, section 6.1.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 48 bytes in length.
 */
void sha3_512(const uint8_t *m, size_t m_len, uint8_t dst[static 64]);

/**
 * SHAKE128, as specified in FIPS 202, section 6.2.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 16 bytes in length.
 */
void shake128(const uint8_t *m, size_t m_len, uint8_t dst[static 16]);

/**
 * SHAKE256, as specified in FIPS 202, section 6.2.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 16 bytes in length.
 */
void shake256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);

/**
 * Initialize SHAKE128 extendable-output function (XOF) context.
 *
 * @param[out] xof SHAKE128 XOF context.
 */
void shake128_xof_init(sha3_xof_t * const xof);

/**
 * Absorb data into SHAKE128 XOF context.
 *
 * @param[in] xof SHAKE128 XOF context.
 * @param[in] m Input data.
 * @param[in] len Input data length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool shake128_xof_absorb(sha3_xof_t * const xof, const uint8_t * const m, const size_t len);

/**
 * Squeeze data from SHAKE128 XOF context into output buffer.
 *
 * @param[in] xof SHAKE128 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake128_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t dst_len);

/**
 * Absorb data into SHAKE128 XOF and then squeeze result into output buffer.
 *
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake128_xof_once(const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

/**
 * Initialize SHAKE256 extendable-output function (XOF) context.
 *
 * @param[out] xof SHAKE256 XOF context.
 */
void shake256_xof_init(sha3_xof_t * const xof);

/**
 * Absorb data into SHAKE256 XOF context.
 *
 * @param[in] xof SHAKE256 XOF context.
 * @param[in] m Input data.
 * @param[in] len Input data length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool shake256_xof_absorb(sha3_xof_t * const xof, const uint8_t * const m, const size_t len);

/**
 * Squeeze data from SHAKE256 XOF context into output buffer.
 *
 * @param[in] xof SHAKE256 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake256_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t dst_len);

/**
 * Absorb data into SHAKE256 XOF and then squeeze result into output buffer.
 *
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake256_xof_once(const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

// cSHAKE parameters.
typedef struct {
  const uint8_t *name; // NIST function name
  const size_t name_len; // length of NIST function name, in bytes
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} cshake_params_t;

void cshake128(const cshake_params_t params, const uint8_t * const msg, const size_t msg_len, uint8_t * const dst, const size_t dst_len);
void cshake256(const cshake_params_t params, const uint8_t * const msg, const size_t msg_len, uint8_t * const dst, const size_t dst_len);

void cshake128_xof_init(sha3_xof_t * const xof, const cshake_params_t params);
_Bool cshake128_xof_absorb(sha3_xof_t * const xof, const uint8_t * const msg, const size_t len);
void cshake128_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t len);

void cshake256_xof_init(sha3_xof_t * const xof, const cshake_params_t params);
_Bool cshake256_xof_absorb(sha3_xof_t * const xof, const uint8_t * const msg, const size_t len);
void cshake256_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t len);

typedef struct {
  const uint8_t *key; // key string
  const size_t key_len; // length of key string, in bytes
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} kmac_params_t;

void kmac128(const kmac_params_t params, const uint8_t * const msg, const size_t msg_len, uint8_t * const dst, const size_t dst_len);
void kmac256(const kmac_params_t params, const uint8_t * const msg, const size_t msg_len, uint8_t * const dst, const size_t dst_len);

void kmac128_xof_init(sha3_xof_t * const xof, const kmac_params_t params);
_Bool kmac128_xof_absorb(sha3_xof_t * const xof, const uint8_t * const msg, const size_t len);
void kmac128_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t len);
void kmac128_xof_once(const kmac_params_t params, const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

void kmac256_xof_init(sha3_xof_t * const xof, const kmac_params_t params);
_Bool kmac256_xof_absorb(sha3_xof_t * const xof, const uint8_t * const msg, const size_t len);
void kmac256_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t len);
void kmac256_xof_once(const kmac_params_t params, const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

typedef struct {
  const uint8_t *ptr;
  size_t len;
} tuplehash_str_t;

typedef struct {
  const tuplehash_str_t *strs; // strings
  const size_t num_strs; // number of strings
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} tuplehash_params_t;

void tuplehash128(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);
void tuplehash256(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);

void tuplehash128_xof_init(sha3_xof_t * const xof, const tuplehash_params_t params);
void tuplehash128_xof_squeeze(sha3_xof_t * const xof, uint8_t *dst, const size_t dst_len);
void tuplehash128_xof_once(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);

void tuplehash256_xof_init(sha3_xof_t * const xof, const tuplehash_params_t params);
void tuplehash256_xof_squeeze(sha3_xof_t * const xof, uint8_t *dst, const size_t dst_len);
void tuplehash256_xof_once(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);

typedef struct {
  const size_t block_len; // block size, in bytes
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} parallelhash_params_t;

typedef struct {
  sha3_xof_t root_xof, // root xof
             curr_xof; // xof for current block (note: shake128, not cshake128)
  size_t ofs, // offset in current block, in bytes
         block_len, // block size, in bytes
         num_blocks; // total number of blocks
  _Bool squeezing; // current state
} parallelhash_t;

void parallelhash128(const parallelhash_params_t params, const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);
void parallelhash256(const parallelhash_params_t params, const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

void parallelhash128_xof_init(parallelhash_t *hash, const parallelhash_params_t params);
void parallelhash128_xof_absorb(parallelhash_t *hash, const uint8_t *msg, const size_t msg_len);
void parallelhash128_xof_squeeze(parallelhash_t *hash, uint8_t *dst, const size_t dst_len);
void parallelhash128_xof_once(const parallelhash_params_t params, const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

void parallelhash256_xof_init(parallelhash_t *hash, const parallelhash_params_t params);
void parallelhash256_xof_absorb(parallelhash_t *hash, const uint8_t *msg, const size_t msg_len);
void parallelhash256_xof_squeeze(parallelhash_t *hash, uint8_t *dst, const size_t dst_len);
void parallelhash256_xof_once(const parallelhash_params_t params, const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SHA3_H */
