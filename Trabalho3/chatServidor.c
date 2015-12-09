/* 
 * File:   chatServidor.c
 * Author: Leonardo Baiser <lpbaiser@gmail.com> & Marco C. Moreira
 *
 */

#include <sys/types.h>              
#include <sys/socket.h>             
#include <netinet/in.h>             
#include <netdb.h>                  

#include <stdio.h>                  
#include <string.h>
#include <stdlib.h>                 

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#include "chatServidor.h"


int listClient[MAX_CLIENT]; //vetor de socktes conectados
int contClient = 0; //contador de conexẽos com clientes
char buffer[MAX_BUFFER]; //buffer para armazenar mensagem
struct sockaddr_in addr_server, //struct que armazenar o IP e a porta do servidor
addr_client; //struct que armazenar o endereco do cliente
int serverSocket; //socket para ouvir conexões
int* clientSocket; // socket para comunicação com o cliente conectado

int main(int argc, char *argv[]) {

    struct hostent *ptrh; //ponteiro para a tabela de hosts
    clientSocket = (int*) malloc(sizeof (int));
    int port; // porta da comunicação

    //verifica os parâmetros estão ok
    if (argc > 1) port = atoi(argv[1]);
    else port = PORTA_PADRAO; // se nenhuma porta foi passada usa a porta padrão

    //testa a da porta
    if ((port <= 1024) || (port >= 65536)) {
        fprintf(stderr, "número de porta inválido %s\n", argv[2]);
        exit(1);
    }

    //pega o endereço local do servidor
    ptrh = gethostbyname(IP_SERVER);

    //preenche com zeros a struct sockaddr_in
    memset((char *) &addr_server, 0, sizeof (addr_server));

    //preenche a struct de endereco
    addr_server.sin_family = AF_INET; //define familia como Internet
    addr_server.sin_port = htons(port); //define a porta
    addr_server.sin_addr.s_addr = INADDR_ANY; //seta o endereco IP local

    //cria um socket servidor TCP
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    //testa o socket
    if (serverSocket < 0) {
        fprintf(stderr, "falha ao criar socket\n");
        exit(1);
    }

    //mapeia um endereco local para o socket
    if (bind(serverSocket, (struct sockaddr *) &addr_server, sizeof (addr_server)) < 0) {
        fprintf(stderr, "erro no mapeamento da porta\n");
        exit(1);
    }

    //Especifica o numero de conexoes suportadas
    if (listen(serverSocket, MAX_CLIENT) < 0) {
        fprintf(stderr, "erro em limitar o número de conexões (listen)\n");
        exit(1);
    }

    aguardaConexoes();

    //fecha o servidor
    close(serverSocket);

    //termina o programa
    exit(0);
}

void aguardaConexoes() {
    while (1) {
        //Aguarda conexões
        int szAddressClient = sizeof (addr_client);
        printf("Servidor iniciado e aguardando conexões ...\n");
        if ((*clientSocket = accept(serverSocket, (struct sockaddr *) &addr_client, &szAddressClient)) < 0) {
            fprintf(stderr, "erro ao aceitar conexão\n");
            exit(1);
        } // if

        listClient[contClient] = *clientSocket; //adiciona o número do socket no vetor
        contClient++;

        //espera uma primeira mensagem automática do cliente
        recv(*clientSocket, buffer, sizeof (buffer), 0); //recebe mensagem
        strcpy(buffer, "Conexao aceita.\n");
        send(*clientSocket, buffer, sizeof (buffer), 0); //envia mensagem

        //Avisa no servidor que o cliente foi conectado
        buffer[MAX_BUFFER - 1] = 0x0;
        printf("%s\n", buffer);
        printf("Cliente [%d] conectado \n\n", *clientSocket);

        //cria uma thread para cada nova conexão
        pthread_t* t;
        t = malloc(sizeof (pthread_t));
        pthread_create(t, NULL, aguardaMensagem, (void*) clientSocket);

    }
}

void aguardaMensagem(void* args) {

    int* socket = (int*) args;

    while (1) {
        memset(buffer, 0, MAX_BUFFER); //limpa o buffer

        recv(*socket, buffer, sizeof (buffer), 0); /* recebe dados */

        int i;
        if (contClient > 1) {
            for (i = 0; i < contClient; i++) {//envia a mensagem para todos os clientes
                if (*socket != listClient[i]) {
                    send(listClient[i], buffer, sizeof (buffer), 0); //envia mensagem
                }
            }
        } else {
            memset(buffer, '\0', MAX_BUFFER);
            strcpy(buffer, "OK");
            send(*socket, buffer, sizeof (buffer), 0); //envia mensagem

        }

        if (strcmp(buffer, "sair") == 0) break;

    }
    //fecha a conexão
    close(*socket);
}
