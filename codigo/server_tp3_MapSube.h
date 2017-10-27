#ifndef SERVER_TP3_MAPSUBE_H_
#define SERVER_TP3_MAPSUBE_H_

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#define ESTA true
#define NO_ESTA false

class MapSube {
private:
	std::map<unsigned int, long int> hashMap;
	std::mutex mutexSube;
public:
	MapSube();
	bool EstaVacia();
	void Insertar(unsigned int unID, long int unMonto);
	long int GetValue(unsigned int unID);
	unsigned int Tamanio();
	bool EstaElemento(unsigned int unID);
	void SetValue(unsigned int unID, long int nuevoMonto);
	void AgregarMonto(unsigned int unID, long int nuevoMonto);
	~MapSube();
};

#endif
