#include <iostream>
#include <string>
#include "client_tp3_Cliente.h"
#include "common_tp3_Excepciones.h"

int main(int argc, char *argv[]) {
	if (argc == 4) {
		Cliente unCliente(argv);
		return unCliente.ProcesarDatos();
	} else {
		std::cerr << "Error cantidad de parametros...";
		return ERROR;
	}
}
