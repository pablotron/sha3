// test neon implementation of keccak permutation
#include <stdio.h> // uint64_t
#include <stdint.h> // uint64_t
#include <string.h> // memcmp(), memcpy()
#include <inttypes.h> // PRI{u,x}64
#include <stdlib.h> // exit()
#include <arm_neon.h>

// 64-bit rotate left
#define ROL(v, n) (((v) << (n)) | ((v) >> (64-(n))))

// iota step round constants
static const uint64_t RCS[] = {
  0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL, 0x8000000080008000ULL,
  0x000000000000808bULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
  0x000000000000008aULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
  0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
  0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800aULL, 0x800000008000000aULL,
  0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL,
};

// theta step of scalar keccak permutation
// (working implementation)
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

// rho step of scalar keccak permutation
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

// pi step of scalar keccak permutation
static inline void pi_scalar(uint64_t src[static 25]) {
  uint64_t dst[25];

  dst[ 0] = src[ 0];
  dst[ 1] = src[ 6];
  dst[ 2] = src[12];
  dst[ 3] = src[18];
  dst[ 4] = src[24];

  dst[ 5] = src[ 3];
  dst[ 6] = src[ 9];
  dst[ 7] = src[10];
  dst[ 8] = src[16];
  dst[ 9] = src[22];

  dst[10] = src[ 1];
  dst[11] = src[ 7];
  dst[12] = src[13];
  dst[13] = src[19];
  dst[14] = src[20];

  dst[15] = src[ 4];
  dst[16] = src[ 5];
  dst[17] = src[11];
  dst[18] = src[17];
  dst[19] = src[23];

  dst[20] = src[ 2];
  dst[21] = src[ 8];
  dst[22] = src[14];
  dst[23] = src[15];
  dst[24] = src[21];

  memcpy(src, dst, sizeof(dst));
}

// chi step of scalar keccak permutation
static inline void chi_scalar(uint64_t src[static 25]) {
  uint64_t dst[25];

  dst[ 0] = src[ 0] ^ (~src[ 1] & src[ 2]);
  dst[ 1] = src[ 1] ^ (~src[ 2] & src[ 3]);
  dst[ 2] = src[ 2] ^ (~src[ 3] & src[ 4]);
  dst[ 3] = src[ 3] ^ (~src[ 4] & src[ 0]);
  dst[ 4] = src[ 4] ^ (~src[ 0] & src[ 1]);

  dst[ 5] = src[ 5] ^ (~src[ 6] & src[ 7]);
  dst[ 6] = src[ 6] ^ (~src[ 7] & src[ 8]);
  dst[ 7] = src[ 7] ^ (~src[ 8] & src[ 9]);
  dst[ 8] = src[ 8] ^ (~src[ 9] & src[ 5]);
  dst[ 9] = src[ 9] ^ (~src[ 5] & src[ 6]);

  dst[10] = src[10] ^ (~src[11] & src[12]);
  dst[11] = src[11] ^ (~src[12] & src[13]);
  dst[12] = src[12] ^ (~src[13] & src[14]);
  dst[13] = src[13] ^ (~src[14] & src[10]);
  dst[14] = src[14] ^ (~src[10] & src[11]);

  dst[15] = src[15] ^ (~src[16] & src[17]);
  dst[16] = src[16] ^ (~src[17] & src[18]);
  dst[17] = src[17] ^ (~src[18] & src[19]);
  dst[18] = src[18] ^ (~src[19] & src[15]);
  dst[19] = src[19] ^ (~src[15] & src[16]);

  dst[20] = src[20] ^ (~src[21] & src[22]);
  dst[21] = src[21] ^ (~src[22] & src[23]);
  dst[22] = src[22] ^ (~src[23] & src[24]);
  dst[23] = src[23] ^ (~src[24] & src[20]);
  dst[24] = src[24] ^ (~src[20] & src[21]);

  memcpy(src, dst, sizeof(dst));
}

// iota step of scalar keccak permutation
static inline void iota_scalar(uint64_t a[static 25], const size_t i) {
  a[0] ^= RCS[i];
}

// 24-round scalar keccak permutation
void permute_scalar(uint64_t a[static 25]) {
  // uint64_t tmp[25] = { 0 };
  for (size_t i = 0; i < 24; i++) {
    theta_scalar(a);
    rho_scalar(a);
    pi_scalar(a); // pi_scalar(tmp, a);
    chi_scalar(a); // chi_scalar(a, tmp);
    iota_scalar(a, i);
  }
}

// vrax1q_u64() not supported on pizza
#define VROLQ(A, N) (vorrq_u64(vshlq_n_u64((A), (N)), vshrq_n_u64((A), 64-(N))))

