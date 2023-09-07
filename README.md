# sha3

[C11][] implementations of the following [SHA-3][] algorithms from [FIPS 202][] and 
[SP 800-185][800-185]:

* SHA3-224
* SHA3-256
* SHA3-384
* SHA3-512
* HMAC-SHA3-224
* HMAC-SHA3-256
* HMAC-SHA3-384
* HMAC-SHA3-512
* SHAKE128 and SHAKE128-XOF
* SHAKE256 and SHAKE256-XOF
* cSHAKE128 and cSHAKE128-XOF
* cSHAKE256 and cSHAKE256-XOF
* KMAC128 and KMAC128-XOF
* KMAC256 and KMAC256-XOF
* TupleHash128 and TupleHash128-XOF
* TupleHash256 and TupleHash256-XOF
* ParallelHash128 and ParallelHash128-XOF
* ParallelHash256 and ParallelHash256-XOF
* TurboSHAKE128 and TurboSHAKE256
* KangarooTwelve

Use `make` to build a minimal test application, and `make test` to run
the test suite.

## Examples

```c
// sha3-256-example: print sha3-256 hash of data to standard output.
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
```

See the `examples/` directory for more examples.

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
[FIPS 202]: https://csrc.nist.gov/pubs/fips/202/final
  "SHA-3 Standard: Permutation-Based Hash and Extendable-Output Functions"
[800-185]: https://csrc.nist.gov/pubs/sp/800/185/final
  "SHA-3 Derived Functions: cSHAKE, KMAC, TupleHash, and ParallelHash"
[FIPS 198-1]: https://csrc.nist.gov/pubs/fips/198-1/final
  "The Keyed-Hash Message Authentication Code (HMAC)"
[csrc-examples]: https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/example-values
  "NIST CSRC: Cryptographic Standards and Guidelines: Examples with Intermediate Values"
[turboshake]: https://eprint.iacr.org/2023/342.pdf
  "TurboSHAKE"
[turboshake-ietf]: https://www.ietf.org/archive/id/draft-irtf-cfrg-kangarootwelve-10.html#name-test-vectors
  "KangarooTwelve and TurboSHAKE"
