#include <stdint.h> // uint64_t
#include <string.h> // memcpy()
#include <stdio.h> // fprintf()
#include <time.h> // time()

// 64-bit rotate left
#define ROL(v, n) (((v) << (n)) | ((v) >> (64-(n))))

// minimum of two values
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// number of rounds for permute()
#define SHA3_NUM_ROUNDS 24

static void theta_scalar(uint64_t a[static 25]) {
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

static void rho_scalar(uint64_t a[static 25]) {
  a[1] = ROL(a[1], 1); // 1 % 64 = 1
  a[2] = ROL(a[2], 62); // 190 % 64 = 62
  a[3] = ROL(a[3], 28); // 28 % 64 = 28
  a[4] = ROL(a[4], 27); // 91 % 64 = 27
  a[5] = ROL(a[5], 36); // 36 % 64 = 36
  a[6] = ROL(a[6], 44); // 300 % 64 = 44
  a[7] = ROL(a[7], 6); // 6 % 64 = 6
  a[8] = ROL(a[8], 55); // 55 % 64 = 55
  a[9] = ROL(a[9], 20); // 276 % 64 = 20
  a[10] = ROL(a[10], 3); // 3 % 64 = 3
  a[11] = ROL(a[11], 10); // 10 % 64 = 10
  a[12] = ROL(a[12], 43); // 171 % 64 = 43
  a[13] = ROL(a[13], 25); // 153 % 64 = 25
  a[14] = ROL(a[14], 39); // 231 % 64 = 39
  a[15] = ROL(a[15], 41); // 105 % 64 = 41
  a[16] = ROL(a[16], 45); // 45 % 64 = 45
  a[17] = ROL(a[17], 15); // 15 % 64 = 15
  a[18] = ROL(a[18], 21); // 21 % 64 = 21
  a[19] = ROL(a[19], 8); // 136 % 64 = 8
  a[20] = ROL(a[20], 18); // 210 % 64 = 18
  a[21] = ROL(a[21], 2); // 66 % 64 = 2
  a[22] = ROL(a[22], 61); // 253 % 64 = 61
  a[23] = ROL(a[23], 56); // 120 % 64 = 56
  a[24] = ROL(a[24], 14); // 78 % 64 = 14
}

static void pi_scalar(uint64_t a[static 25]) {
  uint64_t t[25] = { 0 };
  memcpy(t, a, sizeof(t));
  a[1] = t[6];
  a[2] = t[12];
  a[3] = t[18];
  a[4] = t[24];
  a[5] = t[3];
  a[6] = t[9];
  a[7] = t[10];
  a[8] = t[16];
  a[9] = t[22];
  a[10] = t[1];
  a[11] = t[7];
  a[12] = t[13];
  a[13] = t[19];
  a[14] = t[20];
  a[15] = t[4];
  a[16] = t[5];
  a[17] = t[11];
  a[18] = t[17];
  a[19] = t[23];
  a[20] = t[2];
  a[21] = t[8];
  a[22] = t[14];
  a[23] = t[15];
  a[24] = t[21];
}

static void chi_scalar(uint64_t a[static 25]) {
  uint64_t t[25] = { 0 };
  memcpy(t, a, sizeof(t));
  a[0] = t[0] ^ (~t[1] & t[2]);
  a[1] = t[1] ^ (~t[2] & t[3]);
  a[2] = t[2] ^ (~t[3] & t[4]);
  a[3] = t[3] ^ (~t[4] & t[0]);
  a[4] = t[4] ^ (~t[0] & t[1]);
  a[5] = t[5] ^ (~t[6] & t[7]);
  a[6] = t[6] ^ (~t[7] & t[8]);
  a[7] = t[7] ^ (~t[8] & t[9]);
  a[8] = t[8] ^ (~t[9] & t[5]);
  a[9] = t[9] ^ (~t[5] & t[6]);
  a[10] = t[10] ^ (~t[11] & t[12]);
  a[11] = t[11] ^ (~t[12] & t[13]);
  a[12] = t[12] ^ (~t[13] & t[14]);
  a[13] = t[13] ^ (~t[14] & t[10]);
  a[14] = t[14] ^ (~t[10] & t[11]);
  a[15] = t[15] ^ (~t[16] & t[17]);
  a[16] = t[16] ^ (~t[17] & t[18]);
  a[17] = t[17] ^ (~t[18] & t[19]);
  a[18] = t[18] ^ (~t[19] & t[15]);
  a[19] = t[19] ^ (~t[15] & t[16]);
  a[20] = t[20] ^ (~t[21] & t[22]);
  a[21] = t[21] ^ (~t[22] & t[23]);
  a[22] = t[22] ^ (~t[23] & t[24]);
  a[23] = t[23] ^ (~t[24] & t[20]);
  a[24] = t[24] ^ (~t[20] & t[21]);
}

static void iota_scalar(uint64_t a[static 25], const int i) {
  // round constants (ambiguous in spec)
  static const uint64_t RCS[] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL,
  };

  a[0] ^= RCS[i];
}

