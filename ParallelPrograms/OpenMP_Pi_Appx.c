#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

void Get_Input(int, int, long long int*);

int main(){
    int comm_sz;
    int my_rank;
    double pi_estimate = 0;
    long long int number_of_tosses;
    long long int global_num_in_circle = 0;

    MPI_Init(NULL,NULL);
    time_t t_local;
    srand((unsigned) time(&t_local));

    //Initialize the size of comm messages
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    //Initialize the rank number
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Get_Input(comm_sz, my_rank, &number_of_tosses);

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
    MPI_Reduce(&number_in_circle,&global_num_in_circle,1,MPI_LONG_LONG_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if (my_rank == 0){
        pi_estimate = 4 * global_num_in_circle / ((double)number_of_tosses * comm_sz);
        printf("%.4f", pi_estimate);
    }

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