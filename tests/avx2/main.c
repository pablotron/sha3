#include <stdint.h> // uint64_t, etc
#include <stdio.h> // fprintf()
#include <string.h> // memcpy()
#include <immintrin.h>
#include "rand-bytes.h" // rand_bytes()

// 64-bit rotate left
#define ROL(v, n) (((v) << (n)) | ((v) >> (64-(n))))

// write named state array to given stream.
static void state_write(FILE *f, const char * const name, uint64_t s[static 25]) {
  fprintf(f, "%s = ", name);
  for (size_t i = 0; i < 25; i++) {
    fprintf(f, "%s%016lx", i ? ", " : "", s[i]);
  }
  fputs("\n", f);
}

// check state array against expected value.  print differences and exit
// with an error if it does not match the expected value.
static void check_state(const char * const func, uint64_t got[static 25], uint64_t exp[static 25]) {
  if (memcmp(got, exp, 25 * sizeof(uint64_t))) {
    // write failure message, got, and exp to stderr
    fprintf(stderr, "%s() failed:\n", func);
    state_write(stderr, "got", got);
    state_write(stderr, "exp", exp);

    // print differences
    fputs("differences:\n", stderr);
    for (size_t i = 0; i < 25; i++) {
      if (got[i] != exp[i]) {
        fprintf(stderr, "  %2zu: got %016lx, exp %016lx\n", i, got[i], exp[i]);
      }
    }

    // exit with error code
    exit(-1);
  }
}

static const __m256i K64 = { 64, 64, 64, 64 };

// 64-bit rotate left
#define AVX2_ROLI(v, n) (_mm256_slli_epi64((v), (n)) | _mm256_srli_epi64((v), (64-(n))))
#define AVX2_ROLV(v, n) (_mm256_sllv_epi64((v), (n)) | _mm256_srlv_epi64((v), (K64-(n))))

// load state array to avx2 registers
#define LOAD(s) __m256i \
  r0_lo = _mm256_loadu_epi64(s +  0), /* row 0, cols 0-3 */ \
  r1_lo = _mm256_loadu_epi64(s +  5), /* row 1, cols 0-3 */ \
  r2_lo = _mm256_loadu_epi64(s + 10), /* row 2, cols 0-3 */ \
  r3_lo = _mm256_loadu_epi64(s + 15), /* row 3, cols 0-3 */ \
  r4_lo = _mm256_loadu_epi64(s + 20), /* row 4, cols 0-3 */ \
  r0_hi = { s[ 4] },                  /* row 0, col 4 */ \
  r1_hi = { s[ 9] },                  /* row 1, col 4 */ \
  r2_hi = { s[14] },                  /* row 2, col 4 */ \
  r3_hi = { s[19] },                  /* row 3, col 4 */ \
  r4_hi = { s[24] };                  /* row 4, col 4 */

// store avx2 registers to state array
#define STORE(s) do { \
  static const __m256i M0 = { ~0, 0, 0, 0 }; \
  union { long long int *i64; uint64_t *u64; } p = { .u64 = s }; \
  \
  /* store rows */ \
  _mm256_storeu_epi64(p.i64 +  0, r0_lo); /* row 0, cols 0-3 */ \
  _mm256_storeu_epi64(p.i64 +  5, r1_lo); /* row 1, cols 0-3 */ \
  _mm256_storeu_epi64(p.i64 + 10, r2_lo); /* row 2, cols 0-3 */ \
  _mm256_storeu_epi64(p.i64 + 15, r3_lo); /* row 3, cols 0-3 */ \
  _mm256_storeu_epi64(p.i64 + 20, r4_lo); /* row 4, cols 0-3 */ \
  _mm256_maskstore_epi64(p.i64 +  4, M0, r0_hi); /* row 0, col 4 */ \
  _mm256_maskstore_epi64(p.i64 +  9, M0, r1_hi); /* row 1, col 4 */ \
  _mm256_maskstore_epi64(p.i64 + 14, M0, r2_hi); /* row 2, col 4 */ \
  _mm256_maskstore_epi64(p.i64 + 19, M0, r3_hi); /* row 3, col 4 */ \
  _mm256_maskstore_epi64(p.i64 + 24, M0, r4_hi); /* row 4, col 4 */ \
} while (0)