// keccak permutation (scalar implementation).
void permute_scalar(uint64_t a[static 25], const size_t num_rounds) {
  for (int i = 0; i < (int) num_rounds; i++) {
    theta_scalar(a);
    rho_scalar(a);
    pi_scalar(a);
    chi_scalar(a);
    iota_scalar(a, 24 - num_rounds + i);
  }
}

#ifdef __AVX512F__
#include <immintrin.h>

static void theta_avx512(uint64_t s[static 25]) {
  // unaligned load mask and permutation indices
  uint8_t mask = 0x1f;
  const __mmask8 m = _load_mask8(&mask);
  const __m512i i0 = _mm512_setr_epi64(4, 0, 1, 2, 3, 5, 6, 7),
                i1 = _mm512_setr_epi64(1, 2, 3, 4, 0, 5, 6, 7);

  // load rows
  __m512i r0 = _mm512_maskz_loadu_epi64(m, (void*) (s)),
          r1 = _mm512_maskz_loadu_epi64(m, (void*) (s + 5)),
          r2 = _mm512_maskz_loadu_epi64(m, (void*) (s + 10)),
          r3 = _mm512_maskz_loadu_epi64(m, (void*) (s + 15)),
          r4 = _mm512_maskz_loadu_epi64(m, (void*) (s + 20));

  // c = xor(r0, r1, r2, r3, r4)
  const __m512i r01 = _mm512_xor_epi64(r0, r1),
                r23 = _mm512_xor_epi64(r2, r3),
                r03 = _mm512_xor_epi64(r01, r23),
                c = _mm512_xor_epi64(r03, r4);

  // d = xor(permute(i0, c), permute(i1, rol(c, 1)))
  const __m512i d0 = _mm512_permutexvar_epi64(i0, c),
                d1 = _mm512_permutexvar_epi64(i1, _mm512_rol_epi64(c, 1)),
                d = _mm512_xor_epi64(d0, d1);

  // row = xor(row, d)
  r0 = _mm512_xor_epi64(r0, d);
  r1 = _mm512_xor_epi64(r1, d);
  r2 = _mm512_xor_epi64(r2, d);
  r3 = _mm512_xor_epi64(r3, d);
  r4 = _mm512_xor_epi64(r4, d);

  // store rows
  _mm512_mask_storeu_epi64((void*) (s), m, r0),
  _mm512_mask_storeu_epi64((void*) (s + 5), m, r1),
  _mm512_mask_storeu_epi64((void*) (s + 10), m, r2),
  _mm512_mask_storeu_epi64((void*) (s + 15), m, r3),
  _mm512_mask_storeu_epi64((void*) (s + 20), m, r4);
}

static void rho_avx512(uint64_t s[static 25]) {
  // unaligned load mask and rotate values
  uint8_t mask = 0x1f;
  const __mmask8 m = _load_mask8(&mask);
  static uint64_t vs0[8] = { 0, 1, 62, 28, 27, 0, 0, 0 },
                  vs1[8] = { 36, 44, 6, 55, 20, 0, 0, 0 },
                  vs2[8] = { 3, 10, 43, 25, 39, 0, 0, 0 },
                  vs3[8] = { 41, 45, 15, 21, 8, 0, 0, 0 },
                  vs4[8] = { 18, 2, 61, 56, 14, 0, 0, 0 };

  // load rotate values
  const __m512i v0 = _mm512_loadu_epi64((void*) vs0),
                v1 = _mm512_loadu_epi64((void*) vs1),
                v2 = _mm512_loadu_epi64((void*) vs2),
                v3 = _mm512_loadu_epi64((void*) vs3),
                v4 = _mm512_loadu_epi64((void*) vs4);

  // load rows
  __m512i r0 = _mm512_maskz_loadu_epi64(m, (void*) (s)),
          r1 = _mm512_maskz_loadu_epi64(m, (void*) (s + 5)),
          r2 = _mm512_maskz_loadu_epi64(m, (void*) (s + 10)),
          r3 = _mm512_maskz_loadu_epi64(m, (void*) (s + 15)),
          r4 = _mm512_maskz_loadu_epi64(m, (void*) (s + 20));

  // rotate
  r0 = _mm512_rolv_epi64(r0, v0);
  r1 = _mm512_rolv_epi64(r1, v1);
  r2 = _mm512_rolv_epi64(r2, v2);
  r3 = _mm512_rolv_epi64(r3, v3);
  r4 = _mm512_rolv_epi64(r4, v4);

  // store rows
  _mm512_mask_storeu_epi64((void*) (s), m, r0),
  _mm512_mask_storeu_epi64((void*) (s + 5), m, r1),
  _mm512_mask_storeu_epi64((void*) (s + 10), m, r2),
  _mm512_mask_storeu_epi64((void*) (s + 15), m, r3),
  _mm512_mask_storeu_epi64((void*) (s + 20), m, r4);
}

