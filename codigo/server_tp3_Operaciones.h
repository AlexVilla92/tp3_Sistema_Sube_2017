#ifndef SERVER_TP3_OPERACIONES_H_
#define SERVER_TP3_OPERACIONES_H_

#include "common_tp3_Thread.h"
#include "common_tp3_Socket.h"
#include "server_tp3_MapSube.h"
#include "common_tp3_Excepciones.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <mutex>
#define MAX_BUFFER 30
#define MAX_VALOR "4294967295"
#define CEROS_9 9
#define CEROS_10 10
#define CEROS '0'
#define MAX_LEN 8
#define TAM_ERROR 7
#define LEN_ERROR 6
#define CONSULTAR 'P' 

class Operaciones: public Thread { 
private:
    Socket cliente;
    MapSube &baseDeDatos;
    std::mutex mutexOperacion;
public:
	Operaciones(Socket &&unSocket, MapSube &unaBase);
    virtual void run() override;
    ~Operaciones();
private:
	std::string ProcesarOperacion(char operacion, char campos[]);
	void ObtenerValores(unsigned int &tarjeta, long int &monto, char campos[]);
	void AgregarMonto(char campos[], std::string &mensaje);
	void ConsultarMonto(char campos[], std::string &mensaje);
	void SetearMonto(char campos[], std::string &mensaje);
	void RegistrarTarjeta(char campos[], std::string &mensaje);
	void ForzarAgregarMonto(char campos[], std::string &mensaje);
	void EnviarRespuesta(char datos[], uint32_t &tam, char opcode);
	void ImprimirMensaje(std::string &transaccion, std::string &mensaje);
	std::string ConvertirSaldo(long int saldo);
};

#endif
