#include <stdio.h>
#include <time.h>

#include "shamir.h"

#define N 5
#define K 3

#define LENGTH 1000000

int main(int argc, char *args)
{

 uint32_t shares[N];
 uint32_t recovered_shares[K];

 FILE *fp = fopen("/dev/random" , "r");

 uint32_t x[K];
 int      u[N];

 int d;
 for(d = 0;d < LENGTH;d++)
 {
  uint32_t secret = rand() % SHAMIR_PRIME;
  encode(secret, N, K, shares, fp);
 
  int i;
  for(i = 0;i < N;i++)
  {
   u[i] = 0;
  }

  for(i = 0;i < K;)
  {
   int r = rand() % N;
   if(u[r])
   {
    continue;
   }
   x[i] = r + 1;
   recovered_shares[i] = shares[r];
   u[r] = 1;
   i++;
  }
 
  uint32_t recovered = decode(x, recovered_shares, K, K);
  if(secret != recovered)
  {
   printf("secret = %d  recovered secret = %d (%s)\n" , secret , recovered , secret == recovered ? "Match" : "Mis-match");
  }
 }

 fclose(fp);

}