static void pi_avx512(uint64_t s[static 25]) {
  // mask bytes
  uint8_t mask = 0x1f,
          m01b = 0x03,
          m23b = 0x0c,
          m4b = 0x10;

  // load masks
  const __mmask8 m = _load_mask8(&mask),
                 m01 = _load_mask8(&m01b),
                 m23 = _load_mask8(&m23b),
                 m4 = _load_mask8(&m4b);
  // permutation indices (offsets into state array)
  // static uint64_t vs0[8] = { 0, 6, 12, 18, 24, 0, 0, 0 },
  //                 vs1[8] = { 3, 9, 10, 16, 22, 0, 0, 0 },
  //                 vs2[8] = { 1, 7, 13, 19, 20, 0, 0, 0 },
  //                 vs3[8] = { 4, 5, 11, 17, 23, 0, 0, 0 },
  //                 vs4[8] = { 2, 8, 14, 15, 21, 0, 0, 0 };

  // permutation indices
  //
  // (note: these are masked so only the relevant indices for
  // _mm512_maskz_permutex2var_epi64() in each array are filled in)
  static uint64_t t0_pis_01[8] = { 0, 8 + 1, 0, 0, 0, 0, 0, 0 },
                  t0_pis_23[8] = { 0, 0, 2, 8 + 3, 0, 0, 0, 0 },
                  t0_pis_4[8] = { 0, 0, 0, 0, 4, 0, 0, 0 },

                  t1_pis_01[8] = { 3, 8 + 4, 0, 0, 0, 0, 0, 0 },
                  t1_pis_23[8] = { 0, 0, 0, 8 + 1, 0, 0, 0, 0 },
                  t1_pis_4[8] = { 0, 0, 0, 0, 2, 0, 0, 0 },

                  t2_pis_01[8] = { 1, 8 + 2, 0, 0, 0, 0, 0, 0 },
                  t2_pis_23[8] = { 0, 0, 3, 8 + 4, 0, 0, 0, 0 },
                  t2_pis_4[8] = { 0, 0, 0, 0, 0, 0, 0, 0 },

                  t3_pis_01[8] = { 4, 8 + 0, 0, 0, 0, 0, 0, 0 },
                  t3_pis_23[8] = { 0, 0, 1, 8 + 2, 0, 0, 0, 0 },
                  t3_pis_4[8] = { 0, 0, 0, 0, 3, 0, 0, 0 },

                  t4_pis_01[8] = { 2, 8 + 3, 0, 0, 0, 0, 0, 0 },
                  t4_pis_23[8] = { 0, 0, 4, 8 + 0, 0, 0, 0, 0 },
                  t4_pis_4[8] = { 0, 0, 0, 0, 1, 0, 0, 0 };

  // load permutation indices
  const __m512i t0_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t0_pis_01),
                t0_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t0_pis_23),
                t0_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t0_pis_4),

                t1_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t1_pis_01),
                t1_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t1_pis_23),
                t1_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t1_pis_4),

                t2_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t2_pis_01),
                t2_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t2_pis_23),
                t2_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t2_pis_4),

                t3_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t3_pis_01),
                t3_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t3_pis_23),
                t3_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t3_pis_4),

                t4_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t4_pis_01),
                t4_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t4_pis_23),
                t4_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t4_pis_4);

  // load rows
  const __m512i r0 = _mm512_maskz_loadu_epi64(m, (void*) (s)),
                r1 = _mm512_maskz_loadu_epi64(m, (void*) (s + 5)),
                r2 = _mm512_maskz_loadu_epi64(m, (void*) (s + 10)),
                r3 = _mm512_maskz_loadu_epi64(m, (void*) (s + 15)),
                r4 = _mm512_maskz_loadu_epi64(m, (void*) (s + 20));

  // permute rows
  const __m512i t0e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t0_p01, r1),
                t0e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t0_p23, r3),
                t0e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t0_p4, r0),
                t0 = _mm512_or_epi64(_mm512_or_epi64(t0e0, t0e2), t0e4),

                t1e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t1_p01, r1),
                t1e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t1_p23, r3),
                t1e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t1_p4, r0),
                t1 = _mm512_or_epi64(_mm512_or_epi64(t1e0, t1e2), t1e4),

                t2e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t2_p01, r1),
                t2e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t2_p23, r3),
                t2e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t2_p4, r0),
                t2 = _mm512_or_epi64(_mm512_or_epi64(t2e0, t2e2), t2e4),

                t3e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t3_p01, r1),
                t3e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t3_p23, r3),
                t3e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t3_p4, r0),
                t3 = _mm512_or_epi64(_mm512_or_epi64(t3e0, t3e2), t3e4),

                t4e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t4_p01, r1),
                t4e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t4_p23, r3),
                t4e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t4_p4, r0),
                t4 = _mm512_or_epi64(_mm512_or_epi64(t4e0, t4e2), t4e4);

  // store rows
  _mm512_mask_storeu_epi64((void*) (s), m, t0),
  _mm512_mask_storeu_epi64((void*) (s + 5), m, t1),
  _mm512_mask_storeu_epi64((void*) (s + 10), m, t2),
  _mm512_mask_storeu_epi64((void*) (s + 15), m, t3),
  _mm512_mask_storeu_epi64((void*) (s + 20), m, t4);
}

