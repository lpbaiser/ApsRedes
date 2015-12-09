/* 
 * File:   chatCliente.c
 * Author: Leonardo Baiser <lpbaiser@gmail.com> & Marco C. Moreira
 *
 */

#include <sys/types.h>              
#include <sys/socket.h>             
#include <netinet/in.h>             
#include <netdb.h>                  

#include <stdio.h>                  
#include <string.h>                 
#include <pthread.h>
#include <stdlib.h>

#include "chatServidor.h"
#include "chatCliente.h"


char buffer[MAX_BUFFER]; // buffer que armazena as mensagens recebidas

main(int argc, char *argv[]) {

    struct hostent *ptrh; // ponteiro para a tabela de hosts
    struct sockaddr_in address; // struct para armazenar o IP e a porta
    int *sd; // descritor do socket
    sd = (int*) malloc(sizeof (int));
    int port; // porta de comunicação
    char *host; //guarda o endereço do servidor

    //verifica os parâmetros estão ok
    if (argc > 2) port = atoi(argv[2]); /* converte para inteiro */
    else port = PORTA_PADRAO; /* usa a porta padr�o */

    //testa a porta
    if ((port <= 1024) || (port >= 65536)) {
        fprintf(stderr, "número de porta inválido %s\n", argv[2]);
        exit(1);
    }

    //verifica se o endereço do servidor foi fornecido
    if (argc > 1) host = argv[1];
    else host = IP_SERVER;

    //testa a validade do endereco
    ptrh = gethostbyname(host);
    printf("Host name: %s\n", (char *) ptrh->h_name);
    if (ptrh == NULL) {
        fprintf(stderr, "host inválido: %s\n", host);
        exit(1);
    }

    //preenche com zeros a struct sockaddr_in
    memset((char *) &address, 0, sizeof (address));

    //preenche a struct de endereco
    address.sin_family = AF_INET; //define familia como Internet
    address.sin_port = htons(port); //define a porta
    memcpy(&address.sin_addr, ptrh->h_addr, ptrh->h_length); //especifica o destino (IP)

    //cria um socket TCP
    *sd = socket(PF_INET, SOCK_STREAM, 0);

    //testa o socket
    if (*sd < 0) {
        fprintf(stderr, "falha na criação do socket\n");
        exit(1);
    }

    //conecta o socket ao servidor e testa a conexão
    if (connect(*sd, (struct sockaddr *) &address, sizeof (address)) < 0) {
        fprintf(stderr, "falha na conexão\n");
        exit(1);
    }

    printf("Solicitando conexão ...\n");
    strcpy(buffer, "Solicitando conexão ...!!!\n");
    send(*sd, buffer, sizeof (buffer), 0); //envia mensagem
    recv(*sd, buffer, sizeof (buffer), 0); //recebe mensagem
    printf("Mensagem Servidor: %s\n", buffer);

    printf("------------------------------------------------\n");
    printf("--                   Chat                     --\n");
    printf("------------------------------------------------\n\n");


    memset(buffer, 0, MAX_BUFFER);

    //cria uma thread para ficar responsável em enviar a mensagem
    pthread_t* env;
    env = malloc(sizeof (pthread_t));
    pthread_create(env, NULL, enviaMensagem, (void*) sd);

    recebeMensagem((void*) sd);

    //fecha a conexão
    close(*sd);
}

void* enviaMensagem(void* args) {

    int* socket = (int*) args;

    while (1) {
        printf("Mensagem >> ");
        fgets(buffer, MAX_BUFFER, stdin);
        send(*socket, buffer, sizeof (buffer), 0); // envia mensagem
    }
}

void* recebeMensagem(void* args) {

    int* socket = (int*) args;

    while (1) {
        recv(*socket, buffer, sizeof (buffer), 0); //recebe mensagem
        printf("Mensagem: %s\n", buffer);
    }
}
