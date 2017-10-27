#include "server_tp3_Aceptador.h"
#include <string>
#include <vector>

Aceptador::Aceptador(Socket &unSocket) : socketServer(unSocket) {}

Aceptador::~Aceptador() {
    for (const auto &item: this->threads) {
        item->Join();
        delete item;
    }
}

void Aceptador::run() {
    while (true) {
        Socket client;

        try {
            this->socketServer.Accept(client);
        } catch(const AcceptException &e) {
            break;
        }
        Operaciones *unaOperacion = new Operaciones(std::move(client), 
                                            std::ref(this->baseDeDatos));
        this->threads.push_back(unaOperacion);
        unaOperacion->Start();
    }
}
