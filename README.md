# sha3

[C11] implementations of the following [SHA-3][] algorithms from [FIPS 202][] and 
[SP 800-185][800-185]:

* SHA3-224
* SHA3-256
* SHA3-384
* SHA3-512
* SHAKE128 (fixed and XOF)
* SHAKE256 (fixed and XOF)
* cSHAKE128 (fixed and XOF)
* cSHAKE256 (fixed and XOF)
* KMAC128 (fixed and XOF)
* KMAC256 (fixed and XOF)
* TupleHash128 (fixed and XOF)
* TupleHash256 (fixed and XOF)
* ParallelHash128 (fixed and XOF)
* ParallelHash256 (fixed and XOF)

Use `make` to build a minimal test application, and `make test` to run
the built-in tests.

[C11]: https://en.wikipedia.org/wiki/C11_(C_standard_revision)
  "ISO/IEC 9899:2011"
[SHA-3]: https://en.wikipedia.org/wiki/SHA-3
  "Secure Hash Algorithm 3"
[FIPS 202]: https://csrc.nist.gov/pubs/fips/202/final
  "SHA-3 Standard: Permutation-Based Hash and Extendable-Output Functions"
[800-185]: https://csrc.nist.gov/pubs/sp/800/185/final
  "SHA-3 Derived Functions: cSHAKE, KMAC, TupleHash, and ParallelHash"
