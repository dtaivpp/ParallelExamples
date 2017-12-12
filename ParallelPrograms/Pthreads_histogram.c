#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;

int Find_bin(float, float [], float, float);

void* thread_function(void* rank);

int data_count = 20;
float data[] = {1.3,2.9,0.4,0.3,1.3,4.4,1.7,0.4,3.2,0.3,4.9,2.4,3.1,4.4,3.9,0.4,4.2,4.5,4.9,0.9};
int bin_count = 5;
int bin_counts[5];
float bin_maxes[5];
float max_meas = 5.0, min_meas = 0.0, bin_width;

int main (int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    // Thread Creation
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread],NULL,thread_function,(void*)thread);
    }

    int* local_bin_counts = malloc(thread_count * 5 * sizeof(int));
    
    for (thread = 0; thread < thread_count; thread++){
        pthread_join(&thread_handles[thread], &local_bin_counts);
            printf("Thread Number: %ld \n",thread);
            for (int i = 0; i < bin_count; i++){
                printf("    Recieved: %d", local_bin_counts[i]);
                bin_counts[i] += local_bin_counts[i];
            }
    }

/*     for (int i = 0; i < bin_count; i++){
        printf(" %d\n",bin_counts[i]);
    } */
    
    free(thread_handles);
    return 0;
}

void* thread_function(void* rank){
    long my_rank = (long) rank;
    int bin_width= (max_meas - min_meas) / bin_count;
    int local_bin_counts[] = {0,0,0,0,0};
    
    for(int b = 0; b < bin_count; b++){
        bin_maxes[b] = min_meas + bin_width*(b+1);
        //printf(" %.1f\n", bin_maxes[b]);
    }

    int chunk_size = data_count / thread_count;
    int remainder_size = data_count % thread_count;
    int local_end, local_start;

    // Overlapping
    if (my_rank != 0){
        //Data subset if not process 0
        local_start = my_rank * chunk_size;
        local_end = local_start + chunk_size;
    }
    if (my_rank == 0){
        //Data subset for rank 0
        //Gets the remaining unassigned data points from remainder
        local_start = 0;
        local_end = chunk_size + remainder_size;
    }

    int bin = 0;
    //start at local count
    for (int i = local_start; i < local_end; i++){
        for(int j = 0; j < bin_count; j++){
            if (j != 0){
                //Test of the sorting algorithm
                //printf(" %.1f <= %.1f < %.1f \n", bin_maxes[i-1], data, bin_maxes[i]);
                if (bin_maxes[j-1] <= data[i] && data[i] < bin_maxes[j]){
                    local_bin_counts[j]++;
                }
            } else {
                if (min_meas <= data[i] && data[i] < bin_maxes[0]){
                    local_bin_counts[j]++;
                }
            }
        }
    }
    return local_bin_counts;
}

int Find_bin(float data,float bin_maxes[], float bin_count, float min_meas){
    for(int i = 0; i < bin_count; i++){
        if (i != 0){
            //Test of the sorting algorithm
            //printf(" %.1f <= %.1f < %.1f \n", bin_maxes[i-1], data, bin_maxes[i]);
            if (bin_maxes[i-1] <= data && data < bin_maxes[i]){
                return i;
            }
        } else {
            if (min_meas <= data && data < bin_maxes[0]){
                return i;
            }
        }
    }
}