#if 0
static void pi_avx512(uint64_t s[static 25]) {
  // unaligned gather mask and gather indices
  uint8_t mask = 0x1f;
  const __mmask8 m = _load_mask8(&mask);
  static uint64_t vs0[8] = { 0, 6, 12, 18, 24, 0, 0, 0 },
                  vs1[8] = { 3, 9, 10, 16, 22, 0, 0, 0 },
                  vs2[8] = { 1, 7, 13, 19, 20, 0, 0, 0 },
                  vs3[8] = { 4, 5, 11, 17, 23, 0, 0, 0 },
                  vs4[8] = { 2, 8, 14, 15, 21, 0, 0, 0 };
  // static uint64_t vs0[8] = { 0, 0, 0, 24, 18, 12, 6, 0 },
  //                 vs1[8] = { 0, 0, 0, 22, 16, 10, 9, 3 },
  //                 vs2[8] = { 0, 0, 0, 20, 19, 13, 7, 1 },
  //                 vs3[8] = { 0, 0, 0, 23, 17, 11, 5, 4 },
  //                 vs4[8] = { 0, 0, 0, 21, 15, 14, 8, 2 };

  // load gather indices
  const __m512i v0 = _mm512_loadu_epi64((void*) vs0),
                v1 = _mm512_loadu_epi64((void*) vs1),
                v2 = _mm512_loadu_epi64((void*) vs2),
                v3 = _mm512_loadu_epi64((void*) vs3),
                v4 = _mm512_loadu_epi64((void*) vs4);

  // load rows
  __m512i r0 = _mm512_i64gather_epi64(v0, (void*) (s), 1),
          r1 = _mm512_i64gather_epi64(v1, (void*) (s), 1),
          r2 = _mm512_i64gather_epi64(v2, (void*) (s), 1),
          r3 = _mm512_i64gather_epi64(v3, (void*) (s), 1),
          r4 = _mm512_i64gather_epi64(v4, (void*) (s), 1);

  // store rows
  _mm512_mask_storeu_epi64((void*) (s), m, r0),
  _mm512_mask_storeu_epi64((void*) (s + 5), m, r1),
  _mm512_mask_storeu_epi64((void*) (s + 10), m, r2),
  _mm512_mask_storeu_epi64((void*) (s + 15), m, r3),
  _mm512_mask_storeu_epi64((void*) (s + 20), m, r4);
}
#endif /* 0 */

