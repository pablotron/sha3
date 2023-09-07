//
// tuplehash128-example: Hash tuples with TupleHash128 and key
// and print first 20 bytes of result to standard output.
//
#include <stdint.h>
#include <stdio.h>
#include "hex.h"
#include "sha3.h"

static const struct {
  const char *name;
  tuplehash_str_t strs[10];
  size_t num_strs;
} ROWS[] = {{
  .name = "TupleHash128: \"abc\"",
  .strs = {{
    .ptr = (uint8_t*) "abc",
    .len = 3,
  }},
  .num_strs = 1,
}, {
  .name = "TupleHash128: \"ab\", \"c\"",
  .strs = {{
    .ptr = (uint8_t*) "ab",
    .len = 2,
  }, {
    .ptr = (uint8_t*) "c",
    .len = 1,
  }},
  .num_strs = 2,
}, {
  .name = "TupleHash128: \"a\", \"bc\"",
  .strs = {{
    .ptr = (uint8_t*) "a",
    .len = 1,
  }, {
    .ptr = (uint8_t*) "bc",
    .len = 2,
  }},
  .num_strs = 2,
}, {
  .name = "TupleHash128: \"a\", \"b\", \"c\"",
  .strs = {{
    .ptr = (uint8_t*) "a",
    .len = 1,
  }, {
    .ptr = (uint8_t*) "b",
    .len = 1,
  }, {
    .ptr = (uint8_t*) "c",
    .len = 1,
  }},
  .num_strs = 3,
}};

int main() {
  for (size_t i = 0; i < sizeof(ROWS)/sizeof(ROWS[0]); i++) {
    // build params
    tuplehash_params_t params = {
      .strs = ROWS[i].strs,
      .num_strs = ROWS[i].num_strs,
    };

    // hash data
    uint8_t buf[32];
    tuplehash128(params, buf, sizeof(buf));

    // print to stdout
    printf("%s:\n", ROWS[i].name);
    hex_write(stdout, buf, sizeof(buf));
    printf("\n");
  }

  return 0;
}