// keccak row, represented as 3 128-bit vector registers
//
// columns are stored in the low 5 64-bit lanes.  this wastes one
// 64-bit lane per row at the expense of making many of the instructions
// simpler.
typedef union {
  // uint64_t u64[6];
  uint64x2x3_t u64x2x3;
  // uint8_t u8[48];
  // uint8x16_t u8x16[3];
  uint8x16x3_t u8x16x3;
} row_t;

// FIXME:
// add row_load_fast which reads 6 elems and does this
// r2 = { .u64x2x3 = vld1q_u64_x3(a + 10) },

// load row from array
row_t row_load(const uint64_t p[static 5]) {
  row_t a = { 0 };

  a.u64x2x3.val[0] = vld1q_u64(p + 0);
  a.u64x2x3.val[1] = vld1q_u64(p + 2);
  a.u64x2x3.val[2] = vdupq_n_u64(p[4]);

  return a;
}

// store row to array
void row_store(uint64_t p[static 5], const row_t a) {
  // row_print(stderr, __func__, a);
  vst1q_u64(p + 0, a.u64x2x3.val[0]);
  vst1q_u64(p + 2, a.u64x2x3.val[1]);
  vst1_u64(p + 4, vdup_laneq_u64(a.u64x2x3.val[2], 0));
  // p[4] = vgetq_lane_u64(a.u64x2x3.val[2], 0);
}

// print row
void row_print(FILE *fh, const char *name, const row_t row) {
  uint64_t vals[6];
  row_store(vals, row);
  fprintf(fh, "%s = {", name); // prefix

  // loop over and print columns (ignore 6th column)
  for (size_t i = 0; i < 5; i++) {
    fprintf(fh, "%s %016" PRIx64, (i ? "," : ""), vals[i]);
  }

  fputs(" }\n", fh); // suffix
}

// low lane ids for rol_rc{l,r}()
static const uint8x16_t ROW_RL_LO_IDS = {
   8,  9, 10, 11, 12, 13, 14, 15,  99, 99, 99, 99, 99, 99, 99, 99,
};

// high lane ids for rol_rc{l,r}()
static const uint8x16_t ROW_RL_HI_IDS = {
  99, 99, 99, 99, 99, 99, 99, 99,   0,  1,  2,  3,  4,  5,  6,  7,
};

// low lanes for last iteration of row_rlll() and first iteration of row_rlr()
static const uint8x16_t ROW_RL_TAIL_IDS = {
   0,  1,  2,  3,  4,  5,  6,  7,  99, 99, 99, 99, 99, 99, 99, 99,
};

// rotate row lanes left
//
// ---------------------------     ---------------------------
// |  64-bit Lanes (Before)  |     |  64-bit Lanes (After)   |
// |-------------------------|     |-------------------------|
// | 0 | 1 | 2 | 3 | 4 |  5  | --> | 0 | 1 | 2 | 3 | 4 |  5  |
// |---|---|---|---|---|-----|     |---|---|---|---|---|-----|
// | A | B | C | D | E | n/a |     | E | A | B | C | D | n/a |
// ---------------------------     ---------------------------
//
static inline row_t row_rll(const row_t a) {
  row_t b = { 0 };
  for (size_t i = 0; i < 3; i++) {
    const uint8x16_t lo_ids = i ? ROW_RL_LO_IDS : ROW_RL_TAIL_IDS,
                     hi = vqtbl1q_u8(a.u8x16x3.val[i], ROW_RL_HI_IDS),
                     lo = vqtbl1q_u8(a.u8x16x3.val[(i + 2) % 3], lo_ids);
    b.u8x16x3.val[i] = vorrq_u8(lo, hi);
  }
  return b;
}

// rotate row lanes right
//
// ---------------------------     ---------------------------
// |  64-bit Lanes (Before)  |     |  64-bit Lanes (After)   |
// |-------------------------|     |-------------------------|
// | 0 | 1 | 2 | 3 | 4 |  5  | --> | 0 | 1 | 2 | 3 | 4 |  5  |
// |---|---|---|---|---|-----|     |---|---|---|---|---|-----|
// | A | B | C | D | E | n/a |     | B | C | D | E | A | n/a |
// ---------------------------     ---------------------------
//
static row_t row_rlr(const row_t a) {
  row_t b = { 0 };
  for (size_t i = 0; i < 2; i++) {
    const uint8x16_t lo = vqtbl1q_u8(a.u8x16x3.val[i], ROW_RL_LO_IDS),
                     hi = vqtbl1q_u8(a.u8x16x3.val[(i + 1) % 3], ROW_RL_HI_IDS);
    b.u8x16x3.val[i] = vorrq_u8(lo, hi);
  }
  b.u8x16x3.val[2] = vqtbl1q_u8(a.u8x16x3.val[0], ROW_RL_TAIL_IDS);
  return b;
}

