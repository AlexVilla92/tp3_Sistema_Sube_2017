#include "server_tp3_MapSube.h"
#include <string>
#include <map>

MapSube::MapSube() {}

MapSube::~MapSube() {}

void MapSube::Insertar(unsigned int unID, long int unMonto) {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	this->hashMap[unID] = unMonto;
}

long int MapSube::GetValue(unsigned int unID) {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	return this->hashMap[unID];
}

bool MapSube::EstaElemento(unsigned int unID) {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	auto it = hashMap.find(unID);
  	if (it != hashMap.end()) {
  		return ESTA;
  	} else {
  		return NO_ESTA;
  	}
}

void MapSube::SetValue(unsigned int unID, long int nuevoMonto) {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	std::map<unsigned int, long int>::iterator it = hashMap.find(unID); 
	if (it != hashMap.end()) {
		it->second = nuevoMonto;
	}
}

void MapSube::AgregarMonto(unsigned int unID, long int nuevoMonto) {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	std::map<unsigned int, long int>::iterator it = hashMap.find(unID); 
	if (it != hashMap.end()) {
		it->second += nuevoMonto;
	}
}

unsigned int MapSube::Tamanio() {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	return this->hashMap.size();
}

bool MapSube::EstaVacia() {
	std::unique_lock<std::mutex> myLock(this->mutexSube);
	return (this->hashMap.empty());
}
