// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define PORT 8080
#define FILE_TO_SEND "send.txt"

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hellofromclient@client";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    // check connection with server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char email[1024]; 
    char emailDest[1024];

    printf("Send email to: "); 
    scanf("%s", email); 
    send(sock, email, strlen(email), 0);

    // printf("\nFrom: ");
    // scanf("%s", emailDest);
    // send(sock, emailDest, strlen(emailDest), 0);

    // printf("Hello message sent\n");
    char *accepted = "E-mail confirmed, please, send your message";
    char *rejected = "Wrong e-mail address.";

    // awaits server response 
    valread = read( sock , buffer, 1024);
    
    if(strcmp(buffer, "accepted") == 0) {
        printf(accepted);
    } else {
        printf(rejected);
    }

    // sends message


    return 0;
}