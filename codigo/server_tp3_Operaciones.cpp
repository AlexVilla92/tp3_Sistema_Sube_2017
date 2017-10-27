#include "server_tp3_Operaciones.h"
#include <string>

Operaciones::Operaciones(Socket &&unSocket, MapSube &unaBase) : 
             cliente(std::move(unSocket)),
             baseDeDatos(unaBase) {}

void Operaciones::run() {
    while (true) {
        try {
            char opcode;
            this->cliente.Receive(&opcode, 1);

            uint32_t tam;
            tam = ((opcode == 'R') || (opcode == 'P')) ? 10 : 20;

            char datos[MAX_BUFFER] = {0};
            this->cliente.Receive(datos, tam);
            datos[tam] = '\0';

            this->EnviarRespuesta(datos, tam, opcode);
        } catch(const RecvException &e) {
            break;
        }
    }
}

void Operaciones::EnviarRespuesta(char datos[], uint32_t &tam, char opcode) {
    std::string mensaje = this->ProcesarOperacion(opcode, datos);
    std::string strDatos = std::string(datos);
    std::string transaccion = opcode + strDatos;
    std::string strMonto = "";

    if (opcode == CONSULTAR) {
        tam = 20;
    }
    if (mensaje.size() > LEN_ERROR) {
        tam += 1;
    } else {
        tam = 6;
    }

    this->cliente.Send(mensaje.c_str(),tam);
    mensaje = mensaje + '\n';

    this->ImprimirMensaje(transaccion, mensaje);
}

void Operaciones::ImprimirMensaje(std::string &transaccion, 
                                  std::string &mensaje) {
    if (mensaje.size() == TAM_ERROR) {
        std::unique_lock<std::mutex> myLock(this->mutexOperacion);
        std::cerr << transaccion << " -> " << mensaje;
    } else {
        std::unique_lock<std::mutex> myLock(this->mutexOperacion);
        std::cout << transaccion << " -> " << mensaje;
    }
}

std::string Operaciones::ProcesarOperacion(char operacion, char campos[]) {
    std::string mensaje = "";

    switch (operacion) {
        case 'A': this->AgregarMonto(campos,mensaje); break;
        case 'P': this->ConsultarMonto(campos,mensaje); break;  
        case 'S': this->SetearMonto(campos,mensaje); break;
        case 'R': this->RegistrarTarjeta(campos,mensaje); break;
        case 'F': this->ForzarAgregarMonto(campos,mensaje); break;
        default: std::cout << "opcode invalido " << std::endl; break;
    }

    if (mensaje.size() != LEN_ERROR) {
        mensaje = operacion + mensaje;
    }

    return mensaje;
}

void Operaciones::ObtenerValores(unsigned int &tarjeta, long int &monto, 
                                 char campos[]) {
    unsigned int unaTarjeta;
    long int unMonto;
    std::string strCampos = std::string(campos);
    std::string strTarjeta = strCampos.substr(0,10);
    std::string strMonto = strCampos.substr(10,20);

    if (strTarjeta.length() > MAX_LEN) {
        unaTarjeta = std::stoul(strTarjeta,nullptr,0);
    } else {
        unaTarjeta = std::stol(strTarjeta);
    }
    unMonto = std::stoi(strMonto);
    tarjeta = unaTarjeta;
    monto = unMonto;
}

void Operaciones::AgregarMonto(char campos[], std::string &mensaje) {
    unsigned int tarjeta;
    long int monto;

    this->ObtenerValores(tarjeta,monto,campos);

    if (this->baseDeDatos.EstaVacia()) {
        mensaje = "E00002";
    } else {
        if (this->baseDeDatos.EstaElemento(tarjeta)) {
            long int saldo = this->baseDeDatos.GetValue(tarjeta);
            if ((monto < 0) && (saldo < std::abs(monto))) {
                mensaje = "E00003";
            } else {
                std::string strTarjeta = std::string(campos).substr(0,10);
                this->baseDeDatos.AgregarMonto(tarjeta,monto);
                saldo = this->baseDeDatos.GetValue(tarjeta);
                std::string strSaldo = this->ConvertirSaldo(saldo);
                mensaje = strTarjeta + strSaldo;
            }
        } else {
            mensaje = "E00002";
        }
    }
}

