#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "Server.h"

Server::Server()
{
	int opt = 1;
	
	if ((SockFD = socket(AF_INET, SOCK_STREAM, PROTOCOL)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(SockFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	Address.sin_family = AF_INET;
	Address.sin_addr.s_addr = INADDR_ANY;
	Address.sin_port = htons(PORT);

	if (bind(SockFD, (struct sockaddr*) & Address, sizeof(Address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
	close(SockFD);
	close(NewSocket);
}

void Server::Listen() {
	if (listen(SockFD, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	int addrlen = sizeof(Address);
	printf("Listening on Port %d", PORT);
	if ((NewSocket = accept(SockFD, (struct sockaddr*) & Address, (socklen_t*)& addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	ValRead = read(NewSocket, Buffer, 1024);
	printf("%s\n", Buffer);
	send(NewSocket, ack, strlen(ack), 0);
	printf("Acknowledgement sent\n");
	return;
}

void Server::ProcessCommand() {

}
