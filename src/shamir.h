#ifndef SHAMIR
#define SHAMIR

#define SHAMIR_PRIME     ((uint32_t)65809)
#define SHAMIR_THRESHOLD ((uint32_t)65262)

// SHAMIR_THRESHOLD * SHAMIR_PRIME must be less than 2^32

////////////////////////////////////////////////////////////////////
// encodes a secret                                               //
//                                                                //
// secret - secret to store.  It must be less than SHAAMIR_PRIME  //
// n      - number of shares                                      //
// k      - number of shares required to decode message           //
// shares - buffer to store shares                                //
// random - FILE which provides random numbers                    //
////////////////////////////////////////////////////////////////////

uint32_t *encode(uint32_t secret, int n, int k, uint32_t *shares, FILE *random);

////////////////////////////////////////////////////////////////////
// decodes a secret                                               //
//                                                                //
// x      - array of share indexes                                //
// shares - array of shares
// n      - number of shares                                      //
// k      - number of shares required to decode message           //
////////////////////////////////////////////////////////////////////

uint32_t  decode(uint32_t *x, uint32_t *shares, int n, int k);

// Basic maths functions.  All calculations are mod SHAMIR_PRIME

uint32_t add(uint32_t a, uint32_t b);
uint32_t sub(uint32_t a, uint32_t b);
uint32_t multiply(uint32_t a, uint32_t b);
uint32_t power(uint32_t a, int b);

// Matrix functions for solving the linear equations

void mul_row(uint32_t *row, unsigned int a, int k);
void sub_row(uint32_t *from, uint32_t *to, int k);
void solve_matrix(uint32_t **eqn, int k);
void print_matrix(uint32_t **eqn, int k);
uint32_t linear_solve(uint32_t a, uint32_t b);

#endif
