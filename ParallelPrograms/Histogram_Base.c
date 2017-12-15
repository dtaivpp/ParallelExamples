//Parallel Histogram using OpenMP
// Written by David Tippett
// 12/4/1207
// CSCI 320

#include <stdio.h>
#include <mpi.h>

int Find_bin(float, float [], float, float);

int main (){

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

    int bin = 0;
    
    for (int i = 0; i < data_count; i++){
        bin = Find_bin(data[i], bin_maxes, bin_count, min_meas);
        bin_counts[bin]++;
        //printf(" %d\n", bin);
    }

    for (int i = 0; i < bin_count; i++){
        printf(" %d\n",bin_counts[i]);
    }
    
    return 0;
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