static void chi_avx512(uint64_t s[static 25]) {
  // mask bytes
  uint8_t mask = 0x1f;
  // load masks
  const __mmask8 m = _load_mask8(&mask);

  // permutation indices
  const uint64_t ids0[8] = { 1, 2, 3, 4, 0, 0, 0, 0 },
                 ids1[8] = { 2, 3, 4, 0, 1, 0, 0, 0 };

  // load permutation indices
  const __m512i p0 = _mm512_maskz_loadu_epi64(m, (void*) ids0),
                p1 = _mm512_maskz_loadu_epi64(m, (void*) ids1);

  // load rows
  const __m512i r0 = _mm512_maskz_loadu_epi64(m, (void*) (s)),
                r1 = _mm512_maskz_loadu_epi64(m, (void*) (s + 5)),
                r2 = _mm512_maskz_loadu_epi64(m, (void*) (s + 10)),
                r3 = _mm512_maskz_loadu_epi64(m, (void*) (s + 15)),
                r4 = _mm512_maskz_loadu_epi64(m, (void*) (s + 20));

  // permute rows
  const __m512i t0_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r0),
                t0_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r0),
                t0 = _mm512_xor_epi64(r0, _mm512_andnot_epi64(t0_e0, t0_e1)),

                t1_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r1),
                t1_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r1),
                t1 = _mm512_xor_epi64(r1, _mm512_andnot_epi64(t1_e0, t1_e1)),

                t2_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r2),
                t2_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r2),
                t2 = _mm512_xor_epi64(r2, _mm512_andnot_epi64(t2_e0, t2_e1)),

                t3_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r3),
                t3_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r3),
                t3 = _mm512_xor_epi64(r3, _mm512_andnot_epi64(t3_e0, t3_e1)),

                t4_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r4),
                t4_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r4),
                t4 = _mm512_xor_epi64(r4, _mm512_andnot_epi64(t4_e0, t4_e1));

  // store rows
  _mm512_mask_storeu_epi64((void*) (s), m, t0),
  _mm512_mask_storeu_epi64((void*) (s + 5), m, t1),
  _mm512_mask_storeu_epi64((void*) (s + 10), m, t2),
  _mm512_mask_storeu_epi64((void*) (s + 15), m, t3),
  _mm512_mask_storeu_epi64((void*) (s + 20), m, t4);
}

static void iota_avx512(uint64_t s[static 25], const int i) {
  // round constants (ambiguous in spec)
  static const uint64_t RCS[] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL,
  };
  // load mask
  uint8_t m0b = 0x01;
  const __mmask8 m0 = _load_mask8(&m0b);

  // load/mask row
  const __m512i r0 = _mm512_maskz_loadu_epi64(m0, (void*) s),
                c0 = _mm512_maskz_loadu_epi64(m0, (void*) (RCS + i)),
                t0 = _mm512_xor_epi64(r0, c0);

  // store row
  _mm512_mask_storeu_epi64((void*) s, m0, t0);
}

// keccak permutation (slow avx512 implementation).
void permute_avx512(uint64_t a[static 25], const size_t num_rounds) {
  for (int i = 0; i < (int) num_rounds; i++) {
    theta_avx512(a);
    rho_avx512(a);
    pi_avx512(a);
    chi_avx512(a);
    iota_avx512(a, 24 - num_rounds + i);
  }
}

