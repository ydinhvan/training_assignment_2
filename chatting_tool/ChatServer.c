#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080
#define WORK_SIZE 1024

int room_id = 1;

char work_area[WORK_SIZE];
int server_fd, new_socket, valread;
int opt = 1;   
char buffer[1024] = { 0 };
pthread_t client_thread;
pthread_t client_threads[100];

// void* pthread_client(void* param)
// {
//     printf("inside thread \n");
//     valread = read(new_socket, buffer, 1024);
//     printf("Register room for process %s, room id %d\n", buffer, room_id);
// }


int main(int argc, char const* argv[])
{
    printf("Room %d is not exist. Creating a rooom...\n", room_id);

    //char* hello = "Hello from server";
    
    // Create a stream socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Initialise port number and address
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                SO_REUSEADDR | SO_REUSEPORT, &opt,
                sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Bind the socket to the
    // address and port number.
    if (bind(server_fd, (struct sockaddr*)&address,
            sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen on the socket,
    // with 10 max connection
    // requests queued
    if (listen(server_fd, 1) < 0) 
    {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    int i = 0;

    while(1)
    {

        // Extract the first
        // connection in the queue
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) 
        {
            perror("accept error");
            exit(EXIT_FAILURE);
        }


        
        // if (pthread_create(&client_threads[i++], NULL,
        //                     pthread_client, &new_socket)!= 0)
        //     {
        //         // Error in creating thread
        //         printf("Failed to create thread\n");
        //     }
                
        //read data from socket
        valread = read(new_socket, buffer, 1024);
        printf("Register room for process %s, room id %d\n", buffer, room_id);
        
        valread = read(new_socket, buffer, 1024);
        printf("Unregister room for process %s, room id %d\n", buffer, room_id);
        
        // send data to socket
        //send(new_socket, hello, strlen(hello), 0);
        //printf("Hello message sent\n");
        
        //printf("Register room for process %d, room id %d",pid,room_id);
        //fgets(work_area, WORK_SIZE, stdin);
    }
}