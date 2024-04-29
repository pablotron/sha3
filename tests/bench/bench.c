//
// Benchmark all three ML-KEM parameter sets and print summary
// statistics to standard output in CSV format.
//
// Requires libcpucycles (https://cpucycles.cr.yp.to/).
//
// Note: You may need to adjust your system configuration or run `bench`
// as root to grant libcpucycles access to the high-resolution cycle
// counter.  See the following URL for details:
//
// https://cpucycles.cr.yp.to/security.html
//

#include <stdlib.h> // exit(), qsort()
#include <stdio.h> // printf()
#include <string.h> // memcmp()
#include <math.h> // sqrt(), pow()
#include <cpucycles.h> // cpucycles()
#include "sha3.h" // sha3_*(), shake*()
#include "rand-bytes.h" // rand_bytes()

// default number of trials
#define NUM_TRIALS 100000

// Random data used for key generation and encapsulation.
typedef struct {
  uint8_t keygen[64], // random data for keygen()
          encaps[32]; // random data for encaps()
} seeds_t;

// Aggregate statistics for a series of tests.
typedef struct {
  // min/max/median times
  long long lo, hi, median;

  // mean/stddev
  double mean, stddev;
} stats_t;

static void *checked_calloc(const char *name, const size_t nmemb, const size_t size) {
  // alloc keygen times
  void *mem = calloc(nmemb, size);
  if (!mem) {
    fprintf(stderr, "%s: calloc() failed\n", name);
    exit(-1);
  }
  return mem;
}

// Callback for `qsort()` to sort observed times in ascending order.
static int sort_asc_cb(const void *ap, const void *bp) {
  const long long *a = ap, *b = bp;
  return *a - *b;
}

// Get summary statistics of a series of test times.
static stats_t get_stats(long long * const vals, const size_t num_vals) {
  stats_t stats = { 0 };

  // sort values in ascending order (used for min, max, and median)
  qsort(vals, num_vals, sizeof(long long), sort_asc_cb);

  // get low, high, and median
  stats.lo = vals[0];
  stats.hi = vals[num_vals - 1];
  stats.median = vals[num_vals / 2];

  // calculate mean
  for (size_t i = 0; i < num_vals; i++) {
    stats.mean += vals[i];
  }
  stats.mean /= num_vals;

  // calculate standard deviation
  for (size_t i = 0; i < num_vals; i++) {
    stats.stddev += pow(stats.mean - vals[i], 2);
  }
  stats.stddev = sqrt(stats.stddev / num_vals);

  // return stats
  return stats;
}

// define xof benchmark function
#define DEF_BENCH_XOF(FN) \
  static stats_t bench_ ## FN (const size_t num_trials, const size_t src_len, const size_t dst_len) { \
    /* allocate times, src, and dst buffers */ \
    long long *times = checked_calloc(__func__, num_trials, sizeof(long long)); \
    uint8_t *src = checked_calloc(__func__, num_trials, src_len); \
    uint8_t *dst = checked_calloc(__func__, num_trials, dst_len); \
    \
    /* generate random source data */ \
    rand_bytes(src, num_trials * src_len); \
    \
    /* run trials */ \
    for (size_t i = 0; i < num_trials; i++) { \
      /* call function */ \
      const long long t0 = cpucycles(); \
      FN (src + (i * src_len), src_len, dst + (i * dst_len), dst_len); \
      const long long t1 = cpucycles() - t0; \
      \
      /* save time */ \
      times[i] = t1; \
    } \
    \
    /* generate summary stats */ \
    const stats_t stats = get_stats(times, num_trials); \
    \
    /* free buffers */ \
    free(src); \
    free(times); \
    \
    /* return summary stats */ \
    return stats; \
  }

