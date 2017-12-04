/*
*  Projeto com sockets - Sistemas Distribuídos 
*  Karoline Kimiko Figueiredo Setoue
*  Pos-Graduação em Ciência da Computação - UNESP 
*  Servidor
*
*/
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 8080                       // porta
#define FILE_NAME "sample_file.txt"     // nome do arquivo de saída

int main(int argc, char const *argv[])
{
    int descritor_servidor, socket_cliente, read_ret;
    struct sockaddr_in endereco;
    int opt = 1;
    int end_tam = sizeof(endereco);
    char buffer[1024] = {0};
    char buffer_2[1024] = {0};

    // Criando descritor do socket 
    if ((descritor_servidor = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Falha na criacao do socket");
        exit(EXIT_FAILURE);
    }

    // bind do socket - garante o reuso da porta 8080
    if (setsockopt(descritor_servidor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("erro no setsockopt");
        exit(EXIT_FAILURE);
    }
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORT);

    //  bind da porta 8080 ao socket
    if (bind(descritor_servidor, (struct sockaddr *)&endereco,
             sizeof(endereco)) < 0)
    {
        perror("Falha no bind da porta 8080");
        exit(EXIT_FAILURE);
    }

    // espera conexao com o cliente
    if (listen(descritor_servidor, 3) < 0)
    {
        perror("Erro no listen - conexao entre cliente e servidor");
        exit(EXIT_FAILURE);
    }

    // aceite de conexao
    if ((socket_cliente = accept(descritor_servidor, (struct sockaddr *)&endereco,
                             (socklen_t *)&end_tam)) < 0)
    {
        perror("Erro ao aceitar novo cliente");
        exit(EXIT_FAILURE);
    }

    // esperando dado do cliente - primeiro email
    read_ret = read(socket_cliente, buffer, 1024);
    printf("%s\n", buffer);

    // tamanho do buffer
    int size = strlen(buffer); 
    printf("%d\n", size);

    //  esperando dado do cliente - segundo email
    read_ret = read(socket_cliente, buffer_2, 1024);
    printf("%s\n", buffer_2);

    // tamanho do buffer
    int size2 = strlen(buffer_2);
    printf("%d\n", buffer_2);

    int i = 0;
    int found = 0;
    int found2 = 0;  
    
    // valida se os emails tem @
    // email validation
    for(i = 0; i < size; i++) {
        if (buffer[i] == 64) {
            found = 1;
        }
        // printf("%c", buffer[i]);
    }

    // second mail validation 
    for (i = 0; i < size; i++)
    {
        if (buffer_2[i] == 64)
        {
            found2 = 1;
        }
        // printf("%c", buffer[i]);
    }
    
    // se os dois emails sao validos, retorna para o cliente mensagem de sucesso
    // senao, encerra conexao
    if (found && found2) {
        printf("Found\n");
        char *accepted = "accepted";
        send(socket_cliente, accepted, strlen(accepted), 0);
    } else {
        char *rejected = "rejected";
        send(socket_cliente, rejected, strlen(rejected), 0);
        exit(1);
    }

    // recebe tamanho do arquivo
    int bytesRecebidos = 0;
    char recvBuff[256];

    memset(recvBuff, '0', sizeof(recvBuff));
    
    // cria arquivo onde sera salvo 
    FILE *fp;
    fp = fopen(FILE_NAME, "w");

    if (NULL == fp)
    {
        printf("Error ao abrir arquivo");
        return 1;
    }

    // recebe pedacos de tamanho 256
    while ((bytesRecebidos = read(socket_cliente, recvBuff, 256)) > 0)
    {
        printf("Bytes recebidos %d\n", bytesRecebidos);
        fwrite(recvBuff, 1, bytesRecebidos, fp);
    }

    if (bytesRecebidos < 0)
    {
        printf("\n Erro de leitura \n");
    }

    return 0;
}