/* 
 * File:   chatServidor.h
 * Author: Leonardo Baiser <lpbaiser@gmail.com> & Marco C. Moreira
 *
 */

#ifndef CHATSERVIDOR_H
#define	CHATSERVIDOR_H

#define IP_SERVER     "127.0.0.1"   //especifica o IP padrão do servidor
#define PORTA_PADRAO  9000          //especifica a porta padrao de conexao
#define MAX_CLIENT    10            //define o número máximo de conexoes
#define MAX_BUFFER 255 //define um tamanho máximo para o buffer

int main(int argc, char *argv[]);
void aguardaMensagem(void* args);

#endif	/* CHATSERVIDOR_H */

