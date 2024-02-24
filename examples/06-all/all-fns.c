//
// all-fns: Exercise every function.  Relatively sparsely commented;
// used as a source for inline examples in generated API documentation.
//
#include <stdint.h> // uint8_t
#include <stdio.h> // printf()
#include "hex.h" // hex_write()
#include "rand-bytes.h" // rand_bytes()
#include "sha3.h"

static void sha3_224_example(void) {
  ///! [sha3_224]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate sha3-224 hash of `buf`, write results to `hash`
  uint8_t hash[28] = { 0 };
  sha3_224(buf, sizeof(buf), hash);
  ///! [sha3_224]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_224_absorb_example(void) {
  ///! [sha3_224_absorb]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create sha3-224 context
  sha3_t ctx = { 0 };
  sha3_224_init(&ctx);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    sha3_224_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `hash`
  uint8_t hash[28] = { 0 };
  sha3_224_final(&ctx, hash);
  ///! [sha3_224_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_256_example(void) {
  ///! [sha3_256]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate sha3-256 hash of `buf`, write results to `hash`
  uint8_t hash[32] = { 0 };
  sha3_256(buf, sizeof(buf), hash);
  ///! [sha3_256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_256_absorb_example(void) {
  ///! [sha3_256_absorb]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create sha3-256 context
  sha3_t ctx = { 0 };
  sha3_256_init(&ctx);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    sha3_256_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `hash`
  uint8_t hash[32] = { 0 };
  sha3_256_final(&ctx, hash);
  ///! [sha3_256_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_384_example(void) {
  ///! [sha3_384]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate sha3-384 hash of `buf`, write results to `hash`
  uint8_t hash[48] = { 0 };
  sha3_384(buf, sizeof(buf), hash);
  ///! [sha3_384]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_384_absorb_example(void) {
  ///! [sha3_384_absorb]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create sha3-384 context
  sha3_t ctx = { 0 };
  sha3_384_init(&ctx);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    sha3_384_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `hash`
  uint8_t hash[48] = { 0 };
  sha3_384_final(&ctx, hash);
  ///! [sha3_384_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_512_example(void) {
  ///! [sha3_512]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate sha3-512 hash of `buf`, write results to `hash`
  uint8_t hash[64] = { 0 };
  sha3_512(buf, sizeof(buf), hash);
  ///! [sha3_512]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void sha3_512_absorb_example(void) {
  ///! [sha3_512_absorb]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create sha3-512 context
  sha3_t ctx = { 0 };
  sha3_512_init(&ctx);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    sha3_512_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `hash`
  uint8_t hash[64] = { 0 };
  sha3_512_final(&ctx, hash);
  ///! [sha3_512_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, hash, sizeof(hash));
  fputs("\n", stdout);
}

static void shake128_example(void) {
  ///! [shake128]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into shake128 context with 15 byte output, write result to `out`
  uint8_t out[15] = { 0 };
  shake128(buf, sizeof(buf), out, sizeof(out));
  ///! [shake128]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void shake128_ctx_example(void) {
  ///! [shake128_ctx]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create shake128 context
  sha3_xof_t ctx = { 0 };
  shake128_init(&ctx);

  // absorb `buf` contents in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    shake128_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    shake128_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [shake128_ctx]
}

static void shake256_example(void) {
  ///! [shake256]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb `buf` into shake256 context with 15 byte output, write result to `out`
  uint8_t out[15] = { 0 };
  shake256(buf, sizeof(buf), out, sizeof(out));
  ///! [shake256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void shake256_ctx_example(void) {
  ///! [shake256_ctx]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create shake256 context
  sha3_xof_t ctx = { 0 };
  shake256_init(&ctx);

  // absorb `buf` contents in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    shake256_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    shake256_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [shake256_ctx]
}

