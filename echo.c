//PROGRESS: I know the multithreading should be near the echo after Accepting the connection. I just havent figured where to put it yet
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define PORT 8000
#define MAX_BUFFER_SIZE 100

int main() 
{
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_read;
    struct sockaddr_in server_address;

    //Create TCP Socket
    //AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("SOCKET FAILED TO CREATE\n");
        exit(EXIT_FAILURE);
    }

    //Bind Socket to Port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            perror("Bind failed\n");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

    //Listen for connections
    if(listen(server_socket, 1) < 0)
    {
        perror("LISTEN FAILED\n");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    //Accept Connection/Reconnection
    while (1)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("ACCEPT FAILURE\n");
            continue;
        }
    
        printf("Client Connected.\n");

        //ECHO
        while ((bytes_read = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0))> 0)
        {
            buffer[bytes_read] = '\0';
            printf("Received: %s", buffer);

            if (write(client_socket, buffer, bytes_read) != bytes_read)
            {
                perror("WRITE FAILED\n");
                break;
            }
        }
        close(client_socket);
    }
    close(server_socket);
    return 0;
}