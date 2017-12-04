/*
*  Projeto com sockets - Sistemas Distribuídos 
*  Karoline Kimiko Figueiredo Setoue
*  Pos-Graduação em Ciência da Computação - UNESP 
*  
*  Aplicacao Cliente
*/
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

#define PORT 8080                   // define a porta utilizada 
#define FILE_TO_SEND "send.txt"     // nome do arquivo com o conteudo do email

int main(int argc, char const *argv[])
{
    int socket_servidor = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hellofromclient@client";
    char buffer[1024] = {0};

    // criacao do socket 
    if ((socket_servidor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nErro na criacao do socket \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Converte IPv4 e IPv6 de texto para binario 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    // tenta conectar ao servidor
    if (connect(socket_servidor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nFalha na conexao \n");
        return -1;
    } else {
        printf("\nConexao com o servidor estabelecida\n");
        printf("============================\n");
        
    }

    char email[1024]; 
    char emailDest[1024];

    printf("Send email to: "); 
    scanf("%s", email);
    send(socket_servidor, email, strlen(email), 0);
    printf("------------------------------");

    printf("\nFrom: ");
    scanf("%s", emailDest);
    send(socket_servidor, emailDest, strlen(emailDest), 0);
    printf("------------------------------\n");

    char *accepted = "E-mail confirmado, enviando mensagem...\n";
    char *rejected = "Endereco nao encontrado.\n";

    // espera a resposta do servidor
    valread = read(socket_servidor, buffer, 1024);

    if(strcmp(buffer, "accepted") == 0) {
        printf(accepted);
    } else {
        printf(rejected);
        exit(1);
    }

    // envia mensagem - arquivo

    // abre arquivo com o conteudo do email 
    FILE *file_to_send = fopen("send.txt", "rb"); 
    if(file_to_send == NULL)
    {
        printf("File open error"); 
        return 1; 
    }

    // ler conteudo e enviar
    while (1)
    {
        // lendo pedacos de 256 bytes 
        unsigned char buff[256] = {0};
        int nread = fread(buff, 1, 256, file_to_send);
        printf("Bytes lidos: %d \n", nread);

        // se a leitura foi concluida, enviar os dados
        if (nread > 0)
        {
            printf("Enviando...");
            send(socket_servidor, buff, nread, 0);
            printf("OK.\n");
        }


        if (nread < 256)
        {
            if (feof(file_to_send))
                printf("Fim do arquivo\n");
            if (ferror(file_to_send))
                printf("Erro na leitura\n");
            break;
        }

        // encerra conexao com o servidor
        close(socket_servidor);
    }

    // fecha o arquivo
    fclose(file_to_send);
    return 0;
}