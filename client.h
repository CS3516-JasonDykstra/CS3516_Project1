//
// Created by Jason Dykstra on 9/17/20.
//

#ifndef NETWORKSPROJECT1_CLIENT_H
#define NETWORKSPROJECT1_CLIENT_H

// Include Headers
#include <stdio.h>          // For printf() and fprintf()
#include <sys/socket.h>     // For socket(), connect(), send(), and recv()
#include <arpa/inet.h>      // For sockaddr_in and inet_addr()
#include <stdlib.h>         // For atoi() and exit()
#include <string.h>         // For memset()
#include <unistd.h>         // For close()
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>       // For RTT calculation

// Definitions
#define DEFAULT_PORT 80     // Port 7 is well known for echo, port 80 is well known for http
#define MAXSIZE 4096        // Maximum byte size

// Variables
int showRTT;                         // 1 if show rtt using -p flag
int clientSocket;                    // Socket descriptor
struct sockaddr_in serverAddress;    // Echo server address
unsigned short serverPort;           // Echo server port
char *hostname;                      // Hostname of server
char serverIP[100];                  // String to store server ip
char response[MAXSIZE];              // String to store response from server
struct timeval startTime;            // Start time holder for calculating RTT
struct timeval endTime;              // End time holder for calculating RTT
unsigned long timeInMicroseconds;    // RTT in microseconds
double timeInMilliseconds;           // RTT in milliseconds
long totalExpectedBytes;             // Total expected bytes from server
long totalBytesReceived;             // Counter for bytes received from server
long bytesReceived;                  // Temporary variable to store bytes received per chunk


// Function Prototypes
void dieWithError(char *errorMessage);         // Error handling function
int getFileSize(char response[]);              // Calculates the file size of index.html
int hostnameToIP(char *hostname, char *ip);    // Converts host name address to IP

#endif //NETWORKSPROJECT1_CLIENT_H
