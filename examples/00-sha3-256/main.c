//
// sha3-256-example: print sha3-256 hash of data to standard output.
//
#include <stdint.h>
#include <stdio.h>
#include "hex.h"
#include "sha3.h"

// test data
static const uint8_t DATA[] = "this is some test data";

int main() {
  // hash data
  uint8_t buf[32] = { 0 };
  sha3_256(DATA, sizeof(DATA), buf);

  // print result to stdout
  printf("SHA3-256: ");
  hex_write(stdout, buf, sizeof(buf));
  printf("\n");

  return 0;
}
