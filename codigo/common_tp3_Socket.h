#ifndef COMMON_TP3_SOCKET_H_
#define COMMON_TP3_SOCKET_H_

#include "common_tp3_Excepciones.h"
#include <exception>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#define ERROR -1
#define EXITO 0

class Socket {
private:
	int sock;

public:
	Socket();
	explicit Socket(int unFd);
	Socket(Socket &&other);
	void Bind(unsigned short port);   
	void Listen();
	void Connect(const char *host_name, unsigned short port);
	void Accept(Socket &accepted_socket);
	int AnotherAccept();
	void Send(const char *buffer, size_t length);
	void Receive(char *buffer, size_t length);
	void Shutdown();
	int getFileDescriptor();
	~Socket();
};

#endif
