#!/usr/bin/env python3

#
# sha3.py: Access libsha3.so functions via Python ctypes module and
# compare libsha3.so output against hashlib output.
#

import csv, ctypes, hashlib, sys

# common function argument types
XOF_ARGS = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t]
HASH_ARGS = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p]

# load sha3 module
sha3 = ctypes.CDLL('../../libsha3.so')

# set function argument and return types
# (note: this is not required, but it helps to prevent argument and type
# oopsies)
sha3.shake128_xof_once.argtypes = XOF_ARGS
sha3.shake128_xof_once.rettype = None
sha3.shake256_xof_once.argtypes = XOF_ARGS
sha3.shake256_xof_once.rettype = None
sha3.sha3_224.argtypes = HASH_ARGS
sha3.sha3_224.rettype = None
sha3.sha3_256.argtypes = HASH_ARGS
sha3.sha3_256.rettype = None
sha3.sha3_384.argtypes = HASH_ARGS
sha3.sha3_384.rettype = None
sha3.sha3_512.argtypes = HASH_ARGS
sha3.sha3_512.rettype = None

class Result:
  """Individual digest result"""
  algo : str
  source : str
  digest : bytes

  def __init__(self: object, algo: str, source: str, digest: bytes) -> None:
    """Create hash test result."""
    self.algo = algo
    self.source = source
    self.digest = digest

  def to_row(self: object) -> [str]:
    """Convert result to CSV row."""
    return [self.algo, self.source, self.digest.hex()]

class Results:
  """Collection of digest results."""
  rs : [Result]

  def __init__(self: object) -> None:
    self.rs = []

  def append(self: object, algo: str, source: str, digest: bytes) -> None:
    """append result to self"""
    self.rs.append(Result(algo, source, digest))

  def write(self, io) -> None:
    """write results as CSV to given IO."""
    cw = csv.writer(io) # create csv writer
    cw.writerow(['algo', 'source', 'digest']) # write headers

    # write rows
    for r in self.rs:
      cw.writerow(r.to_row())

def test_shake128_xof(rs: Results, data: bytes) -> None:
  """append SHAKE128-XOF digest of data from hashlib and libsha3 to results"""
  algo = 'shake128-xof' # algorithm name
  size = 32 # output size, in bytes

  # append hashlib digest
  rs.append(algo, 'hashlib', hashlib.shake_128(data).digest(size))

  # append libsha3 digest
  buf = ctypes.create_string_buffer(size)
  sha3.shake128_xof_once(data, len(data), buf, size)
  rs.append(algo, 'libsha3', buf.raw)

def test_shake256_xof(rs: Results, data: bytes) -> None:
  """append SHAKE256-XOF digest of data from hashlib and libsha3 to results"""
  algo = 'shake256-xof' # algorithm name
  size = 32 # output size, in bytes

  # append hashlib digest
  rs.append(algo, 'hashlib', hashlib.shake_256(data).digest(size))

  # append libsha3 digest
  buf = ctypes.create_string_buffer(size)
  sha3.shake256_xof_once(data, len(data), buf, size)
  rs.append(algo, 'libsha3', buf.raw)

def test_sha3_224(rs: Results, data: bytes) -> None:
  """append SHA3-224 digest of data from hashlib and libsha3 to results"""
  algo = 'sha3-224' # algorithm name

  # append hashlib digest
  rs.append(algo, 'hashlib', hashlib.sha3_224(data).digest())

  # append libsha3 digest
  buf = ctypes.create_string_buffer(28)
  sha3.sha3_224(data, len(data), buf)
  rs.append(algo, 'libsha3', buf.raw)

def test_sha3_256(rs: Results, data: bytes) -> None:
  """append SHA3-256 digest of data from hashlib and libsha3 to results"""
  algo = 'sha3-256' # algorithm name

  # append hashlib digest
  rs.append(algo, 'hashlib', hashlib.sha3_256(data).digest())

  # append libsha3 digest
  buf = ctypes.create_string_buffer(32)
  sha3.sha3_256(data, len(data), buf)
  rs.append(algo, 'libsha3', buf.raw)

def test_sha3_384(rs: Results, data: bytes) -> None:
  """append SHA3-384 digest of data from hashlib and libsha3 to results"""
  algo = 'sha3-384' # algorithm name

  # append hashlib digest
  rs.append(algo, 'hashlib', hashlib.sha3_384(data).digest())

  # append libsha3 digest
  buf = ctypes.create_string_buffer(48)
  sha3.sha3_384(data, len(data), buf)
  rs.append(algo, 'libsha3', buf.raw)

def test_sha3_512(rs: Results, data: bytes) -> None:
  """append SHA3-512 digest of data from hashlib and libsha3 to results"""
  algo = 'sha3-512' # algorithm name

  # append hashlib digest
  rs.append(algo, 'hashlib', hashlib.sha3_512(data).digest())

  # append libsha3 digest
  buf = ctypes.create_string_buffer(64)
  sha3.sha3_512(data, len(data), buf)
  rs.append(algo, 'libsha3', buf.raw)

# declare test data and results
DATA = b'foo bar'
rs = Results()

# run tests, add to results
test_shake128_xof(rs, DATA)
test_shake256_xof(rs, DATA)
test_sha3_224(rs, DATA)
test_sha3_256(rs, DATA)
test_sha3_384(rs, DATA)
test_sha3_512(rs, DATA)

# print results to stdout as csv
rs.write(sys.stdout)
