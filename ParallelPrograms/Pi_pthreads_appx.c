#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int thread_count;

long long int number_of_tosses;

void* thread_worker(void* rank);
void Get_Input(int, int, long long int*);

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    // Thread Creation
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread],NULL,thread_worker,(void*)thread);
    }

    long long int local_num = 0;
    for (thread = 0; thread < thread_count; thread++){
        pthread_join(&thread_handles[thread], local_num);
        global_num_in_circle += &local_num;
    }

    double pi_estimate = 0;
    long long int global_num_in_circle = 0;

        pi_estimate = 4 * global_num_in_circle / ((double)number_of_tosses * thread_count);
        printf("%.4f", pi_estimate);

    MPI_Finalize();
    return 0;
}

void Get_Input (int comm_sz, int my_rank, long long int* number_of_tosses){
    if (my_rank == 0){
        fflush( stdout );
        printf("Input the number of darts for each process to throw\n");
        scanf("%lld", number_of_tosses);
    }
    MPI_Bcast(number_of_tosses,1,MPI_LONG_LONG_INT,0,MPI_COMM_WORLD);
}

void* thread_worker(void* rank){
    time_t t_local;

    srand((unsigned) time(&t_local));
    double x_local;
    double y_local;

    long long int number_in_circle = 0;
    double distance_squared = 0;
    for (long long int toss = 0; toss < number_of_tosses; toss++){
        x_local = -1 + rand() % 2;
        y_local = -1 + rand() % 2;
        distance_squared = x_local * x_local + y_local * y_local;
        (distance_squared <= 1) ? number_in_circle++ : number_in_circle;
    }
    return *number_in_circle;
}