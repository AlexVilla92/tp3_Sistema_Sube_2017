#include "client_tp3_Cliente.h"
#include <string>

Cliente::Cliente(char *argv[]) {
	this->host = std::string(argv[1]);
	this->port = std::stoi(argv[2]);
	this->fileName = std::string(argv[3]);
}

int Cliente::ProcesarDatos() {
	Socket socketClient;
	socketClient.Connect(this->host.c_str(), this->port);
	std::ifstream input;
	input.open(this->fileName, std::ios::in | std::ios::binary);
	if (!input.is_open()){
        throw std::string("Archivo Invalido...");
	}

	while (true) {
		if (input.peek() == EOF) break;	

		uint16_t cheksum;
		input.read(reinterpret_cast<char*>(&cheksum), sizeof(uint16_t));
		cheksum = ntohs(cheksum);	
		
		uint32_t inputTarjeta;
		input.read(reinterpret_cast<char*>(&inputTarjeta), sizeof(uint32_t));
		inputTarjeta = ntohl(inputTarjeta);

		const uint16_t chekTarjeta = (cheksum & 0xF800) >> 11;
		const uint16_t chekMonto = (cheksum & 0x7C0) >> 6;
		const uint16_t op = (cheksum & 0x38) >> 3;

		std::bitset<5> bitsChekTarjeta(chekTarjeta);
		std::bitset<5> bitsChekMonto(chekMonto);
		std::bitset<32> bitsCampoTarjeta(inputTarjeta); 
		
		const char operacion = this->DeterminarOperacion(op);

		if (! this->ChecksumOK(bitsCampoTarjeta, bitsChekTarjeta)) {
			int monto = 0;
			this->MensajeError(operacion, bitsCampoTarjeta.to_ulong(), monto);
			continue;
		}

		if ((op == DOS) || (op == TRES)) {
			this->EnviarCampoTarjeta(socketClient,
									 bitsCampoTarjeta,
									 operacion, bitsChekMonto);
		} else {
			uint32_t inputMonto;
			input.read(reinterpret_cast<char*>(&inputMonto),
					sizeof(uint32_t));
			inputMonto = ntohl(inputMonto);

			std::bitset<32> bitsCampoMonto(inputMonto);
			if (! this->ChecksumOK(bitsCampoMonto,bitsChekMonto)) {
				this->MensajeError(operacion, bitsCampoTarjeta.to_ulong(),
								   bitsCampoMonto.to_ulong());
				continue;
			}
			this->EnviarCampoTarjetaYmonto(socketClient,
										   bitsCampoTarjeta,
										   bitsCampoMonto,
										   operacion);
		}
	}
	input.close();
	socketClient.Shutdown();
	return EXITO;
}

void Cliente::MensajeError(char op, unsigned int numTarjeta, int monto) {
	std::string campo = this->ConvertirValor(numTarjeta);

	if (monto) {
		std::string strMonto = this->ConvertirValor(monto);
		campo = campo + strMonto;
	}

	std::string msg = op + campo + " -> E00001" + '\n';
    std::cout << msg;	
}

void Cliente::EnviarCampoTarjetaYmonto(Socket &socket,
									   std::bitset<32> campoTarjeta, 
								       std::bitset<32> campoMonto,
								       char operacion) {
	unsigned int numTarjeta = campoTarjeta.to_ulong();
	int numMonto = campoMonto.to_ulong();

	std::string tarjeta = std::to_string(numTarjeta);
	std::stringstream ss;
	ss << std::setw(10) << std::setfill('0') << tarjeta;
	tarjeta = ss.str();

	std::string monto = this->ConvertirValor(numMonto);
	std::string texto = tarjeta + monto;
	this->EnviarRecibirMensajes(operacion, socket,texto);
}

void Cliente::EnviarCampoTarjeta(Socket &socket, std::bitset<32> campoTarjeta,
								 char operacion,std::bitset<5> bitsChekMonto) {
	unsigned int numTarjeta = campoTarjeta.to_ulong();

	if (bitsChekMonto.to_ulong() == 0) {
		std::string texto = std::to_string(numTarjeta);
		std::stringstream ss;
		ss << std::setw(10) << std::setfill('0') << texto;
		texto = ss.str();

		this->EnviarRecibirMensajes(operacion, socket, texto);
	} else {
		int monto = 0;
		this->MensajeError(operacion, numTarjeta, monto);
	}
}

void Cliente::EnviarRecibirMensajes(char &op, Socket &socket, 
									std::string &texto) {
	char respuesta[MAX_BUFFER] = {0};
	texto = op + texto;
	uint32_t tamBuffer = texto.size();
	socket.Send(texto.c_str(),tamBuffer);
	socket.Receive(respuesta, sizeof(char));
	char opcode = respuesta[0];

    if (opcode == 'E') {
    	tamBuffer = 5; 
    } else if (opcode == 'R') {
    	tamBuffer = 10;
    } else {
    	tamBuffer = 20;
    }

    socket.Receive(respuesta, tamBuffer);
  	respuesta[tamBuffer] = '\0';
  	std::string mensaje = opcode + std::string(respuesta) + '\n';
  	std::cout << mensaje; 
}

char Cliente::DeterminarOperacion(uint16_t opcode) {
	char operacion;

	switch (opcode) {
		case 0: operacion = 'A'; break;
		case 1: operacion = 'F'; break;	
		case 2: operacion = 'P'; break;
		case 3: operacion = 'R'; break;
		default: operacion = 'S'; break;
	}

	return operacion;	
}

std::string Cliente::ConvertirValor(int valor) {
	std::string strValor;

	if (valor < 0) {
		valor = std::abs(valor);
		strValor = std::to_string(valor);
		std::stringstream ss;
		ss << std::setw(9) << std::setfill('0') << strValor;
		strValor =  "-" + ss.str();
	} else {
		strValor = std::to_string(valor);
		std::stringstream ss;
		ss << std::setw(10) << std::setfill('0') << strValor;
		strValor = ss.str();
	}

	return strValor;
}

bool Cliente::ChecksumOK(std::bitset<32> campo, std::bitset<5> chekSum) {
	int cantUnos = campo.count();

	if (cantUnos == 32) {
		cantUnos = 0;
	}

	int valorBits = chekSum.to_ulong(); 

	return (cantUnos == valorBits);
}

Cliente::~Cliente() {}
