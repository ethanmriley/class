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

        for(unsigned int j = 2; j <= (int) ceil(sqrt(i)); j++) {
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
    unsigned int i = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    for(i = 3; i < 100; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        printf("\n");

        int sqrt_i = floor(sqrt(i));
        int range = (sqrt_i - 1);
        int chunk_size = ceil(range/ (float)count);
        int start = 2 + (chunk_size * (id));
        int end;

        if(id == 3) {
            end = sqrt_i + 1;
        } else {
            end = start + chunk_size;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        printf("rank: %d i: %d start: %d end: %d sqrt_i: %d chunk_size: %d\n", id, i, start, end, sqrt_i, chunk_size);
        
    }

    signal(SIGUSR1, sig_handler);

    //i need to wait until all processes are done until i move to the next candidate
    //each process should check the numbers between (int) ceil(i/count) * id and (int) ceil(i/couint) * (id + 1)
    
    //while (1) {


    //    if (end_now == 1) break;
    //}
    
    MPI_Finalize();
    
    return 0;
}
