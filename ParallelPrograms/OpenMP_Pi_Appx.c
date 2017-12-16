#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

void Get_Input();
long long int number_of_tosses = 0;

int main(int argc, char* argv[]){
    int thread_count;
    double pi_estimate = 0;
    long long int global_num_in_circle = 0;

    thread_count = strtol(argv[1], NULL, 10);

    Get_Input();

# pragma omp parallel num_threads(thread_count)
{
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

# pragma omp critical
    global_num_in_circle += number_in_circle;
}

    pi_estimate = 4 * (double)global_num_in_circle / ((double)(number_of_tosses * thread_count));
    printf("%.4f", pi_estimate);

    return 0;
}

void Get_Input (){
        fflush( stdout );
        printf("Input the number of darts for each process to throw\n");
        scanf("%lld", &number_of_tosses);
}