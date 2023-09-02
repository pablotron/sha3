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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SHA3_H */
