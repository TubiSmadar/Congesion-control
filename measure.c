#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>


#define SERVER_PORT 12345
#define BILLION  1000000000.0
#define BUFF_SIZE 1024

int main() {
    // on linux to prevent crash on closing socket.
    signal(SIGPIPE, SIG_IGN);
    char buffer[BUFF_SIZE];

    // create a socket lisener.
    int server_socket = -1;
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Couldn't create a socket listener : %d",errno);
    }

    int enable_reuse = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,&enable_reuse, sizeof(int)) < 0) {
        printf("setsockopt() failed with error code: %d", errno);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT); // short, network byte order

    // connect the server to a port which can read and write on.
    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed with error code : %d" , errno);
        return -1;
    }

    // printf("the server is ready!\n\n");


    if(listen(server_socket, 10) == -1) {
        printf("listen() failed with error code : %d",errno);
        return -1;
    }

    //Accept and incoming connection
    printf("Waiting for connections\n");

    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);
    int i = 0;

    while(i < 2){
        int j = 0;
        double sum_for_average = 0.0;
        while(j < 5) {
            memset(&client_addr, 0, sizeof(client_addr));

            // updates the length in each iteration.
            client_addr_length = sizeof(client_addr);

            int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_length);
            if(client_socket == -1) {
              printf("listen failed with error code : %d",errno);
              close(server_socket);
            return -1;
            } else {
                printf("connection %d accepted\n",j);
            }
            struct timespec start, end;
            clock_gettime(CLOCK_REALTIME, &start);

            int bytes = -1;
            while(bytes != 0) {
                bytes = recv(client_socket,buffer, BUFF_SIZE,0);
            }
            clock_gettime(CLOCK_REALTIME, &end);
            // time_spent = end - start
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
            printf("Took %f seconds to recive\n\n", time_spent);
            sum_for_average += time_spent;
            j++;
        }
        char cc_type[20];
        if(i == 0) {
            strcpy(cc_type,"cubic");
        } else {
            strcpy(cc_type,"reno");
        }
        printf("\nAverage time for CC %s is %f .\n\n",cc_type,sum_for_average/5);
        i++;
    }

    close(server_socket);
    return 0;
}
