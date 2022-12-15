/* TCP Server */


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


#define SERVER_PORT 1604
#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE] = {0};

void sendMsg(int socket, char message[]);

void receiveMsg(int socket);

int receivalTimes[1000] = {-1};
int receivalTimesIndex = 0;

int main() {

    // Array to track clientSockets opened - will be called upon close().
    int clientSockets[50] = {0};
    int clientSocketsIndex = 0;

    // Create socket for receiving files. This socket acts as a TCP server socket.
    // @returns int representing receiverSocket fd, or -1 if socket failed to initialize.
    int receiverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (receiverSocket == -1){
        printf("Socket not created: %d", errno);
    }

    // Reuse the address if the server socket on was closed
    // and remains for 45 seconds in TIME-WAIT state till the final removal.
    int enableReuse = 1;
    if (setsockopt(receiverSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) < 0) {
        printf("setsockopt() failed with error code : %d", errno);
    }

    // Initialize an internet socket-address object, named serverAddress.
    // Socket will accept connections from any IP.
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    // Initialize an internet-address socket object, named clientAddress, for connections made with Receiver.
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    // Bind the socket to the Receiver's given internet address.
    // @returns 0 if bind was successful, -1 if failed
    int bindStatus = bind(receiverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindStatus == -1){
        printf("Bind failed with error code : %d" , errno);
        close(receiverSocket);
        return -1;
    }
    printf("Bind was successful\n");

    // Put socket in listening mode, waiting for connections.
    // @return 0 if successful, -1 if failed.
    int listenStatus = listen(receiverSocket, 20);
    if (listenStatus == -1){
        printf("Listen failed with error code : %d" , errno);
        close(receiverSocket);
        return -1;
    }
    printf("Receiver ready for connections\n");

    while(1) {
        memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(receiverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (clientSocket == -1) {
            printf("listen failed with error code : %d" ,errno);
            close(clientSocket);
            //close(receiverSocket);
            return -1;
        }
        receiveMsg(clientSocket);
        clientSockets[clientSocketsIndex++] = clientSocket;
        printf("A new client connection has been accepted\n");

    }


    // Close all sockets - first client sockets, then receiver socket.
    for(int i = 0; i < clientSocketsIndex; i++) {
        close(clientSockets[i]);
    }
    close(receiverSocket);

    return 0;
}


void sendMsg(int socket, char message[]) {
    int bytesSent;
    int messageLen = strlen(message) + 1;
    bytesSent = send(socket, message, messageLen, 0);
    if (bytesSent == -1){
        printf("Message failed to send, with error code : %d" ,errno);
    } else if (bytesSent == 0) {
        printf("TCP connection was closed by Receiver prior to send().\n");
    } else if (messageLen > bytesSent) {
        printf("Sent only %d bytes of the required %d.\n", bytesSent, messageLen);
    } else {
        printf("Message was successfully sent.\n");
    }
}

void receiveMsg(int socket) {
    //char message[] = {0};
    long bytesReceived;
    // Record the start time
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    // Receive the TCP packet

    bytesReceived = recv(socket, buffer, BUFFER_SIZE, 0);

    // Record the end time
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate the elapsed time in nanoseconds
    long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000;
    elapsed_time += end.tv_nsec - start.tv_nsec;


    if (bytesReceived == -1) {
        printf("Failure to receive message, with error code : %d" ,errno);
    } else if (bytesReceived == 0) {
        printf("TCP connection was closed prior to recv().\n");
    } else if (bytesReceived == 1) {
        printf("Exit code received.\n");
        exit(1);
    } else {
        printf("Message received from Sender.\nReceived %ld bytes\n", bytesReceived);
        printf("Elapsed time: %ld nanoseconds\n", elapsed_time);
        receivalTimes[receivalTimesIndex++] = elapsed_time;
    }
}


