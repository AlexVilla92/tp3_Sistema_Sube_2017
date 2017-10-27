#ifndef SERVER_TP3_SERVER_H_
#define SERVER_TP3_SERVER_H_

#include "common_tp3_Socket.h"
#include "common_tp3_Thread.h"
#include "server_tp3_Aceptador.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define ERROR -1

class Server {
private:
	unsigned short port;
public:
	explicit Server(int unPuerto);
	int EjecutarOperaciones();
	~Server();
};

#endif
