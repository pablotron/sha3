#include <stdio.h>
#include <string.h>
#include "sha3.h"

// available functions
static const struct {
  const char *name;
  const size_t size;
  void (*func)(const uint8_t *, size_t, uint8_t *);
} fns[] = {{
  .name = "sha3-224",
  .size = 28,
  .func = sha3_224,
}, {
  .name = "sha3-256",
  .size = 32,
  .func = sha3_256,
}, {
  .name = "sha3-384",
  .size = 48,
  .func = sha3_384,
}, {
  .name = "sha3-512",
  .size = 64,
  .func = sha3_512,
}, {
  .name = "shake128",
  .size = 16,
  .func = shake128,
}, {
  .name = "shake256",
  .size = 32,
  .func = shake256,
}};

#define USAGE "Usage: %s <algo> <data>\n" \
              "\n" \
              "Algorithms:\n" \
              "- sha3-224\n" \
              "- sha3-256\n" \
              "- sha3-384\n" \
              "- sha3-512\n" \
              "- shake128\n" \
              "- shake256\n" \
              "\n" \
              "Example:\n" \
              "  %s sha3-256 \"asdf\"\n" \
              "  dd2781f4c51bccdbe23e4d398b8a82261f585c278dbb4b84989fea70e76723a9\n"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    const char *app = (argc > 0) ? argv[0] : "sha3";
    fprintf(stderr, USAGE, app, app);
    return -1;
  }

  for (size_t i = 0; i < sizeof(fns)/sizeof(fns[0]); i++) {
    if (strncmp(argv[1], fns[i].name, strlen(fns[i].name) + 1)) {
      continue;
    }

    // hash into buf
    uint8_t buf[64];
    fns[i].func((uint8_t*) argv[2], strlen(argv[2]), buf);

    // print result
    for (size_t j = 0; j < fns[i].size; j++) {
      printf("%02x", buf[j]);
    }
    fputs("\n", stdout);

    // exit with success
    return 0;
  }

  fprintf(stderr, "Unknown algorithm: %s", argv[1]);
  return -1;
}
