
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
#include <arpa/inet.h> /* htons, htonl */
#include <netinet/in.h> /* various functions and constants */
#include <stdio.h> /* stdin, stdout, printf */
#include <stdlib.h> /* exit */
#include <string.h> /* bzero */
#include <time.h> /* time functions for server logging */

// Constant value definitions
#define DEFAULT_SERVER_PORT 9877
#define LISTENQ 10 //max # of pending connections
#define MAXCHARS 1024 //max # bytes sent between server & client

// Type redefinitions (for more clarity)
typedef struct sockaddr_in SA_in;
typedef struct sockaddr SA;

// Global variables
int room_id = 1;
char prc_id_mes[MAXCHARS];

// Function prototypes
void run_server(int* serv_fd, fd_set* cli_fds, fd_set* main_fd, int* max_fd, SA_in* cli_addr);
void init_server(fd_set* main_fd, fd_set* cli_fds, int* serv_fd, SA_in* serv_addr);
void accept_client(int id, fd_set* main_fd, int* max_fd, int serv_fd, SA_in* cli_addr);
void process_client(int id, fd_set* main_fd, int serv_fd, int max_fd);
void log_activity(char* msg);

/**
 * Runs the program.
 **/
int main(int argc, char** argv)
{
	// Local variables
	fd_set main_fd, cli_fds; //fd sets for main server and clients
	int serv_fd = 0, max_fd; //fd for server and max fd for select
	SA_in serv_addr, cli_addr; //address structs for server and clients
	
	// Do all pre-connection establishment for server properties
	init_server(&main_fd, &cli_fds, &serv_fd, &serv_addr);

	// Until a higher fd connects, server is highest level fd
	max_fd = serv_fd;
	printf("Room 1 is not exist. Creating a room...\n");

	// Begin server processing
	run_server(&serv_fd, &cli_fds, &main_fd, &max_fd, &cli_addr);

	return 0;
} //end main

/**
 * Begin running an infinite loop to process client connections as well
 * as client input. Runs through a loop when activity occurs, checks which
 * kind of activity was detected, and performs appropriate actions based
 * on the activity.
 **/
void run_server(int* serv_fd, fd_set* cli_fds, fd_set* main_fd, int* max_fd, SA_in* cli_addr)
{
	// Infinite loop to process clients
	int i; //loop control
	for (;;)
	{
		*cli_fds = *main_fd;
		
		// When client activity occurs, store that client's fd
		// in cli_fds for processing
		select(*max_fd+1, cli_fds, NULL, NULL, NULL);
		
		// Cycle through clients and see which one had activity
		for (i = 0; i < *max_fd + 1; i++)
		{
			// If the current client is the active one
			if (FD_ISSET(i, cli_fds))
			{
				// Check if it is a new client connecting...
				if (i == *serv_fd)
					accept_client(i, main_fd, max_fd, *serv_fd, cli_addr);
				else //...or an existing client sending a msg
					process_client(i, main_fd, *serv_fd, *max_fd);
			} //end if
		} //end for
	} //end infinite for	
} //end process_clients

/**
 * Ensure the server is properly set up before connections can be made.
 * Creates the socket, ensures the socket can handle multiple connections,
 * binds the socket and server address, and allows the server to listen.
 **/
void init_server(fd_set* main_fd, fd_set* cli_fds, int* serv_fd, SA_in* serv_addr)
{
	// Local variables
	int reuse = 1; //TCP address reusability option

	// Clear fds and prepare to accept a connection
	FD_ZERO(main_fd);
	FD_ZERO(cli_fds);
		
	// Initialize server address structure attributes
	bzero(serv_addr, sizeof(SA_in)); //also sets unused bytes to 0
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(DEFAULT_SERVER_PORT);
	serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);

	// Attempt to create TCP socket
	*serv_fd = socket(AF_INET, SOCK_STREAM, 0);		
		
	// Allow address reuse so multiple clients can connect
	setsockopt(*serv_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	// Bind socket and server address
	bind(*serv_fd, (SA *)serv_addr, sizeof(SA));

	// Listen for up to LISTENQ clients
	listen(*serv_fd, LISTENQ);

	// Store server fd in main fd set
	FD_SET(*serv_fd, main_fd);

	// Print out message for anyone viewing server; this not logged	
	//printf("Terminate server at any time with CTRL + c.\n");
	//printf("Current port: %d\n", DEFAULT_SERVER_PORT);
	//printf("Waiting for clients to connect...\n");
} //end init_server		

/**
 * If an incoming connection is requested, get the client's information
 * and add them to the main fd_set, allowing them to communicate through
 * the server.
 **/
void accept_client(int id, fd_set* main_fd, int* max_fd, int serv_fd, SA_in* cli_addr)
{
	char msg_buf[MAXCHARS];
	// Accept a new connection
	socklen_t addrlen = sizeof(SA_in);
	int new_fd = accept(serv_fd, (SA*)cli_addr, &addrlen);
	//printf("==============cli_addr: %d\n",cli_addr->sin_addr.s_addr);

	char prc_id_mes[MAXCHARS];
	int valread;
	valread = read(new_fd, prc_id_mes, MAXCHARS);
	//prc_id = strcat(prc_id,prc_id_mes);
	//printf("==============process id: %s\n",prc_id_mes);
	printf("Register room for process %s, room id %d\n", prc_id_mes, room_id);
	// Make sure client connected properly
	if(new_fd < 0)
	{
		printf("Error accepting client\n");
		exit(1);
	} //end if
	else
	{
		// Add new client to fd set
		FD_SET(new_fd, main_fd);
		
		// If new client fd is highest, record it for select
		if(new_fd > *max_fd)
			*max_fd = new_fd;

		// Log client connection to server
		//char activity[75];
		//printf("==============port: %d\n",cli_addr->sin_port);
		//printf("==============convert port: %d\n",ntohs(cli_addr->sin_port));
		//printf("==============convert port: %d\n",htons(ntohs(cli_addr->sin_port)));
		
		//log_activity(prc_id_mes);
		
		// snprintf(activity, sizeof(activity),
		// 	"Client connected from %s on port %d\n",
		// 	inet_ntoa(cli_addr->sin_addr), ntohs(cli_addr->sin_port));
		// log_activity(activity);
		//printf("%s\n", activity);
		
		//printf ("Register room for process %s, room id 1 \n", prc_id_mes);
		// snprintf(activity, sizeof(activity),
		// 	"Register room for process %s, room id %d\n",
		// 	prc_id_mes, room_id);
		// printf("%s\n", activity);
		// log_activity(activity);
	} //end else
} //end accept_client

