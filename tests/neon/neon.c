// test neon keccak permutation which validates steps against scalar
// counterparts

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
typedef struct {
  uint64x2_t p0, p1, p2;
} row_t;

// load row from array
row_t row_load(const uint64_t p[static 5]) {
  row_t a = { 0 };

  a.p0 = vld1q_u64(p + 0);
  a.p1 = vld1q_u64(p + 2);
  a.p2 = vdupq_n_u64(p[4]);

  return a;
}

// store row to array
void row_store(uint64_t p[static 5], const row_t a) {
  // row_print(stderr, __func__, a);
  vst1q_u64(p + 0, a.p0);
  vst1q_u64(p + 2, a.p1);
  vst1_u64(p + 4, vdup_laneq_u64(a.p2, 0));
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

#if 0
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
#endif

// get Nth pair of u64s from row
#define ROW_GET(A, N) ((A).p ## N)

// set contents of row
static inline row_t row_set(const uint64x2_t a, const uint64x2_t b, const uint64x2_t c) {
  return (row_t) { a, b, c };
}

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
  return row_set(
    vzip1q_u64(ROW_GET(a, 2), ROW_GET(a, 0)), // { a4, a0 }
    vextq_u64(ROW_GET(a, 0), ROW_GET(a, 1), 1), // { a1, a2 }
    vdupq_laneq_u64(ROW_GET(a, 1), 1) // { a3, n/a }
  );
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
  return row_set(
    vextq_u64(ROW_GET(a, 0), ROW_GET(a, 1), 1), // { a1, a2 }
    vextq_u64(ROW_GET(a, 1), ROW_GET(a, 2), 1), // { a3, a4 }
    ROW_GET(a, 0) // { a0, n/a }
  );
}

// c = a ^ b
#define ROW_EOR(A, B) row_set( \
  ROW_GET(A, 0) ^ ROW_GET(B, 0), \
  ROW_GET(A, 1) ^ ROW_GET(B, 1), \
  ROW_GET(A, 2) ^ ROW_GET(B, 2) \
)

// f = a ^ b ^ c ^ d ^ e
// FIXME want: veor3_u64(a, b, c);
static inline row_t row_eor5(const row_t a, const row_t b, const row_t c, const row_t d, const row_t e) {
  const uint64x2_t p0 = ROW_GET(a, 0) ^ ROW_GET(b, 0) ^ ROW_GET(c, 0) ^ ROW_GET(d, 0) ^ ROW_GET(e, 0),
                   p1 = ROW_GET(a, 1) ^ ROW_GET(b, 1) ^ ROW_GET(c, 1) ^ ROW_GET(d, 1) ^ ROW_GET(e, 1),
                   p2 = ROW_GET(a, 2) ^ ROW_GET(b, 2) ^ ROW_GET(c, 2) ^ ROW_GET(d, 2) ^ ROW_GET(e, 2);

  return row_set(p0, p1, p2);
}


// rotate bits in each lane left one bit
static inline row_t row_rol1_u64(const row_t a) {
  const uint64x2_t p0 = ROW_GET(a, 0),
                   p1 = ROW_GET(a, 1),
                   p2 = ROW_GET(a, 2);

  return row_set(VROLQ(p0, 1), VROLQ(p1, 1), VROLQ(p2, 1));
}

// apply rho rotation to row
static inline row_t row_rho(const row_t a, const int64x2_t v0, const int64x2_t v1, const int64x2_t v2) {
  const uint64x2_t p0 = ROW_GET(a, 0),
                   p1 = ROW_GET(a, 1),
                   p2 = ROW_GET(a, 2);

  return row_set(
    vorrq_u64(vshlq_u64(p0, v0), vshlq_u64(p0, v0 - 64)),
    vorrq_u64(vshlq_u64(p1, v1), vshlq_u64(p1, v1 - 64)),
    vorrq_u64(vshlq_u64(p2, v2), vshlq_u64(p2, v2 - 64))
  );
}

// c = (~a & b)
// note: was using ~(a | ~b) = (~a & b) (demorgan's laws), but changed
// to BIC b, a instead (b & ~a)
static inline row_t row_andn(const row_t a, const row_t b) {
  return row_set(
    vbicq_u64(ROW_GET(b, 0), ROW_GET(a, 0)),
    vbicq_u64(ROW_GET(b, 1), ROW_GET(a, 1)),
    vbicq_u64(ROW_GET(b, 2), ROW_GET(a, 2))
  );
}

