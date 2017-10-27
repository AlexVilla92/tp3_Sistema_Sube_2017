#include <iostream>
#include <string>
#include "server_tp3_Server.h"
#include "common_tp3_Excepciones.h"

int main(int argc, char *argv[]) {
	if (argc == 2) {
		Server unServer(std::stoi(argv[1]));
		return unServer.EjecutarOperaciones();
	} else {
		std::cerr << "Cantidad erronea de parametros ";
	return ERROR;
	}
}
