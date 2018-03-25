#include <stdio.h>

/* 
   Use d0, d1, and carry_in to compute a return value.
   If necessary, use the carry_out pointer to return additional information
*/
int add_two_bits(int d0, int d1, int carry_in, int *carry_out)
{
    *carry_out = ((d0 ^ d1) & (carry_in)) || (d0 & d1);
    return (d0 ^ d1) ^ carry_in;
}

int main()
{
  int c_out;
  int d0[4];
  int d1[4];
  int ci[4] = {0};
  int sum[4];

  /* Call add_two_bits multiple times */
 
  for(int i = 0; i < 4; i++) {
      printf("Enter the binary digit for d0[%d]: ", i);
      scanf("%d", &d0[i]);
      d0[i] = !!d0[i];
  }

  for(int j = 0; j < 4; j++) {
      printf("Enter the binary digit for d1[%d]: ", j);
      scanf("%d", &d1[j]);
      d1[j] = !!d1[j];
  }

  for(int k = 3; k > -1; k--) { //up to the final computation
      if(k > 0) {
        sum[k] = add_two_bits(d0[k], d1[k], ci[k], &ci[k - 1]);
      }
      else if (k == 0){
        sum[k] = add_two_bits(d0[k], d1[k], ci[k], &c_out);
      }
  }
   
  for(int l = 0; l < 4; l++){
    printf("%d", d0[l]);
  }

  printf(" + ");

  for(int m = 0; m < 4; m++){
    printf("%d", d1[m]);
  }

  printf(" = ");

  for(int n = 0; n < 4; n++) {
    printf("%d", sum[n]);
  }

  if(c_out){
      printf(" with a carry\n");
  } else {
      printf(" without a carry\n");
  }
             
  return 0; 
  
}