// c = a ^ b
row_t row_eor(const row_t a, const row_t b) {
  row_t c = a;
  for (size_t i = 0; i < 3; i++) {
    c.u8x16x3.val[i] ^= b.u8x16x3.val[i];
  }
  return c;
}

// rotate bits in each lane left one bit
row_t row_rol1_u64(const row_t a) {
  row_t b = { 0 };
  for (size_t i = 0; i < 3; i++) {
    b.u64x2x3.val[i] = VROLQ(a.u64x2x3.val[i], 1);
  }
  return b;
}

// rotate bits in each lane left by amounts in vector
static inline row_t row_rotn_u64(const row_t a, const int64_t v[static 5]) {
  row_t b = { 0 };
  static const int64x2_t k64 = { 64, 64 };
  for (size_t i = 0; i < 3; i++) {
    const int64x2_t hi_ids = (i < 2) ? vld1q_s64(v + 2 * i) : vdupq_n_s64(v[4]),
                    lo_ids = vsubq_s64(hi_ids, k64);
    b.u64x2x3.val[i] = vorrq_u64(vshlq_u64(a.u64x2x3.val[i], hi_ids), vshlq_u64(a.u64x2x3.val[i], lo_ids));
  }
  return b;
}

// row compare (not constant-time)
_Bool row_eq(const row_t a, const row_t b) {
  uint64_t a_u64[5], b_u64[5];
  row_store(a_u64, a);
  row_store(b_u64, b);
  return !memcmp(a_u64, b_u64, sizeof(a_u64));
}

// return logical NOT of row
static row_t row_not(const row_t a) {
  row_t b;
  for (size_t i = 0; i < 3; i++) {
    b.u8x16x3.val[i] = vmvnq_u8(a.u8x16x3.val[i]);
  }
  return b;
}

// return logical OR NOT of rows
static row_t row_orn(const row_t a, const row_t b) {
  row_t c;
  for (size_t i = 0; i < 3; i++) {
    c.u8x16x3.val[i] = vornq_u8(a.u8x16x3.val[i], b.u8x16x3.val[i]);
  }
  return c;
}

// apply chi permutation to entire row
// note: ~(a | ~b) = (~a & b) (demorgan's laws)
static inline row_t row_chi(const row_t a) {
  const row_t b = row_rlr(a),
              c = row_rlr(b); // fixme, permute would be faster
  return row_eor(a, row_not(row_orn(b, c)));
}


// theta step of neon keccak permutation
void theta_neon(uint64_t a[static 25]) {
  // ---------------------------------------------------------
  // |     |                Column / Register                |
  // |-------------------------------------------------------|
  // | Row |    3    |    4    |    0    |    1    |    2    |
  // |-----|---------|---------|---------|---------|---------|
  // |  2  | r2_23.1 |  r2_4   | r2_01.0 | r2_01.1 | r2_23.0 |
  // |  1  | r1_23.1 |  r1_4   | r1_01.0 | r1_01.1 | r1_23.0 |
  // |  0  | r0_23.1 |  r0_4   | r0_01.0 | r0_01.1 | r0_23.0 |
  // |  4  | r4_23.1 |  r4_4   | r4_01.0 | r4_01.1 | r4_23.0 |
  // |  3  | r3_23.1 |  r3_4   | r3_01.0 | r3_01.1 | r3_23.0 |
  // ---------------------------------------------------------

  // load rows
  row_t r0 = row_load(a +  0),
        r1 = row_load(a +  5),
        r2 = row_load(a + 10),
        r3 = row_load(a + 15),
        r4 = row_load(a + 20);

  // c = r0 ^ r1 ^ r2 ^ r3 ^ r4
  const row_t c = row_eor(row_eor(row_eor(r0, r1), row_eor(r2, r3)), r4);

  // calculate d...
  const row_t d = row_eor(row_rll(c), row_rol1_u64(row_rlr(c)));

  // store rows
  row_store(a +  0, row_eor(r0, d));
  row_store(a +  5, row_eor(r1, d));
  row_store(a + 10, row_eor(r2, d));
  row_store(a + 15, row_eor(r3, d));
  row_store(a + 20, row_eor(r4, d));
}

static const int64_t RHO_IDS[25] = {
   0,  1, 62, 28, 27,
  36, 44,  6, 55, 20,
   3, 10, 43, 25, 39,
  41, 45, 15, 21,  8,
  18,  2, 61, 56, 14,
};