// apply chi permutation to entire row
// note: ~(a | ~b) = (~a & b) (demorgan's laws)
static inline row_t row_chi(const row_t a) {
  // a ^ (rlr(a, 1) & rlr(a, 2)) (rlr = rotate lane right)
  return ROW_EOR(a, row_andn(row_rlr(a), row_set(
    ROW_GET(a, 1),                            // { a2, a3 }
    vtrn1q_u64(ROW_GET(a, 2), ROW_GET(a, 0)), // { a4, a0 }
    vdupq_laneq_u64(ROW_GET(a, 0), 1)         // { a1, n/a }
  )));
}

// row compare (not constant-time)
_Bool row_eq(const row_t a, const row_t b) {
  uint64_t a_u64[5], b_u64[5];
  row_store(a_u64, a);
  row_store(b_u64, b);
  return !memcmp(a_u64, b_u64, sizeof(a_u64));
}

// theta step of neon keccak permutation
void theta_neon(uint64_t a[static 25]) {
  // ---------------------------------------------------------
  // |     |        Column / Register and 64-Bit Lane        |
  // |-------------------------------------------------------|
  // | Row |    3    |    4    |    0    |    1    |    2    |
  // |-----|---------|---------|---------|---------|---------|
  // |  2  | r2.p1.1 | r2.p2.0 | r2.p0.0 | r2.p0.1 | r2.p1.0 |
  // |  1  | r1.p1.1 | r1.p2.0 | r1.p0.0 | r1.p0.1 | r1.p1.0 |
  // |  0  | r0.p1.1 | r0.p2.0 | r0.p0.0 | r0.p0.1 | r1.p1.0 |
  // |  4  | r4.p1.1 | r4.p2.0 | r4.p0.0 | r4.p0.1 | r1.p1.0 |
  // |  3  | r3.p1.1 | r3.p2.0 | r3.p0.0 | r3.p0.1 | r1.p1.0 |
  // ---------------------------------------------------------

  // load rows
  row_t r0 = row_load(a +  0),
        r1 = row_load(a +  5),
        r2 = row_load(a + 10),
        r3 = row_load(a + 15),
        r4 = row_load(a + 20);

  {
    /* c = r0 ^ r1 ^ r2 ^ r3 ^ r4, d = rll(c) ^ (rlr(c) << 1) */
    const row_t c = row_eor5(r0, r1, r2, r3, r4),
                d = ROW_EOR(row_rll(c), row_rol1_u64(row_rlr(c)));

    r0 = ROW_EOR(r0, d); /* r0 ^= d */
    r1 = ROW_EOR(r1, d); /* r1 ^= d */
    r2 = ROW_EOR(r2, d); /* r2 ^= d */
    r3 = ROW_EOR(r3, d); /* r3 ^= d */
    r4 = ROW_EOR(r4, d); /* r4 ^= d */
  }

  // store rows
  row_store(a +  0, r0);
  row_store(a +  5, r1);
  row_store(a + 10, r2);
  row_store(a + 15, r3);
  row_store(a + 20, r4);
}

