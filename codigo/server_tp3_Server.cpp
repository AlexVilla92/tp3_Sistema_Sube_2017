#include "server_tp3_Server.h"
#include <string>

Server::Server(int unPuerto) {
	this->port = (unsigned short)unPuerto;
}

int Server::EjecutarOperaciones() {
	Socket socketServer;
	socketServer.Bind(this->port);
	socketServer.Listen();	
	
	Aceptador aceptador(std::ref(socketServer));
	aceptador.Start();
	
	while (true) if (std::cin.get() == 'q') break;

	socketServer.Shutdown();
	aceptador.Join();
	return 0;
}

Server::~Server() {}
