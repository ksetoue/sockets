// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define FILENAME "teste.txt"

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    // waiting for the first connection 
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);

    // get buffer size 
    int size = strlen(buffer); 
    printf("%d\n", size);

    int i = 0;
    int found = 0;  
    
    // email validation
    for(i = 0; i < size; i++) {
        if (buffer[i] == 64) {
            found = 1;
        }
    }
    
    if (found) {
        printf("Found\n");
        char *accepted = "accepted";
        send(new_socket, accepted, strlen(accepted), 0);        
    } else {
        char *rejected = "rejected";
        send(new_socket, rejected, strlen(rejected), 0);
        // valread = read(new_socket, buffer, 1024);
        exit(1);
    }

    //char message[1024] = {0};
    //valread = read(new_socket, message, 1024);

    // send(new_socket, hello, strlen(hello), 0);
    // printf("Hello message sent\n");

    // receive email file


    return 0;
}