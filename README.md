# sha3

Embeddable, dependency-free, [MIT-0][] licensed [C11][] implementation
of the [SHA-3][] [cryptographic hash functions][hash], [eXtendable
Output Functions (XOFs)][xof], and [Hash-based Message Authentication
Code functions (HMAC)][hmac] defined in [FIPS 202][], [SP
800-185][800-185], and the [draft KangarooTwelve and TurboSHAKE
specification][turboshake-ietf].

Includes [AVX-512][] acceleration, [Doxygen][]-friendly [API][]
documentation, and a full test suite with sanitizers enabled and test
vectors from the [NIST CSRC "Examples With Intermediate Values"
site][csrc-examples], the [NIST Cryptographic Algorithm Validation Program
(CAVP)][cavp], and the [Test Vectors section of the draft KangarooTwelve
and TurboSHAKE specification][turboshake-ietf-test-vectors].

The following algorithms are implemented:

* SHA3-224, SHA3-256, SHA3-384, and SHA3-512: [SHA-3][] [cryptographic
  hash functions][hash], as defined in section 6.1 of [FIPS 202][].
* SHAKE128 and SHAKE256: [XOFs][xof] as defined in section 6.2 of [FIPS 202][].
* HMAC-SHA3-224, HMAC-SHA3-256, HMAC-SHA3-384, HMAC-SHA3-512:
  [HMAC][hmac] instantiated with [SHA-3][] hash functions, as specified
  in section 7 of [FIPS 202][], [RFC 2104][], and [FIPS 198-1][].
* cSHAKE128, cSHAKE128-XOF, cSHAKE256, and cSHAKE256-XOF: Fixed-length
  and [XOF][] variants of the customizable-SHAKE primitive, as defined
  in section 3 of [SP 800-185][800-185].
* KMAC128, KMAC128-XOF, KMAC256 and KMAC256-XOF: Keccak [Message
  Authentication Code (MAC)][mac], as defined in section 4 of [SP
  800-185][800-185].
* TupleHash128, TupleHash128-XOF, TupleHash256, and TupleHash256-XOF:
  Misuse-resistant hash function and [XOF][] for hashing a [tuple][] of
  byte strings, as defined in section 5 of [SP 800-185][800-185].
* ParallelHash128, ParallelHash128-XOF, ParallelHash256, and
  ParallelHash256-XOF: Hash function and [XOF][], as defined in section
  6 of [SP 800-185][800-185].
* TurboSHAKE128 and TurboSHAKE256: Faster, reduced-round [XOFs][xof],
  as defined in the [draft KangarooTwelve and TurboSHAKE
  specification][turboshake-ietf].
* KangarooTwelve: Faster, reduced-round [XOF][] with a customzation
  string, as defined in the [draft KangarooTwelve and TurboSHAKE
  specification][turboshake-ietf].

Use `make` to build a shared library and minimal sample application,
`make doc` to generate [HTML][]-formatted [API][] documentation, and
`make test` to run the test suite.

## Examples

Calculate the SHA3-256 hash of a test string and print the result to
standard output:

```c
// sha3-256-example: print sha3-256 hash of data to standard output.
#include <stdint.h> // uint8_t
#include <stdio.h> // printf()
#include "hex.h" // hex_write()
#include "sha3.h" // sha3_256()

// test data
static const uint8_t DATA[] = "this is some test data";

int main(void) {
  // hash `DATA` into `buf`
  uint8_t buf[32] = { 0 };
  sha3_256(DATA, sizeof(DATA), buf);

  // print result to stdout
  printf("SHA3-256: ");
  hex_write(stdout, buf, sizeof(buf));
  printf("\n");

  return 0;
}
```

Calculate the SHAKE128 hash of a test string and print the first 200
bytes to standard output:

```c
//
// shake128-example: hash contents of DATA with SHAKE128 and print first
// 200 bytes of SHAKE128 hash of data to standard output.
//
#include <stdint.h> // uint8_t
#include <stdio.h> // printf()
#include "hex.h" // hex_write()
#include "sha3.h" // shake128()

// test data
static const uint8_t DATA[] = "this is some test data";

int main(void) {
  // hash data
  uint8_t buf[200] = { 0 };
  shake128(DATA, sizeof(DATA), buf, sizeof(buf));

  // print result to stdout
  printf("SHAKE128 (200 bytes): ");
  hex_write(stdout, buf, sizeof(buf));
  printf("\n");

  return 0;
}
```

TurboSHAKE128 example, using the iterative API:

```c
//
// turboshake128-example: Absorb 4096 bytes of data from /dev/urandom in
// 1024 byte chunks, hash it with TurboShake128, then print 128 bytes of
// result in 32 byte chunks (hex-encoded) to standard output.
// output.
//
#include <stdint.h> // uint8_t
#include <stdio.h> // fopen(), fread(), fclose(), printf()
#include <err.h> // err(), warn()
#include "hex.h" // hex_write()
#include "sha3.h" // turboshake128_*()

int main(void) {
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
```

