//
// Created by Jason Dykstra on 9/17/20.
//

#include "server.h"

// Exits the program gracefully when any signal is processed
void sigHandler(int sigNum){
    printf("\nCaught signal %d, closing gracefully...\n", sigNum);

    printf("Freeing up memory...\n");
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));
    memset(&serverPort, 0, sizeof(serverPort));
    memset(&clientLen, 0, sizeof(clientLen));
    memset(&httpHeader, 0, sizeof(httpHeader));
    memset(&fileSize, 0, sizeof(fileSize));

    printf("Closing sockets...\n");
    close(clientSocket);
    close(serverSocket);

    printf("All clean. Goodbye!\n");
    exit(0);
}

void dieWithError(char *errorMessage){  // Error handling function
    printf("%s", errorMessage);
}

// Returns file size in int
long calculateFileSize(){
    FILE * f;
    f = fopen("index.html", "r");
    // Find File size
    fseek(f,0,SEEK_END);
    fileSize = ftell(f);
    fseek(f,0,SEEK_SET);
    fclose(f);
    return fileSize;
}

// Sends the HTML file to the client
void handleHTTPClient(int clientSocket){
    // Opening files in the server
    FILE * htmlData;                                    // File for HTML
    htmlData = fopen("index.html", "r");  // Open HTML and read the file
    unsigned char responseData[1024];
    memset(responseData, 0, 1024);
    // Read the whole html file chunk by chunk where each chunk is the size of responseData
    int bytesPerChunk;
    while(1){
        bytesPerChunk = fread(responseData, 1, 1024, htmlData);

        // If there was at least 1 byte in the chunk, send it
        if(bytesPerChunk > 0){
            if(send(clientSocket, responseData, sizeof(responseData), 0) < 0){
                dieWithError("send() failed");
            }
            memset(responseData, 0, 1024);
        }

        // Break after sending the last chunk
        if(bytesPerChunk < 1024){
            break;
        }
    }

    fclose(htmlData);
}

int main(int argc, char *argv[]) {

    // Catch signals
    signal(SIGINT, sigHandler);

    if(argc != 2){ // Test for correct number of arguments
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    serverPort = atoi(argv[1]); // First arg: local port

    // Create socket for incoming connections
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        dieWithError("socket() failed");
    }

    // Define the address
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = INADDR_ANY;


    // Bind to local address
    if(bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
        dieWithError("bind() failed");
    }

    // Listen for connections
    if(listen(serverSocket, MAXPENDING) < 0){
        dieWithError("listen() failed");
    }

    while(1){  // Endless Loop
        clientLen = sizeof(clientAddress); // Wait for a client to connect
        if ((clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLen)) < 0) {
            dieWithError("accept() failed");
        }

        // Client socket is connected to a client
        // Send back 200 OK message along with content length
        sprintf(httpHeader, "%s%ld%s", "HTTP/1.1 200 OK\nContentLength: ", calculateFileSize(), "\r\n\r\n");

        printf("Handling client %s\n", inet_ntoa(clientAddress.sin_addr));
        if(send(clientSocket, httpHeader, sizeof(httpHeader), 0) < 0){
            dieWithError("send() failed");
        }

        // Handle client
        handleHTTPClient(clientSocket);
        close(clientSocket);
    }
}
