# sha3.py

Load top-level `libsha3.so` in [Python][], calculate the digest of
hard-coded sample data with several `libsha3.so` functions and their
[hashlib][] equivalents, then print the results to standard output in
[CSV][] format.

## Usage

Run `make` in the top-level directory to build the `libsha3.so` shared
object, then run the example script like so:

    > python3 ./sha3.py
    algo,source,digest
    shake128-xof,hashlib,e408293a442d5f560a1d0a0204d7f80d725dfd87938f37f4795c87292c1e4f28
    shake128-xof,libsha3,e408293a442d5f560a1d0a0204d7f80d725dfd87938f37f4795c87292c1e4f28
    shake256-xof,hashlib,c1b3465ab14fe3b553bfbb4f78c936bcbe3f9246c493c129e566b99b0a6d6914
    shake256-xof,libsha3,c1b3465ab14fe3b553bfbb4f78c936bcbe3f9246c493c129e566b99b0a6d6914
    sha3-224,hashlib,b0226965a571c805ceea3e9d89d0614bf679ad795bc9fb0aa48750e0
    sha3-224,libsha3,b0226965a571c805ceea3e9d89d0614bf679ad795bc9fb0aa48750e0
    sha3-256,hashlib,c57ca12c10652293ca4fa61f3ee90d6896cf695d7b7891263e2f01cf2fa61cf8
    sha3-256,libsha3,c57ca12c10652293ca4fa61f3ee90d6896cf695d7b7891263e2f01cf2fa61cf8
    sha3-384,hashlib,d184840c5f1fe627ea81e2a8b763a36dafb1efa02ccd53118ac519cc51fb8ae1c1350da7707c5801fb944dce26afb813
    sha3-384,libsha3,d184840c5f1fe627ea81e2a8b763a36dafb1efa02ccd53118ac519cc51fb8ae1c1350da7707c5801fb944dce26afb813
    sha3-512,hashlib,dea8f3d0f7b6735dea209db3c8ab59c6fa19dd1abe6f07095d4e846c29da0e484ee8d8dc0bb567419ecfc806e00d4e61982b6536fdf40ad47cb4397628fc8c88
    sha3-512,libsha3,dea8f3d0f7b6735dea209db3c8ab59c6fa19dd1abe6f07095d4e846c29da0e484ee8d8dc0bb567419ecfc806e00d4e61982b6536fdf40ad47cb4397628fc8c88

[csv]: https://en.wikipedia.org/wiki/Comma-separated_values
  "Comma-separated value."
[python]: https://python.org/
  "Python programming language."
[ctypes]: https://docs.python.org/3/library/ctypes.html
  "Python ctypes module."
[hashlib]: https://docs.python.org/3/library/hashlib.html
  "Python hashlib module."