// rho step of neon keccak permutation
void rho_neon(uint64_t a[static 25]) {
  // load rows
  row_t r0 = row_load(a +  0),
        r1 = row_load(a +  5),
        r2 = row_load(a + 10),
        r3 = row_load(a + 15),
        r4 = row_load(a + 20);

  // rotate rows
  r0 = row_rotn_u64(r0, RHO_IDS +  0);
  r1 = row_rotn_u64(r1, RHO_IDS +  5);
  r2 = row_rotn_u64(r2, RHO_IDS + 10);
  r3 = row_rotn_u64(r3, RHO_IDS + 15);
  r4 = row_rotn_u64(r4, RHO_IDS + 20);

  // store rows
  row_store(a +  0, r0);
  row_store(a +  5, r1);
  row_store(a + 10, r2);
  row_store(a + 15, r3);
  row_store(a + 20, r4);
}

// permute IDS to take low lane of first pair and low lane of second pair
// TODO: replace with transpose or zip1q_u64?
// a = [ a0, a1 ], b = [ b0, b1 ] => c = [ a0, b0 ]
// static const uint8x16_t PI_LO_LO_IDS = {
//    0,  1,  2,  3,  4,  5,  6,  7,  16, 17, 18, 19, 20, 21, 22, 23,
// };

// permute IDS to take low lane of first pair and hi lane of second pair
// a = [ a0, a1 ], b = [ b0, b1 ] => c = [ a0, b1 ]
static const uint8x16_t PI_LO_HI_IDS = {
   0,  1,  2,  3,  4,  5,  6,  7,  24, 25, 26, 27, 28, 29, 30, 31,
};

// permute IDS to take high lane of first pair and low lane of second pair
// a = [ a0, a1 ], b = [ b0, b1 ] => c = [ a1, b0 ]
static const uint8x16_t PI_HI_LO_IDS = {
   8,  9, 10, 11, 12, 13, 14, 15,  16, 17, 18, 19, 20, 21, 22, 23,
};

// permute IDS to take high lane of both pairs
// a = [ a0, a1 ], b = [ b0, b1 ] => c = [ a1, b1 ]
// static const uint8x16_t PI_HI_HI_IDS = {
//   8,  9, 10, 11, 12, 13, 14, 15,  24, 25, 26, 27, 28, 29, 30, 31,
// };

static uint8x16_t pi_tbl(const uint8x16_t a, const uint8x16_t b, const uint8x16_t ids) {
  uint8x16x2_t quad = { .val = { a, b } };
  return vqtbl2q_u8(quad, ids);
}

// pi step of neon keccak permutation
void pi_neon(uint64_t a[static 25]) {
  // load source rows
  row_t r0 = row_load(a +  0),
        r1 = row_load(a +  5),
        r2 = row_load(a + 10),
        r3 = row_load(a + 15),
        r4 = row_load(a + 20);

  {
    row_t t0 = { 0 };

    // dst[ 0] = src[ 0]; dst[ 1] = src[ 6];
    t0.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[0], r1.u8x16x3.val[0], PI_LO_HI_IDS);
    // dst[ 2] = src[12]; dst[ 3] = src[18];
    t0.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[1], r3.u8x16x3.val[1], PI_LO_HI_IDS);
    // dst[ 4] = src[24];
    t0.u8x16x3.val[2] = r4.u8x16x3.val[2];

    row_store(a +  0, t0);
  }

  {
    row_t t1 = { 0 };

    // dst[ 5] = src[ 3]; dst[ 6] = src[ 9];
    t1.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[1], r1.u8x16x3.val[2], PI_HI_LO_IDS);
    // dst[ 7] = src[10]; dst[ 8] = src[16];
    t1.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[0], r3.u8x16x3.val[0], PI_LO_HI_IDS);
    // dst[ 9] = src[22];
    t1.u8x16x3.val[2] = r4.u8x16x3.val[1];

    row_store(a +  5, t1);
  }

  {
    row_t t2 = { 0 };

    // dst[10] = src[ 1]; dst[11] = src[ 7];
    t2.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[0], r1.u8x16x3.val[1], PI_HI_LO_IDS);
    // dst[12] = src[13]; dst[13] = src[19];
    t2.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[1], r3.u8x16x3.val[2], PI_HI_LO_IDS);
    // dst[14] = src[20];
    t2.u8x16x3.val[2] = r4.u8x16x3.val[0];

    row_store(a + 10, t2);
  }

  {
    row_t t3 = { 0 };

    // dst[15] = src[ 4]; dst[16] = src[ 5];
    // t3.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[2], r1.u8x16x3.val[0], PI_LO_LO_IDS);
    t3.u64x2x3.val[0] = vtrn1q_u64(r0.u64x2x3.val[2], r1.u64x2x3.val[0]);
    // dst[17] = src[11]; dst[18] = src[17];
    t3.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[0], r3.u8x16x3.val[1], PI_HI_LO_IDS);
    // dst[19] = src[23];
    t3.u8x16x3.val[2] = pi_tbl(r4.u8x16x3.val[1], r4.u8x16x3.val[1], PI_HI_LO_IDS);

    row_store(a + 15, t3);
  }

  {
    row_t t4 = { 0 };

    // dst[20] = src[ 2]; dst[21] = src[ 8];
    t4.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[1], r1.u8x16x3.val[1], PI_LO_HI_IDS);
    // dst[22] = src[14]; dst[23] = src[15];
    // t4.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[2], r3.u8x16x3.val[0], PI_LO_LO_IDS);
    t4.u64x2x3.val[1] =  vtrn1q_u64(r2.u64x2x3.val[2], r3.u64x2x3.val[0]);

    // dst[24] = src[21];
    t4.u8x16x3.val[2] = pi_tbl(r4.u8x16x3.val[0], r4.u8x16x3.val[0], PI_HI_LO_IDS);

    row_store(a + 20, t4);
  }
}

