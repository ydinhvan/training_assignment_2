#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int sock = 0, valread, client_fd;
char prc_id_mes [10];
int status = 1;

void handle_sigint(int sig)
{
    printf("Catch Ctrl + C\n");
    printf("===================%s\n", prc_id_mes);
    printf("===================%d\n",client_fd);
    send(sock, prc_id_mes, strlen(prc_id_mes), 0);
    status = 0;
}


int main(int argc, char const* argv[])
{
    int pid;

    signal(SIGINT, handle_sigint);

    // printf("Welcome to chatting system!\n");
    // printf("Please enter your name: ");
    // fgets(work_area, WORK_SIZE, stdin);
    // printf("Please enter room id : ");
    // fgets(work_area, WORK_SIZE, stdin);
    // get process_id
    pid =getpid();
    printf("Process ID: %d\n",pid);
    //printf("you : you :");
    
    // message send to client
    
    sprintf(prc_id_mes, "%d", pid);
    printf("=================%d\n",pid);

    //char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    
    //creat a stream socket, IPV6
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) 
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    // Initialise port number and address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Initiate a socket connection
    client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    // Check for connection error
    if (client_fd < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Connection established\n");

    // Send data to the socket
    send(sock, prc_id_mes, strlen(prc_id_mes), 0);
    //send(sock, "DinhVanY", 8, 0);
    //printf("Hello message sent\n");

    //valread = read(sock, buffer, 1024);
    //printf("%s\n", buffer);
    while (status == 1)
    {
    }
}