See the `examples/` directory for more.

## Usage

To use this library in your application:

1. Copy `sha3.h` and `sha3.c` into your source tree.
2. Update your build system to compile `sha3.o`.  See the `Makefile` for
   compiler flags.
3. Include `sha3.h` in your application.

See the top-level `Makefile` and the examples for recommended
compiler flags.

## Documentation

Full [API][] documentation is available online [here][api-docs] and in
the comments of `sha3.h`.  If you have [Doxygen][] installed, you can
generate [HTML][]-formatted [API][] documentation by typing `make doc`.

## Tests

Use `make test` to build and run the test suite.

The test suite checks each component of this implementation for expected
answers and is built with several sanitizers supported by both [GCC][]
and [Clang][].  The source code for the test suite is embedded at the
bottom of `sha3.c` behind a `TEST_SHA3` define.

An additional set of tests for the [FIPS 202][] hash and [XOFs][xof] is
available in `tests/cavp-tests/`.  These test cases are generated from
the [Cryptographic Algorithm Validation Program (CAVP)][cavp] byte test
vectors, and are excluded from the main test suite because of their
size.

## Backends

This library includes several accelerated backends which are selectable
at compile time via the `SHA3_BACKEND` make argument and define.  By
default the fastest backend is selected at compile-time.

The available backends are:

- Scalar (`SHA3_BACKEND=1`): Default if no faster backend is available.
- [AVX-512][] (`SHA3_BACKEND=2`): [AVX-512][] acceleration.  Selected by
  default if [AVX-512][] is supported.
- [Neon][] (`SHA3_BACKEND=3`): ARM [Neon][] acceleration.  Currently
  slower than the scalar backend on ARM CPUs and not enabled by default.

The name of the selected backend is available at run-time via the
`sha3_backend()` function.  See the `tests/bench/` for examples of the
`SHA3_BACKEND` make argument and the `sha3_backend()` function.

## Benchmarks

A minimal [libcpucycles][]-based benchmarking tool is available in
`tests/bench/`.  The `bench` tool measures the [median][] [cycles per
byte (cpb)][cpb] for a variety of message lengths, then prints a table
of results to standard output in [CSV][] format.

The results from running `bench` on a couple of my systems are available
in the tables below.

### Lenovo ThinkPad X1 Carbon, 6th Gen (i7-1185G7, AVX-512 Backend)

| function | 64 | 256 | 1024 | 4096 | 16384 |
| -------- | -- | --- | ---- | ---- | ----- |
| sha3-224 | 15.4 | 7.8 | 7.8 | 7.1 | 7.0 |
| sha3-256 | 15.4 | 7.8 | 7.8 | 7.6 | 7.4 |
| sha3-384 | 15.5 | 11.7 | 9.8 | 9.8 | 9.7 |
| sha3-512 | 15.4 | 15.5 | 14.6 | 13.9 | 13.9 |
| shake128 | 15.5 | 7.8 | 6.9 | 6.2 | 6.1 |
| shake256 | 15.6 | 7.8 | 7.9 | 7.6 | 7.4 |

### Raspberry Pi 5 (Cortex-A76, Scalar Backend)

| function | 64 | 256 | 1024 | 4096 | 16384 |
| -------- | -- | --- | ---- | ---- | ----- |
| sha3-224 | 20.2 | 10.3 | 10.3 | 9.3 | 9.2 |
| sha3-256 | 20.2 | 10.3 | 10.3 | 9.9 | 9.7 |
| sha3-384 | 20.9 | 15.3 | 12.8 | 12.7 | 12.5 |
| sha3-512 | 20.2 | 20.2 | 18.9 | 25.3 | 17.9 |
| shake128 | 20.2 | 10.1 | 9.0 | 8.1 | 7.9 |
| shake256 | 20.2 | 10.3 | 10.3 | 9.9 | 9.7 |

### Odroid N2L (Cortex-A73, Scalar Backend)

| function | 64 | 256 | 1024 | 4096 | 16384 |
| -------- | -- | --- | ---- | ---- | ----- |
| sha3-224 | 34.0 | 16.1 | 15.5 | 14.0 | 13.7 |
| sha3-256 | 34.0 | 16.1 | 15.4 | 14.8 | 14.4 |
| sha3-384 | 34.0 | 23.4 | 19.0 | 18.8 | 18.6 |
| sha3-512 | 34.0 | 30.8 | 28.1 | 26.5 | 26.5 |
| shake128 | 34.0 | 16.1 | 13.6 | 12.1 | 11.8 |
| shake256 | 34.0 | 16.1 | 15.5 | 14.8 | 14.4 |

