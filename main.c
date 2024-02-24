// main.c: sha3 test application
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hex.h" // hex_write()
#include "sha3.h"

// shake128 handler
static void do_shake128(const uint8_t *msg, const size_t msg_len, const size_t out_len) {
  // init xof
  sha3_xof_t xof;
  shake128_init(&xof);

  // absorb
  if (!shake128_absorb(&xof, msg, msg_len)) {
    fprintf(stderr, "Error: shake128_absorb() failed\n");
    exit(-1);
  }

  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    // squeeze and print
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    shake128_squeeze(&xof, buf, len);
    hex_write(stdout, buf, len);
  }

  fputs("\n", stdout);
}

// shake256 handler
static void do_shake256(const uint8_t * const msg, const size_t msg_len, const size_t out_len) {
  // init xof
  sha3_xof_t xof;
  shake256_init(&xof);

  // absorb
  if (!shake256_absorb(&xof, msg, msg_len)) {
    fprintf(stderr, "Error: shake256_absorb() failed\n");
    exit(-1);
  }

  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    // squeeze and print
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    shake256_squeeze(&xof, buf, len);
    hex_write(stdout, buf, len);
  }

  fputs("\n", stdout);
}

// turboshake128 handler
static void do_turboshake128(const uint8_t * const msg, const size_t msg_len, const size_t out_len) {
  // init xof
  turboshake_t ts;
  turboshake256_init(&ts);

  // absorb
  if (!turboshake128_absorb(&ts, msg, msg_len)) {
    fprintf(stderr, "Error: turborshake128_absorb() failed\n");
    exit(-1);
  }

  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    // squeeze and print
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    turboshake128_squeeze(&ts, buf, len);
    hex_write(stdout, buf, len);
  }

  fputs("\n", stdout);
}

// turboshake256 handler
static void do_turboshake256(const uint8_t * const msg, const size_t msg_len, const size_t out_len) {
  // init xof
  turboshake_t ts;
  turboshake256_init(&ts);

  // absorb
  if (!turboshake256_absorb(&ts, msg, msg_len)) {
    fprintf(stderr, "Error: turborshake256_absorb() failed\n");
    exit(-1);
  }

  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    // squeeze and print
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    turboshake256_squeeze(&ts, buf, len);
    hex_write(stdout, buf, len);
  }

  fputs("\n", stdout);
}

// k12 handler
static void do_k12(const uint8_t * const msg, const size_t msg_len, const size_t out_len) {
  // init, absorb
  k12_t k12;
  k12_init(&k12, msg, msg_len, NULL, 0);

  uint8_t buf[64];
  for (size_t i = 0; i < out_len; i += sizeof(buf)) {
    // squeeze and print
    const size_t len = (out_len - i < sizeof(buf)) ? out_len - i : sizeof(buf);
    k12_squeeze(&k12, buf, len);
    hex_write(stdout, buf, len);
  }

  fputs("\n", stdout);
}

// available hash functions
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
  .size = 16, // default size
  .xof_func = do_shake128,
}, {
  .name = "shake256",
  .size = 32, // default size
  .xof_func = do_shake256,
}, {
  .name = "turboshake128",
  .size = 32,
  .xof_func = do_turboshake128,
}, {
  .name = "turboshake256",
  .size = 64,
  .xof_func = do_turboshake256,
}, {
  .name = "k12",
  .size = 32,
  .xof_func = do_k12,
}};

// number of hash functions
#define NUM_FNS (sizeof(fns) / sizeof(fns[0]))

// get hash function offset in fns.  returns NUM_FNS if the function is
// unknown.
static size_t get_fn_ofs(const char * const name) {
  for (size_t i = 0; i < NUM_FNS; i++) {
    if (!strncmp(name, fns[i].name, strlen(fns[i].name) + 1)) {
      return i;
    }
  }

  // not found
  return NUM_FNS;
}

// usage format string
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
              "- turboshake128 (XOF)\n" \
              "- turboshake256 (XOF)\n" \
              "- k12 (XOF)\n" \
              "\n" \
              "Examples:\n" \
              "  # get SHA3-256 hash of string \"asdf\"\n" \
              "  %s sha3-256 \"asdf\"\n" \
              "  dd2781f4c51bccdbe23e4d398b8a82261f585c278dbb4b84989fea70e76723a9\n" \
              "\n" \
              "  # get first 40 bytes of SHAKE128-XOF output of string \"foo\"\n" \
              "  %s shake128-xof foo 40\n" \
              "  f84e95cb5fbd2038863ab27d3cdeac295ad2d4ab96ad1f4b070c0bf36078ef0881db3194a9d0f3dd\n"

int main(int argc, char *argv[]) {
  // check command-line arguments
  if (argc < 3) {
    const char *app = (argc > 0) ? argv[0] : "sha3";
    fprintf(stderr, USAGE, app, app, app);
    return -1;
  }

  // get message and message length
  const uint8_t * const msg = (uint8_t*) argv[2];
  const size_t len = strlen(argv[2]);

  // get function offset
  const size_t ofs = get_fn_ofs(argv[1]);
  if (ofs == NUM_FNS) {
    fprintf(stderr, "Unknown algorithm: %s\n", argv[1]);
    return -1;
  }

  if (fns[ofs].xof_func) {
    // get output size from argument, or use default output size
    const size_t out_size = (argc == 4) ? (size_t) atoi(argv[3]) : fns[ofs].size;
    fns[ofs].xof_func(msg, len, out_size);
  } else {
    // hash into buffer, print buffer, print newline
    uint8_t buf[64];
    fns[ofs].hash_func(msg, len, buf);
    hex_write(stdout, buf, fns[ofs].size);
    fputs("\n", stdout);
  }

  // exit with success
  return 0;
}