// keccak permutation (fast avx512 implementation).
void permute_avx512_fast(uint64_t s[static 25], const size_t num_rounds) {
  // unaligned load mask and permutation indices
  uint8_t mask = 0x1f;
  const __mmask8 m = _load_mask8(&mask);

  // load rc permutation (TODO)
  // static const uint64_t rc_ps[8] = { 1, 2, 3, 4, 5, 6, 7, 0 },
  // const __m512i rc_p = _mm512_loadu_epi64(m, (void*) rc_ps);

  // load rows
  __m512i r0 = _mm512_maskz_loadu_epi64(m, (void*) (s)),
          r1 = _mm512_maskz_loadu_epi64(m, (void*) (s + 5)),
          r2 = _mm512_maskz_loadu_epi64(m, (void*) (s + 10)),
          r3 = _mm512_maskz_loadu_epi64(m, (void*) (s + 15)),
          r4 = _mm512_maskz_loadu_epi64(m, (void*) (s + 20));

  for (int i = 0; i < (int) num_rounds; i++) {
    // theta
    {
      const __m512i i0 = _mm512_setr_epi64(4, 0, 1, 2, 3, 5, 6, 7),
                    i1 = _mm512_setr_epi64(1, 2, 3, 4, 0, 5, 6, 7);

      // c = xor(r0, r1, r2, r3, r4)
      const __m512i r01 = _mm512_xor_epi64(r0, r1),
                    r23 = _mm512_xor_epi64(r2, r3),
                    r03 = _mm512_xor_epi64(r01, r23),
                    c = _mm512_xor_epi64(r03, r4);

      // d = xor(permute(i0, c), permute(i1, rol(c, 1)))
      const __m512i d0 = _mm512_permutexvar_epi64(i0, c),
                    d1 = _mm512_permutexvar_epi64(i1, _mm512_rol_epi64(c, 1)),
                    d = _mm512_xor_epi64(d0, d1);

      // row = xor(row, d)
      r0 = _mm512_xor_epi64(r0, d);
      r1 = _mm512_xor_epi64(r1, d);
      r2 = _mm512_xor_epi64(r2, d);
      r3 = _mm512_xor_epi64(r3, d);
      r4 = _mm512_xor_epi64(r4, d);
    }

    // rho
    {
      // unaligned load mask and rotate values
      static const uint64_t vs0[8] = { 0, 1, 62, 28, 27, 0, 0, 0 },
                            vs1[8] = { 36, 44, 6, 55, 20, 0, 0, 0 },
                            vs2[8] = { 3, 10, 43, 25, 39, 0, 0, 0 },
                            vs3[8] = { 41, 45, 15, 21, 8, 0, 0, 0 },
                            vs4[8] = { 18, 2, 61, 56, 14, 0, 0, 0 };

      // load rotate values
      const __m512i v0 = _mm512_loadu_epi64((void*) vs0),
                    v1 = _mm512_loadu_epi64((void*) vs1),
                    v2 = _mm512_loadu_epi64((void*) vs2),
                    v3 = _mm512_loadu_epi64((void*) vs3),
                    v4 = _mm512_loadu_epi64((void*) vs4);

      // rotate
      r0 = _mm512_rolv_epi64(r0, v0);
      r1 = _mm512_rolv_epi64(r1, v1);
      r2 = _mm512_rolv_epi64(r2, v2);
      r3 = _mm512_rolv_epi64(r3, v3);
      r4 = _mm512_rolv_epi64(r4, v4);
    }

    // pi
    {
      // mask bytes
      uint8_t m01b = 0x03,
              m23b = 0x0c,
              m4b = 0x10;

      // load masks
      const __mmask8 m01 = _load_mask8(&m01b),
                     m23 = _load_mask8(&m23b),
                     m4 = _load_mask8(&m4b);
      // permutation indices (offsets into state array)
      // static uint64_t vs0[8] = { 0, 6, 12, 18, 24, 0, 0, 0 },
      //                 vs1[8] = { 3, 9, 10, 16, 22, 0, 0, 0 },
      //                 vs2[8] = { 1, 7, 13, 19, 20, 0, 0, 0 },
      //                 vs3[8] = { 4, 5, 11, 17, 23, 0, 0, 0 },
      //                 vs4[8] = { 2, 8, 14, 15, 21, 0, 0, 0 };

      // permutation indices
      //
      // (note: these are masked so only the relevant indices for
      // _mm512_maskz_permutex2var_epi64() in each array are filled in)
      static const uint64_t t0_pis_01[8] = { 0, 8 + 1, 0, 0, 0, 0, 0, 0 },
                            t0_pis_23[8] = { 0, 0, 2, 8 + 3, 0, 0, 0, 0 },
                            t0_pis_4[8] = { 0, 0, 0, 0, 4, 0, 0, 0 },

                            t1_pis_01[8] = { 3, 8 + 4, 0, 0, 0, 0, 0, 0 },
                            t1_pis_23[8] = { 0, 0, 0, 8 + 1, 0, 0, 0, 0 },
                            t1_pis_4[8] = { 0, 0, 0, 0, 2, 0, 0, 0 },

                            t2_pis_01[8] = { 1, 8 + 2, 0, 0, 0, 0, 0, 0 },
                            t2_pis_23[8] = { 0, 0, 3, 8 + 4, 0, 0, 0, 0 },
                            t2_pis_4[8] = { 0, 0, 0, 0, 0, 0, 0, 0 },

                            t3_pis_01[8] = { 4, 8 + 0, 0, 0, 0, 0, 0, 0 },
                            t3_pis_23[8] = { 0, 0, 1, 8 + 2, 0, 0, 0, 0 },
                            t3_pis_4[8] = { 0, 0, 0, 0, 3, 0, 0, 0 },

                            t4_pis_01[8] = { 2, 8 + 3, 0, 0, 0, 0, 0, 0 },
                            t4_pis_23[8] = { 0, 0, 4, 8 + 0, 0, 0, 0, 0 },
                            t4_pis_4[8] = { 0, 0, 0, 0, 1, 0, 0, 0 };

      // load permutation indices
      const __m512i t0_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t0_pis_01),
                    t0_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t0_pis_23),
                    t0_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t0_pis_4),

                    t1_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t1_pis_01),
                    t1_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t1_pis_23),
                    t1_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t1_pis_4),

                    t2_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t2_pis_01),
                    t2_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t2_pis_23),
                    t2_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t2_pis_4),

                    t3_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t3_pis_01),
                    t3_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t3_pis_23),
                    t3_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t3_pis_4),

                    t4_p01 = _mm512_maskz_loadu_epi64(m01, (void*) t4_pis_01),
                    t4_p23 = _mm512_maskz_loadu_epi64(m23, (void*) t4_pis_23),
                    t4_p4  = _mm512_maskz_loadu_epi64(m4, (void*) t4_pis_4);

      // permute rows
      const __m512i t0e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t0_p01, r1),
                    t0e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t0_p23, r3),
                    t0e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t0_p4, r0),
                    t0 = _mm512_or_epi64(_mm512_or_epi64(t0e0, t0e2), t0e4),

                    t1e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t1_p01, r1),
                    t1e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t1_p23, r3),
                    t1e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t1_p4, r0),
                    t1 = _mm512_or_epi64(_mm512_or_epi64(t1e0, t1e2), t1e4),

                    t2e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t2_p01, r1),
                    t2e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t2_p23, r3),
                    t2e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t2_p4, r0),
                    t2 = _mm512_or_epi64(_mm512_or_epi64(t2e0, t2e2), t2e4),

                    t3e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t3_p01, r1),
                    t3e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t3_p23, r3),
                    t3e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t3_p4, r0),
                    t3 = _mm512_or_epi64(_mm512_or_epi64(t3e0, t3e2), t3e4),

                    t4e0 = _mm512_maskz_permutex2var_epi64(m01, r0, t4_p01, r1),
                    t4e2 = _mm512_maskz_permutex2var_epi64(m23, r2, t4_p23, r3),
                    t4e4 = _mm512_maskz_permutex2var_epi64(m4, r4, t4_p4, r0),
                    t4 = _mm512_or_epi64(_mm512_or_epi64(t4e0, t4e2), t4e4);

      // store rows
      r0 = t0;
      r1 = t1;
      r2 = t2;
      r3 = t3;
      r4 = t4;
    }

    // chi
    {
      // permutation indices
      static const uint64_t ids0[8] = { 1, 2, 3, 4, 0, 0, 0, 0 },
                            ids1[8] = { 2, 3, 4, 0, 1, 0, 0, 0 };

      // load permutation indices
      const __m512i p0 = _mm512_maskz_loadu_epi64(m, (void*) ids0),
                    p1 = _mm512_maskz_loadu_epi64(m, (void*) ids1);

      // permute rows
      const __m512i t0_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r0),
                    t0_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r0),
                    t0 = _mm512_xor_epi64(r0, _mm512_andnot_epi64(t0_e0, t0_e1)),

                    t1_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r1),
                    t1_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r1),
                    t1 = _mm512_xor_epi64(r1, _mm512_andnot_epi64(t1_e0, t1_e1)),

                    t2_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r2),
                    t2_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r2),
                    t2 = _mm512_xor_epi64(r2, _mm512_andnot_epi64(t2_e0, t2_e1)),

                    t3_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r3),
                    t3_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r3),
                    t3 = _mm512_xor_epi64(r3, _mm512_andnot_epi64(t3_e0, t3_e1)),

                    t4_e0 = _mm512_maskz_permutexvar_epi64(m, p0, r4),
                    t4_e1 = _mm512_maskz_permutexvar_epi64(m, p1, r4),
                    t4 = _mm512_xor_epi64(r4, _mm512_andnot_epi64(t4_e0, t4_e1));

      // store rows
      r0 = t0;
      r1 = t1;
      r2 = t2;
      r3 = t3;
      r4 = t4;
    }

    // iota
    {
      // round constants (ambiguous in spec)
      static const uint64_t RCS[] = {
        0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL, 0x8000000080008000ULL,
        0x000000000000808bULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
        0x000000000000008aULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
        0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
        0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800aULL, 0x800000008000000aULL,
        0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL,
      };

      // TODO
      // if ((24 - num_rounds + i + 1) % 8) {
      //   rc = _mm512_permutexvar_epi64(p0, r4),
      // } else {
      // }

      // get rc address
      const uint64_t *rc = RCS + (24 - num_rounds + i);

      // load mask
      uint8_t m0b = 0x01;
      const __mmask8 m0 = _load_mask8(&m0b);

      // mask/store row
      const __m512i c0 = _mm512_maskz_loadu_epi64(m0, (void*) rc);
      r0 = _mm512_xor_epi64(r0, c0);
    }
  }

  // store rows
  _mm512_mask_storeu_epi64((void*) (s), m, r0),
  _mm512_mask_storeu_epi64((void*) (s + 5), m, r1),
  _mm512_mask_storeu_epi64((void*) (s + 10), m, r2),
  _mm512_mask_storeu_epi64((void*) (s + 15), m, r3),
  _mm512_mask_storeu_epi64((void*) (s + 20), m, r4);
}
#endif /* __AVX512F__ */