// rho step of neon keccak permutation
void rho_neon(uint64_t a[static 25]) {
  // encoded rho rotate values
  //
  // original values:
  //
  //   static const int64x2_t
  //     r0_a = {  0,  1 }, r0_b = { 62, 28 }, r02_c = { 27, 39 },
  //     r1_a = { 36, 44 }, r1_b = {  6, 55 }, r13_c = { 20, 8 },
  //     r2_a = {  3, 10 }, r2_b = { 43, 25 },
  //     r3_a = { 41, 45 }, r3_b = { 15, 21 },
  //     r4_a = { 18,  2 }, r4_b = { 61, 56 }, r4_c = { 14, 0 };
  //
  // low element of r[0-4]_{a,b} packed into low lane of r_ab, like so:
  //
  //   >> v = [0, 36, 3, 41, 18, 62, 6, 43, 15, 61].each_with_index.reduce(0) { |r, (c, i)| r+(64**i)*
  //   c }
  //   => 1103290028930644224
  //   >> (v >> 6*9) & 0x3f
  //   => 61
  //   >> 6*9
  //   => 54
  //   >> v
  //   => 1103290028930644224
  //   >> '0x%016x' % v
  //   => "0x0f4fac6f92a43900"
  //
  // high element of r[0-4]_{a,b} packed into high lane of r_ab, like so:
  //
  //   >> v = [1, 44, 10, 45, 2, 28, 55, 25, 21, 56].each_with_index.reduce(0) { |r, (c, i)| r+(64**i)
  //   *c }
  //   => 1014831051886078721
  //   >> '0x%016x' % v
  //   => "0x0e15677702b4ab01"
  //
  // low elements of r[0-4]_c packed into low lane of r_c, like so:
  //
  //   >> v = [27, 20, 39, 8, 14].each_with_index.reduce(0) { |r, (c, i)| r+(64**i)*c }
  //   => 237139227
  //   >> '0x%016x' % v
  //   => "0x000000000e22751b"
  //
  // (there are no high elements of r[0-4]_c, all zero)
  //
  // to extract elements, right shift by 6*Y (where Y is the row
  // number), then mask to lower 6 bits (0x3f).  for example, to
  // extract r4_b:
  //
  //   >> (v >> 6*9) & 0x3f
  //   => 61
  static const int64x2_t r_ab = { 0x0f4fac6f92a43900LL, 0x0e15677702b4ab01LL },
                         r_c = { 0x000000000e22751bLL, 0 };

  // load rows
  row_t r0 = row_load(a +  0),
        r1 = row_load(a +  5),
        r2 = row_load(a + 10),
        r3 = row_load(a + 15),
        r4 = row_load(a + 20);

  r0 = row_rho(r0, r_ab         & 0x3f, (r_ab >> 30) & 0x3f, r_c         & 0x3f);
  r1 = row_rho(r1, (r_ab >>  6) & 0x3f, (r_ab >> 36) & 0x3f, (r_c >>  6) & 0x3f);
  r2 = row_rho(r2, (r_ab >> 12) & 0x3f, (r_ab >> 42) & 0x3f, (r_c >> 12) & 0x3f);
  r3 = row_rho(r3, (r_ab >> 18) & 0x3f, (r_ab >> 48) & 0x3f, (r_c >> 18) & 0x3f);
  r4 = row_rho(r4, (r_ab >> 24) & 0x3f, (r_ab >> 54) & 0x3f, (r_c >> 24) & 0x3f);

  // store rows
  row_store(a +  0, r0);
  row_store(a +  5, r1);
  row_store(a + 10, r2);
  row_store(a + 15, r3);
  row_store(a + 20, r4);
}

