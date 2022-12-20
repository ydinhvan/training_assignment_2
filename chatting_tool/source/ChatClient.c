
/*************************************************
 * Author: Dinh Van Y
 * 
 * Date Complete: 16 December 2022
 * 
 * Description: A program with server and multi clients.
 * First, server is created, after clients connect to server.
 * When client connect/disconnect to server, notify on serser.
 * Client send message to server, server receive it and
 * send message to all client in room.
 * 
*************************************************/
// Include statements
#include "include.h"

// Global variables
char prc_id_mes [100];

// Function prototypes
void run_client(fd_set* cli_fds, fd_set* main_fd, int* max_fd, int* sock_fd, char* name);
void init_connection(char* ip_addr, fd_set* main_fd, fd_set* cli_fds, int* sock_fd, SA_in* serv_addr);
void send_msg(char* name, int sock_fd);
void recv_msg(int sock_fd);
void sigintHandler(int sig_num);
// int getpid();
// int close();
// int write();
// int read();

/**
 * Runs the program.
 **/
int main(int argc, char** argv)
{
	// Local variables
	int sock_fd, max_fd, pid;   //fd for this client's connection and max fd value
	fd_set main_fd, cli_fds;    //fd sets for main server and clients
	SA_in serv_addr;            //address struct for server
	char* name = malloc(sizeof(char) * 50) ;    //username
	char* room_id = malloc(sizeof(char) * 50);  //room id
	
	printf("Welcome to chatting system!\n");
    printf("Please enter your name: ");
    fgets(name, MAXCHARS, stdin);
    
    // skip newline "\n"
	if ((strlen(name) > 0) && (name[strlen (name) - 1] == '\n'))
    {
        name[strlen (name) - 1] = '\0';
    }

    printf("Please enter room id: ");
    fgets(room_id, MAXCHARS, stdin);
    //scanf("%m[^\n]%*c",room_id);
	if ((strlen(room_id) > 0) && (room_id[strlen (room_id) - 1] == '\n'))
    {
        room_id[strlen (room_id) - 1] = '\0';
    }
    //printf("%s_:%s_\n", name, room_id);
	
    // get process_id
    pid = getpid();

    printf("Process ID: %d\n",pid);
	
    char mess[100] = "Register for process: ";
	sprintf(prc_id_mes,"%d", pid);
	strcat(mess, prc_id_mes);
	//printf("==============Process ID: %s\n",prc_id_mes);
    
	// Initialize connection to server
	init_connection("127.0.0.1", &main_fd, &cli_fds, &sock_fd, &serv_addr);
	
	// Output successful connection	
	//printf("Successfully connected to %s.\n", "127.0.0.1");
	printf("Typing at any time to send message to other clients.\n");
	
	int val_send = send(sock_fd, prc_id_mes, strlen(prc_id_mes), 0);
	if (send < 0)
	{
		printf("send fail\n");
	}
	// Begin client interface
	run_client(&cli_fds, &main_fd, &max_fd, &sock_fd, name);

	// Catch signal
	signal(SIGINT, sigintHandler);

	// Close the connection
	close(sock_fd);
	return 0;
} //end main

// Signal Handler for SIGINT 
void sigintHandler(int sig_num)
{
	exit(0);
}

/**
 * Begin running an infinite loop to process client activity, both from the
 * user and other clients. Sends a message if it's the user, and otherwise
 * receives a message if activity is from another client.
 **/
void run_client(fd_set* cli_fds, fd_set* main_fd, int* max_fd, int* sock_fd, char* name)
{
	*max_fd = *sock_fd;
	
	// Infinite loop to send and receive data
	int i; //loop control
	for (;;)
	{
		// Wait for activity from either you or another client
		*cli_fds = *main_fd;
		select(*max_fd+1, cli_fds, NULL, NULL, NULL);

		// Iterate through clients and process activity
		for (i = 0; i < *max_fd + 1; ++i)
			if (FD_ISSET(i, cli_fds)) //if the client exists...
				if (i == 0) //...send if it's you...
					send_msg(name, *sock_fd);
				else //...or receive from someone else
					recv_msg(*sock_fd);
	} //end for
} //end run_client

/**
 * Creates a TCP socket and initiates a connection with the server.
 **/
