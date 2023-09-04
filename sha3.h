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

// Internal XOF state
typedef struct {
  size_t num_bytes; // number of bytes absorbed
  sha3_state_t a; // internal state
  _Bool squeezing; // mode (absorbing or squeezing)
} sha3_xof_t;

/**
 * Hash input message in input buffer `m` of length `m_len` bytes with
 * SHA3-224 (FIPS 202, section 6.1) and write 28 bytes of output to
 * destination buffer `dst`.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 28 bytes in length.
 */
void sha3_224(const uint8_t *m, size_t m_len, uint8_t dst[static 28]);

/**
 * Hash input message in input buffer `m` of length `m_len` bytes with
 * SHA3-256 (FIPS 202, section 6.1) and write 32 bytes of output to
 * destination buffer `dst`.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 32 bytes in length.
 */
void sha3_256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);

/**
 * Hash input message in input buffer `m` of length `m_len` bytes with
 * SHA3-384 (FIPS 202, section 6.1) and write 48 bytes of output to
 * destination buffer `dst`.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 48 bytes in length.
 */
void sha3_384(const uint8_t *m, size_t m_len, uint8_t dst[static 48]);

/**
 * Hash input message in input buffer `m` of length `m_len` bytes with
 * SHA3-512 (FIPS 202, section 6.1) and write 64 bytes of output to
 * destination buffer `dst`.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 48 bytes in length.
 */
void sha3_512(const uint8_t *m, size_t m_len, uint8_t dst[static 64]);

/**
 * Hash input message in buffer `m` of length `m_len` bytes with
 * SHAKE128 (FIPS 202, section 6.2) and write 16 bytes of output to
 * destination buffer `dst`.
 *
 * @param[in] m Input message.
 * @param[in] m_len Input message length, in bytes.
 * @param[out] dst Destination array.  Must be at least 16 bytes in length.
 */
void shake128(const uint8_t *m, size_t m_len, uint8_t dst[static 16]);

/**
 * Hash input message in buffer `m` of length `m_len` bytes with
 * SHAKE256 (FIPS 202, section 6.2) and write the result to output
 * buffer `dst`.
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
 * Absorb input data in `m` of length `len` bytes into SHAKE128 XOF
 * context `xof`.  Can be called iteratively to absorb input data in
 * chunks.
 *
 * @param[in/out] xof SHAKE128 XOF context.
 * @param[in] m Input data.
 * @param[in] len Input data length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool shake128_xof_absorb(sha3_xof_t *xof, const uint8_t *m, const size_t len);

/**
 * Squeeze `dst_len` bytes data into output buffer `dst` from SHAKE128
 * XOF context `xof`.  Can be called iteratively to squeeze output data
 * in chunks.
 *
 * @param[in/out] xof SHAKE128 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake128_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t dst_len);

/**
 * Absorb data in buffer `src` of length `src_len` bytes into SHAKE128
 * XOF context, then squeeze `dst_len` bytes of output into destination
 * buffer `dst`.
 *
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake128_xof_once(const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

/**
 * Initialize SHAKE256 extendable-output function (XOF) context.
 *
 * @param[out] xof SHAKE256 XOF context.
 */
void shake256_xof_init(sha3_xof_t *xof);

/**
 * Absorb input data in `m` of length `len` bytes into SHAKE256 XOF
 * context `xof`.  Can be called iteratively to absorb input data in
 * chunks.
 *
 * @param[in/out] xof SHAKE256 XOF context.
 * @param[in] m Input data.
 * @param[in] len Input data length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool shake256_xof_absorb(sha3_xof_t *xof, const uint8_t *m, const size_t len);

/**
 * Squeeze `dst_len` bytes data into output buffer `dst` from SHAKE256
 * XOF context `xof`.  Can be called iteratively to squeeze output data
 * in chunks.
 *
 * @param[in/out] xof SHAKE256 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake256_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t dst_len);

/**
 * Absorb data in buffer `src` of length `src_len` bytes into SHAKE256
 * XOF context, then squeeze `dst_len` bytes of output into destination
 * buffer `dst`.
 *
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void shake256_xof_once(const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

// cSHAKE parameters.
typedef struct {
  const uint8_t *name; // NIST function name
  const size_t name_len; // length of NIST function name, in bytes
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} cshake_params_t;

/**
 * Initialize internal cSHAKE128 (customizable SHAKE128, as defined in
 * section 3 of NIST SP 800-185) context with customization parameters
 * `params`, absorb data in buffer `src` of length `src_len` bytes into
 * internal context, then squeeze `dst_len` bytes of output into
 * destination buffer `dst`.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[in] params cSHAKE customization parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void cshake128(const cshake_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

/**
 * Initialize internal cSHAKE256 (customizable SHAKE256, as defined in
 * section 3 of NIST SP 800-185) context with customization parameters
 * `params`, absorb data in buffer `src` of length `src_len` bytes into
 * internal context, then squeeze `dst_len` bytes of output into
 * destination buffer `dst`.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[in] params cSHAKE customization parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void cshake256(const cshake_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

/**
 * Initialize cSHAKE128 (customizable SHAKE128, as defined in section 3 of
 * NIST SP 800-185) XOF context with customization parameters `params`.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[out] xof cSHAKE128 context.
 * @param[in] params cSHAKE128 customization parameters.
 */