// return new vector with low lane of first argument and high lane of
// second argument
static inline uint64x2_t pi_lo_hi(const uint64x2_t a, const uint64x2_t b) {
  // was using vqtbl2q_u8() with tables, but this is faster
  const uint64x2_t c = vextq_u64(b, a, 1);
  return vextq_u64(c, c, 1);
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
    const row_t t0 = row_set(
      pi_lo_hi(ROW_GET(r0, 0), ROW_GET(r1, 0)),
      pi_lo_hi(ROW_GET(r2, 1), ROW_GET(r3, 1)),
      ROW_GET(r4, 2)
    );

    const row_t t1 = row_set(
      vextq_u64(ROW_GET(r0, 1), ROW_GET(r1, 2), 1),
      pi_lo_hi(ROW_GET(r2, 0), ROW_GET(r3, 0)),
      ROW_GET(r4, 1)
    );

    const row_t t2 = row_set(
      vextq_u64(ROW_GET(r0, 0), ROW_GET(r1, 1), 1),
      vextq_u64(ROW_GET(r2, 1), ROW_GET(r3, 2), 1),
      ROW_GET(r4, 0)
    );

    const row_t t3 = row_set(
      vtrn1q_u64(ROW_GET(r0, 2), ROW_GET(r1, 0)),
      vextq_u64(ROW_GET(r2, 0), ROW_GET(r3, 1), 1),
      vdupq_laneq_u64(ROW_GET(r4, 1), 1)
    );

    const row_t t4 = row_set(
      pi_lo_hi(ROW_GET(r0, 1), ROW_GET(r1, 1)),
      vtrn1q_u64(ROW_GET(r2, 2), ROW_GET(r3, 0)),
      vdupq_laneq_u64(ROW_GET(r4, 0), 1)
    );

    /* store rows */
    r0 = t0;
    r1 = t1;
    r2 = t2;
    r3 = t3;
    r4 = t4;
  }

  row_store(a +  0, r0);
  row_store(a +  5, r1);
  row_store(a + 10, r2);
  row_store(a + 15, r3);
  row_store(a + 20, r4);
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
  r0.p0 ^= rc;
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
  // |     |        Column / Register and 64-Bit Lane        |
  // |-------------------------------------------------------|
  // | Row |    3    |    4    |    0    |    1    |    2    |
  // |-----|---------|---------|---------|---------|---------|
  // |  2  | r2.p1.1 | r2.p2.0 | r2.p0.0 | r2.p0.1 | r2.p1.0 |
  // |  1  | r1.p1.1 | r1.p2.0 | r1.p0.0 | r1.p0.1 | r1.p1.0 |
  // |  0  | r0.p1.1 | r0.p2.0 | r0.p0.0 | r0.p0.1 | r1.p1.0 |
  // |  4  | r4.p1.1 | r4.p2.0 | r4.p0.0 | r4.p0.1 | r1.p1.0 |
  // |  3  | r3.p1.1 | r3.p2.0 | r3.p0.0 | r3.p0.1 | r1.p1.0 |
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
      /* c = r0 ^ r1 ^ r2 ^ r3 ^ r4, d = rll(c) ^ (rlr(c) << 1) */
      const row_t c = row_eor5(r0, r1, r2, r3, r4),
                  d = ROW_EOR(row_rll(c), row_rol1_u64(row_rlr(c)));

      r0 = ROW_EOR(r0, d); /* r0 ^= d */
      r1 = ROW_EOR(r1, d); /* r1 ^= d */
      r2 = ROW_EOR(r2, d); /* r2 ^= d */
      r3 = ROW_EOR(r3, d); /* r3 ^= d */
      r4 = ROW_EOR(r4, d); /* r4 ^= d */
    }

    // rho
    {
      // encoded rho rotate values
      //
      // original values:
      //
      //   static const int64x2_t
      //     r0_a = {  0,  1 }, r0_b = { 62, 28 }, r02_c = { 27, 39 },
      //     r1_a = { 36, 44 }, r1_b = {  6, 55 }, r13_c = { 20, 8 },
      //     r2_a = {  3, 10 }, r2_b = { 43, 25 },
      //     r3_a = { 41, 45 }, r3_b = { 15, 21 },
      //     r4_a = { 18,  2 }, r4_b = { 61, 56 }, r4_c = { 14, 0 };
      //
      // low element of r[0-4]_{a,b} packed into low lane of r_ab, like so:
      //
      //   >> v = [0, 36, 3, 41, 18, 62, 6, 43, 15, 61].each_with_index.reduce(0) { |r, (c, i)| r+(64**i)*
      //   c }
      //   => 1103290028930644224
      //   >> (v >> 6*9) & 0x3f
      //   => 61
      //   >> 6*9
      //   => 54
      //   >> v
      //   => 1103290028930644224
      //   >> '0x%016x' % v
      //   => "0x0f4fac6f92a43900"
      //
      // high element of r[0-4]_{a,b} packed into high lane of r_ab, like so:
      //
      //   >> v = [1, 44, 10, 45, 2, 28, 55, 25, 21, 56].each_with_index.reduce(0) { |r, (c, i)| r+(64**i)
      //   *c }
      //   => 1014831051886078721
      //   >> '0x%016x' % v
      //   => "0x0e15677702b4ab01"
      //
      // low elements of r[0-4]_c packed into low lane of r_c, like so:
      //
      //   >> v = [27, 20, 39, 8, 14].each_with_index.reduce(0) { |r, (c, i)| r+(64**i)*c }
      //   => 237139227
      //   >> '0x%016x' % v
      //   => "0x000000000e22751b"
      //
      // (there are no high elements of r[0-4]_c, all zero)
      //
      // to extract elements, right shift by 6*Y (where Y is the row
      // number), then mask to lower 6 bits (0x3f).  for example, to
      // extract r4_b:
      //
      //   >> (v >> 6*9) & 0x3f
      //   => 61
      static const int64x2_t r_ab = { 0x0f4fac6f92a43900LL, 0x0e15677702b4ab01LL },
                             r_c = { 0x000000000e22751bLL, 0 };

      r0 = row_rho(r0, r_ab         & 0x3f, (r_ab >> 30) & 0x3f, r_c         & 0x3f);
      r1 = row_rho(r1, (r_ab >>  6) & 0x3f, (r_ab >> 36) & 0x3f, (r_c >>  6) & 0x3f);
      r2 = row_rho(r2, (r_ab >> 12) & 0x3f, (r_ab >> 42) & 0x3f, (r_c >> 12) & 0x3f);
      r3 = row_rho(r3, (r_ab >> 18) & 0x3f, (r_ab >> 48) & 0x3f, (r_c >> 18) & 0x3f);
      r4 = row_rho(r4, (r_ab >> 24) & 0x3f, (r_ab >> 54) & 0x3f, (r_c >> 24) & 0x3f);
    }

    // pi
    {
      const row_t t0 = row_set(
        pi_lo_hi(ROW_GET(r0, 0), ROW_GET(r1, 0)),
        pi_lo_hi(ROW_GET(r2, 1), ROW_GET(r3, 1)),
        ROW_GET(r4, 2)
      );

      const row_t t1 = row_set(
        vextq_u64(ROW_GET(r0, 1), ROW_GET(r1, 2), 1),
        pi_lo_hi(ROW_GET(r2, 0), ROW_GET(r3, 0)),
        ROW_GET(r4, 1)
      );

      const row_t t2 = row_set(
        vextq_u64(ROW_GET(r0, 0), ROW_GET(r1, 1), 1),
        vextq_u64(ROW_GET(r2, 1), ROW_GET(r3, 2), 1),
        ROW_GET(r4, 0)
      );

      const row_t t3 = row_set(
        vtrn1q_u64(ROW_GET(r0, 2), ROW_GET(r1, 0)),
        vextq_u64(ROW_GET(r2, 0), ROW_GET(r3, 1), 1),
        vdupq_laneq_u64(ROW_GET(r4, 1), 1)
      );

      const row_t t4 = row_set(
        pi_lo_hi(ROW_GET(r0, 1), ROW_GET(r1, 1)),
        vtrn1q_u64(ROW_GET(r2, 2), ROW_GET(r3, 0)),
        vdupq_laneq_u64(ROW_GET(r4, 0), 1)
      );

      /* store rows */
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
      r0.p0 ^= rc;
    }
  }

  // store rows
  row_store(a +  0, r0);
  row_store(a +  5, r1);
  row_store(a + 10, r2);
  row_store(a + 15, r3);
  row_store(a + 20, r4);
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

