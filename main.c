#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sha3.h"

static void run_shake128_xof(const uint8_t *msg, const size_t msg_len, const size_t out_len) {
  // init xof
  sha3_xof_t xof;
  shake128_xof_init(&xof);

  // absorb
  if (!shake128_xof_absorb(&xof, msg, msg_len)) {
    fprintf(stderr, "Error: shake128_xof_absorb() failed\n");
    exit(-1);
  }

  // squeeze
  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    shake128_xof_squeeze(&xof, buf, len);

    // print result
    for (size_t j = 0; j < len; j++) {
      printf("%02x", buf[j]);
    }
  }

  fputs("\n", stdout);
}

static void run_shake256_xof(const uint8_t * const msg, const size_t msg_len, const size_t out_len) {
  // init xof
  sha3_xof_t xof;
  shake256_xof_init(&xof);

  // absorb
  if (!shake256_xof_absorb(&xof, msg, msg_len)) {
    fprintf(stderr, "Error: shake256_xof_absorb() failed\n");
    exit(-1);
  }

  // squeeze
  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    shake256_xof_squeeze(&xof, buf, len);

    // print result
    for (size_t j = 0; j < len; j++) {
      printf("%02x", buf[j]);
    }
  }

  fputs("\n", stdout);
}

// available functions
static const struct {
  const char *name;
  const size_t size;
  void (*hash_func)(const uint8_t *, size_t, uint8_t *);
  void (*xof_func)(const uint8_t *, size_t, size_t);
} fns[] = {{
  .name = "sha3-224",
  .size = 28,
  .hash_func = sha3_224,
}, {
  .name = "sha3-256",
  .size = 32,
  .hash_func = sha3_256,
}, {
  .name = "sha3-384",
  .size = 48,
  .hash_func = sha3_384,
}, {
  .name = "sha3-512",
  .size = 64,
  .hash_func = sha3_512,
}, {
  .name = "shake128",
  .size = 16,
  .hash_func = shake128,
}, {
  .name = "shake256",
  .size = 32,
  .hash_func = shake256,
}, {
  .name = "shake128-xof",
  .size = 16,
  .xof_func = run_shake128_xof,
}, {
  .name = "shake256-xof",
  .size = 32,
  .xof_func = run_shake256_xof,
}};

#define USAGE "Usage: %s <algo> <data> [xof-size]\n" \
              "\n" \
              "Algorithms:\n" \
              "- sha3-224\n" \
              "- sha3-256\n" \
              "- sha3-384\n" \
              "- sha3-512\n" \
              "- shake128\n" \
              "- shake256\n" \
              "- shake128-xof (XOF)\n" \
              "- shake256-xof (XOF)\n" \
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

  const uint8_t * const msg = (uint8_t*) argv[2];
  const size_t len = strlen(argv[2]);

  for (size_t i = 0; i < sizeof(fns)/sizeof(fns[0]); i++) {
    if (strncmp(argv[1], fns[i].name, strlen(fns[i].name) + 1)) {
      continue;
    }

    if (fns[i].xof_func) {
      // get output size
      const size_t out_size = (argc == 4) ? (size_t) atoi(argv[3]) : fns[i].size;
      fns[i].xof_func(msg, len, out_size);
    } else {
      // hash into buf
      uint8_t buf[64];
      fns[i].hash_func(msg, len, buf);

      // print result
      for (size_t j = 0; j < fns[i].size; j++) {
        printf("%02x", buf[j]);
      }
      fputs("\n", stdout);
    }

    // exit with success
    return 0;
  }

  fprintf(stderr, "Unknown algorithm: %s", argv[1]);
  return -1;
}