void init_connection(char* ip_addr, fd_set* main_fd, fd_set* cli_fds, int* sock_fd, SA_in* serv_addr)
{
	// Establish socket
	*sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	// Initialize server address structure for connection
	bzero(serv_addr, sizeof(SA_in)); 		// Set sizeof(SA_in) bytes of serv_addr to 0
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(DEFAULT_SERVER_PORT);
	
	// Convert IPv4 addresses from text to binary form
	inet_pton(AF_INET, ip_addr, &(serv_addr->sin_addr));
	//printf("==============ip address: %s\n", ip_addr);
	//printf("==============%d\n", serv_addr->sin_addr);
	
    // Connect to the server
	connect(*sock_fd, (SA*)serv_addr, sizeof(SA));
	//write(sock_fd, prc_id_mes, strlen(prc_id_mes));

	// Clear fds before being added to the main fd_set
	FD_ZERO(main_fd);
    FD_SET(0, main_fd);
	FD_ZERO(cli_fds);
	FD_SET(*sock_fd, main_fd);	
} //end init_connection

/**
 * Send a message through the socket to the server (and the other
 * clients).
 **/
void send_msg(char* name, int sock_fd)
{
	char msg_buf[MAXCHARS];

	// Get input from user
	fgets(msg_buf, MAXCHARS, stdin);

	// See if user wants to quit (check the commands)...
	if (strcmp(msg_buf, "/quit\n") == 0)
	{
		exit(0);
	} //end if
	else // ...or send a message!
	{
		char full_msg[MAXCHARS] = {'0'};
		snprintf(full_msg, sizeof(msg_buf),
			 "%s: %s", name, msg_buf);
		write(sock_fd, full_msg, strlen(full_msg));
	} //end else
} //end send_msg

/**
 * Receive a message from other clients through the socket.
 **/
void recv_msg(int sock_fd)
{
	// Read message from socket
	char msg_buf[MAXCHARS];
	int n = read(sock_fd, msg_buf, MAXCHARS); //num bytes received
	msg_buf[n - 1] = '\0'; //prevent extra bytes from going through

	// Print the message
	printf("%s\n", msg_buf);
} //end recv_msg


// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <string.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <signal.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>

// #define PORT 8080

// #define WORK_SIZE 1024
// char work_area[WORK_SIZE];
// int sock = 0, valread, client_fd;
// char prc_id_mes [10];
// int status = 1;

// void handle_sigint(int sig)
// {
//     printf("Catch Ctrl + C\n");
//     printf("===================%s\n", prc_id_mes);
//     printf("===================%d\n",client_fd);
//     send(sock, prc_id_mes, strlen(prc_id_mes), 0);
//     status = 0;
// }


// int main(int argc, char const* argv[])
// {
//     int pid;

//     signal(SIGINT, handle_sigint);

//     // printf("Welcome to chatting system!\n");
//     // printf("Please enter your name: ");
//     // fgets(work_area, WORK_SIZE, stdin);
//     // printf("Please enter room id : ");
//     // fgets(work_area, WORK_SIZE, stdin);
//     // get process_id
//     pid =getpid();
//     printf("Process ID: %d\n",pid);
//     //printf("you : you :");
    
//     // message send to client
    
//     sprintf(prc_id_mes, "%d", pid);
//     printf("=================%d\n",pid);

//     //char* hello = "Hello from client";
//     char buffer[1024] = { 0 };
    
//     //creat a stream socket, IPV6
//     sock = socket(AF_INET, SOCK_STREAM, 0);
//     if (socket < 0) 
//     {
//         printf("\n Socket creation error \n");
//         return -1;
//     }
//     // Initialise port number and address
//     struct sockaddr_in serv_addr;
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // Convert IPv4 and IPv6 addresses from text to binary form
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
//     {
//         printf("\nInvalid address/ Address not supported \n");
//         return -1;
//     }

//     // Initiate a socket connection
//     client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//     // Check for connection error
//     if (client_fd < 0)
//     {
//         printf("\nConnection Failed \n");
//         return -1;
//     }
//     printf("Connection established\n");

//     // Send data to the socket
//     send(sock, prc_id_mes, strlen(prc_id_mes), 0);
//     //send(sock, "DinhVanY", 8, 0);
//     //printf("Hello message sent\n");

//     //valread = read(sock, buffer, 1024);
//     //printf("%s\n", buffer);
//     while (status == 1)
//     {
//     }
// }