void cshake128_xof_init(sha3_xof_t *xof, const cshake_params_t params);

/**
 * Absorb data in buffer `src` of length `len` bytes into cSHAKE128 XOF
 * context `xof`.  Can be called iteratively to absorb input data in
 * chunks.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[in/out] xof cSHAKE128 context.
 * @param[in] msg Input data buffer.
 * @param[in] len Input data buffer length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool cshake128_xof_absorb(sha3_xof_t *xof, const uint8_t *src, const size_t len);

/**
 * Squeeze `dst_len` bytes data into output buffer `dst` from cSHAKE128
 * XOF context `xof`.  Can be called iteratively to squeeze output data
 * in chunks.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[in/out] xof cSHAKE128 context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void cshake128_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t len);

/**
 * Initialize cSHAKE256 (customizable SHAKE256, as defined in section 3 of
 * NIST SP 800-185) XOF context with customization parameters `params`.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[out] xof cSHAKE256 context.
 * @param[in] params cSHAKE256 customization parameters.
 */
void cshake256_xof_init(sha3_xof_t *xof, const cshake_params_t params);

/**
 * Absorb data in buffer `src` of length `len` bytes into cSHAKE256 XOF
 * context `xof`.  Can be called iteratively to absorb input data in
 * chunks.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[in/out] xof cSHAKE256 context.
 * @param[in] msg Input data buffer.
 * @param[in] len Input data buffer length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool cshake256_xof_absorb(sha3_xof_t *xof, const uint8_t *src, const size_t len);

/**
 * Squeeze `dst_len` bytes data into output buffer `dst` from cSHAKE256
 * XOF context `xof`.  Can be called iteratively to squeeze output data
 * in chunks.
 *
 * Note: cSHAKE is used to implement the hash and extendable output
 * functions (XOF) defined in NIST SP 800-185 and should generally not
 * be used directly.
 *
 * @param[in/out] xof cSHAKE256 context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void cshake256_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t len);

// KMAC configuration parameters (key and customization string).
typedef struct {
  const uint8_t *key; // key string
  const size_t key_len; // length of key string, in bytes
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} kmac_params_t;

/**
 * Initialize internal KMAC128 (Keccak Message Authentication Code, as
 * defined in section 4 of NIST SP 800-185) context with configuration
 * parameters `params`, absorb data in buffer `src` of length `src_len`
 * bytes into internal context, then squeeze `dst_len` bytes of output
 * into destination buffer `dst`.
 *
 * @param[in] params KMAC configuration parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void kmac128(const kmac_params_t params, const uint8_t *msg, const size_t msg_len, uint8_t *dst, const size_t dst_len);

/**
 * Initialize internal KMAC256 (Keccak Message Authentication Code, as
 * defined in section 4 of NIST SP 800-185) XOF context with
 * configuration parameters `params`, absorb data in buffer `src` of
 * length `src_len` bytes into internal context, then squeeze `dst_len`
 * bytes of output into destination buffer `dst`.
 *
 * @param[in] params KMAC configuration parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void kmac256(const kmac_params_t params, const uint8_t *msg, const size_t msg_len, uint8_t *dst, const size_t dst_len);

/**
 * Initialize KMAC128 XOF (Keccak Message Authentication Code eXtendable
 * Output Function, as defined in section 4.3.1 of NIST SP 800-185)
 * context with configuration parameters `params`.
 *
 * Note: KMAC128 and KMAC128 XOF produce different output, because
 * KMAC128 encodes the fixed output size as part of the input while
 * KMAC128 XOF does not.  See section 4.3.1 of NIST SP 800-185 for
 * details.
 *
 * @param[out] xof KMAC128 XOF context.
 * @param[in] params KMAC configuration parameters.
 */
void kmac128_xof_init(sha3_xof_t *xof, const kmac_params_t params);

