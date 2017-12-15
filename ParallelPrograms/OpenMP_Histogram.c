#include <stdio.h>
#include <omp.h>

int Find_bin(float, float [], float, float);

int main (int argc, char* argv[]){
    int data_count = 20;
    float data[] = {1.3,2.9,0.4,0.3,1.3,4.4,1.7,0.4,3.2,0.3,4.9,2.4,3.1,4.4,3.9,0.4,4.2,4.5,4.9,0.9};
    int bin_count = 5;
    int bin_counts[bin_count];
    float bin_maxes[bin_count];
    float max_meas = 5.0, min_meas = 0.0, bin_width;
    int thread_count;

    thread_count = strtol(argv[1], NULL, 10);

    bin_width= (max_meas - min_meas) / bin_count;

    for(int b = 0; b < bin_count; b++){
        bin_maxes[b] = min_meas + bin_width*(b+1);
        //printf(" %.1f\n", bin_maxes[b]);
    }

# pragma omp parallel num_threads(thread_count)
{
    int my_rank = omp_get_thread_num();
    int chunk_size = data_count / thread_count;
    int remainder_size = data_count % thread_count;
    int local_end, local_start;

    if (my_rank != thread_count - 1){
        // If I am not the last process
        local_start = my_rank * chunk_size;
        local_end = local_start + chunk_size;
    }
    if (my_rank == thread_count - 1){
        //If I am the last process
        //Gets the remaining unassigned data points from remainder
        local_start = my_rank * chunk_size;
        local_end = local_start + chunk_size + remainder_size;
    }

    if (my_rank != 0){
    //Process N
        //Creates local bins
        int local_bin_counts[]={0,0,0,0,0};

        //Processes its subset of data
        int bin = 0;
        for (int i = local_start; i < local_end; i++){
            bin = Find_bin(data[i], bin_maxes, bin_count, min_meas);
            local_bin_counts[bin]++;
        }
#       pragma omp critical 
        for (int i = 0; i < bin_count; i++){
            bin_counts[i] += local_bin_counts[i];
        }
    
    } else {
    //Process 0
        //Processes its subset and saves to main bin_counts
        int bin = 0;
        int local_bin_counts[]={0,0,0,0,0};
        for (int i = local_start; i < local_end; i++){
            bin = Find_bin(data[i], bin_maxes, bin_count, min_meas);
            local_bin_counts[bin]++;
        }
        
#       pragma omp critical 
        for (int i = 0; i < bin_count; i++){
            bin_counts[i] += local_bin_counts[i];
        }
    }
}

    for (int i = 0; i < bin_count; i++){
        printf(" %d\n",bin_counts[i]);
    }


    return 0;
}

//Checks to find the bin a number belongs in
int Find_bin(float data,float bin_maxes[], float bin_count, float min_meas){
    for(int i = 0; i < bin_count; i++){
        if (i != 0){
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

