//
// shake128-example: hash contents of DATA with SHAKE128 and print first
// 200 bytes of SHAKE128 hash of data to standard output.
//
#include <stdint.h>
#include <stdio.h>
#include "hex.h"
#include "sha3.h"

// test data
static const uint8_t DATA[] = "this is some test data";

int main() {
  // hash data
  uint8_t buf[200] = { 0 };
  shake128_xof_once(DATA, sizeof(DATA), buf, sizeof(buf));

  // print result to stdout
  printf("SHAKE128 (200 bytes): ");
  hex_write(stdout, buf, sizeof(buf));
  printf("\n");

  return 0;
}