void Operaciones::ConsultarMonto(char campos[], std::string &mensaje) {
    unsigned int tarjeta;

    if (std::string(campos).length() > MAX_LEN) {
        tarjeta = std::stoul(campos,nullptr,0);
    } else {
        tarjeta = std::stol(campos);
    }
    long int unMonto;

    if (this->baseDeDatos.EstaVacia()) {
        mensaje = "E00002";
    } else {
        if (this->baseDeDatos.EstaElemento(tarjeta)) {
            std::string campoTarjeta = std::string(campos);
            unMonto = this->baseDeDatos.GetValue(tarjeta);
            std::string strMonto = this->ConvertirSaldo(unMonto);
            mensaje = campoTarjeta + strMonto;
        } else {
            mensaje = "E00002";
        }
    }
}

void Operaciones::SetearMonto(char campos[], std::string &mensaje) {
    unsigned int tarjeta;
    long int unMonto;

    this->ObtenerValores(tarjeta,unMonto,campos);

    if (this->baseDeDatos.EstaVacia()) {
        mensaje = "E00002";
    } else {
        if (this->baseDeDatos.EstaElemento(tarjeta)) {
            std::string strTarjeta = std::string(campos).substr(0,10);
            std::string strMonto = std::string(campos).substr(10,20);
            this->baseDeDatos.SetValue(tarjeta,unMonto);
            mensaje = strTarjeta + strMonto;
        } else {
            mensaje = "E00002";
        }
    }
}

void Operaciones::RegistrarTarjeta(char campos[], std::string &mensaje) {
    unsigned int tarjeta;
    if (std::string(campos).length() > MAX_LEN) {
        tarjeta = std::stoul(campos,nullptr,0);
    } else {
        tarjeta = std::stol(campos);
    }

    if (this->baseDeDatos.EstaVacia()) {
        this->baseDeDatos.Insertar(tarjeta,0);
        mensaje = std::string(campos);
    } else {
        if (this->baseDeDatos.EstaElemento(tarjeta)) {
            mensaje = "E00004";
        } else {
            this->baseDeDatos.Insertar(tarjeta, 0);
            mensaje = std::string(campos);
        }
    }
}

void Operaciones::ForzarAgregarMonto(char campos[], std::string &mensaje) {
    unsigned int tarjeta;
    long int monto;

    this->ObtenerValores(tarjeta,monto,campos);

    if (this->baseDeDatos.EstaVacia()) {
        mensaje = "E00002";
    } else {
        if (this->baseDeDatos.EstaElemento(tarjeta)) {
            std::string strTarjeta = std::string(campos).substr(0,10);
            this->baseDeDatos.AgregarMonto(tarjeta,monto);
            long int saldo = this->baseDeDatos.GetValue(tarjeta);
            std::string strSaldo = this->ConvertirSaldo(saldo);
            mensaje = strTarjeta + strSaldo;
        } else {
            mensaje = "E00002";
        }
    }
}

std::string Operaciones::ConvertirSaldo(long int saldo) {
    std::string strSaldo;

    if (saldo < 0) {
        saldo = std::abs(saldo);
        strSaldo = std::to_string(saldo);
        std::stringstream ss;
        ss << std::setw(CEROS_9) << std::setfill(CEROS) << strSaldo;
        strSaldo =  "-" + ss.str();
    } else {
        strSaldo = std::to_string(saldo);
        std::stringstream ss;
        ss << std::setw(CEROS_10) << std::setfill(CEROS) << strSaldo;
        strSaldo = ss.str();
    }
    return strSaldo;
}

Operaciones::~Operaciones() {
    this->cliente.Shutdown();
}
