#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>

int end_now = 0;

void sig_handler(int signo)
{
    if (signo == SIGUSR1) {
        end_now = 1;
    }
}

int main(int argc, char **argv)
{
    int count, id;
    unsigned int i = 3;
    int prime_count = 1;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

   //     printf("rank: %d i: %d start: %d end: %d sqrt_i: %d chunk_size: %d\n", id, i, start, end, sqrt_i, chunk_size);
        
    signal(SIGUSR1, sig_handler);

    //i need to wait until all processes are done until i move to the next candidate
    
    while (i < 100) {
        int sqrt_i = floor(sqrt(i));
        int range = (sqrt_i - 1);
        int chunk_size = ceil(range/ (float)count);
        int start = 2 + (chunk_size * (id));
        int end;
        int prime = 1;


        if(id == 3) {
            end = sqrt_i + 1;
        } else {
            end = start + chunk_size;
        }

        for(unsigned int j = start; j < end; j++) {
            if(i % j == 0) {
                prime = 0;
                break;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if(id != 0) {
            MPI_Send(&prime, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else {
            int all_prime;
            int results[3];

            if(prime) {
                all_prime = 1;
            } else {
                all_prime = 0;
            }

            for(unsigned int j = 1; j < count; j++) {
                MPI_Recv(&results[j], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if(results[j] != 1) {
                    all_prime = 0;
                }
            }

            if(all_prime) {
                prime_count++;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        i++;

        if (end_now == 1) break;
    }
    
    if(id == 0) {
        printf("%d\n", prime_count);
    }
    MPI_Finalize();

    
    return 0;
}
