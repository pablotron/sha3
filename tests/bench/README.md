# bench

Benchmark [hash][] functions and [XOFs][xof], then print metadata to
standard error and print a table of [median][] [cycles per byte (cpb)][]
for each function and input message length to standard output in [CSV][]
format.

Requires [libcpucycles][].

The columns of the [CSV][] printed to standard output are as follows:

* `function`: Function name.
* `dst_len`: Output digest length, in bytes.
* `64`: [Median][] [cycles per byte (cpb)][cpb] for a 64 byte input message.
* `256`: [Median][] [cycles per byte (cpb)][cpb] for a 256 byte input message.
* `1024`: [Median][] [cycles per byte (cpb)][cpb] for a 1024 byte input message.
* `4096`: [Median][] [cycles per byte (cpb)][cpb] for a 4096 byte input message.
* `16384`: [Median][] [cycles per byte (cpb)][cpb] for a 16384 byte input message.

The metadata printed to standard error is as follows:

* `version`: version of [libcpucycles][] as reported by `cpucycles_version()`
* `implementation`: [libcpucycles][] backend as reported by `cpucycles_implementation()`
* `persecond`: CPU cycles per second, as reported by `cpucycles_persecond()`
* `backend`: Name of backend.  One of `avx512` or `scalar`.
* `num_trials`: Number of trials.
* `src_lens`: Comma-delimited list of input messages lengths, in bytes.
* `dst_lens`: Comma-delimited list of output digest lengths, in bytes
  (only used for [XOFs][]).

## Build

1. Install [libcpucycles][].
2. Type `make`.  Creates an executable named `./bench` in the current
   directory.

## Run

Type `./bench` to run benchmarks with the default number of trials
(2000), or `./bench NUM` to run benchmarks with a custom number of
trials.

**Note:** You may need to adjust your system configuration or run
`bench` as root to grant [libcpucycles][] access to the high-resolution
cycle counter.

See [the libcpucycles security page][libcpucycles-security] for details.

## Examples

Below are example runs of `bench` on a ThinkPad X1 Carbon ([x86-64][]),
a [Raspberry Pi 5][pi5] ([ARM64][]), and on an [Odroid N2L][]
([ARM64][]).

### Lenovo ThinkPad X1 Carbon, 6th Gen (i7-1185G7)

```
# enable user-level RDPMC access (run as root)
root> echo 2 > /proc/sys/kernel/perf_event_paranoid

# print cpu and compiler info
> lscpu | grep -i '^model name:' | sed 's/.*: *//'
11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz
> gcc --version | head -1
gcc (Debian 12.2.0-14) 12.2.0

# compile with avx512 backend
> make clean all BACKEND=2

# benchmark with 2k trials
> ./bench
info: cpucycles: version=20240318 implementation=amd64-pmc persecond=4800000000
info: backend=avx512 num_trials=2000 src_lens=64,256,1024,4096,16384 dst_lens=32
function,dst_len,64,256,1024,4096,16384
sha3_224,28,15.4,7.8,7.8,7.1,7.0
sha3_256,32,15.4,7.8,7.8,7.6,7.4
sha3_384,48,15.5,11.7,9.8,9.8,9.7
sha3_512,64,15.4,15.5,14.6,13.9,13.9
shake128,32,15.5,7.8,6.9,6.2,6.1
shake256,32,15.6,7.8,7.9,7.6,7.4

# compile with scalar backend
> make clean all BACKEND=1
...

# benchmark scalar backend with 2k trials
> ./bench
info: cpucycles: version=20240318 implementation=amd64-pmc persecond=4800000000
info: backend=scalar num_trials=2000 src_lens=64,256,1024,4096,16384 dst_lens=32
function,dst_len,64,256,1024,4096,16384
sha3_224,28,19.5,10.1,9.9,8.9,8.8
sha3_256,32,19.5,10.1,9.9,9.5,9.3
sha3_384,48,20.2,14.8,12.3,12.2,12.0
sha3_512,64,19.9,19.5,18.1,17.1,17.1
shake128,32,20.0,10.0,8.7,7.7,7.6
shake256,32,19.6,10.1,9.9,9.5,9.3
```

#### Raspberry Pi 5 (Cortex-A76)