static void check(const char *name, uint64_t a_scalar[static 25], uint64_t a_avx512[static 25]) {
  // compare
  if (!memcmp(a_scalar, a_avx512, 25 * sizeof(uint64_t))) {
    printf("%s passed\n", name);
    return;
  }

  // print error
  fprintf(stderr, "%s failed: a_scalar != a_avx512:\n", name);

  // print scalar state
  fprintf(stderr, "a_scalar = {\n");
  for (size_t i = 0; i < 25; i++) {
    const char *mark = (a_scalar[i] == a_avx512[i]) ? "" : "*";
    fprintf(stderr, "%s%016lx%s, ", mark, a_scalar[i], mark);
    if (((i + 1) % 5) == 0) {
      fprintf(stderr, "\n");
    }
  }
  fprintf(stderr, "}\n");

  // print avx512 state
  fprintf(stderr, "a_avx512 = {\n");
  for (size_t i = 0; i < 25; i++) {
    const char *mark = (a_scalar[i] == a_avx512[i]) ? "" : "*";
    fprintf(stderr, "%s%016lx%s, ", mark, a_avx512[i], mark);
    if (((i + 1) % 5) == 0) {
      fprintf(stderr, "\n");
    }
  }
  fprintf(stderr, "}\n");

  exit(-1);
}

