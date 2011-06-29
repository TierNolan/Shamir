#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "shamir.h"

uint32_t *encode(uint32_t secret, int n, int k, uint32_t *shares, FILE *random)
{

 if(secret >= SHAMIR_PRIME || secret < 0)
 {
  return NULL;
 }

 if(n >= SHAMIR_PRIME || k > n)
 {
  return NULL;
 }

 if(shares == NULL)
 {
  return NULL;
 }

 uint32_t *c_buffer = malloc(sizeof(*c_buffer)*k);
 
 c_buffer[0] = secret;

 int c;

 for(c = 1;c < k;c++)
 {
  uint32_t t;

  t =            fgetc(random);
  t = (t << 8) ^ fgetc(random);
  t = (t << 8) ^ fgetc(random);
  t = (t << 8) ^ fgetc(random);

  c_buffer[c] = t % SHAMIR_PRIME;
 }

 int x;

 for(x = 1;x <= n;x++)
 {
  uint32_t s = 0;

  int xp = 1;

  for(c = 0;c < k;c++)
  {
   s = add(s, multiply(c_buffer[c], xp));
   xp = multiply(xp, x);
  }

  shares[x-1] = s;
 }

 free(c_buffer);

 return shares;

}

uint32_t  decode(uint32_t *x, uint32_t *shares, int n, int k)
{

 if(n < k)
 {
  return -1;
 }

 uint32_t **eqn;
 uint32_t *eqn_all;

 eqn = malloc(sizeof(*eqn) * k);
 eqn_all = malloc(sizeof(*eqn_all) * k *(k + 1));

 int a;
 for(a = 0;a < k;a++)
  {
  eqn[a] = eqn_all + ((k + 1) * a);
  }

 int b;
 for(b = 0;b < k; b++)
 {

  uint32_t xp = 1;
  uint32_t xr = x[b];

  for(a = 0;a < k;a++)
  {
   eqn[b][a] = xp;
   xp = multiply(xp, xr);
  }
  eqn[b][k] = shares[b];
 }

 solve_matrix(eqn, k); 

 return linear_solve(eqn[0][0], eqn[0][k]);

}

uint32_t linear_solve(uint32_t a, uint32_t b)
{

 uint32_t inv = power(a, SHAMIR_PRIME - 2);

 return multiply(inv, b);

}

void solve_matrix(uint32_t **eqn, int k)
{

 int a, b;

 for(a = 0;a < k;a++)
 {
  for(b = 0;b < k;b++)
  {
   if(a == b)
   {
    continue;
   }
  
   uint32_t c, o; 

   c = eqn[a][a];
   o = eqn[b][a];

   mul_row(eqn[a], o, k);
   mul_row(eqn[b], c, k);

   sub_row(eqn[a], eqn[b], k);
  }
 }

}

void print_matrix(uint32_t **eqn, int k)
{
 int a, b;

 for(a = 0;a < k;a++)
 {
  for(b = 0;b <= k;b++)
  {
   printf("%d " , eqn[a][b]);
  }
 printf("\n");
 }

}

void mul_row(uint32_t *row, unsigned int a, int k)
{

 int i;
 for(i = 0;i <= k;i++)
 {
  row[i] = multiply(row[i], a);
 }

}

void sub_row(uint32_t *from, uint32_t *to, int k)
{

 int i;
 for(i = 0;i <= k;i++)
 {
  to[i] = sub(from[i], to[i]);
 }

}

uint32_t add(uint32_t a, uint32_t b)
{

 return (a + b) % SHAMIR_PRIME;

}

uint32_t sub(uint32_t a, uint32_t b)
{

 return (a - b + SHAMIR_PRIME) % SHAMIR_PRIME;

}

uint32_t multiply(uint32_t a, uint32_t b)
{

 if(a > SHAMIR_THRESHOLD)
  {
  uint64_t alarge = a;
  uint64_t blarge = b;

  return (alarge * blarge) % SHAMIR_PRIME;
  }
 else
  {

   return (a * b) % SHAMIR_PRIME;

  }

}

uint32_t power(uint32_t a, int b)
{

 int32_t t = 1;

 int i;

 int m = 0x0001;
 uint32_t e = a;

 while(m != 0)
 {
  if(m & b)
  {
   t = multiply(t, e);
  }
  m = (m << 1) & 0x1FFFF;
  e = multiply(e, e);
 }

 return t;

}