/**
 * If an existing client performs any action, check the action. If
 * they disconnected, remove them from the fd_set; otherwise, if they
 * sent a message, read the message and relay it to all other clients.
 **/			
void process_client(int id, fd_set* main_fd, int serv_fd, int max_fd)
{
	// Local variables
	int n; //number of bytes received
	int j; //used for loops
	char msg_buf[MAXCHARS];
	
	// If no message received, the client disconnected
	if ((n = read(id, msg_buf, MAXCHARS)) < 1)
	{
		// Log client disconnection from server
		char activity[24];
		snprintf(activity, sizeof(activity),
			 "Socket %-2d disconnected\n", id);
		log_activity(activity);

		// Close the connection with ex-client
		close(id);
		FD_CLR(id, main_fd);
	} //end if
	else // Otherwise, a message was received
	{ 
		int logged = 0; //track if logged already; default to false

		// Send the message to all clients except the one who sent it
		for(j = 0; j < max_fd + 1; j++)
		{
			// Output to other clients
			if (FD_ISSET(j, main_fd)) //ensure client exists
				if (j != serv_fd && j != id) //ignore server and sender
					write(j, msg_buf, n);

			// Log messages sent by clients only once; placing
			// this code here ensures that even if only 1 client
			// is connected, sent messages are still recorded
			if (logged == 0)
			{
				// Log messages sent by clients
				msg_buf[n] = '\0'; //for logging
				log_activity(msg_buf);
				logged = 1;
			} //end if
		} //end for
	} //end else	
} //end process_client

/**
 * Logs a given message to the server log, as well as the
 * time the message occurred. Also prints to the server
 * console. If the log file doesn't already exist, it will
 * be created.
 **/
void log_activity(char* msg)
{	
	// Local variables
	FILE* log_file = fopen("serverlog.txt", "a"); //serverlog file
	time_t raw_time; //raw time data
	struct tm* time_info; //structured time data

	// Format the time and store as a char array
	time(&raw_time);
	time_info = localtime(&raw_time);
	char* time_str = asctime(time_info); //get formatted time
	time_str[strlen(time_str)-1] = '\0';

	// Print to the server console
	printf("[%s]%s", time_str, msg);
	// Log into file
	fprintf(log_file, "[%s]%s", time_str, msg);

	// Close the connection	
	fclose(log_file);
} //end log


// int room_id = 1;

// char work_area[WORK_SIZE];
// int server_fd, new_socket, valread;
// int opt = 1;   
// char buffer[1024] = { 0 };
// pthread_t client_thread;
// pthread_t client_threads[100];

// // void* pthread_client(void* param)
// // {
// //     printf("inside thread \n");
// //     valread = read(new_socket, buffer, 1024);
// //     printf("Register room for process %s, room id %d\n", buffer, room_id);
// // }


// int main(int argc, char const* argv[])
// {
//     printf("Room %d is not exist. Creating a rooom...\n", room_id);

//     //char* hello = "Hello from server";
    
//     // Create a stream socket
//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd < 0) 
//     {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }
    
//     // Initialise port number and address
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
    
//     // Forcefully attaching socket to the port 8080
//     if (setsockopt(server_fd, SOL_SOCKET,
//                 SO_REUSEADDR | SO_REUSEPORT, &opt,
//                 sizeof(opt))) {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }
    
//     // Bind the socket to the
//     // address and port number.
//     if (bind(server_fd, (struct sockaddr*)&address,
//             sizeof(address)) < 0) 
//     {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // Listen on the socket,
//     // with 10 max connection
//     // requests queued
//     if (listen(server_fd, 1) < 0) 
//     {
//         perror("listen error");
//         exit(EXIT_FAILURE);
//     }
//     int i = 0;

//     while(1)
//     {

//         // Extract the first
//         // connection in the queue
//         new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//         if (new_socket < 0) 
//         {
//             perror("accept error");
//             exit(EXIT_FAILURE);
//         }


        
//         // if (pthread_create(&client_threads[i++], NULL,
//         //                     pthread_client, &new_socket)!= 0)
//         //     {
//         //         // Error in creating thread
//         //         printf("Failed to create thread\n");
//         //     }
                
//         //read data from socket
//         valread = read(new_socket, buffer, 1024);
//         printf("Register room for process %s, room id %d\n", buffer, room_id);
        
//         valread = read(new_socket, buffer, 1024);
//         printf("Unregister room for process %s, room id %d\n", buffer, room_id);
        
//         // send data to socket
//         //send(new_socket, hello, strlen(hello), 0);
//         //printf("Hello message sent\n");
        
//         //printf("Register room for process %d, room id %d",pid,room_id);
//         //fgets(work_area, WORK_SIZE, stdin);
//     }
// }