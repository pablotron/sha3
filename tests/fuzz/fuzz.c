#include "sha3.h"

int LLVMFuzzerTestOneInput(const uint8_t *src, size_t len) {
  uint8_t dst[128];
  shake128_xof_once(src, len, dst, sizeof(dst));
  return 0;
}