// Iota round constants.
static const uint64_t RCS[] = {
  0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL, 0x8000000080008000ULL,
  0x000000000000808bULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
  0x000000000000008aULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
  0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
  0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800aULL, 0x800000008000000aULL,
  0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL,
};

// scalar noop, used to test avx2 load/store
static void noop_scalar(uint64_t a[static 25]) {
  (void) a;
}

/**
 * @brief Theta step of scalar Keccak permutation.
 * @param[in,out] a Keccak state (array of 25 64-bit integers).
 *
 * @note Only built when using the scalar backend or when building the
 * test suite.
 */
static inline void theta_scalar(uint64_t a[static 25]) {
  const uint64_t c[5] = {
    a[0] ^ a[5] ^ a[10] ^ a[15] ^ a[20],
    a[1] ^ a[6] ^ a[11] ^ a[16] ^ a[21],
    a[2] ^ a[7] ^ a[12] ^ a[17] ^ a[22],
    a[3] ^ a[8] ^ a[13] ^ a[18] ^ a[23],
    a[4] ^ a[9] ^ a[14] ^ a[19] ^ a[24],
  };

  const uint64_t d[5] = {
    c[4] ^ ROL(c[1], 1),
    c[0] ^ ROL(c[2], 1),
    c[1] ^ ROL(c[3], 1),
    c[2] ^ ROL(c[4], 1),
    c[3] ^ ROL(c[0], 1),
  };

  a[ 0] ^= d[0]; a[ 1] ^= d[1]; a[ 2] ^= d[2]; a[ 3] ^= d[3]; a[ 4] ^= d[4];
  a[ 5] ^= d[0]; a[ 6] ^= d[1]; a[ 7] ^= d[2]; a[ 8] ^= d[3]; a[ 9] ^= d[4];
  a[10] ^= d[0]; a[11] ^= d[1]; a[12] ^= d[2]; a[13] ^= d[3]; a[14] ^= d[4];
  a[15] ^= d[0]; a[16] ^= d[1]; a[17] ^= d[2]; a[18] ^= d[3]; a[19] ^= d[4];
  a[20] ^= d[0]; a[21] ^= d[1]; a[22] ^= d[2]; a[23] ^= d[3]; a[24] ^= d[4];
}

/**
 * @brief Rho step of scalar Keccak permutation.
 * @param[in,out] a Keccak state (array of 25 64-bit integers).
 *
 * @note Only built when using the scalar backend or when building the
 * test suite.
 */
static inline void rho_scalar(uint64_t a[static 25]) {
  a[ 1] = ROL(a[ 1],  1); // 1 % 64 = 1
  a[ 2] = ROL(a[ 2], 62); // 190 % 64 = 62
  a[ 3] = ROL(a[ 3], 28); // 28 % 64 = 28
  a[ 4] = ROL(a[ 4], 27); // 91 % 64 = 27

  a[ 5] = ROL(a[ 5], 36); // 36 % 64 = 36
  a[ 6] = ROL(a[ 6], 44); // 300 % 64 = 44
  a[ 7] = ROL(a[ 7],  6); // 6 % 64 = 6
  a[ 8] = ROL(a[ 8], 55); // 55 % 64 = 55
  a[ 9] = ROL(a[ 9], 20); // 276 % 64 = 20

  a[10] = ROL(a[10],  3); // 3 % 64 = 3
  a[11] = ROL(a[11], 10); // 10 % 64 = 10
  a[12] = ROL(a[12], 43); // 171 % 64 = 43
  a[13] = ROL(a[13], 25); // 153 % 64 = 25
  a[14] = ROL(a[14], 39); // 231 % 64 = 39

  a[15] = ROL(a[15], 41); // 105 % 64 = 41
  a[16] = ROL(a[16], 45); // 45 % 64 = 45
  a[17] = ROL(a[17], 15); // 15 % 64 = 15
  a[18] = ROL(a[18], 21); // 21 % 64 = 21
  a[19] = ROL(a[19],  8); // 136 % 64 = 8

  a[20] = ROL(a[20], 18); // 210 % 64 = 18
  a[21] = ROL(a[21],  2); // 66 % 64 = 2
  a[22] = ROL(a[22], 61); // 253 % 64 = 61
  a[23] = ROL(a[23], 56); // 120 % 64 = 56
  a[24] = ROL(a[24], 14); // 78 % 64 = 14
}

