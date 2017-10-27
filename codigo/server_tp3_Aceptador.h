#ifndef SERVER_TP3_ACEPTADOR_H_
#define SERVER_TP3_ACEPTADOR_H_

#include "common_tp3_Thread.h"
#include "common_tp3_Socket.h"
#include "server_tp3_Operaciones.h"
#include <string>
#include <vector>

class Aceptador: public Thread { 
private:
    Socket &socketServer;
	std::vector<Operaciones*> threads;
	MapSube baseDeDatos;
public:
	explicit Aceptador(Socket &unSocket);
    virtual void run() override;
    ~Aceptador();
};

#endif
