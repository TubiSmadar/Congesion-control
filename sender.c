//
// Created by yair on 12/15/22.
//
/* TCP Client */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <netinet/tcp.h>

#define SERVER_PORT 1604
#define SERVER_IP "127.0.0.1"
#define FILE_NAME "test.txt" // TODO
#define BUFFER_SIZE 1024
#define FILE_SIZE 1109892


char buffer[BUFFER_SIZE] = {0};
void sendNew(int socket,FILE *filePointer);

void sendMsg(int socket, char message[]);

void receiveMsg(int socket);

long int getFileSize(FILE *fp);

int main() {

    // Read file content, check file size (bytes) and save it as an array of that size.
    // Close file when done copying it.
    FILE *fp;
    fp = fopen((char *) FILE_NAME, "r");
    if (fp == NULL) {
        printf("File reading failed.");
        return -1;
    }
    long fileSize = getFileSize(fp);
    char fileText[fileSize];
    int fileIndex = 0;
    while (!feof(fp)) {
        char c = fgetc(fp);
        fileText[fileIndex] = c;
        fileIndex++;
    }
    fclose(fp);

    // Split file content in to two (each half is 50% of file) -
    char filePart1[fileSize / 2];
    for (int i = 0; i <= fileSize / 2; i++) {
        filePart1[i] = fileText[i];
    }
    char filePart2[(fileSize / 2) + 1];
    for (int i = (fileSize / 2) + 1; i <= fileSize; i++) {
        filePart2[i] = fileText[i];
    }

    // Create socket for sending files to Receiver. This socket acts as a TCP client socket.
    // @returns int representing senderSocket fd, or -1 if socket failed to initialize.
    int senderSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (senderSocket == -1) {
        printf("Socket not created: %d", errno);
    }

    // Initialize an internet socket-address object, named serverAddress.
    // Socket will accept connections to Server IP.
    // @returns -1 if IP encoding failed.
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    int ip_addr = inet_pton(AF_INET, (const char *) SERVER_IP, &serverAddress.sin_addr);
    if (ip_addr < 1) {
        ip_addr == -1 ? printf("inet_pton() failed %d: ", errno) : printf("inet_pton() src invalid");
    }

    // Connect senderSocket to Receiver's (Server's) internet address.
    // @returns -1 if connection failed, otherwise 0.
    int connectionStatus = connect(senderSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (connectionStatus == -1) {
        printf("Socket not connected: %d", errno);
    } else {
        printf("Connected to server succesfully\n");
    }
    char buf[256];
    char buf1[256];
    int len = sizeof(buf);
    int len1 = sizeof(buf);
    int userDecision = 1;
    int firstSend = 0;
    do {
        if (userDecision != 1) {
            printf("Please enter a valid option, 0 or 1:\n");
            scanf("%d\n", &userDecision);
        }
        // TODO: authenticate (+ receive)

        //
        //
        //
        //

        strcpy(buf, "reno");
        len = strlen(buf);
        strcpy(buf1, "cubic");
        len1 = strlen(buf1);
        if (firstSend = 0) {
            firstSend = 1;
            receiveMsg(senderSocket);
        }

        if (setsockopt(senderSocket, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
            perror("setsockopt");
            return -1;
        }
        printf("Current: %s\n", buf);
        sendMsg(senderSocket, filePart1);

        if (setsockopt(senderSocket, IPPROTO_TCP, TCP_CONGESTION, buf1, len1) < 0) {
            perror("setsockopt");
            return -1;
        }
        printf("Current: %s\n", buf1);
        sendMsg(senderSocket, filePart2);

        printf("Would you like to send the file again or exit program?\n0: Exit\n1: Send again\n");
        scanf("%d\n", &userDecision);
        //userDecision()
    }while (userDecision != 0);

        sendMsg(senderSocket, "");
        close(senderSocket);

        return 0;

    }


    void sendMsg(int socket, char message[]) {
        int bytesSent;
        int messageLen = strlen(message) + 1;
        bytesSent = send(socket, message, messageLen, 0);
        if (bytesSent == -1) {
            printf("Message failed to send, with error code : %d", errno);
        } else if (bytesSent == 0) {
            printf("TCP connection was closed by Receiver prior to send().\n");
        } else if (messageLen > bytesSent) {
            printf("Sent only %d bytes of the required %d.\n", bytesSent, messageLen);
        } else {
            printf("Message was successfully sent.\n");
        }
    }

//    void sendIt(int socket,FILE *filePointer[])
//    {
//        int data_stream;
//        int size = 0;
//        while( ( data_stream = fread(buffer,1,sizeof(buffer)/2,filePointer) ) > 0 ) {
//            size += send(socket, buffer, data_stream, 0);
//        }
//
//        if(size == FILE_SIZE) {
//            printf("sent all the 1MB file: %d\n",size);
//        }else {
//            printf("sent just %d out of %d\n",size,FILE_SIZE);
//        }
//        close(socket);
//    }

    void receiveMsg(int socket) {
        //char message[];
        long int bytesReceived;
        bytesReceived = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == -1) {
            printf("Failure to receive message, with error code : %d", errno);
        } else if (bytesReceived == 0) {
            printf("TCP connection was closed prior to recv().\n");
        } else {
            printf("Message received from Receiver.\n");
        }
    }

    long int getFileSize(FILE *fp) {
        fseek(fp, 0, SEEK_END);
        return ftell(fp);
    }
