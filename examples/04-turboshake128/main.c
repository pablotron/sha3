//
// turboshake128-example: Absorb 4096 bytes of data from /dev/urandom in
// 1024 byte chunks, hash it with TurboShake128, then print 128 bytes of
// result in 32 byte chunks (hex-encoded) to standard output.
// output.
//
#include <stdint.h>
#include <stdio.h>
#include <err.h>
#include "hex.h"
#include "sha3.h"

int main() {
  // init turboshake
  turboshake_t ts;
  turboshake128_init(&ts);

  // open source
  FILE *fh = fopen("/dev/urandom", "rb");
  if (!fh) {
    err(-1, "fopen()");
  }

  {
    // read and absorb
    uint8_t buf[1024] = { 0 };
    for (size_t i = 0; i < 4096/sizeof(buf); i++) {
      // read data
      if (!fread(buf, sizeof(buf), 1, fh)) {
        err(-1, "fread()");
      }

      // absorb
      turboshake128_absorb(&ts, buf, sizeof(buf));
    }
  }

  // close source
  if (!fclose(fh)) {
    warn("fclose()");
  }


  printf("TurboShake128 output:\n");
  {
    // squeeze in 32 byte chunks, write to stdout
    uint8_t buf[32] = { 0 };
    for (size_t ofs = 0; ofs < 128; ofs += sizeof(buf)) {
      turboshake128_squeeze(&ts, buf, sizeof(buf));
      hex_write(stdout, buf, sizeof(buf));
    }
  }
  printf("\n");

  return 0;
}
