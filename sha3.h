#ifndef SHA3_H
#define SHA3_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

void sha3_224(const uint8_t *m, size_t m_len, uint8_t dst[static 28]);
void sha3_256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);
void sha3_384(const uint8_t *m, size_t m_len, uint8_t dst[static 48]);
void sha3_512(const uint8_t *m, size_t m_len, uint8_t dst[static 64]);

void shake128(const uint8_t *m, size_t m_len, uint8_t dst[static 16]);
void shake256(const uint8_t *m, size_t m_len, uint8_t dst[static 32]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SHA3_H */
