/* CSCI-2500 Lab #8 - Digital Logic Emulation */
/* Include standard I/O */
#include <stdio.h>

#define FALSE 0
#define TRUE 1

/* Redefine short int as BIT */ 
typedef short int BIT;

/* Function Declarations (basic gates) */
BIT not_gate(BIT);
BIT or_gate(BIT, BIT);
BIT and_gate(BIT, BIT);
BIT xor_gate(BIT, BIT);

/* Function Declarations (combinational circuits) */
BIT multiplexer(BIT, BIT, BIT, BIT, BIT, BIT);
void decoder(BIT, BIT, BIT*, BIT*, BIT*, BIT*);

/* Function Declarations (sequential circuits) */
BIT sr_latch(BIT, BIT, BIT);

/* main function */
int main(int argc, char *argv[])
{

   BIT X, Y;
   BIT I0, I1, I2, I3;
   BIT O0, O1, O2, O3;
   BIT S0, S1;
   BIT S, R, Q;

   // Unit test for NOT gate - produce truth table
   // X   NOT(X)
   // 0     1
   // 1     0
   printf("===== Unit test for not_gate =====\n");
   printf("not_gate(X) | RESULT \n");
   printf("not_gate(0) | %d \n", not_gate(0) );
   printf("not_gate(1) | %d \n", not_gate(1) );
  
   
   // Unit test for OR gate
   // X Y (X OR Y)
   // 0 0   0
   // 0 1   1
   // 1 0   1
   // 1 1   1
   printf("===== Unit test for or_gate =====\n");
   printf("or_gate(X,Y) | RESULT\n");

   for( X = 0; X < 2; X++)
      for( Y = 0; Y < 2; Y++)
         printf("or_gate(%d,%d) | %d\n",X,Y,or_gate(X,Y));

   
   // Unit test for AND gate
   // X Y (X AND Y)
   // 0 0   0
   // 0 1   0
   // 1 0   0
   // 1 1   1
   printf("===== Unit test for and_gate =====\n");
   printf("and_gate(X,Y) | RESULT\n");
   for( X = 0; X < 2; X++)
      for( Y = 0; Y < 2; Y++)
         printf("and_gate(%d,%d) | %d\n",X,Y,and_gate(X,Y));

   
   // Unit test for XOR gate
   // X Y (X AND Y)
   // 0 0   0
   // 0 1   1
   // 1 0   1
   // 1 1   0
   printf("===== Unit test for and_gate =====\n");
   for( X = 0; X < 2; X++)
      for( Y = 0; Y < 2; Y++)
         printf("xor_gate(%d,%d) | %d\n",X,Y,xor_gate(X,Y));

   

   // Unit test for Multiplexer
   // (S0,S1) | (I0,I1,I2,I3) = RESULT (I0|I1|I2|I3)
   printf("===== Unit test for 4-input multiplexer =====\n");
   printf("multiplexer(S0,S1) | (I0,I1,I2,I3) | RESULT (I0|I1|I2|I3) \n");
   for( S0 = 0; S0 < 2; S0++) // Selector bit 1
   for( S1 = 0; S1 < 2; S1++) // Selector bit 2
   {

      for( I0 = 0; I0 < 2; I0++)
         for( I1 = 0; I1 < 2; I1++)
            for( I2 = 0; I2 < 2; I2++)
               for( I3 = 0; I3 < 2; I3++)
               {
                  printf("multiplexer(%2d,%2d) | (%2d,%2d,%2d,%2d) | %2d \n",S0,S1,I0,I1,I2,I3,
                         multiplexer(I0,I1,I2,I3,S0,S1) );
               }
   }

   // Unit test for Decoder
   // I0 I1 O0 O1 O2 O3
   // 0  0   1  0  0  0
   // 0  1   0  1  0  0
   // 1  0   0  0  1  0
   // 1  1   0  0  0  1
   printf("===== Unit test for 2-input decoder =====\n");
   printf("decoder(I0,I1) | (O0,O1,O2,O3)\n");

   for(I0 = 0; I0 < 2; I0++) {
       for(I1 = 0; I1 < 2; I1++) {
           decoder(I0, I1, &O0, &O1, &O2, &O3);
           printf("decoder(%2d,%2d) | (%2d,%2d,%2d,%2d)\n", I0, I1, O0, O1, O2, O3);
        }
   }
   
   
   // Unit test for S-R Latch
   // S R | Q | Q(t+1)
   // 0 0 | 0 | 0
   // 0 0 | 1 | 1
   // 0 1 | 0 | 0
   // 0 1 | 1 | 0
   // 1 0 | 0 | 1
   // 1 0 | 1 | 1
   // 1 1 | 0 | -1
   // 1 1 | 1 | -1
   printf("===== Unit test for sr_latch =====\n");
   printf("sr_latch( S, R ) | Q | Q(t+1)\n");
   // TODO: Implement test cases

   for(S = 0; S < 2; S++) {
       for(R = 0; R < 2; R++) {
           for(Q = 0; Q < 2; Q++) {
               printf("%2d%2d |%2d |%2d\n", S, R, Q, sr_latch(S, R, Q));
           }
       }
   }
           
   return 0;
}

BIT not_gate(BIT X)
{
    if (X==FALSE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BIT or_gate(BIT X, BIT Y)
{
    if (X==FALSE && Y==FALSE) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BIT and_gate(BIT X, BIT Y)
{
   return (X && Y);
}

BIT xor_gate(BIT X, BIT Y)
{
   return (X ^ Y);
}

BIT multiplexer(BIT I0, BIT I1, BIT I2, BIT I3, BIT S0, BIT S1)
{
   if(S0) {
       if(S1)
           return I3;
       return I2;
   }
   else {
       if(S1)
           return I1;
       return I0;
   }
}

void decoder(BIT I0, BIT I1, BIT *O0, BIT *O1, BIT *O2, BIT *O3)
{
   *O0 = 0;
   *O1 = 0;
   *O2 = 0;
   *O3 = 0;
   if(I0) {
       if(I1) {
           *O3 = 1;
       }
       else {
           *O2 = 1;
       }
   } else {
       if(I1) {
           *O1 = 1; 
       }
       else {
           *O0 = 1;
       }
   } 
   return;
}

/* Returns the next state of the S-R Latch */
BIT sr_latch(BIT S, BIT R, BIT Q)
{

   // if both bits are true then behavior of S-R latch
   // is undefined so we will have the simulation return -1 or UNDEF
   if( S==TRUE && R==TRUE) return -1;
   if(!S && !R) return Q;
   if(!S && R) return 0;
   if(S && !R) return 1;

   return -1;

}
