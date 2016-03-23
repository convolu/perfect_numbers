//
//  main.cpp
//  perfect_numbers
//
//  Created by Constantinos Voloudakis on 25/11/2013.
//  Copyright (c) 2013 Constantinos Voloudakis. All rights reserved.
//

#define NUM_OF_THREADS 1

#include <iostream>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

typedef unsigned long int no_t;

typedef struct {
    no_t start;
    no_t stop;
} thread_args_t;

int is_perfect(no_t no_to_be_checked);
void * run_on_length(void * input_tmp);

pthread_mutex_t run_mutex = PTHREAD_MUTEX_INITIALIZER;
int running_threads = 0;


int main(int argc, const char * argv[])
{
    no_t arg_value = 0;
    pthread_t threads[NUM_OF_THREADS];
    thread_args_t args[NUM_OF_THREADS];
    int rc;
    no_t prev=1ul;
    time_t time_1, time_2;
    
    if (argc!=2) {
        std::cout<< "Not enough parameters\n";
        return 1;
    }
    
    arg_value = atoi(argv[1]);
    
    std::cout<<"Argument value: "<< arg_value << "\n";
    std::cout<<"Size of value: "<<sizeof(arg_value)<<" bytes\n";
    
    std::time(&time_1);
    
    for (int i = 0; i< NUM_OF_THREADS; i++) {
        args[i].start = prev;
        args[i].stop = (i+1)*arg_value/NUM_OF_THREADS;
        prev = args[i].stop+1;
        
        pthread_mutex_lock(&run_mutex);
        running_threads++;
        pthread_mutex_unlock(&run_mutex);
        
        rc = pthread_create(&threads[i], NULL, run_on_length, (void *)&(args[i]));
        if (rc){
            std::cout<<"Thread error\n";
            exit(EXIT_FAILURE);
        }
    }
    while(running_threads>0)
    {
        sleep( (useconds_t) 1);
    }
    
    std::time(&time_2);
    std::cout<< "Time taken: " <<std::difftime(time_2, time_1) << " secs \n";
    pthread_exit(NULL);
}

int is_perfect(no_t no_to_be_checked)
{
    int success = 0; //fail by default
    no_t i;
    no_t acc=0;
    
    for (i=1; i<no_to_be_checked; i++) {
        if (no_to_be_checked % i==0) {
            acc+= i;
        }
    }
    if (acc==no_to_be_checked) {
        std::cout<<no_to_be_checked<<" is a perfect number\n";
    }
    
    success = 1;
    return success;
}

void * run_on_length(void * input_tmp)
{
    thread_args_t * input;
    no_t i;
    
    input = (thread_args_t *) input_tmp;
    //std::cout<<"Start: "<<input->start<<" End: "<<input->stop<<"\n";
    
    for (i=input->start; i<=input->stop; i++) {
        is_perfect(i);
    }
    
    pthread_mutex_lock(&run_mutex);
    running_threads--;
    pthread_mutex_unlock(&run_mutex);
    
    pthread_exit(NULL);
}