static void test_theta(void) {
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  theta_scalar(a_scalar);
  theta_avx512(a_avx512);

  // compare
  check("test_theta()", a_scalar, a_avx512);
}

static void test_rho(void) {
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  rho_scalar(a_scalar);
  rho_avx512(a_avx512);

  // compare
  check("test_rho()", a_scalar, a_avx512);
}

static void test_pi(void) {
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  pi_scalar(a_scalar);
  pi_avx512(a_avx512);

  // compare
  check("test_pi()", a_scalar, a_avx512);
}

static void test_chi(void) {
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  chi_scalar(a_scalar);
  chi_avx512(a_avx512);

  // compare
  check("test_chi()", a_scalar, a_avx512);
}

static void test_iota(void) {
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  for (size_t i = 0; i < 24; i++) {
    // build name
    char buf[128] = { 0 };
    snprintf(buf, sizeof(buf), "test_iota(%zu)", i);

    // permute
    iota_scalar(a_scalar, i);
    iota_avx512(a_avx512, i);

    // compare
    check(buf, a_scalar, a_avx512);
  }
}

static void test_permute_slow(void) {
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  // permute
  permute_scalar(a_scalar, 24);
  permute_avx512(a_avx512, 24);

  // compare
  check("test_permute_slow()", a_scalar, a_avx512);
}

static void test_permute_fast(void) {
#ifdef __AVX512F__
  uint64_t a_scalar[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  uint64_t a_avx512[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  // permute
  permute_scalar(a_scalar, 24);
  permute_avx512_fast(a_avx512, 24);

  // compare
  check("test_permute_fast()", a_scalar, a_avx512);
#endif /* __AVX512F__ */
}

#define NUM_TIME_PERMUTES 10000000

static void time_permute_scalar(void) {
  uint64_t a[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  const time_t t0 = time(NULL);

  // permute
  for (size_t i = 0; i < NUM_TIME_PERMUTES; i++) {
    permute_scalar(a, 24);
  }

  const time_t t1 = time(NULL);

  printf("time_permute_scalar(): %zu\n", t1 - t0);
}

static void time_permute_avx512_slow(void) {
#ifdef __AVX512F__
  uint64_t a[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  const time_t t0 = time(NULL);

  // permute
  for (size_t i = 0; i < NUM_TIME_PERMUTES; i++) {
    permute_avx512(a, 24);
  }

  const time_t t1 = time(NULL);

  printf("time_permute_avx512_slow(): %zu\n", t1 - t0);
#else
  printf("time_permute_avx512_slow(): n/a\n");
#endif /* __AVX512F__ */
}

static void time_permute_avx512_fast(void) {
#ifdef __AVX512F__
  uint64_t a[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

  const time_t t0 = time(NULL);

  // permute
  for (size_t i = 0; i < NUM_TIME_PERMUTES; i++) {
    permute_avx512_fast(a, 24);
  }

  const time_t t1 = time(NULL);

  printf("time_permute_avx512_fast(): %zu\n", t1 - t0);
#else
  printf("time_permute_avx512_fast(): n/a\n");
#endif /* __AVX512F__ */
}

int main() {
  test_theta();
  test_rho();
  test_pi();
  test_chi();
  test_iota();
  test_permute_slow();
  test_permute_fast();
  time_permute_scalar();
  time_permute_avx512_slow();
  time_permute_avx512_fast();
  return 0;
}