// vshlq_u64() tests
static const struct {
  const uint64x2_t val; // value
  const uint64x2_t exp; // expected result
} VSHL_TESTS[] = {{
  .val = { 1, 2 },
  .exp = { 4, 8 },
}};

// vshlq_u64() shift amount
//
// clang complains if the second parameter to vshlq_n_u64() is not a
// constant value (gcc does not)
#define TEST_VSHL_N 2

// test vshlq_u64()
static void test_vshl(void) {
  for (size_t i = 0; i < sizeof(VSHL_TESTS)/sizeof(VSHL_TESTS[0]); i++) {
    const uint64x2_t got = vshlq_n_u64(VSHL_TESTS[i].val, TEST_VSHL_N),
                     exp = VSHL_TESTS[i].exp;
    if (got[0] != exp[0] || got[1] != exp[1]) {
      fprintf(stderr, "%s[%zu] failed: got = { 0x%016" PRIx64 ", 0x%016" PRIx64 " }, exp { 0x%016" PRIx64 ", 0x%016" PRIx64 " }\n", __func__, i, got[0], got[1], exp[0], exp[1]);
    }
  }
}

// vshr, n=3 tests
static const struct {
  const uint64x2_t val; // value
  const uint64x2_t exp; // expected result
} VSHR_N3_TESTS[] = {{
  .val = { 128, 64 },
  .exp = { 16, 8 },
}};

// vshr, n=32 tests
static const struct {
  const uint64x2_t val; // value
  const uint64x2_t exp; // expected result
} VSHR_N32_TESTS[] = {{
  .val = { 0xffffffffffffffffULL, 1 },
  .exp = { 0x00000000ffffffffULL, 0x0000000000000000ULL },
}};

