# cavp-tests

Run [Cryptographic Algorithm Validation Program (CAVP)][cavp] tests
for the following algorithms:

* SHA-3 224
* SHA-3 256
* SHA-3 384
* SHA-3 512
* SHAKE128
* SHAKE256

The test functions themselves are in `main.c`, which is generated from
the [CAVP][] byte test vector archives by `gen-main.rb`.  These tests
are excluded from the tests in `sha3.c` because they are huge.

[cavp]: https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing
  "Cryptographic Algorithm Validation Program (CAVP)"