/**
 * Absorb data in buffer `src` of length `len` bytes into KMAC128 XOF
 * context.  Can be called iteratively to absorb input data in chunks.
 *
 * @param[in/out] xof KMAC128 XOF context.
 * @param[in] src Input data buffer.
 * @param[in] len Input data buffer length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool kmac128_xof_absorb(sha3_xof_t *xof, const uint8_t *src, const size_t len);

/**
 * Squeeze `len` bytes data into output buffer `dst` from KMAC128 XOF
 * context `xof`.  Can be called iteratively to squeeze output data in
 * chunks.
 *
 * Note: KMAC128 and KMAC128 XOF produce different output, because
 * KMAC128 encodes the fixed output size as part of the input while
 * KMAC128 XOF does not.  See section 4.3.1 of NIST SP 800-185 for
 * details.
 *
 * @param[in/out] xof KMAC128 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void kmac128_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t len);

/**
 * Initialize internal KMAC128 XOF (Keccak Message Authentication Code
 * eXtendable Output Function, as defined in section 4 of NIST SP
 * 800-185) context with configuration parameters `params`, absorb data
 * in buffer `src` of length `src_len` bytes into internal context, then
 * squeeze `dst_len` bytes of output into destination buffer `dst`.
 *
 * Note: KMAC128 and KMAC128 XOF produce different output, because
 * KMAC128 encodes the fixed output size as part of the input while
 * KMAC128 XOF does not.  See section 4.3.1 of NIST SP 800-185 for
 * details.
 *
 * @param[in] params KMAC configuration parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void kmac128_xof_once(const kmac_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

/**
 * Initialize KMAC256 XOF (Keccak Message Authentication Code eXtendable
 * Output Function, as defined in section 4.3.1 of NIST SP 800-185)
 * context with configuration parameters `params`.
 *
 * Note: KMAC256 and KMAC256 XOF produce different output, because
 * KMAC256 encodes the fixed output size as part of the input while
 * KMAC256 XOF does not.  See section 4.3.1 of NIST SP 800-185 for
 * details.
 *
 * @param[out] xof KMAC256 XOF context.
 * @param[in] params KMAC configuration parameters.
 */
void kmac256_xof_init(sha3_xof_t *xof, const kmac_params_t params);

/**
 * Absorb data in buffer `src` of length `len` bytes into KMAC256 XOF
 * context.  Can be called iteratively to absorb input data in chunks.
 *
 * @param[in/out] xof KMAC256 XOF context.
 * @param[in] src Input data buffer.
 * @param[in] len Input data buffer length, in bytes.
 *
 * @return True if data was absorbed, and false otherwise (e.g., if context has already been squeezed).
 */
_Bool kmac256_xof_absorb(sha3_xof_t *xof, const uint8_t *msg, const size_t len);

/**
 * Squeeze `len` bytes data into output buffer `dst` from KMAC256 XOF
 * context `xof`.  Can be called iteratively to squeeze output data in
 * chunks.
 *
 * Note: KMAC256 and KMAC256 XOF produce different output, because
 * KMAC256 encodes the fixed output size as part of the input while
 * KMAC256 XOF does not.  See section 4.3.1 of NIST SP 800-185 for
 * details.
 *
 * @param[in/out] xof KMAC256 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void kmac256_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t len);

/**
 * Initialize internal KMAC256 XOF (Keccak Message Authentication Code
 * eXtendable Output Function, as defined in section 4 of NIST SP
 * 800-185) context with configuration parameters `params`, absorb data
 * in buffer `src` of length `src_len` bytes into internal context, then
 * squeeze `dst_len` bytes of output into destination buffer `dst`.
 *
 * Note: KMAC256 and KMAC256 XOF produce different output, because
 * KMAC256 encodes the fixed output size as part of the input while
 * KMAC256 XOF does not.  See section 4.3.1 of NIST SP 800-185 for
 * details.
 *
 * @param[in] params KMAC configuration parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void kmac256_xof_once(const kmac_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

// TupleHash tuple element.
typedef struct {
  const uint8_t *ptr; // pointer to byte string
  size_t len; // byte string length, in bytes.
} tuplehash_str_t;

// TupleHash configuration parameters.
typedef struct {
  const tuplehash_str_t *strs; // strings
  const size_t num_strs; // number of strings
  const uint8_t *custom; // customization string
  const size_t custom_len; // length of customization string, in bytes
} tuplehash_params_t;

/**
 * Initialize internal TupleHash128 (NIST SP 800-185, section 5) context
 * with configuration parameters `params`, then squeeze `dst_len` bytes
 * of output from internal context into destination buffer `dst`.
 *
 * @param[in] params TupleHash128 configuration parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void tuplehash128(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);

/**
 * Initialize internal TupleHash256 (NIST SP 800-185, section 5) context
 * with configuration parameters `params`, then squeeze `dst_len` bytes
 * of output from internal context into destination buffer `dst`.
 *
 * @param[in] params TupleHash256 configuration parameters.
 * @param[in] src Input data buffer.
 * @param[in] src_len Input data buffer length, in bytes.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void tuplehash256(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);

/**
 * Initialize TupleHash128 XOF (TupleHash eXtendable Output Function, as
 * defined in section 5.3.1 of NIST SP 800-185) context with
 * configuration parameters `params`.
 *
 * Note: TupleHash128 and TupleHash128 XOF produce different output,
 * because TupleHash128 encodes the fixed output size as part of the
 * input while TupleHash128 XOF does not.  See section 5.3.1 of NIST SP
 * 800-185 for details.
 *
 * @param[out] xof TupleHash128 XOF context.
 * @param[in] params TupleHash configuration parameters.
 */