/**
 * @brief Pi step of scalar Keccak permutation.
 * @param[in,out] a Keccak state (array of 25 64-bit integers).
 *
 * @note Only built when using the scalar backend or when building the
 * test suite.
 */
static inline void pi_scalar(uint64_t s[static 25]) {
  uint64_t t[25] = { 0 };

  t[ 0] = s[ 0];
  t[ 1] = s[ 6];
  t[ 2] = s[12];
  t[ 3] = s[18];
  t[ 4] = s[24];

  t[ 5] = s[ 3];
  t[ 6] = s[ 9];
  t[ 7] = s[10];
  t[ 8] = s[16];
  t[ 9] = s[22];

  t[10] = s[ 1];
  t[11] = s[ 7];
  t[12] = s[13];
  t[13] = s[19];
  t[14] = s[20];

  t[15] = s[ 4];
  t[16] = s[ 5];
  t[17] = s[11];
  t[18] = s[17];
  t[19] = s[23];

  t[20] = s[ 2];
  t[21] = s[ 8];
  t[22] = s[14];
  t[23] = s[15];
  t[24] = s[21];

  memcpy(s, t, sizeof(t));
}

/**
 * @brief Chi step of scalar Keccak permutation.
 * @param[in,out] a Keccak state (array of 25 64-bit integers).
 *
 * @note Only built when using the scalar backend or when building the
 * test suite.
 */
static inline void chi_scalar(uint64_t s[static 25]) {
  uint64_t t[25] = { 0 };

  t[ 0] = s[ 0] ^ (~s[ 1] & s[ 2]);
  t[ 1] = s[ 1] ^ (~s[ 2] & s[ 3]);
  t[ 2] = s[ 2] ^ (~s[ 3] & s[ 4]);
  t[ 3] = s[ 3] ^ (~s[ 4] & s[ 0]);
  t[ 4] = s[ 4] ^ (~s[ 0] & s[ 1]);

  t[ 5] = s[ 5] ^ (~s[ 6] & s[ 7]);
  t[ 6] = s[ 6] ^ (~s[ 7] & s[ 8]);
  t[ 7] = s[ 7] ^ (~s[ 8] & s[ 9]);
  t[ 8] = s[ 8] ^ (~s[ 9] & s[ 5]);
  t[ 9] = s[ 9] ^ (~s[ 5] & s[ 6]);

  t[10] = s[10] ^ (~s[11] & s[12]);
  t[11] = s[11] ^ (~s[12] & s[13]);
  t[12] = s[12] ^ (~s[13] & s[14]);
  t[13] = s[13] ^ (~s[14] & s[10]);
  t[14] = s[14] ^ (~s[10] & s[11]);

  t[15] = s[15] ^ (~s[16] & s[17]);
  t[16] = s[16] ^ (~s[17] & s[18]);
  t[17] = s[17] ^ (~s[18] & s[19]);
  t[18] = s[18] ^ (~s[19] & s[15]);
  t[19] = s[19] ^ (~s[15] & s[16]);

  t[20] = s[20] ^ (~s[21] & s[22]);
  t[21] = s[21] ^ (~s[22] & s[23]);
  t[22] = s[22] ^ (~s[23] & s[24]);
  t[23] = s[23] ^ (~s[24] & s[20]);
  t[24] = s[24] ^ (~s[20] & s[21]);

  memcpy(s, t, sizeof(t));
}

// mock iota step of scalar keccak
// note: not the real iota, because it iterates all 24 round constants
static inline void iota_scalar(uint64_t a[static 25]) {
  for (size_t i = 0; i < 24; i++) {
    a[0] ^= RCS[i];
  }
}

// avx2 noop, used to test avx2 load/store
static void noop_avx2(uint64_t s[static 25]) {
  LOAD(s);
  STORE(s);
}

