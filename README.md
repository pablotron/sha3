# sha3

Embeddable, dependency-free [C11][] implementation of the [SHA-3][]
[cryptographic hash functions][hash], [eXtendable Output Functions
(XOF)][xof], and [Hash-based Message Authentication Code functions
(HMAC)][hmac] defined in [FIPS 202][], [SP 800-185][800-185], and the
[draft KangarooTwelve and TurboSHAKE specification][turboshake-ietf].

Includes [AVX-512][] acceleration, [Doxygen][]-friendly [API][]
documentation, and a full test suite with sanitizers enabled and test
vectors from the [NIST CSRC "Examples With Intermediate Values"
site][csrc-examples] and the [Test Vectors section of the draft
KangarooTwelve and TurboSHAKE specification][turboshake-ietf-test-vectors].

The following algorithms are implemented:

* SHA3-224, SHA3-256, SHA3-384, and SHA3-512: [SHA-3][] [cryptographic
  hash functions][hash] with fixed-length output, as defined in section
  6.1 of [FIPS 202][].
* HMAC-SHA3-224, HMAC-SHA3-256, HMAC-SHA3-384, HMAC-SHA3-512:
  [HMAC][hmac] instantiated with [SHA-3][] hash functions, as specified
  in section 7 of [FIPS 202][], [RFC 2104][], and [FIPS 198-1][].
* SHAKE128, SHAKE128-XOF, SHAKE256, and SHAKE256-XOF: [SHA-3][]
  [XOFs][xof] with both fixed-length output and arbitrary-length output,
  as defined in section 6.2 of [FIPS 202][].
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
#include <stdint.h>
#include <stdio.h>
#include "hex.h"
#include "sha3.h"

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
#include <stdint.h>
#include <stdio.h>
#include "hex.h"
#include "sha3.h"

// test data
static const uint8_t DATA[] = "this is some test data";

int main(void) {
  // hash data
  uint8_t buf[200] = { 0 };
  shake128_xof_once(DATA, sizeof(DATA), buf, sizeof(buf));

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
```

See the `examples/` directory for more examples.

## Documentation

Full [API][] documentation is available in the comments of `sha3.h`.  If
you have [Doxygen][] installed, you can generate [HTML][]-formatted
[API][] documentation by typing `make doc`.

## References

* [FIPS 202 - SHA-3 Standard: Permutation-Based Hash and Extendable-Output Functions][FIPS 202]
* [NIST SP 800-185 - SHA-3 Derived Functions: cSHAKE, KMAC, TupleHash, and ParallelHash][800-185]
* [FIPS 198-1 - The Keyed-Hash Message Authentication Code (HMAC)][FIPS 198-1]
* [NIST CSRC: Cryptographic Standards and Guidelines: Examples with Intermediate Values][csrc-examples]
* [TurboSHAKE (PDF)][turboshake]
* [KangarooTwelve and TurboSHAKE][turboshake-ietf]

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
