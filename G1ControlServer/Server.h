#pragma once
#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>

constexpr int PROTOCOL = 0;
constexpr int PORT = 8080;

class Server {
private:
	int SockFD;
	int NewSocket;
	int ValRead;
	struct sockaddr_in Address;
	char Buffer[1024] = { 0 };
	char* ack = "Connection Acknowledged";
public:
	Server();
	~Server();
	void Listen();
	void ProcessCommand();
};
#endif // !SERVER_H
