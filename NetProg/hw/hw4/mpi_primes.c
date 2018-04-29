#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

int end_now = 0;

void sig_handler(int signo)
{
    if (signo == SIGUSR1) {
        end_now = 1;
    }
}

void find_primes() {
    int prime_count = 1; //accounting for the fact that 2 is prime
    for(unsigned int i = 3; i <= 1000; i++) {
        int prime = 1;

        for(unsigned int j = 2; j <= (int) ceil(sqrt( (double) i)); j++) {
            if(i % j == 0) {
                prime = 0;
                break;
            }
        }

        if(prime) prime_count++;

        if(i == 10 || i == 100 || i == 1000) printf("%d %d\n", i, prime_count);
    }
}

int main(int argc, char **argv)
{
    int count, id;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    printf("rank: %d\n", id);
    
    signal(SIGUSR1, sig_handler);
    
    while (1) {
        if (end_now == 1) {
            break;
        }
    }
    
    MPI_Finalize();
    
    return 0;
}