static void hmac_sha3_224_example(void) {
  ///! [hmac_sha3_224]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate HMAC-SHA3-224, write results to `mac`
  uint8_t mac[28] = { 0 };
  hmac_sha3_224(key, key_len, buf, sizeof(buf), mac);
  ///! [hmac_sha3_224]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_224_absorb_example(void) {
  ///! [hmac_sha3_224_absorb]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create HMAC-SHA3-224 context
  hmac_sha3_t ctx = { 0 };
  hmac_sha3_224_init(&ctx, key, key_len);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    hmac_sha3_224_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `mac`
  uint8_t mac[28] = { 0 };
  hmac_sha3_224_final(&ctx, mac);
  ///! [hmac_sha3_224_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_256_example(void) {
  ///! [hmac_sha3_256]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate HMAC-SHA3-256, write results to `mac`
  uint8_t mac[32] = { 0 };
  hmac_sha3_256(key, key_len, buf, sizeof(buf), mac);
  ///! [hmac_sha3_256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_256_absorb_example(void) {
  ///! [hmac_sha3_256_absorb]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create HMAC-SHA3-256 context
  hmac_sha3_t ctx = { 0 };
  hmac_sha3_256_init(&ctx, key, key_len);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    hmac_sha3_256_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `mac`
  uint8_t mac[32] = { 0 };
  hmac_sha3_256_final(&ctx, mac);
  ///! [hmac_sha3_256_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_384_example(void) {
  ///! [hmac_sha3_384]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate HMAC-SHA3-384, write results to `mac`
  uint8_t mac[48] = { 0 };
  hmac_sha3_384(key, key_len, buf, sizeof(buf), mac);
  ///! [hmac_sha3_384]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_384_absorb_example(void) {
  ///! [hmac_sha3_384_absorb]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create HMAC-SHA3-384 context
  hmac_sha3_t ctx = { 0 };
  hmac_sha3_384_init(&ctx, key, key_len);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    hmac_sha3_384_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `mac`
  uint8_t mac[48] = { 0 };
  hmac_sha3_384_final(&ctx, mac);
  ///! [hmac_sha3_384_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_512_example(void) {
  ///! [hmac_sha3_512]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // calculate HMAC-SHA3-512, write results to `mac`
  uint8_t mac[64] = { 0 };
  hmac_sha3_512(key, key_len, buf, sizeof(buf), mac);
  ///! [hmac_sha3_512]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void hmac_sha3_512_absorb_example(void) {
  ///! [hmac_sha3_512_absorb]
  // key and key size, in bytes (w/o trailing NUL)
  const uint8_t key[] = "secret!"; // key
  const size_t key_len = sizeof(key) - 1; // key size

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create HMAC-SHA3-512 context
  hmac_sha3_t ctx = { 0 };
  hmac_sha3_512_init(&ctx, key, key_len);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    hmac_sha3_512_absorb(&ctx, buf + i, 32);
  }

  // finalize context, write result to `mac`
  uint8_t mac[64] = { 0 };
  hmac_sha3_512_final(&ctx, mac);
  ///! [hmac_sha3_512_absorb]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, mac, sizeof(mac));
  fputs("\n", stdout);
}

