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
  int d0, d1, sum, c_out;
  
  printf("Enter binary digit 0: ");
  scanf("%d", &d0);
  /* Clean the user-generated input */
  d0 = !!d0;

  printf("Enter binary digit 1: ");
  scanf("%d", &d1);
  /* Clean the user-generated input */
  d1 = !!d1;

  sum = add_two_bits(d0, d1, 0, &c_out);
  if (c_out) {
    printf("%d + %d = %d with a carry\n", d0, d1, add_two_bits(d0, d1, 0, &c_out));
  }
  else {
    printf("%d + %d = %d without a carry\n", d0, d1, add_two_bits(d0, d1, 0, &c_out));
  }
  
  return 0;
}
