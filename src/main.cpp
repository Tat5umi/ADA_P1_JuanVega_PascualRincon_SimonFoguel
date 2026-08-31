#include <iostream>
#include "undoredo.hpp"

int main(int argc, char* argv[]){
  if(argc < 2){
        std::cerr << "uso: " << argv[0] << " <archivo_de_eventos>\n";
        return 1;
  }
  try {
       ResultadoLectura datos = LeerArchivo(argv[1]);
       Motor<ArrayStack<Registro>> motor;
       motor.Procesar(datos.eventos);
       Reportar(motor, datos.errores, std::cout);
  } catch(const std::exception& error) {
       std::cerr <<"error: " << error.what() << "\n";
       return 1;
  }
  return 0;
}
