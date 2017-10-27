#ifndef CLIENT_TP3_CLIENTE_H_
#define CLIENT_TP3_CLIENTE_H_

#include "common_tp3_Socket.h"
#include <bitset>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <cstdint>
#include <netinet/in.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#define MAX_BUFFER 30
#define EXITO 0
#define ERROR -1
#define DOS 2
#define TRES 3

class Cliente {
private:
	std::string host;
	unsigned short port;
	std::string fileName;

public:
	explicit Cliente(char *argv[]);
	int ProcesarDatos();
	~Cliente();

private:
	bool ChecksumOK(std::bitset<32> campo, std::bitset<5> chekTarjeta);
	char DeterminarOperacion(uint16_t opcode);
	void EnviarCampoTarjetaYmonto(Socket &socket, std::bitset<32> campoTarjeta, 
								  std::bitset<32> campoMonto, char operacion);
	void EnviarCampoTarjeta(Socket &socket, std::bitset<32> campoTarjeta,
							char operacion, std::bitset<5> bitsChekMonto);
	void MensajeError(char op, unsigned int numTarjeta, int monto);
	void EnviarRecibirMensajes(char &op, Socket &socket, std::string &texto);
	std::string ConvertirValor(int valor);
};

#endif