// chi step of neon keccak permutation
void chi_neon(uint64_t a[static 25]) {
  for (size_t i = 0; i < 25; i += 5) {
    row_store(a + i, row_chi(row_load(a + i)));
  }
}

// iota step of neon keccak permutation
void iota_neon(uint64_t a[static 25], const size_t i) {
  row_t r0 = row_load(a);
  const uint64x2_t rc = { RCS[i], 0 };
  r0.u64x2x3.val[0] ^= rc;
  row_store(a, r0);
}

// 24-round neon keccak permutation with function calls
void permute_neon_slow(uint64_t a[static 25]) {
  // uint64_t tmp[25] = { 0 };
  for (size_t i = 0; i < 24; i++) {
    theta_neon(a);
    rho_neon(a);
    pi_neon(a); // pi_neon(tmp, a);
    chi_neon(a); // chi_neon(a, tmp);
    iota_neon(a, i);
  }
}

// 24-round neon keccak permutation with inlined steps
void permute_neon_inline(uint64_t a[static 25]) {
  // ---------------------------------------------------------
  // |     |                Column / Register                |
  // |-------------------------------------------------------|
  // | Row |    3    |    4    |    0    |    1    |    2    |
  // |-----|---------|---------|---------|---------|---------|
  // |  2  | r2_23.1 |  r2_4   | r2_01.0 | r2_01.1 | r2_23.0 |
  // |  1  | r1_23.1 |  r1_4   | r1_01.0 | r1_01.1 | r1_23.0 |
  // |  0  | r0_23.1 |  r0_4   | r0_01.0 | r0_01.1 | r0_23.0 |
  // |  4  | r4_23.1 |  r4_4   | r4_01.0 | r4_01.1 | r4_23.0 |
  // |  3  | r3_23.1 |  r3_4   | r3_01.0 | r3_01.1 | r3_23.0 |
  // ---------------------------------------------------------

  // load rows
  row_t r0 = row_load(a +  0),
        r1 = row_load(a +  5),
        r2 = row_load(a + 10),
        r3 = row_load(a + 15),
        r4 = row_load(a + 20);

  for (size_t i = 0; i < 24; i++) {
    // theta
    {
      // c = r0 ^ r1 ^ r2 ^ r3 ^ r4
      const row_t c = row_eor(row_eor(row_eor(r0, r1), row_eor(r2, r3)), r4);

      // calculate d...
      const row_t d = row_eor(row_rll(c), row_rol1_u64(row_rlr(c)));

      r0 = row_eor(r0, d);
      r1 = row_eor(r1, d);
      r2 = row_eor(r2, d);
      r3 = row_eor(r3, d);
      r4 = row_eor(r4, d);
    }

    // rho
    {
      r0 = row_rotn_u64(r0, RHO_IDS +  0);
      r1 = row_rotn_u64(r1, RHO_IDS +  5);
      r2 = row_rotn_u64(r2, RHO_IDS + 10);
      r3 = row_rotn_u64(r3, RHO_IDS + 15);
      r4 = row_rotn_u64(r4, RHO_IDS + 20);
    }

    // pi
    {
      row_t t0 = { 0 };
      {
        // dst[ 0] = src[ 0]; dst[ 1] = src[ 6];
        t0.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[0], r1.u8x16x3.val[0], PI_LO_HI_IDS);
        // dst[ 2] = src[12]; dst[ 3] = src[18];
        t0.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[1], r3.u8x16x3.val[1], PI_LO_HI_IDS);
        // dst[ 4] = src[24];
        t0.u8x16x3.val[2] = r4.u8x16x3.val[2];
      }

      row_t t1 = { 0 };
      {

        // dst[ 5] = src[ 3]; dst[ 6] = src[ 9];
        t1.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[1], r1.u8x16x3.val[2], PI_HI_LO_IDS);
        // dst[ 7] = src[10]; dst[ 8] = src[16];
        t1.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[0], r3.u8x16x3.val[0], PI_LO_HI_IDS);
        // dst[ 9] = src[22];
        t1.u8x16x3.val[2] = r4.u8x16x3.val[1];
      }

      row_t t2 = { 0 };
      {
        // dst[10] = src[ 1]; dst[11] = src[ 7];
        t2.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[0], r1.u8x16x3.val[1], PI_HI_LO_IDS);
        // dst[12] = src[13]; dst[13] = src[19];
        t2.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[1], r3.u8x16x3.val[2], PI_HI_LO_IDS);
        // dst[14] = src[20];
        t2.u8x16x3.val[2] = r4.u8x16x3.val[0];
      }

      row_t t3 = { 0 };
      {
        // dst[15] = src[ 4]; dst[16] = src[ 5];
        // t3.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[2], r1.u8x16x3.val[0], PI_LO_LO_IDS);
        t3.u64x2x3.val[0] = vtrn1q_u64(r0.u64x2x3.val[2], r1.u64x2x3.val[0]);
        // dst[17] = src[11]; dst[18] = src[17];
        t3.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[0], r3.u8x16x3.val[1], PI_HI_LO_IDS);
        // dst[19] = src[23];
        t3.u8x16x3.val[2] = pi_tbl(r4.u8x16x3.val[1], r4.u8x16x3.val[1], PI_HI_LO_IDS);
      }

      row_t t4 = { 0 };
      {
        // dst[20] = src[ 2]; dst[21] = src[ 8];
        t4.u8x16x3.val[0] = pi_tbl(r0.u8x16x3.val[1], r1.u8x16x3.val[1], PI_LO_HI_IDS);
        // dst[22] = src[14]; dst[23] = src[15];
        // t4.u8x16x3.val[1] = pi_tbl(r2.u8x16x3.val[2], r3.u8x16x3.val[0], PI_LO_LO_IDS);
        t4.u64x2x3.val[1] = vtrn1q_u64(r2.u64x2x3.val[2], r3.u64x2x3.val[0]);
        // dst[24] = src[21];
        t4.u8x16x3.val[2] = pi_tbl(r4.u8x16x3.val[0], r4.u8x16x3.val[0], PI_HI_LO_IDS);
      }

      r0 = t0;
      r1 = t1;
      r2 = t2;
      r3 = t3;
      r4 = t4;
    }

    // chi
    r0 = row_chi(r0);
    r1 = row_chi(r1);
    r2 = row_chi(r2);
    r3 = row_chi(r3);
    r4 = row_chi(r4);

    // iota
    {
      const uint64x2_t rc = { RCS[i], 0 };
      r0.u64x2x3.val[0] ^= rc;
    }
  }

  // store rows
  row_store(a +  0, r0);
  row_store(a +  5, r1);
  row_store(a + 10, r2);
  row_store(a + 15, r3);
  row_store(a + 20, r4);
}