// theta step of avx2 keccak permutation.
static void theta_avx2(uint64_t s[static 25]) {
  LOAD(s);

  // c = xor(r0, r1, r2, r3, r4)
  const __m256i c_lo = r0_lo ^ r1_lo ^ r2_lo ^ r3_lo ^ r4_lo,
                c_hi = r0_hi ^ r1_hi ^ r2_hi ^ r3_hi ^ r4_hi;

  // avx512 permute ids (for reference)
  // static const __m512i I0 = { 4, 0, 1, 2, 3 },
  //                      I1 = { 1, 2, 3, 4, 0 };

  // masks
  static const __m256i M0 = { ~0,  0,  0,  0 }, // { 1, 0, 0, 0 }
                       M1 = { ~0, ~0, ~0,  0 }; // { 1, 1, 1, 0 }

  // permutes
  static const uint8_t I0_LO = 0x90, // 0, 0, 1, 2 -> 0b10010000 -> 0x90
                       I0_HI = 0x03, // 3, 0, 0, 0 -> 0b00000011 -> 0x03
                       I1_LO = 0x39, // 1, 2, 3, 0 -> 0b00111001 -> 0x39
                       I1_HI = 0x00; // 0, 0, 0, 0 -> 0b00000000 -> 0x00

  // d = xor(permute(i0, c), permute(i1, rol(c, 1)))
  const __m256i d0_lo = (_mm256_permute4x64_epi64(c_lo, I0_LO) & ~M0) | (c_hi & M0),
                d0_hi = _mm256_permute4x64_epi64(c_lo, I0_HI) & M0,
                d1_lo = (_mm256_permute4x64_epi64(c_lo, I1_LO) & M1) | (_mm256_permute4x64_epi64(c_hi, I1_HI) & ~M1),
                d1_hi = (c_lo & M0),
                d_lo = d0_lo ^ AVX2_ROLI(d1_lo, 1),
                d_hi = d0_hi ^ AVX2_ROLI(d1_hi, 1);

  // row = xor(row, d)
  r0_lo ^= d_lo; r1_lo ^= d_lo; r2_lo ^= d_lo; r3_lo ^= d_lo; r4_lo ^= d_lo;
  r0_hi ^= d_hi; r1_hi ^= d_hi; r2_hi ^= d_hi; r3_hi ^= d_hi; r4_hi ^= d_hi;

  STORE(s);
}

// rho step of avx2 keccak permutation.
static void rho_avx2(uint64_t s[static 25]) {
  LOAD(s);

  // rotate values
  static const __m256i V0_LO = {  0,  1, 62, 28 }, V0_HI = { 27 },
                       V1_LO = { 36, 44,  6, 55 }, V1_HI = { 20 },
                       V2_LO = {  3, 10, 43, 25 }, V2_HI = { 39 },
                       V3_LO = { 41, 45, 15, 21 }, V3_HI = {  8 },
                       V4_LO = { 18,  2, 61, 56 }, V4_HI = { 14 };

  // rotate rows
  // FIXME: could reduce rotates by permuting
  r0_lo = AVX2_ROLV(r0_lo, V0_LO); r0_hi = AVX2_ROLV(r0_hi, V0_HI);
  r1_lo = AVX2_ROLV(r1_lo, V1_LO); r1_hi = AVX2_ROLV(r1_hi, V1_HI);
  r2_lo = AVX2_ROLV(r2_lo, V2_LO); r2_hi = AVX2_ROLV(r2_hi, V2_HI);
  r3_lo = AVX2_ROLV(r3_lo, V3_LO); r3_hi = AVX2_ROLV(r3_hi, V3_HI);
  r4_lo = AVX2_ROLV(r4_lo, V4_LO); r4_hi = AVX2_ROLV(r4_hi, V4_HI);

  STORE(s);
}