// test vshrq_u64()
//
// note: tests are split so we can use a constant value for N;  clang
// complains if the second parameter to vshrq_n_u64() is not a constant
// value (gcc does not)
static void test_vshr(void) {
  // loop over n=3 tests
  for (size_t i = 0; i < sizeof(VSHR_N3_TESTS)/sizeof(VSHR_N3_TESTS[0]); i++) {
    const uint64x2_t got = vshrq_n_u64(VSHR_N3_TESTS[i].val, 3),
                     exp = VSHR_N3_TESTS[i].exp;
    if (got[0] != exp[0] || got[1] != exp[1]) {
      fprintf(stderr, "%s[n=3, %zu] failed: got = { 0x%016" PRIx64 ", 0x%016" PRIx64 " }, exp { 0x%016" PRIx64 ", 0x%016" PRIx64 " }\n", __func__, i, got[0], got[1], exp[0], exp[1]);
    }
  }
  
  // loop over n=32 tests
  for (size_t i = 0; i < sizeof(VSHR_N32_TESTS)/sizeof(VSHR_N32_TESTS[0]); i++) {
    const uint64x2_t got = vshrq_n_u64(VSHR_N32_TESTS[i].val, 32),
                     exp = VSHR_N32_TESTS[i].exp;
    if (got[0] != exp[0] || got[1] != exp[1]) {
      fprintf(stderr, "%s[n=32, %zu] failed: got = { 0x%016" PRIx64 ", 0x%016" PRIx64 " }, exp { 0x%016" PRIx64 ", 0x%016" PRIx64 " }\n", __func__, i, got[0], got[1], exp[0], exp[1]);
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
  // loop over tests
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    // load test data
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    // run scalar theta and neon theta
    theta_scalar(exp);
    theta_neon(got);

    // compare scalar and neon results
    check_state(__func__, i, got, exp);
  }
}

void test_rho(void) {
  // loop over tests
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    // load test data
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    // run scalar rho and neon rho
    rho_scalar(exp);
    rho_neon(got);

    // compare scalar and neon results
    check_state(__func__, i, got, exp);
  }
}

void test_pi(void) {
  // loop over tests
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    // load test data
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    // run scalar pi and neon pi
    pi_scalar(exp);
    pi_neon(got);

    // compare scalar and neon results
    check_state(__func__, i, got, exp);
  }
}

void test_chi(void) {
  // loop over tests
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    // load test data
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    // run scalar chi and neon chi
    chi_scalar(exp);
    chi_neon(got);

    // compare scalar and neon results
    check_state(__func__, i, got, exp);
  }
}

void test_iota(void) {
  // loop over tests
  for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
    // load test data
    uint64_t got[25] = { 0 }, exp[25] = { 0 };
    memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
    memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

    for (size_t j = 0; j < 24; j++) {
      // run scalar iota and neon iota
      iota_scalar(exp, j);
      iota_neon(got, j);

      // compare scalar and neon results
      check_state(__func__, i * 1000 + j, got, exp);
    }
  }
}

void test_permute_slow(void) {
  // loop over tests
  for (size_t z = 0; z < 100000; z++) {
    for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
      // load test data
      uint64_t got[25] = { 0 }, exp[25] = { 0 };
      memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
      memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

      // run scalar permute and slow neon permute
      permute_scalar(exp);
      permute_neon_slow(got);

      // compare scalar and neon results
      check_state(__func__, i, got, exp);
    }
  }
}

void test_permute_inline(void) {
  // loop over tests
  for (size_t z = 0; z < 100000; z++) {
    for (size_t i = 0; i < sizeof(STEP_TESTS)/sizeof(STEP_TESTS[0]); i++) {
      // load test data
      uint64_t got[25] = { 0 }, exp[25] = { 0 };
      memcpy(got, STEP_TESTS[i].vals, sizeof(exp));
      memcpy(exp, STEP_TESTS[i].vals, sizeof(exp));

      // run scalar permute and inline neon permute
      permute_scalar(exp);
      permute_neon_inline(got); // TODO

      // compare scalar and neon results
      check_state(__func__, i, got, exp);
    }
  }
}

int main(void) {
  // test primitives
  test_row_rll();
  test_row_rlr();
  test_vshl();
  test_vshr();
  test_row_rol1_u64();

  // test steps
  test_theta();
  test_rho();
  test_pi();
  test_chi();
  test_iota();

  // test permute
  test_permute_slow();
  test_permute_inline();

  // print/return success
  printf("ok\n");
  return 0;
}
