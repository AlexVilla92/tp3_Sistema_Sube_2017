#include "common_tp3_Socket.h"

Socket::Socket() {
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock < 0) { 
		throw NewSocketException();
	}
}

Socket::Socket(int unFd) {
	this->sock = unFd;
}

Socket::Socket(Socket &&other) : sock(other.sock) {
	other.sock = -1;
}

Socket::~Socket() {
	close(this->sock);
}

void Socket::Bind(unsigned short port) {
	int codigo = 0;
	struct sockaddr_in server;

	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htons(INADDR_ANY);
	codigo = bind(this->sock, (struct sockaddr*) &server,
				 (socklen_t)sizeof(struct sockaddr));
	if (codigo < 0) {
		throw BindException();
	} 
}

void Socket::Listen() {
	const int maximo = 20;
	listen(this->sock, maximo); 
}

void Socket::Connect(const char *host_name, unsigned short port) {
	struct sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(host_name);
	int dato = connect(this->sock,(struct sockaddr*) &server,sizeof(server));
	
	if (dato < 0) {
		throw ConnectException();
	}
}

int Socket::AnotherAccept() {
	int unFd;

	unFd = accept(this->sock,NULL,NULL);
	if (unFd < 0) {
		throw AcceptException();
	}

	return unFd;
}

void Socket::Accept(Socket &acceptSock) {
	acceptSock.sock = accept(this->sock,NULL,NULL);

	if (acceptSock.sock < 0) {
		throw AcceptException();
	}
}

void Socket::Send(const char *buffer, size_t length) {
	unsigned int s = 0, bytesEnviados = 0;
	while (length > bytesEnviados) { 
		s = send(this->sock, buffer + bytesEnviados,
				 length - bytesEnviados, MSG_NOSIGNAL);
		if (s <= 0) { 
			throw SendException();
		} else {
			bytesEnviados += s;
		}
	}
}

void Socket::Receive(char *buffer, size_t length) {
	unsigned int s = 0, bytesRecibidos = 0;
	while (length > bytesRecibidos) {
		s = recv(this->sock,buffer + bytesRecibidos,
				 length - bytesRecibidos, MSG_NOSIGNAL);
		if (s <= 0) {
			throw RecvException();
		} else {
			bytesRecibidos += s;
		}
	}
}

int Socket::getFileDescriptor() {
	return this->sock;
}

void Socket::Shutdown() {
	shutdown(this->sock, SHUT_RDWR);
}

