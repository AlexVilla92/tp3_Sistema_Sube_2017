#ifndef COMMON_TP3_EXCEPCIONES_H
#define COMMON_TP3_EXCEPCIONES_H

#include <exception>

struct NewSocketException : public std::exception {
   const char *what() const throw() {
      return "No se pudo abrir el socket... ";
   }
};

struct BindException : public std::exception {
   const char *what() const throw() {
      return "Error al realizar el bind... ";
   }
};

struct ConnectException : public std::exception {
   const char *what() const throw() {
      return "Error al realizar el connect... ";
   }
};

struct AcceptException : public std::exception {
   const char *what() const throw() {
      return "Error al realizar el accept... ";
   }
};

struct RecvException : public std::exception {
   const char *what() const throw() {
      return "Error al recibir los datos... ";
   }
};

struct SendException : public std::exception {
   const char *what() const throw() {
      return "Error al envira los datos... ";
   }
};

#endif 
