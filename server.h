//
// Created by Jason Dykstra on 9/17/20.
//

#ifndef NETWORKSPROJECT1_SERVER_H
#define NETWORKSPROJECT1_SERVER_H

// Include Headers
#include <stdio.h>      // For printf() and fprintf()
#include <sys/socket.h> // For socket(), bind(), and connect()
#include <arpa/inet.h>  // For sockaddr_in and inet_ntoa()
#include <stdlib.h>     // For atoi() and exit()
#include <string.h>     // For memset()
#include <unistd.h>     // For close()
#include <err.h>        // For stderr
#include <signal.h>     // For handling signals

// Definitions
#define MAXPENDING 5    // Maximum outstanding connection requests
#define MAXSIZE 4096    // Size for buffer

// Variables
int serverSocket;                   // Socket descriptor for server
int clientSocket;
struct sockaddr_in serverAddress;   // Server address// Socket descriptor for client
struct sockaddr_in clientAddress;   // Client address
unsigned short serverPort;          // Server port
unsigned int clientLen;             // Length of client address data structure
char httpHeader[MAXSIZE];           // Header for 200 OK message and content length
long fileSize;                      // Size of index.html

// Function Prototypes
void sigHandler(int);                    // Function for handling signals
void dieWithError(char *errorMessage);   // Error handling function
long calculateFileSize();                // Calculates file size of index.html
void handleHTTPClient(int clientSocket); // TCP client handling function

#endif //NETWORKSPROJECT1_SERVER_H
