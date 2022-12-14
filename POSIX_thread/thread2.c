#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);
char message[] = "Hello World";
int run_now = 1;

int main() 
{
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = pthread_create(&a_thread, NULL, thread_function, 0);
    if (res != 0) 
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    // two threads will now be running: priginal thread and a_thread
    printf("Waiting for thread to finish...\n");
    
    int print_count1 = 0;
    while(print_count1++ < 20) 
    {
    if (run_now == 1) 
    {
        printf("1");
        run_now = 2;
    }
    else {
        sleep(1);
    }
    }

    res = pthread_join(a_thread, &thread_result);
    if (res != 0) 
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined");
    //printf("Message is now %s\n", message);
    exit(EXIT_SUCCESS);
    pause();
}

void *thread_function(void *arg)
{
    //printf("thread_function is running. Argument was %s\n", (char *)arg);
    //sleep(3);
    // syntax: strcpy(dest, src)
    // copy "bye" to message
    //strcpy(message, "Bye!");

    int print_count2 = 0;
    while(print_count2++ < 20) 
    {
        if (run_now == 2) {
        printf("2");
        run_now = 1;
    }
    else {
        sleep(1);
    }
    }
    //pthread_exit("Thank you for the CPU time");
  
}