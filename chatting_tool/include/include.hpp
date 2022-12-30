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
#include <signal.h>  /*signal handler*/
#include <unistd.h>  /*implicit declaration of function*/   

// Constant value definitions
#define DEFAULT_SERVER_PORT 9877
#define LISTENQ 10 //max # of pending connections
#define MAXCHARS 1024 //max # bytes sent between server & client

// Type redefinitions (for more clarity)
typedef struct sockaddr_in SA_in;
typedef struct sockaddr SA;