void permute_neon(uint64_t a[static 25]) {
  permute_neon_inline(a);
}

static void check_state(const char *func, const size_t test_id, const uint64_t got[static 25], const uint64_t exp[static 25]) {
  if (!memcmp(got, exp, 25*sizeof(uint64_t))) {
    return;
  }

  fprintf(stderr, "%s[%zu] failed:\n", func, test_id);
  for (size_t i = 0; i < 25; i++) {
    if (got[i] != exp[i]) {
      fprintf(stderr, "  %2zu: got 0x%016" PRIx64 ", exp 0x%016" PRIx64 "\n", i, got[i], exp[i]);
    }
  }
}


static const struct {
  uint64_t val[5], exp[5];
} ROW_RLL_TESTS[] = {{
  .val = {
    0x1111000011110000ULL, 0x2222000022220000ULL, 0x3333000033330000ULL,
    0x4444000044440000ULL, 0x5555000055550000ULL,
  },

  .exp = {
    0x5555000055550000ULL, 0x1111000011110000ULL, 0x2222000022220000ULL,
    0x3333000033330000ULL, 0x4444000044440000ULL,
  },
}};

void test_row_rll(void) {
  for (size_t i = 0; i < sizeof(ROW_RLL_TESTS)/sizeof(ROW_RLL_TESTS[i]); i++) {
    const row_t got = row_rll(row_load(ROW_RLL_TESTS[i].val)),
                exp = row_load(ROW_RLL_TESTS[i].exp);
    if (!row_eq(got, exp)) {
      fprintf(stderr, "%s[%zu] failed:\n", __func__, i);
      row_print(stderr, "got", got);
      row_print(stderr, "exp()", exp);
    }
  }
}

