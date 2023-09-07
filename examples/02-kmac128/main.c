//
// kmac128-example: Hash contents of DATA with KMAC128 and key and print first
// 32 bytes of result to standard output.
//
#include <stdint.h>
#include <stdio.h>
#include "hex.h"
#include "sha3.h"

// test data
static const uint8_t DATA[] = "this is some test data";

static const struct {
  const char *name;
  kmac_params_t params;
} ROWS[] = {{
  .name = "KMAC128 (key = \"\", custom = \"\")",
}, {
  .name = "KMAC128 (key = \"foo\", custom = \"\")",
  .params = {
    .key = (uint8_t*) "foo",
    .key_len = 3,
  },
}, {
  .name = "KMAC128 (key = \"bar\", custom = \"\")",
  .params = {
    .key = (uint8_t*) "bar",
    .key_len = 3,
  },
}, {
  .name = "KMAC128 (key = \"bar\", custom = \"blum\")",
  .params = {
    .key = (uint8_t*) "bar",
    .key_len = 3,
    .custom = (uint8_t*) "blum",
    .custom_len = 4,
  },
}};

int main() {
  for (size_t i = 0; i < sizeof(ROWS)/sizeof(ROWS[0]); i++) {
    // hash data
    uint8_t buf[32] = { 0 };
    kmac128_xof_once(ROWS[i].params, DATA, sizeof(DATA), buf, sizeof(buf));

    // print to stdout
    printf("%s:\n", ROWS[i].name);
    hex_write(stdout, buf, sizeof(buf));
    printf("\n");
  }

  return 0;
}
