#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>

int main(int argc, char* argv []){
	int fd, leido;
	char buff[512];
	struct sockaddr_in server;
	
	
	fd = socket(AF_INET, SOCK_STREAM, 0); /*Protocolo TPC*/
	assert (argc == 3);
	
	server.sin_family = AF_INET;
	server.sin_port =htons(atoi(argv[2])); 
	inet_aton(argv[1],  &server.sin_addr);

	if (connect(fd, (struct sockaddr *) &server, sizeof server) < 0) {
		perror("connect");
		return 0;
	}
	write(fd, "hola /\r\n",7);
	
	leido = read(fd, buff, sizeof buff);
	if(leido < 0){
		perror("leido");
		return -1;
	}
	
	buff[leido] = 0;
	printf("leido: %s\n", buff);
	close(fd);
	return 0;
}

