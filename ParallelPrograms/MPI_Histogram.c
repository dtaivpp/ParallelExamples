//Parallel Histogram using OpenMP
// Written by David Tippett
// 12/4/1207
// CSCI 320

#include <stdio.h>
#include <mpi.h>

int Find_bin(float, float [], float, float);

int main (){
    int comm_sz;
    int my_rank;
    int data_count = 20;
    float data[] = {1.3,2.9,0.4,0.3,1.3,4.4,1.7,0.4,3.2,0.3,4.9,2.4,3.1,4.4,3.9,0.4,4.2,4.5,4.9,0.9};
    int bin_count = 5;
    int bin_counts[bin_count];
    float bin_maxes[bin_count];
    float max_meas = 5.0, min_meas = 0.0, bin_width;

    bin_width= (max_meas - min_meas) / bin_count;

    for(int b = 0; b < bin_count; b++){
        bin_maxes[b] = min_meas + bin_width*(b+1);
        //printf(" %.1f\n", bin_maxes[b]);
    }

    //No MPI Calls before here
    MPI_Init(NULL,NULL);

    //Initialize the size of comm messages
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    //Initialize the rank number
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Define size of chunks
    // Not the perfect solution as the 0 process gets all of the 
    //   remaining data that was not allocated which just increases
    //   its already heavy workload.
    int chunk_size = data_count / comm_sz;
    int remainder_size = data_count % comm_sz;
    int local_end, local_start;

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

        //Sends bin counts to process 0
        MPI_Send(local_bin_counts, bin_count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    
    } else {
    //Process 0
        //Processes its subset and saves to main bin_counts
        int bin = 0;
        for (int i = local_start; i < local_end; i++){
            bin = Find_bin(data[i], bin_maxes, bin_count, min_meas);
            bin_counts[bin]++;
        }
        
        //Recieves count from other process
        for (int q=1; q < comm_sz; q++){
            int local_bin_counts[] = {0,0,0,0,0};
            MPI_Recv(local_bin_counts, bin_count, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            //Adds the other local counts to bin counts
            for (int i = 0; i < bin_count; i++){
                bin_counts[i] += local_bin_counts[i];
            }
        }

        //Prints results from process 0
        for (int i = 0; i < bin_count; i++){
            printf(" %d\n",bin_counts[i]);
        }
    }

    //No MPI Calls after here
    MPI_Finalize();
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