## References

* [FIPS 202 - SHA-3 Standard: Permutation-Based Hash and Extendable-Output Functions][FIPS 202]
* [NIST SP 800-185 - SHA-3 Derived Functions: cSHAKE, KMAC, TupleHash, and ParallelHash][800-185]
* [FIPS 198-1 - The Keyed-Hash Message Authentication Code (HMAC)][FIPS 198-1]
* [NIST CSRC: Cryptographic Standards and Guidelines: Examples with Intermediate Values][csrc-examples]
* [NIST CSRC: Cryptographic Algorithm Validation Program (CAVP)][cavp]
* [TurboSHAKE (PDF)][turboshake]
* [KangarooTwelve and TurboSHAKE][turboshake-ietf]

## License

[MIT No Attribution (MIT-0)][MIT-0]

Copyright 2023, 2024 Paul Duncan

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[C11]: https://en.wikipedia.org/wiki/C11_(C_standard_revision)
  "ISO/IEC 9899:2011"
[SHA-3]: https://en.wikipedia.org/wiki/SHA-3
  "Secure Hash Algorithm 3"
[hash]: https://en.wikipedia.org/wiki/Cryptographic_hash_function
  "Cryptographic hash function"
[xof]: https://en.wikipedia.org/wiki/Extendable-output_function
  "Extendable-Output Function (XOF)"
[mac]: https://en.wikipedia.org/wiki/Message_authentication_code
  "Message authentication code"
[hmac]: https://en.wikipedia.org/wiki/HMAC
  "Keyed Hash Message Authentication Code (HMAC)"
[tuple]: https://en.wikipedia.org/wiki/Tuple
  "Ordered list of elements."
[FIPS 202]: https://csrc.nist.gov/pubs/fips/202/final
  "SHA-3 Standard: Permutation-Based Hash and Extendable-Output Functions"
[800-185]: https://csrc.nist.gov/pubs/sp/800/185/final
  "SHA-3 Derived Functions: cSHAKE, KMAC, TupleHash, and ParallelHash"
[rfc 2104]: https://datatracker.ietf.org/doc/html/rfc2104
  "RFC 2104: HMAC: Keyed-Hashing for Message Authentication"
[FIPS 198-1]: https://csrc.nist.gov/pubs/fips/198-1/final
  "The Keyed-Hash Message Authentication Code (HMAC)"
[csrc]: https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/
  "NIST Computer Security Resource Center"
[csrc-examples]: https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/example-values
  "NIST CSRC: Cryptographic Standards and Guidelines: Examples with Intermediate Values"
[turboshake]: https://eprint.iacr.org/2023/342.pdf
  "TurboSHAKE"
[turboshake-ietf]: https://www.ietf.org/archive/id/draft-irtf-cfrg-kangarootwelve-10.html
  "KangarooTwelve and TurboSHAKE"
[turboshake-ietf-test-vectors]: https://www.ietf.org/archive/id/draft-irtf-cfrg-kangarootwelve-10.html#name-test-vectors
  "KangarooTwelve and TurboSHAKE test vectors"
[avx-512]: https://en.wikipedia.org/wiki/AVX-512
  "Advanced Vector Extensions 512 (AVX-512): 512-bit SIMD vector instruction set"
[doxygen]: https://en.wikipedia.org/wiki/Doxygen
  "API documentation generator."
[api]: https://en.wikipedia.org/wiki/API
  "Application Programming Interface (API)"
[html]: https://en.wikipedia.org/wiki/HTML
  "HyperText Markup Language (HTML)"
[gcc]: https://en.wikipedia.org/wiki/GNU_Compiler_Collection
  "GNU Compiler Collection."
[clang]: https://en.wikipedia.org/wiki/Clang
  "LLVM compiler front end."
[mit-0]: https://opensource.org/license/mit-0/
  "MIT No Attribution license"
[api-docs]: https://pmdn.org/api-docs/sha3/
  "sha3 API documentation."
[cavp]: https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing
  "Cryptographic Algorithm Validation Program (CAVP)"
[neon]: https://en.wikipedia.org/wiki/ARM_architecture_family#Advanced_SIMD_(Neon)
  "Advanced SIMD extension for ARM CPUs"
[csv]: https://en.wikipedia.org/wiki/Comma-separated_values
  "Comma-Separated Value (CSV)"
[libcpucycles]: https://cpucycles.cr.yp.to/
  "Microlibrary for counting CPU cycles."
[cpb]: https://en.wikipedia.org/wiki/Encryption_software#Performance
  "Observed CPU cycles divided by the number of input bytes."
[median]: https://en.wikipedia.org/wiki/Median
  "Median"
