
all: 
	    gcc -c chatServidor.c -ggdb -Wall
	    gcc -o servidor chatServidor.o -ggdb -Wall -lpthread
	    gcc -c chatCliente.c -ggdb -Wall -lpthread
	    gcc -o cliente chatCliente.o -ggdb -Wall -lpthread