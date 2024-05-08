//
// Benchmark hash functions and extendable output functions (XOFs), then
// print metadata to standard error and print a table of median cycles
// per byte (cpb) for each function and input message length to standard
// output in CSV format.
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
#define NUM_TRIALS 2000

// input sizes (used for hashes and xofs)
static const size_t SRC_LENS[] = { 64, 256, 1024, 4096, 16384 };
#define NUM_SRC_LENS (sizeof(SRC_LENS)/sizeof(SRC_LENS[0]))

// output sizes (used for xofs)
static const size_t DST_LENS[] = { 32 };
#define NUM_DST_LENS (sizeof(DST_LENS)/sizeof(DST_LENS[0]))

// get maximum source length
static size_t get_max_src_len(void) {
  size_t r = 0;

  for (size_t i = 0; i < NUM_SRC_LENS; i++) {
    r = (SRC_LENS[i] > r) ? SRC_LENS[i] : r;
  }

  return r;
}

// Aggregate statistics for a series of tests.
typedef struct {
  // min/max/median times
  long long lo, hi, median;

  // mean/stddev, median_cpb
  double mean, stddev, median_cpb;
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
static stats_t get_stats(long long * const vals, const size_t num_vals, const size_t len) {
  stats_t stats = { 0 };

  // sort values in ascending order (used for min, max, and median)
  qsort(vals, num_vals, sizeof(long long), sort_asc_cb);

  // get low, high, and median
  stats.lo = vals[0];
  stats.hi = vals[num_vals - 1];
  stats.median = vals[num_vals / 2];

  // calculate median cpb
  stats.median_cpb = 1.0 * stats.median / len;

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
  static void bench_ ## FN (double * const cpbs, const size_t num_trials, const size_t dst_len) { \
    /* allocate times, src, and dst buffers */ \
    long long *times = checked_calloc(__func__, num_trials, sizeof(long long)); \
    uint8_t *src = checked_calloc(__func__, 1, get_max_src_len()); \
    uint8_t *dst = checked_calloc(__func__, num_trials, dst_len); \
    \
    for (size_t i = 0; i < NUM_SRC_LENS; i++) { \
      const size_t src_len = SRC_LENS[i]; /* get source length */ \
      \
      /* run trials */ \
      for (size_t j = 0; j < num_trials; j++) { \
        /* generate random source data */ \
        rand_bytes(src, src_len); \
        \
        /* call function */ \
        const long long t0 = cpucycles(); \
        FN (src, src_len, dst + (j * dst_len), dst_len); \
        const long long t1 = cpucycles() - t0; \
        \
        /* save time */ \
        times[j] = t1; \
      } \
      \
      /* generate summary stats, save cpb */ \
      cpbs[i] = 1.0 * get_stats(times, num_trials, src_len).median_cpb; \
    } \
    \
    /* free buffers */ \
    free(times); \
    free(src); \
    free(dst); \
  }

// define hash benchmark function
#define DEF_BENCH_HASH(FN, OUT_LEN) \
  static void bench_ ## FN (double * const cpbs, const size_t num_trials) { \
    /* allocate times, src, and dst buffers */ \
    long long *times = checked_calloc(__func__, num_trials, sizeof(long long)); \
    uint8_t *src = checked_calloc(__func__, 1, get_max_src_len()); \
    uint8_t *dst = checked_calloc(__func__, num_trials, OUT_LEN); \
    \
    for (size_t i = 0; i < NUM_SRC_LENS; i++) { \
      const size_t src_len = SRC_LENS[i]; /* get source length */ \
      \
      /* run trials */ \
      for (size_t j = 0; j < num_trials; j++) { \
        /* generate random source data */ \
        rand_bytes(src, src_len); \
        \
        /* call function */ \
        const long long t0 = cpucycles(); \
        FN (src, src_len, dst + (j * OUT_LEN)); \
        const long long t1 = cpucycles() - t0; \
        \
        /* save time */ \
        times[j] = t1; \
      } \
      \
      /* generate summary stats, save cpb */ \
      cpbs[i] = 1.0 * get_stats(times, num_trials, src_len).median_cpb; \
    } \
    \
    /* free buffers */ \
    free(times); \
    free(src); \
    free(dst); \
  }

// define xof benchmarks
DEF_BENCH_XOF(shake128)
DEF_BENCH_XOF(shake256)

// define hash benchmarks
DEF_BENCH_HASH(sha3_224, 28)
DEF_BENCH_HASH(sha3_256, 32)
DEF_BENCH_HASH(sha3_384, 48)
DEF_BENCH_HASH(sha3_512, 64)

// print function stats to standard output as CSV row.
static void print_row(const char *name, const size_t dst_len, double * const cpbs) {
  printf("%s,%zu", name, dst_len);
  for (size_t i = 0; i < NUM_SRC_LENS; i++) {
    printf(",%.1f", cpbs[i]);
  }
  fputs("\n", stdout);
}

int main(int argc, char *argv[]) {
  double cpbs[NUM_SRC_LENS];

  // get number of trials from first command-line argument, or fall back
  // to default if no argument was provided
  const size_t num_trials = (argc > 1) ? atoi(argv[1]) : NUM_TRIALS;
  if (num_trials < 2) {
    fprintf(stderr, "num_trials must be greater than 1\n");
    return -1;
  }

  // print metadata to stderr
  fprintf(stderr,"info: cpucycles: version=%s implementation=%s persecond=%lld\ninfo: backend=%s num_trials=%zu src_lens", cpucycles_version(), cpucycles_implementation(), cpucycles_persecond(), sha3_backend(), num_trials);
  for (size_t i = 0; i < NUM_SRC_LENS; i++) {
    fprintf(stderr, "%s%zu", (i > 0) ? "," : "=", SRC_LENS[i]);
  }
  fputs(" dst_lens", stderr);
  for (size_t i = 0; i < NUM_DST_LENS; i++) {
    fprintf(stderr, "%s%zu", (i > 0) ? "," : "=", DST_LENS[i]);
  }
  fputs("\n", stderr);

  // print column headers to stdout
  fputs("function,dst_len", stdout);
  for (size_t i = 0; i < NUM_SRC_LENS; i++) {
    printf(",%zu", SRC_LENS[i]);
  }
  fputs("\n", stdout);

  // sha3-224
  bench_sha3_224(cpbs, num_trials);
  print_row("sha3_224", 28, cpbs);

  // sha3-256
  bench_sha3_256(cpbs, num_trials);
  print_row("sha3_256", 32, cpbs);

  // sha3-384
  bench_sha3_384(cpbs, num_trials);
  print_row("sha3_384", 48, cpbs);

  // sha3-512
  bench_sha3_512(cpbs, num_trials);
  print_row("sha3_512", 64, cpbs);

  // test xofs
  for (size_t i = 0; i < NUM_DST_LENS; i++) {
    const size_t dst_len = DST_LENS[i];

    // shake128
    bench_shake128(cpbs, num_trials, dst_len);
    print_row("shake128", dst_len, cpbs);

    // shake256
    bench_shake256(cpbs, num_trials, dst_len);
    print_row("shake256", dst_len, cpbs);
  }

  // return success
  return 0;
}