```
# print cpu and compiler info
> lscpu |grep -i '^model name:' | sed 's/.*: *//'
Cortex-A76
> gcc --version | head -1
gcc (Debian 12.2.0-14) 12.2.0

# compile with scalar backend
> make clean all BACKEND=1
...

# benchmark scalar backend with 2k trials
> ./bench 2000
info: cpucycles: version=20240318 implementation=arm64-vct persecond=2400000000
info: backend=scalar num_trials=2000 src_lens=64,256,1024,4096,16384 dst_lens=32
function,dst_len,64,256,1024,4096,16384
sha3_224,28,20.2,10.3,10.3,9.3,9.2
sha3_256,32,20.2,10.3,10.3,9.9,9.7
sha3_384,48,20.9,15.3,12.8,12.7,12.5
sha3_512,64,20.2,20.2,18.9,25.3,17.9
shake128,32,20.2,10.1,9.0,8.1,7.9
shake256,32,20.2,10.3,10.3,9.9,9.7

# compile with neon backend
> make clean all BACKEND=3
...

# benchmark neon backend with 2k trials
> ./bench 2000
info: cpucycles: version=20240318 implementation=arm64-vct persecond=2400000000
info: backend=neon num_trials=2000 src_lens=64,256,1024,4096,16384 dst_lens=32
function,dst_len,64,256,1024,4096,16384
sha3_224,28,32.7,16.3,16.4,14.9,14.6
sha3_256,32,32.0,16.2,16.4,15.9,15.5
sha3_384,48,32.7,24.2,20.4,20.2,20.0
sha3_512,64,32.0,32.2,30.1,28.6,28.5
shake128,32,32.7,16.2,14.2,12.8,12.5
shake256,32,32.7,16.2,16.3,15.7,15.4
```

### Odroid N2L (Cortex-A73)

```
# enable user-level perf_even access (run as root)
root> echo 2 > /proc/sys/kernel/perf_event_paranoid

# print cpu and compiler info
> lscpu | grep -i '^model name' | sed 's/.*: *//'
Cortex-A73
> gcc --version | head -1
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

# compile with scalar backend
> make clean all BACKEND=1
...

# benchmark scalar backend with 2k trials
> ./bench 2000
info: cpucycles: version=20240318 implementation=arm64-vct persecond=1800000000
info: backend=scalar num_trials=2000 src_lens=64,256,1024,4096,16384 dst_lens=32
function,dst_len,64,256,1024,4096,16384
sha3_224,28,34.0,16.1,15.5,14.0,13.7
sha3_256,32,34.0,16.1,15.4,14.8,14.4
sha3_384,48,34.0,23.4,19.0,18.8,18.6
sha3_512,64,34.0,30.8,28.1,26.5,26.5
shake128,32,34.0,16.1,13.6,12.1,11.8
shake256,32,34.0,16.1,15.5,14.8,14.4

# compile with neon backend
> make clean all BACKEND=3

# benchmark neon backend with 2k trials
> ./bench 2000
info: cpucycles: version=20240318 implementation=arm64-vct persecond=1800000000
info: backend=neon num_trials=2000 src_lens=64,256,1024,4096,16384 dst_lens=32
function,dst_len,64,256,1024,4096,16384
sha3_224,28,66.8,32.5,32.1,29.0,28.5
sha3_256,32,66.8,32.5,31.9,30.8,30.0
sha3_384,48,66.8,48.3,39.8,39.6,39.0
sha3_512,64,66.8,63.9,59.1,56.0,55.9
shake128,32,66.8,32.5,28.3,25.3,24.7
shake256,32,66.8,32.5,32.0,30.9,30.1
```

[csv]: https://en.wikipedia.org/wiki/Comma-separated_values
  "Comma-Separated Value (CSV)"
[libcpucycles]: https://cpucycles.cr.yp.to/
  "Microlibrary for counting CPU cycles."
[libcpucycles-security]: https://cpucycles.cr.yp.to/security.html
  "libcpucycles security documentation"
[median]: https://en.wikipedia.org/wiki/Median
  "Median"
[mean]: https://en.wikipedia.org/wiki/Arithmetic_mean
  "Arithmetic mean"
[stddev]: https://en.wikipedia.org/wiki/Standard_deviation
  "Standard deviation"
[odroid n2l]: https://en.odroid.se/products/odroid-n2l-4gb
  "Odroid N2L"
[x86-64]: https://en.wikipedia.org/wiki/X86-64
  "64-bit x86 instruction set."
[arm64]: https://en.wikipedia.org/wiki/AArch64
  "64-bit extension to the ARM instruction set."
[avx-512]: https://en.wikipedia.org/wiki/AVX-512
  "AVX-512: 512-bit extensions to the Advanced Vector Extensions (AVX) instruction set."
[cpb]: https://en.wikipedia.org/wiki/Encryption_software#Performance
  "Observed CPU cycles divided by the number of input bytes."
[xof]: https://en.wikipedia.org/wiki/Extendable-output_function
  "Extendable-Output Function (XOF)"
[hash]: https://en.wikipedia.org/wiki/Cryptographic_hash_function
  "Cryptographic hash function"
[pi5]: https://en.wikipedia.org/wiki/Raspberry_Pi
  "Raspberry Pi 5"
