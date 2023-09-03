#ifndef SHA3_H
#define SHA3_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

typedef union {
  uint8_t u8[200];
  uint64_t u64[25];
} sha3_state_t;

typedef struct {
  size_t num_bytes;
  sha3_state_t a;
  _Bool squeezing;
} sha3_xof_t;

void sha3_224(const uint8_t *m, size_t m_len, uint8_t dst[static 28]);
void sha3_256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);
void sha3_384(const uint8_t *m, size_t m_len, uint8_t dst[static 48]);
void sha3_512(const uint8_t *m, size_t m_len, uint8_t dst[static 64]);

void shake128(const uint8_t *m, size_t m_len, uint8_t dst[static 16]);
void shake256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);

void shake128_xof_init(sha3_xof_t * const xof);
_Bool shake128_xof_absorb(sha3_xof_t * const xof, const uint8_t * const m, const size_t len);
void shake128_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t dst_len);
void shake128_xof_once(const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

void shake256_xof_init(sha3_xof_t * const xof);
_Bool shake256_xof_absorb(sha3_xof_t * const xof, const uint8_t * const m, const size_t len);
void shake256_xof_squeeze(sha3_xof_t * const xof, uint8_t * const dst, const size_t dst_len);
void shake256_xof_once(const uint8_t * const src, const size_t src_len, uint8_t * const dst, const size_t dst_len);

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SHA3_H */