// pi step of avx2 keccak permutation.
static inline void pi_avx2(uint64_t s[static 25]) {
  static const __m256i LM0 = { ~0,  0,  0,  0 },
                       LM1 = {  0, ~0,  0,  0 },
                       LM2 = {  0,  0, ~0,  0 },
                       LM3 = {  0,  0,  0, ~0 };
  static const uint8_t T0_LO = 0xe4, T0_HI = 0x00, // 0b11100100 -> 0xe4
                       T1_LO = 0x43, T1_HI = 0x02, // 0b01000011 -> 0x43
                       T2_LO = 0x39, T2_HI = 0x00, // 0b00111001 -> 0x39
                       T3_LO = 0x90, T3_HI = 0x03, // 0b10010000 -> 0x90
                       T4_LO = 0x0e, T4_HI = 0x01; // 0b00001110 -> 0x0e
  LOAD(s);
/* 
 *   static const __m256i V0_LO = {  0,  6, 12, 18 },
 *                        V1_LO = {  3,  9, 10, 16 },
 *                        V2_LO = {  1,  7, 13, 19 },
 *                        V3_LO = {  4,  5, 11, 17 },
 *                        V4_LO = {  2,  8, 14, 15 };
 *   static const size_t V0_HI = 24, V1_HI = 22, V2_HI = 20, V3_HI = 23, V4_HI = 21;
 */ 

  const __m256i t0_lo = (_mm256_permute4x64_epi64(r0_lo, T0_LO) & LM0) |
                        (_mm256_permute4x64_epi64(r1_lo, T0_LO) & LM1) |
                        (_mm256_permute4x64_epi64(r2_lo, T0_LO) & LM2) |
                        (_mm256_permute4x64_epi64(r3_lo, T0_LO) & LM3),
                t0_hi = (_mm256_permute4x64_epi64(r4_hi, T0_HI) & LM0),
                t1_lo = (_mm256_permute4x64_epi64(r0_lo, T1_LO) & LM0) |
                        (_mm256_permute4x64_epi64(r1_hi, T1_LO) & LM1) |
                        (_mm256_permute4x64_epi64(r2_lo, T1_LO) & LM2) |
                        (_mm256_permute4x64_epi64(r3_lo, T1_LO) & LM3),
                t1_hi = (_mm256_permute4x64_epi64(r4_lo, T1_HI) & LM0),
                t2_lo = (_mm256_permute4x64_epi64(r0_lo, T2_LO) & LM0) |
                        (_mm256_permute4x64_epi64(r1_lo, T2_LO) & LM1) |
                        (_mm256_permute4x64_epi64(r2_lo, T2_LO) & LM2) |
                        (_mm256_permute4x64_epi64(r3_hi, T2_LO) & LM3),
                t2_hi = (_mm256_permute4x64_epi64(r4_lo, T2_HI) & LM0),
                t3_lo = (_mm256_permute4x64_epi64(r0_hi, T3_LO) & LM0) |
                        (_mm256_permute4x64_epi64(r1_lo, T3_LO) & LM1) |
                        (_mm256_permute4x64_epi64(r2_lo, T3_LO) & LM2) |
                        (_mm256_permute4x64_epi64(r3_lo, T3_LO) & LM3),
                t3_hi = (_mm256_permute4x64_epi64(r4_lo, T3_HI) & LM0),
                t4_lo = (_mm256_permute4x64_epi64(r0_lo, T4_LO) & LM0) |
                        (_mm256_permute4x64_epi64(r1_lo, T4_LO) & LM1) |
                        (_mm256_permute4x64_epi64(r2_hi, T4_LO) & LM2) |
                        (_mm256_permute4x64_epi64(r3_lo, T4_LO) & LM3),
                t4_hi = (_mm256_permute4x64_epi64(r4_lo, T4_HI) & LM0);

  r0_lo = t0_lo; r0_hi = t0_hi;
  r1_lo = t1_lo; r1_hi = t1_hi;
  r2_lo = t2_lo; r2_hi = t2_hi;
  r3_lo = t3_lo; r3_hi = t3_hi;
  r4_lo = t4_lo; r4_hi = t4_hi;

  STORE(s);
}

