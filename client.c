//
// Created by Jason Dykstra on 9/17/20.
//

#include "client.h"

void dieWithError(char *errorMessage) {
    printf("%s", errorMessage);
}

int getFileSize(char response[]){
    char needle[20] = "ContentLength:";
    char *location = strstr(response, needle);
    if(location) {
        int pos = location - response;
        int responseLen = strlen(response);
        int c = 0;
        char fileSize[20];
        int startPos = pos + strlen(needle);
        while (c < responseLen - 1 - startPos - 1 - 2) { // The -2 on the end comes from the hidden \r\n\r\n tags
            fileSize[c] = response[startPos + c];
            c++;
        }
        return atoi(fileSize);
    }
    return 0;
}

int hostnameToIP(char *host, char*ip){
    struct hostent *hEntity;
    struct in_addr **addrList;
    int i;

    if((hEntity = gethostbyname(host)) == NULL) {
        dieWithError("gethostbyname() failed\n");
        exit(1);
    }

    addrList = (struct in_addr **) hEntity->h_addr_list;

    for(i = 0; addrList[i] != NULL; i++){
        strcpy(ip, inet_ntoa(*addrList[i]));
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    // Test for correct number of arguments
    if ((argc < 2) || (argc > 4)) {
        fprintf(stderr, "Usage: %s [-p] <Server IP> [<Echo Port>]\n", argv[0]);
        exit(1);
    }

    /* possible inputs:
     * ./client google.com
     * ./client google.com 80
     * ./client -p google.com
     * ./client -p google.com 80
     */

    // Set showRTT to 0 by default
    showRTT = 0;

    // Initialize variables depending on argument count and presence of -p flag
    switch(argc){
        case 2:
            hostname = argv[1];
            serverPort = DEFAULT_PORT;
            break;
        case 3:
            if(strcmp(argv[1], "-p") == 0){ // If the two strings are equal...
                showRTT = 1;
                hostname = argv[2];
                serverPort = DEFAULT_PORT;
                break;
            } else {
                hostname = argv[1];
                serverPort = atoi(argv[2]);
                break;
            }
        case 4:
            showRTT = 1;
            hostname = argv[2];
            serverPort = atoi(argv[3]);
            break;
        default:
            fprintf(stderr, "Usage: %s [-p] <Server IP> [<Echo Port>]\n", argv[0]);
            exit(1);
    }

    hostnameToIP(hostname, serverIP);
    printf("%s resolved to %s\n", hostname, serverIP);

    // Create reliable stream socket using TCP
    if ((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        dieWithError("socket() failed\n");
    }


    // Construct the server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));     // Zero out structure
    serverAddress.sin_family = AF_INET;                   // Internet address family
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);  // Server IP address
    serverAddress.sin_port = htons(serverPort);           // Server port


    inet_aton(hostname, (struct in_addr *) &serverAddress.sin_addr.s_addr);

    // Establish connection to server
    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        dieWithError("connect() failed\n");
    }


    // GET command
    char request[] = "GET / HTTP/1.1\r\n\r\n";

    // Start calculate RTT
    gettimeofday(&startTime, NULL);

    // Send request to server
    if(send(clientSocket, request, sizeof(request), 0) != sizeof(request)){
        dieWithError("send() sent a different number of bytes than expected\n");
    }


    // Receive the message requested from server
    if(recv(clientSocket, &response, sizeof(response), 0) < 0){
        dieWithError("recv() failed\n");
    }


    // End calculate RTT
    gettimeofday(&endTime, NULL);
    timeInMicroseconds = ((endTime.tv_sec - startTime.tv_sec) * 1000000 + endTime.tv_usec - startTime.tv_usec);
    timeInMilliseconds = (double) timeInMicroseconds/1000;

    printf("Response from the server: %s\n", response); // Should print HTTP/1.1 200 OK


    // If connected to localhost and you're expecting an index.html file...
    //if((hostname = "localhost") || (hostname = "127.0.0.1")) {

        // Set up while loop to receive the html from the server given the file size
        // Start by getting the file size as an int from the response
        totalExpectedBytes = getFileSize(response);
        totalBytesReceived = 0;
        // While not all data has been received, keep expecting data
        bytesReceived = 0;
        while (totalBytesReceived < totalExpectedBytes) {
            if ((bytesReceived = recv(clientSocket, response, sizeof(response), 0)) <= 0) {
                dieWithError("recv() failed or connection closed prematurely");
                bytesReceived = 0;
            }
            printf("Bytes received: %ld, Total bytes received: %ld\n", bytesReceived, totalBytesReceived);
            totalBytesReceived += bytesReceived;
            response[bytesReceived] = '\0';
            printf("%s", response);
        }
        response[bytesReceived] = '\0';

        // Print the RTT if the -p flag is present
        if (showRTT) {
            printf("RTT: %fms\n", timeInMilliseconds);
        }


    close(clientSocket);
    exit(0);
}