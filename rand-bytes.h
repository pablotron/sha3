#ifndef RAND_BYTES_H
#define RAND_BYTES_H

#include <stddef.h> // size_t
#include <sys/random.h> // getentropy()
#include <err.h> // errx()

// Fill `buf` with `len` random bytes using `getentropy()`.
//
// Prints an error message to standard error and exits with a non-zero
// error code if `len` random bytes could not be read.
static void rand_bytes(void * const buf, size_t len) {
  uint8_t *u8 = (uint8_t*) buf;

  // the maximum length for getentropy() is 256 bytes, so split requests
  // for more than 256 bytes into multiple calls to `getentropy()`.
  while (len > 256) {
    // read 256 bytes, check for error
    if (getentropy(u8, 256) == -1) {
      // print error message, exit with error
      errx(-1, "getentropy() failed");
    }

    // increment pointer, decrement length
    u8 += 256;
    len -= 256;
  }

  // read remaining bytes, check for error
  if (getentropy(u8, len) == -1) {
    // print error message, exit with error
    errx(-1, "getentropy() failed");
  }
}

#endif /* RAND_BYTES_H */