// chi step of avx2 keccak permutation
static inline void chi_avx2(uint64_t s[static 25]) {
  // masks
  static const __m256i M0 = { ~0,  0,  0,  0 }, // { 1, 0, 0, 0 }
                       M1 = { ~0, ~0, ~0,  0 }, // { 1, 1, 1, 0 }
                       M2 = { ~0, ~0,  0, ~0 }; // { 1, 1, 0, 1 }

  // permutes
  static const uint8_t I0_LO = 0x39, // 1, 2, 3, 0 -> 0b00111001 -> 0x39
                       I1_LO = 0x0e, // 2, 3, 0, 0 -> 0b00001110 -> 0x0e
                       I1_HI = 0x01; // 1, 0, 0, 0 -> 0b00000001 -> 0x01
  
  LOAD(s);

  // r0
  {
    const __m256i a_lo = (_mm256_permute4x64_epi64(r0_lo, I0_LO) & M1) | (_mm256_permute4x64_epi64(r0_hi, I0_LO) & ~M1),
                  a_hi = r0_lo & M0,
                  b_lo = (_mm256_permute4x64_epi64(r0_lo, I1_LO) & M2) | (_mm256_permute4x64_epi64(r0_hi, I1_LO) & ~M0),
                  b_hi = _mm256_permute4x64_epi64(r0_lo, I1_HI) & M0;

    r0_lo ^= ~a_lo & b_lo; r0_hi ^= ~a_hi & b_hi; // r0 ^= ~a & b
  }

  // r1
  {
    const __m256i a_lo = (_mm256_permute4x64_epi64(r1_lo, I0_LO) & M1) | (_mm256_permute4x64_epi64(r1_hi, I0_LO) & ~M1),
                  a_hi = r1_lo & M0,
                  b_lo = (_mm256_permute4x64_epi64(r1_lo, I1_LO) & M2) | (_mm256_permute4x64_epi64(r1_hi, I1_LO) & ~M0),
                  b_hi = _mm256_permute4x64_epi64(r1_lo, I1_HI) & M0;

    r1_lo ^= ~a_lo & b_lo; r1_hi ^= ~a_hi & b_hi; // r1 ^= ~a & b
  }
  
  // r2
  {
    const __m256i a_lo = (_mm256_permute4x64_epi64(r2_lo, I0_LO) & M1) | (_mm256_permute4x64_epi64(r2_hi, I0_LO) & ~M1),
                  a_hi = r2_lo & M0,
                  b_lo = (_mm256_permute4x64_epi64(r2_lo, I1_LO) & M2) | (_mm256_permute4x64_epi64(r2_hi, I1_LO) & ~M0),
                  b_hi = _mm256_permute4x64_epi64(r2_lo, I1_HI) & M0;

    r2_lo ^= ~a_lo & b_lo; r2_hi ^= ~a_hi & b_hi; // r2 ^= ~a & b
  }

  // r3
  {
    const __m256i a_lo = (_mm256_permute4x64_epi64(r3_lo, I0_LO) & M1) | (_mm256_permute4x64_epi64(r3_hi, I0_LO) & ~M1),
                  a_hi = r3_lo & M0,
                  b_lo = (_mm256_permute4x64_epi64(r3_lo, I1_LO) & M2) | (_mm256_permute4x64_epi64(r3_hi, I1_LO) & ~M0),
                  b_hi = _mm256_permute4x64_epi64(r3_lo, I1_HI) & M0;

    r3_lo ^= ~a_lo & b_lo; r3_hi ^= ~a_hi & b_hi; // r3 ^= ~a & b
  }

  // r4
  {
    const __m256i a_lo = (_mm256_permute4x64_epi64(r4_lo, I0_LO) & M1) | (_mm256_permute4x64_epi64(r4_hi, I0_LO) & ~M1),
                  a_hi = r4_lo & M0,
                  b_lo = (_mm256_permute4x64_epi64(r4_lo, I1_LO) & M2) | (_mm256_permute4x64_epi64(r4_hi, I1_LO) & ~M0),
                  b_hi = _mm256_permute4x64_epi64(r4_lo, I1_HI) & M0;

    r4_lo ^= ~a_lo & b_lo; r4_hi ^= ~a_hi & b_hi; // r4 ^= ~a & b
  }

  STORE(s);
}

// mock iota step of avx2 keccak
// note: not the real iota, because it iterates all 24 round constants
static inline void iota_avx2(uint64_t s[static 25]) {
  LOAD(s);
  for (size_t i = 0; i < 24; i++) {
    const __m256i rc = { RCS[i], 0, 0, 0 };
    r0_lo ^= rc;
  }
  STORE(s);
}

#define DEF_TEST(STEP) void test_ ## STEP (void) { \
  /* get random data */ \
  uint64_t src[25] = { 0 }; \
  rand_bytes(src, sizeof(src)); \
  \
  /* get expected value */ \
  uint64_t exp[25] = { 0 }; \
  memcpy(exp, src, sizeof(src)); \
  STEP ## _scalar(exp); \
  \
  /* get test value */ \
  uint64_t got[25] = { 0 }; \
  memcpy(got, src, sizeof(src)); \
  STEP ## _avx2(got); \
  \
  /* check avx2 state against scalar state */ \
  check_state(__func__, got, exp); \
}

DEF_TEST(noop) // test load/store
DEF_TEST(theta) // test theta step
DEF_TEST(rho) // test rho step
DEF_TEST(pi) // test pi step
DEF_TEST(chi) // test chi step
DEF_TEST(iota) // test iota step

int main(void) {
  test_noop();
  test_theta();
  test_rho();
  test_pi();
  test_chi();
  test_iota();
  printf("ok\n");
  return 0;
}