// define hash benchmark function
#define DEF_BENCH_HASH(FN, OUT_LEN) \
  static stats_t bench_ ## FN (const size_t num_trials, const size_t src_len) { \
    /* allocate times and src buffers */ \
    long long *times = checked_calloc(__func__, num_trials, sizeof(long long)); \
    uint8_t *src = checked_calloc(__func__, src_len, 1); \
    \
    /* run trials */ \
    for (size_t i = 0; i < num_trials; i++) { \
      /* generate random source data */ \
      rand_bytes(src, src_len); \
      \
      /* call function */ \
      uint8_t dst[OUT_LEN] = { 0 }; \
      const long long t0 = cpucycles(); \
      FN (src, src_len, dst); \
      const long long t1 = cpucycles() - t0; \
      \
      /* save time */ \
      times[i] = t1; \
    } \
    \
    /* generate summary stats */ \
    const stats_t stats = get_stats(times, num_trials); \
    \
    /* free buffers */ \
    free(src); \
    free(times); \
    \
    /* return summary stats */ \
    return stats; \
  }

// define xof benchmarks *()
DEF_BENCH_XOF(shake128)
DEF_BENCH_XOF(shake256)

// define hash benchmarks
DEF_BENCH_HASH(sha3_224, 28)
DEF_BENCH_HASH(sha3_256, 32)
DEF_BENCH_HASH(sha3_384, 48)
DEF_BENCH_HASH(sha3_512, 64)

// print function stats to standard output as CSV row.
static void print_row(const char *name, const size_t src_len, const size_t dst_len, stats_t fs) {
    const double median_cpb = 1.0 * fs.median / src_len,
                 mean_cpb = 1.0 * fs.mean / src_len;
  printf("%s,%zu,%zu,%.0f,%.0f,%lld,%.0f,%.0f,%lld,%lld\n", name, dst_len, src_len, median_cpb, mean_cpb, fs.median, fs.mean, fs.stddev, fs.lo, fs.hi);
}

#define MIN_SRC_LEN 64
#define MAX_SRC_LEN 2048

#define MIN_DST_LEN 32
#define MAX_DST_LEN 128

int main(int argc, char *argv[]) {
  // get number of trials from first command-line argument, or fall back
  // to default if no argument was provided
  const size_t num_trials = (argc > 1) ? atoi(argv[1]) : NUM_TRIALS;
  if (num_trials < 2) {
    fprintf(stderr, "num_trials must be greater than 1\n");
    return -1;
  }

  // print metadata to stderr
  fprintf(stderr,"info: cpucycles: version=%s implementation=%s persecond=%lld\ninfo: num_trials=%zu\n", cpucycles_version(), cpucycles_implementation(), cpucycles_persecond(), num_trials);

  // print column headers to stdout
  printf("function,dst,src,median_cpb,mean_cpb,median_cycles,mean_cycles,stddev_cycles,min_cycles,max_cycles\n");

  // sha3-224
  for (size_t src_len = MIN_SRC_LEN; src_len < MAX_SRC_LEN; src_len <<= 1) {
    print_row("sha3_224", src_len, 28, bench_sha3_224(num_trials, src_len));
  }

  // sha3-256
  for (size_t src_len = MIN_SRC_LEN; src_len < MAX_SRC_LEN; src_len <<= 1) {
    print_row("sha3_256", src_len, 32, bench_sha3_256(num_trials, src_len));
  }

  // sha3-384
  for (size_t src_len = MIN_SRC_LEN; src_len < MAX_SRC_LEN; src_len <<= 1) {
    print_row("sha3_384", src_len, 48, bench_sha3_384(num_trials, src_len));
  }

  // sha3-512
  for (size_t src_len = MIN_SRC_LEN; src_len < MAX_SRC_LEN; src_len <<= 1) {
    print_row("sha3_512", src_len, 64, bench_sha3_512(num_trials, src_len));
  }

  for (size_t dst_len = MIN_DST_LEN; dst_len < MAX_DST_LEN; dst_len <<= 1) {
    // shake128
    for (size_t src_len = MIN_SRC_LEN; src_len < MAX_SRC_LEN; src_len <<= 1) {
      print_row("shake128", src_len, dst_len, bench_shake128(num_trials, src_len, dst_len));
    }

    // shake256
    for (size_t src_len = MIN_SRC_LEN; src_len < MAX_SRC_LEN; src_len <<= 1) {
      print_row("shake256", src_len, dst_len, bench_shake256(num_trials, src_len, dst_len));
    }
  }

  // return success
  return 0;
}
