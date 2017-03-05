// Made by Stephen Tue 7th Feb 2017

// Caculating pi using Nilakantha series
// pi = 3 + 4/(2*3*4) - 4/(4*5*6) + 4/(5*6*7)
// creating set number of threads and number of caculations
// if threads = 5 and cals = 1000000 thread 1: 1 -> 1000000/5, thread: 1000000/5 -> 2*1000000/5 ect


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define NUMBER_THREADS 5
#define CACULATIONS 100

typedef struct data
{
    int thread_number;
    long double *arrayElement;
}thread_data;



void *cal(void *pt)
{
    // seting vals
    int thread_number;
    thread_data *my_data;
    
    my_data = pt;
    thread_number = my_data->thread_number;
    
    // init vals
    long double count = (thread_number*(CACULATIONS/NUMBER_THREADS))+2; // count -> caculate denominator
    long double result = 0;
    long double fraction;
    int j;
    
    for(j = 0; j < (CACULATIONS/(NUMBER_THREADS*2)); j++)
    {
        fraction = 4/(count*(count+1)*(count+2));
        if(j % 2 == 0)                                          // if even
        {
            result += fraction;
        }
        else                                                    // if odd
        {
            result -= fraction;
        }
        count = count + 2;
    }
    
    *my_data->arrayElement = result;                           // store result in arrayElement
    
    return pt;
}



int main (){
    
    
    
    long double Array[NUMBER_THREADS];   // creating array to store results of each thread
    pthread_t threads[NUMBER_THREADS];   // creating array of pthreads
    thread_data data[NUMBER_THREADS];
    
    int rc, t;
    long double pi = 3;
    
    
    for(t = 0; t < NUMBER_THREADS; t++)
    {
        data[t].thread_number = t;
        data[t].arrayElement = &Array[t];
        
        
        rc = pthread_create(&threads[t], NULL, cal, (void *) &data[t]);
        if (rc) {
            printf("ERROR return code from pthread_create(): %d\n",rc);
            exit(-1);
        }
        
    }
    
    for(t = 0; t < NUMBER_THREADS; t++)  // threads exit
    {
        pthread_join(threads[t], NULL);
    }
    
    for(int i = 0; i < NUMBER_THREADS; i++)
    {
        pi = pi + Array[i];
    }
    
    printf("Pi = %.20Lf\n", pi);
    
}