void tuplehash128_xof_init(sha3_xof_t *xof, const tuplehash_params_t params);

/**
 * Squeeze `len` bytes data into output buffer `dst` from TupleHash128
 * XOF context `xof`.  Can be called iteratively to squeeze output data
 * in chunks.
 *
 * Note: TupleHash128 and TupleHash128 XOF produce different output,
 * because TupleHash128 encodes the fixed output size as part of the
 * input while TupleHash128 XOF does not.  See section 5.3.1 of NIST SP
 * 800-185 for details.
 *
 * @param[in/out] xof TupleHash128 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void tuplehash128_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t len);

/**
 * Initialize internal TupleHash128 XOF (TupleHash eXtendable Output
 * Function, as defined in section 5 of NIST SP 800-185) context with
 * configuration parameters `params`, then squeeze `dst_len` bytes of
 * output into destination buffer `dst`.
 *
 * Note: TupleHash128 and TupleHash128 XOF produce different output,
 * because TupleHash128 encodes the fixed output size as part of the
 * input while TupleHash128 XOF does not.  See section 5.3.1 of NIST SP
 * 800-185 for details.
 *
 * @param[in] params TupleHash128 configuration parameters.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void tuplehash128_xof_once(const tuplehash_params_t params, uint8_t *dst, const size_t dst_len);

/**
 * Initialize TupleHash256 XOF (TupleHash eXtendable Output Function, as
 * defined in section 5.3.1 of NIST SP 800-185) context with
 * configuration parameters `params`.
 *
 * Note: TupleHash256 and TupleHash256 XOF produce different output,
 * because TupleHash256 encodes the fixed output size as part of the
 * input while TupleHash256 XOF does not.  See section 5.3.1 of NIST SP
 * 800-185 for details.
 *
 * @param[out] xof TupleHash256 XOF context.
 * @param[in] params TupleHash configuration parameters.
 */
void tuplehash256_xof_init(sha3_xof_t *xof, const tuplehash_params_t params);

/**
 * Squeeze `len` bytes data into output buffer `dst` from TupleHash256
 * XOF context `xof`.  Can be called iteratively to squeeze output data
 * in chunks.
 *
 * Note: TupleHash256 and TupleHash256 XOF produce different output,
 * because TupleHash256 encodes the fixed output size as part of the
 * input while TupleHash256 XOF does not.  See section 5.3.1 of NIST SP
 * 800-185 for details.
 *
 * @param[in/out] xof TupleHash256 XOF context.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void tuplehash256_xof_squeeze(sha3_xof_t *xof, uint8_t *dst, const size_t len);

/**
 * Initialize internal TupleHash256 XOF (TupleHash eXtendable Output
 * Function, as defined in section 5 of NIST SP 800-185) context with
 * configuration parameters `params`, then squeeze `dst_len` bytes of
 * output into destination buffer `dst`.
 *
 * Note: TupleHash256 and TupleHash256 XOF produce different output,
 * because TupleHash256 encodes the fixed output size as part of the
 * input while TupleHash256 XOF does not.  See section 5.3.1 of NIST SP
 * 800-185 for details.
 *
 * @param[in] params TupleHash256 configuration parameters.
 * @param[out] dst Destination buffer.
 * @param[in] len Destination buffer length, in bytes.
 */
void tuplehash256_xof_once(const tuplehash_params_t params, uint8_t *dst, const size_t len);

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

void parallelhash128(const parallelhash_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);
void parallelhash256(const parallelhash_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

void parallelhash128_xof_init(parallelhash_t *hash, const parallelhash_params_t params);
void parallelhash128_xof_absorb(parallelhash_t *hash, const uint8_t *msg, const size_t msg_len);
void parallelhash128_xof_squeeze(parallelhash_t *hash, uint8_t *dst, const size_t dst_len);
void parallelhash128_xof_once(const parallelhash_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

void parallelhash256_xof_init(parallelhash_t *hash, const parallelhash_params_t params);
void parallelhash256_xof_absorb(parallelhash_t *hash, const uint8_t *msg, const size_t msg_len);
void parallelhash256_xof_squeeze(parallelhash_t *hash, uint8_t *dst, const size_t dst_len);
void parallelhash256_xof_once(const parallelhash_params_t params, const uint8_t *src, const size_t src_len, uint8_t *dst, const size_t dst_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SHA3_H */