static void cshake128_example(void) {
  ///! [cshake128]
  const uint8_t custom[] = "hello"; // customization string

  // cSHAKE parameters
  const cshake_params_t params = {
    .custom = custom,
    .custom_len = sizeof(custom) - 1, // length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into cSHAKE128 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  cshake128(params, buf, sizeof(buf), out, sizeof(out));
  ///! [cshake128]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void cshake128_xof_example(void) {
  ///! [cshake128_xof]
  const uint8_t custom[] = "hello"; // customization string

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // cSHAKE parameters
  const cshake_params_t params = {
    .custom = custom,
    .custom_len = sizeof(custom) - 1, // length, in bytes (w/o trailing NUL)
  };

  // create cSHAKE128 context from parameters
  sha3_xof_t ctx = { 0 };
  cshake128_xof_init(&ctx, params);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    cshake128_xof_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    cshake128_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [cshake128_xof]
}

static void cshake256_example(void) {
  ///! [cshake256]
  const uint8_t custom[] = "hello"; // customization string

  // cSHAKE parameters
  const cshake_params_t params = {
    .custom = custom,
    .custom_len = sizeof(custom) - 1, // length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into cshake256 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  cshake256(params, buf, sizeof(buf), out, sizeof(out));
  ///! [cshake256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void cshake256_xof_example(void) {
  ///! [cshake256_xof]
  const uint8_t custom[] = "hello"; // customization string

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // cSHAKE parameters
  const cshake_params_t params = {
    .custom = custom,
    .custom_len = sizeof(custom) - 1, // length, in bytes (w/o trailing NUL)
  };

  // create cSHAKE256 context from parameters
  sha3_xof_t ctx = { 0 };
  cshake256_xof_init(&ctx, params);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    cshake256_xof_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    cshake256_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [cshake256_xof]
}

static void kmac128_example(void) {
  ///! [kmac128]
  const uint8_t key[] = "secret!"; // secret key
  const uint8_t custom[] = "hello"; // customization string

  // kmac parameters
  const kmac_params_t params = {
    .key = key, // secret key
    .key_len = sizeof(custom) - 1, // key length, in bytes (w/o trailing NUL)
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into kmac128 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  kmac128(params, buf, sizeof(buf), out, sizeof(out));
  ///! [kmac128]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void kmac128_xof_example(void) {
  ///! [kmac128_xof]
  const uint8_t key[] = "secret!"; // secret key
  const uint8_t custom[] = "hello"; // customization string

  // kmac parameters
  const kmac_params_t params = {
    .key = key, // secret key
    .key_len = sizeof(custom) - 1, // key length, in bytes (w/o trailing NUL)
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create KMAC128 XOF context from parameters
  sha3_xof_t ctx = { 0 };
  kmac128_xof_init(&ctx, params);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    kmac128_xof_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    kmac128_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [kmac128_xof]
}

static void kmac128_xof_once_example(void) {
  ///! [kmac128_xof_once]
  const uint8_t key[] = "secret!"; // secret key
  const uint8_t custom[] = "hello"; // customization string

  // kmac parameters
  const kmac_params_t params = {
    .key = key, // secret key
    .key_len = sizeof(custom) - 1, // key length, in bytes (w/o trailing NUL)
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into KMAC128 XOF, squeeze into `out`
  uint8_t out[128] = { 0 }; // output buffer
  kmac128_xof_once(params, buf, sizeof(buf), out, sizeof(out));
  ///! [kmac128_xof_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void kmac256_example(void) {
  ///! [kmac256]
  const uint8_t key[] = "secret!"; // secret key
  const uint8_t custom[] = "hello"; // customization string

  // kmac parameters
  const kmac_params_t params = {
    .key = key, // secret key
    .key_len = sizeof(custom) - 1, // key length, in bytes (w/o trailing NUL)
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into KMAC256 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  kmac256(params, buf, sizeof(buf), out, sizeof(out));
  ///! [kmac256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void kmac256_xof_example(void) {
  ///! [kmac256_xof]
  const uint8_t key[] = "secret!"; // secret key
  const uint8_t custom[] = "hello"; // customization string

  // kmac parameters
  const kmac_params_t params = {
    .key = key, // secret key
    .key_len = sizeof(custom) - 1, // key length, in bytes (w/o trailing NUL)
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create KMAC256 XOF context from parameters
  sha3_xof_t ctx = { 0 };
  kmac256_xof_init(&ctx, params);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    kmac256_xof_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    kmac256_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [kmac256_xof]
}

static void kmac256_xof_once_example(void) {
  ///! [kmac256_xof_once]
  const uint8_t key[] = "secret!"; // secret key
  const uint8_t custom[] = "hello"; // customization string

  // kmac parameters
  const kmac_params_t params = {
    .key = key, // secret key
    .key_len = sizeof(custom) - 1, // key length, in bytes (w/o trailing NUL)
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into KMAC256 XOF, squeeze into `out`
  uint8_t out[128] = { 0 }; // output buffer
  kmac256_xof_once(params, buf, sizeof(buf), out, sizeof(out));
  ///! [kmac256_xof_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void tuplehash128_example(void) {
  ///! [tuplehash128]
  // byte strings
  const tuplehash_str_t strs[] = {
    { .ptr = (uint8_t*) "foo", .len = 3 },
    { .ptr = (uint8_t*) "bar", .len = 3 },
    { .ptr = (uint8_t*) "blum", .len = 4 },
  };
  const uint8_t custom[] = "hello"; // customization string

  // tuplehash parameters
  const tuplehash_params_t params = {
    .strs = strs, // array of byte strings
    .num_strs = sizeof(strs) / sizeof(strs[0]), // byte string count
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // absorb into TupleHash128 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  tuplehash128(params, out, sizeof(out));
  ///! [tuplehash128]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void tuplehash128_xof_example(void) {
  ///! [tuplehash128_xof]
  // byte strings
  const tuplehash_str_t strs[] = {
    { .ptr = (uint8_t*) "foo", .len = 3 },
    { .ptr = (uint8_t*) "bar", .len = 3 },
    { .ptr = (uint8_t*) "blum", .len = 4 },
  };
  const uint8_t custom[] = "hello"; // customization string

  // tuplehash parameters
  const tuplehash_params_t params = {
    .strs = strs, // array of byte strings
    .num_strs = sizeof(strs) / sizeof(strs[0]), // byte string count
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // create TupleHash128 XOF context from parameters
  sha3_xof_t ctx = { 0 };
  tuplehash128_xof_init(&ctx, params);

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    tuplehash128_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [tuplehash128_xof]
}

static void tuplehash128_xof_once_example(void) {
  ///! [tuplehash128_xof_once]
  // byte strings
  const tuplehash_str_t strs[] = {
    { .ptr = (uint8_t*) "foo", .len = 3 },
    { .ptr = (uint8_t*) "bar", .len = 3 },
    { .ptr = (uint8_t*) "blum", .len = 4 },
  };
  const uint8_t custom[] = "hello"; // customization string

  // tuplehash parameters
  const tuplehash_params_t params = {
    .strs = strs, // array of byte strings
    .num_strs = sizeof(strs) / sizeof(strs[0]), // byte string count
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // absorb into TupleHash128 XOF, squeeze into `out`
  uint8_t out[128] = { 0 }; // output buffer
  tuplehash128_xof_once(params, out, sizeof(out));
  ///! [tuplehash128_xof_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void tuplehash256_example(void) {
  ///! [tuplehash256]
  // byte strings
  const tuplehash_str_t strs[] = {
    { .ptr = (uint8_t*) "foo", .len = 3 },
    { .ptr = (uint8_t*) "bar", .len = 3 },
    { .ptr = (uint8_t*) "blum", .len = 4 },
  };
  const uint8_t custom[] = "hello"; // customization string

  // tuplehash parameters
  const tuplehash_params_t params = {
    .strs = strs, // array of byte strings
    .num_strs = sizeof(strs) / sizeof(strs[0]), // byte string count
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // absorb into TupleHash256 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  tuplehash256(params, out, sizeof(out));
  ///! [tuplehash256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void tuplehash256_xof_example(void) {
  ///! [tuplehash256_xof]
  // byte strings
  const tuplehash_str_t strs[] = {
    { .ptr = (uint8_t*) "foo", .len = 3 },
    { .ptr = (uint8_t*) "bar", .len = 3 },
    { .ptr = (uint8_t*) "blum", .len = 4 },
  };
  const uint8_t custom[] = "hello"; // customization string

  // tuplehash parameters
  const tuplehash_params_t params = {
    .strs = strs, // array of byte strings
    .num_strs = sizeof(strs) / sizeof(strs[0]), // byte string count
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // create TupleHash256 XOF context from parameters
  sha3_xof_t ctx = { 0 };
  tuplehash256_xof_init(&ctx, params);

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    tuplehash256_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [tuplehash256_xof]
}

static void tuplehash256_xof_once_example(void) {
  ///! [tuplehash256_xof_once]
  // byte strings
  const tuplehash_str_t strs[] = {
    { .ptr = (uint8_t*) "foo", .len = 3 },
    { .ptr = (uint8_t*) "bar", .len = 3 },
    { .ptr = (uint8_t*) "blum", .len = 4 },
  };
  const uint8_t custom[] = "hello"; // customization string

  // tuplehash parameters
  const tuplehash_params_t params = {
    .strs = strs, // array of byte strings
    .num_strs = sizeof(strs) / sizeof(strs[0]), // byte string count
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // absorb into TupleHash256 XOF, squeeze into `out`
  uint8_t out[128] = { 0 }; // output buffer
  tuplehash256_xof_once(params, out, sizeof(out));
  ///! [tuplehash256_xof_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void parallelhash128_example(void) {
  ///! [parallelhash128]
  const size_t block_len = 200; // block size, in bytes
  const uint8_t custom[] = "hello"; // customization string

  // parallelhash parameters
  const parallelhash_params_t params = {
    .block_len = block_len,
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into ParallelHash128 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  parallelhash128(params, buf, sizeof(buf), out, sizeof(out));
  ///! [parallelhash128]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void parallelhash128_xof_example(void) {
  ///! [parallelhash128_xof]
  const size_t block_len = 200; // block size, in bytes
  const uint8_t custom[] = "hello"; // customization string

  // parallelhash parameters
  const parallelhash_params_t params = {
    .block_len = block_len,
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create ParallelHash128 XOF context from parameters
  parallelhash_t ctx = { 0 };
  parallelhash128_xof_init(&ctx, params);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    parallelhash128_xof_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    parallelhash128_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [parallelhash128_xof]
}

static void parallelhash128_xof_once_example(void) {
  ///! [parallelhash128_xof_once]
  const size_t block_len = 200; // block size, in bytes
  const uint8_t custom[] = "hello"; // customization string

  // parallelhash parameters
  const parallelhash_params_t params = {
    .block_len = block_len,
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into ParallelHash128 XOF, squeeze into `out`
  uint8_t out[128] = { 0 }; // output buffer
  parallelhash128_xof_once(params, buf, sizeof(buf), out, sizeof(out));
  ///! [parallelhash128_xof_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void parallelhash256_example(void) {
  ///! [parallelhash256]
  const size_t block_len = 200; // block size, in bytes
  const uint8_t custom[] = "hello"; // customization string

  // parallelhash parameters
  const parallelhash_params_t params = {
    .block_len = block_len,
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into ParallelHash256 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  parallelhash256(params, buf, sizeof(buf), out, sizeof(out));
  ///! [parallelhash256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void parallelhash256_xof_example(void) {
  ///! [parallelhash256_xof]
  const size_t block_len = 200; // block size, in bytes
  const uint8_t custom[] = "hello"; // customization string

  // parallelhash parameters
  const parallelhash_params_t params = {
    .block_len = block_len,
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create ParallelHash256 XOF context from parameters
  parallelhash_t ctx = { 0 };
  parallelhash256_xof_init(&ctx, params);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    parallelhash256_xof_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    parallelhash256_xof_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [parallelhash256_xof]
}

static void parallelhash256_xof_once_example(void) {
  ///! [parallelhash256_xof_once]
  const size_t block_len = 200; // block size, in bytes
  const uint8_t custom[] = "hello"; // customization string

  // parallelhash parameters
  const parallelhash_params_t params = {
    .block_len = block_len,
    .custom = custom, // customization string
    .custom_len = sizeof(custom) - 1, // customization string length, in bytes (w/o trailing NUL)
  };

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into ParallelHash256 XOF, squeeze into `out`
  uint8_t out[128] = { 0 }; // output buffer
  parallelhash256_xof_once(params, buf, sizeof(buf), out, sizeof(out));
  ///! [parallelhash256_xof_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void turboshake128_example(void) {
  ///! [turboshake128]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into TurboSHAKE128 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  turboshake128(buf, sizeof(buf), out, sizeof(out));
  ///! [turboshake128]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void turboshake128_custom_example(void) {
  ///! [turboshake128_custom]
  const uint8_t pad = 3; // custom padding byte

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into TurboSHAKE128 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  turboshake128_custom(pad, buf, sizeof(buf), out, sizeof(out));
  ///! [turboshake128_custom]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void turboshake128_xof_example(void) {
  ///! [turboshake128_xof]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create TurboSHAKE128 XOF context which supports arbitrary-length
  // output
  turboshake_t ctx = { 0 };
  turboshake128_init(&ctx);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    turboshake128_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    turboshake128_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [turboshake128_xof]
}

static void turboshake128_custom_xof_example(void) {
  ///! [turboshake128_custom_xof]
  const uint8_t pad = 3; // custom padding byte

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create TurboSHAKE128 XOF context with custom padding byte which
  // supports arbitrary-length output
  turboshake_t ctx = { 0 };
  turboshake128_init_custom(&ctx, pad);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    turboshake128_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    turboshake128_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [turboshake128_custom_xof]
}

static void turboshake256_example(void) {
  ///! [turboshake256]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into TurboSHAKE256 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  turboshake256(buf, sizeof(buf), out, sizeof(out));
  ///! [turboshake256]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void turboshake256_custom_example(void) {
  ///! [turboshake256_custom]
  const uint8_t pad = 3; // custom padding byte

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb into TurboSHAKE256 with fixed-length output, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  turboshake256_custom(pad, buf, sizeof(buf), out, sizeof(out));
  ///! [turboshake256_custom]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void turboshake256_xof_example(void) {
  ///! [turboshake256_xof]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create TurboSHAKE256 XOF context which supports arbitrary-length
  // output
  turboshake_t ctx = { 0 };
  turboshake256_init(&ctx);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    turboshake256_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    turboshake256_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [turboshake256_xof]
}

static void turboshake256_custom_xof_example(void) {
  ///! [turboshake256_custom_xof]
  const uint8_t pad = 3; // custom padding byte

  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // create TurboSHAKE256 XOF context with custom padding byte which
  // supports arbitrary-length output
  turboshake_t ctx = { 0 };
  turboshake256_init_custom(&ctx, pad);

  // absorb `buf` in 32 byte chunks
  for (size_t i = 0; i < sizeof(buf); i += 32) {
    turboshake256_absorb(&ctx, buf + i, 32);
  }

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    turboshake256_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [turboshake256_custom_xof]
}

static void k12_once_example(void) {
  ///! [k12_once]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // absorb `buf` into KangarooTwelve, write 32 bytes to `out`
  uint8_t out[32] = { 0 };
  k12_once(buf, sizeof(buf), out, sizeof(out));
  ///! [k12_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void k12_custom_once_example(void) {
  ///! [k12_custom_once]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // customization string and length (in bytes, w/o NUL)
  const uint8_t custom[] = "hello";
  const size_t custom_len = sizeof(custom) - 1;

  // absorb `buf` and `custom` into KangarooTwelve, write 32 bytes to
  // `out`
  uint8_t out[32] = { 0 };
  k12_custom_once(buf, sizeof(buf), custom, custom_len, out, sizeof(out));
  ///! [k12_custom_once]

  // print to stdout
  printf("%s: ", __func__);
  hex_write(stdout, out, sizeof(out));
  fputs("\n", stdout);
}

static void k12_xof_example(void) {
  ///! [k12_xof]
  // get 1024 random bytes
  uint8_t buf[1024] = { 0 };
  rand_bytes(buf, sizeof(buf));

  // customization string and length (in bytes, w/o NUL)
  const uint8_t custom[] = "hello";
  const size_t custom_len = sizeof(custom) - 1;

  // create KangarooTwelve context from `buf and `custom`
  k12_t ctx = { 0 };
  k12_init(&ctx, buf, sizeof(buf), custom, custom_len);

  // print result prefix to stdout
  printf("%s: ", __func__);

  // squeeze first 128 bytes of of result in 32 byte chunks
  for (size_t i = 0; i < 128; i += 32) {
    // squeeze 32 bytes of result into `out`
    uint8_t out[32] = { 0 };
    k12_squeeze(&ctx, out, sizeof(out));

    // encode as hex, print to stdout
    hex_write(stdout, out, sizeof(out));
  }

  // print result suffix to stdout
  fputs("\n", stdout);
  ///! [k12_xof]
}

int main(void) {
  sha3_224_example();
  sha3_224_absorb_example();
  sha3_256_example();
  sha3_256_absorb_example();
  sha3_384_example();
  sha3_384_absorb_example();
  sha3_512_example();
  sha3_512_absorb_example();
  shake128_example();
  shake128_ctx_example();
  shake256_example();
  shake256_ctx_example();
  hmac_sha3_224_example();
  hmac_sha3_224_absorb_example();
  hmac_sha3_256_example();
  hmac_sha3_256_absorb_example();
  hmac_sha3_384_example();
  hmac_sha3_384_absorb_example();
  hmac_sha3_512_example();
  hmac_sha3_512_absorb_example();
  cshake128_example();
  cshake128_xof_example();
  cshake256_example();
  cshake256_xof_example();
  kmac128_example();
  kmac128_xof_example();
  kmac128_xof_once_example();
  kmac256_example();
  kmac256_xof_example();
  kmac256_xof_once_example();
  tuplehash128_example();
  tuplehash128_xof_example();
  tuplehash128_xof_once_example();
  tuplehash256_example();
  tuplehash256_xof_example();
  tuplehash256_xof_once_example();
  parallelhash128_example();
  parallelhash128_xof_example();
  parallelhash128_xof_once_example();
  parallelhash256_example();
  parallelhash256_xof_example();
  parallelhash256_xof_once_example();
  turboshake128_example();
  turboshake128_custom_example();
  turboshake128_xof_example();
  turboshake128_custom_xof_example();
  turboshake256_example();
  turboshake256_custom_example();
  turboshake256_xof_example();
  turboshake256_custom_xof_example();
  k12_once_example();
  k12_custom_once_example();
  k12_xof_example();

  return 0;
}
