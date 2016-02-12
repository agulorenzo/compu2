#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int server(int argc, char* argv[]){

	int fd_serv, fd_cli;
	int opt, leido;
	char * addrcliente; /* para printf */
	char buff[512];
	socklen_t *len_cliente; /* se crea para no castear en el tercer parametro de la funcion accept */
	len_cliente = sizeof addrcliente;
	
	fd_serv = socket (AF_INET, SOCK_STREAM,0); /* abrimos socket, devuelve un entero que es el descriptor, caso contrario devuelve -1 */
	
	assert(argc==3);
	
/* Si se ha obtenido un descriptor correcto, se puede indicar al sistema que ya tenemos abierto el socket. Para ello utilizamos la función bind(). El problema de la función bind() es que lleva un parámetro bastante complejo que debemos rellenar. */
	
	struct sockaddr_in server;
	struct sockaddr_in cliente;
	
	server.sin_family = AF_INET; /* es el tipo de conexión (por red o interna), igual que el primer parámetro de socket() */
	server.sin_port = htons(atoi(argv[2])) ; /* es el puerto que le pasaremos por argumento */
	server.sin_addr.s_addr = htonl(INADDR_ANY); /* es la dirección del cliente al que queremos atender, nosotros atendemos todos */
		
	opt=1;
	setsockopt(fd_serv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt); /*Se desea reutilizar la direccion local sino hay NADIE escuchando */
	
	if((bind(fd_serv,(struct sockaddr*) &server, sizeof server))<0){
		perror("bind:");
		return -1;
	}
	
	if(listen(fd_serv, 10) < 0)	{ // encola hasta 10 clientes
		perror("listen");
		return -1;
	}
	
	while((fd_cli=accept(fd_serv,(struct sockaddr*) &cliente,&len_cliente))>0){
		if(fork() == 0){ /* le devuelve un 0 al hijo */
			leido = read(fd_cli, buff, sizeof buff); /* lee la info de accept, la guarda en buff */
        
			if(leido < 0){
				perror("Leido:");
				return -1;
			}
			//buff[leido] = 0; /* Lee hasta el 0 binario, para el printf */
			//printf("Leido : %s ",buff);
			//sleep(5);
			addrcliente = inet_ntoa(cliente.sin_addr);
			printf("IP cliente : %s , puerto Cliente: %d \n",addrcliente,ntohs(cliente.sin_port));
			write(fd_cli,buff,leido);
			close(fd_cli);
			close(fd_serv);
			return 0;
		}
		close(fd_cli);
	}
	close(fd_serv);
	return 0;
	
}