static const struct {
  uint64_t val[5], exp[5];
} ROW_RLR_TESTS[] = {{
  .val = {
    0x1111000011110000ULL, 0x2222000022220000ULL, 0x3333000033330000ULL,
    0x4444000044440000ULL, 0x5555000055550000ULL,
  },

  .exp = {
    0x2222000022220000ULL, 0x3333000033330000ULL, 0x4444000044440000ULL,
    0x5555000055550000ULL, 0x1111000011110000ULL,
  },
}};

void test_row_rlr(void) {
  for (size_t i = 0; i < sizeof(ROW_RLR_TESTS)/sizeof(ROW_RLR_TESTS[i]); i++) {
    const row_t got = row_rlr(row_load(ROW_RLR_TESTS[i].val)),
                exp = row_load(ROW_RLR_TESTS[i].exp);
    if (!row_eq(got, exp)) {
      fprintf(stderr, "%s[%zu] failed:\n", __func__, i);
      row_print(stderr, "got", got);
      row_print(stderr, "exp", exp);
    }
  }
}

static const struct {
  const uint64_t val[5], exp[5];
} ROW_ROL1_U64_TESTS[] = {{
  .val = {
    0x8080808080808080ULL, 0x4000000000000001ULL, 0xFFFFFFFFFFFFFFFFULL,
    1, 0xf00ff00ff00ff00fULL,
  },

  .exp = {
    0x0101010101010101ULL, 0x8000000000000002ULL, 0xFFFFFFFFFFFFFFFFULL,
    2, 0xe01fe01fe01fe01fULL,
  },
}, {
  .val = {
    0, 0, 0, 0, 0xffffffffffffffffULL,
  },

  .exp = {
    0, 0, 0, 0, 0xffffffffffffffffULL,
  },
}, {
  .val = {
    0, 0, 0, 0, 0x1402e6186b180445ULL,
  },

  .exp = {
    0, 0, 0, 0, 0x2805cc30d630088aULL,
  },
}};

void test_row_rol1_u64(void) {
  for (size_t i = 0; i < sizeof(ROW_ROL1_U64_TESTS)/sizeof(ROW_ROL1_U64_TESTS[0]); i++) {
    const row_t got = row_rol1_u64(row_load(ROW_ROL1_U64_TESTS[i].val)),
                exp = row_load(ROW_ROL1_U64_TESTS[i].exp);
    if (!row_eq(got, exp)) {
      fprintf(stderr, "%s[%zu] failed:\n", __func__, i);
      row_print(stderr, "got", got);
      row_print(stderr, "exp()", exp);
    }
  }
}

static const struct {
  const uint64x2_t val; // value
  const int n; // shift amount
  const uint64x2_t exp; // expected result
} VSHL_TESTS[] = {{
  .val = { 1, 2 },
  .n = 2,
  .exp = { 4, 8 },
}};

static void test_vshl(void) {
  for (size_t i = 0; i < sizeof(VSHL_TESTS)/sizeof(VSHL_TESTS[0]); i++) {
    const uint64x2_t got = vshlq_n_u64(VSHL_TESTS[i].val, VSHL_TESTS[i].n),
                     exp = VSHL_TESTS[i].exp;
    if (got[0] != exp[0] || got[1] != exp[1]) {
      fprintf(stderr, "%s[%zu] failed: got = { 0x%016" PRIx64 ", 0x%016" PRIx64 " }, exp { 0x%016" PRIx64 ", 0x%016" PRIx64 " }\n", __func__, i, got[0], got[1], exp[0], exp[1]);
    }
  }
}

static const struct {
  const uint64x2_t val; // value
  const int n; // shift amount
  const uint64x2_t exp; // expected result
} VSHR_TESTS[] = {{
  .val = { 128, 64 },
  .n = 3,
  .exp = { 16, 8 },
}, {
  .val = { 0xffffffffffffffffULL, 1 },
  .n = 32,
  .exp = { 0x00000000ffffffffULL, 0x0000000000000000ULL },
}};

