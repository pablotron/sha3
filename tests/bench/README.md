# bench

Benchmark hash and XOF speed and then print summary statistics to
standard output in [CSV][] format and metadata to standard error.

Requires [libcpucycles][].

The columns of the [CSV][] printed to standard output are as follows:

* `function`: Function name.
* `dst`: Output size, in bytes.
* `src`: Input size, in bytes.
* `median_cpb`: [Median][] of observed CPU cycles divided by the input size.
* `mean_cpb`: [Arithmetic mean][mean] of observed CPU cycles divided by the input size.
* `median_cycles`: [Median][] of observed CPU cycles.
* `mean_cycles`: [Arithmetic mean][mean] of observed CPU cycles.
* `stddev_cycles`: [Standard deviation][stddev] of observed CPU cycles.
* `min_cycles`: Minimum observed CPU cycles.
* `max_cycles`: Maximum observed CPU cycles.

The metadata printed to standard error is as follows:

* `version`: version of [libcpucycles][] as reported by `cpucycles_version()`
* `implementation`: [libcpucycles][] backend as reported by `cpucycles_implementation()`
* `persecond`: CPU cycles per second, as reported by `cpucycles_persecond()`
* `num_trials`: Number of trials.

## Build

1. Install [libcpucycles][].
2. Type `make`.  Creates an executable named `./bench` in the current
   directory.

## Run

Type `./bench` to run benchmarks with the default number of trials
(100,000), or `./bench NUM` to run benchmarks with a custom number of
trials.

**Note:** You may need to adjust your system configuration or run
`bench` as root to grant [libcpucycles][] access to the high-resolution
cycle counter.

See [the libcpucycles security page][libcpucycles-security] for details.

## Examples

Below are example runs of `bench` on a ThinkPad X1 Carbon ([x86-64][],
[AVX-512][] backend) and on an [Odroid N2L][] ([ARM64][], scalar
backend).

### Lenovo ThinkPad X1 Carbon, 6th Gen (i7-1185G7)

```
# enable user-level RDPMC access (run as root)
root> echo 2 > /proc/sys/kernel/perf_event_paranoid

# print cpu and compiler info
> lscpu | grep -i '^model name:' | sed 's/.*: *//'
11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz
> gcc --version | head -1
gcc (Debian 12.2.0-14) 12.2.0

# benchmark with 100k trials
> ./bench
TODO...

# benchmark with 1M trials
> ./bench 1000000
TODO...
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

# benchmark with 100k trials
> ./bench
info: cpucycles: version=20240318 implementation=arm64-vct persecond=1800000000
info: num_trials=100000
TODO...
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