static void test_vshr(void) {
  for (size_t i = 0; i < sizeof(VSHR_TESTS)/sizeof(VSHR_TESTS[0]); i++) {
    const uint64x2_t got = vshrq_n_u64(VSHR_TESTS[i].val, VSHR_TESTS[i].n),
                     exp = VSHR_TESTS[i].exp;
    if (got[0] != exp[0] || got[1] != exp[1]) {
      fprintf(stderr, "%s[%zu] failed: got = { 0x%016" PRIx64 ", 0x%016" PRIx64 " }, exp { 0x%016" PRIx64 ", 0x%016" PRIx64 " }\n", __func__, i, got[0], got[1], exp[0], exp[1]);
    }
  }
}

static const struct {
  uint64_t vals[25];
} STEP_TESTS[] = {{
  .vals = {
     0,  1,  2,  3,  4,
     5,  6,  7,  8,  9,
    10, 11, 12, 13, 14,
    15, 16, 17, 18, 19,
    20, 21, 22, 23, 24,
  },
}, {
  .vals = {
    0xed75cc99f95d7eb8ULL, 0x70522b2a8e27ccfcULL, 0xdc00ed4f6013a7ffULL,
    0xa1e8fbe368b7fdfdULL, 0x70983572fc9db432ULL, 0x44b2462e484fa34aULL,
    0xb396e470cb8f3acaULL, 0x7c04234942b8453eULL, 0x718572a8690ebca1ULL,
    0xc4c97efc0a50125aULL, 0x2d1793cf7ffbe712ULL, 0xbeab71cb246ec315ULL,
    0x777d1c38bbabe52eULL, 0xbd3d94b1b6108e43ULL, 0xc33c38c7bd1c8091ULL,
    0x95ac83da732f1e1dULL, 0x52fee1f599362359ULL, 0x33135911aa6a29d9ULL,
    0xb894164e4f8cc6c4ULL, 0x7fe8cb1f98653558ULL, 0xaaad1b5aa5d44f6eULL,
    0xdc6fe61d7058204cULL, 0x4b2077ac105f263aULL, 0x9cb508404efe40d1ULL,
    0x4f35804331736405ULL,
  },
}, {
  .vals = {
    0x44a62d10e00d5090ULL, 0x493dcf18eeec7508ULL, 0x555c6b97944c6c97ULL,
    0x44e0336998cca855ULL, 0x5393340977d45449ULL, 0x63528a24675c690cULL,
    0xe1dab4ea5caede86ULL, 0xc99bf515b1199a3fULL, 0xcb7b737793edda74ULL,
    0xe346f250802395c9ULL, 0x48d237f16ac0b16dULL, 0x842833fbfc359020ULL,
    0x9d72b5a715bea1b7ULL, 0x1329019e464b9a6dULL, 0xe26d067ce59a3481ULL,
    0x25c86cf15682af6fULL, 0x5c3016c68256fd2dULL, 0xb64b832f7fe12cf3ULL,
    0x4865532bea1b1b92ULL, 0xcd83f1dcc8476c12ULL, 0x5eec1a2cd00b23dbULL,
    0xc204190b7d638e42ULL, 0x315f40ae8a08694dULL, 0x8b3bfcc0b60d3480ULL,
    0xe4802d7dde1ee648ULL,
  },
}};

void test_theta(void) {
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    theta_scalar(exp);
    theta_neon(got);

    check_state(__func__, i, got, exp);
  }
}

void test_rho(void) {
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    rho_scalar(exp);
    rho_neon(got);

    check_state(__func__, i, got, exp);
  }
}

void test_pi(void) {
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    pi_scalar(exp);
    pi_neon(got);

    check_state(__func__, i, got, exp);
  }
}

void test_chi(void) {
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    chi_scalar(exp);
    chi_neon(got);

    check_state(__func__, i, got, exp);
  }
}

void test_iota(void) {
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    for (size_t j = 0; j < 24; j++) {
      iota_scalar(exp, j);
      iota_neon(got, j);

      check_state(__func__, i * 1000 + j, got, exp);
    }
  }
}

void test_permute(void) {
  for (size_t z = 0; z < 100000; z++) {
    for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
      uint64_t got[25] = { 0 }, exp[25] = { 0 };
      memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
      memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

      permute_scalar(exp);
      permute_neon(got);

      check_state(__func__, i, got, exp);
    }
  }
}

int main(void) {
  test_row_rll();
  test_row_rlr();
  test_vshl();
  test_vshr();
  test_row_rol1_u64();
  test_theta();
  test_rho();
  test_pi();
  test_chi();
  test_iota();
  test_permute();

  return 0;
}
