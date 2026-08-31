#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include <string>
#include <stdexcept>
#include <sstream>

enum class TipoOp {Insert, Delete, Replace};

struct Registro{
TipoOp tipo;
size_t pos;
std::string antes;
std::string despues;
};

enum class TipoEvento { Edicion, Undo, Redo };

struct Evento{
TipoEvento clase;
TipoOp op;
size_t pos;
size_t cuantos;
std::string contenido;
};

class Documento{
      public:
              Documento() = default; 

              void Modificar(size_t pos, size_t cuantosQuitar, const std::string& textoNuevo) {
              if (pos > texto.size()){
                throw std::runtime_error ("La posicion no esta en el documento");
              }
              if (pos + cuantosQuitar > texto.size()){
                throw std::runtime_error ("La cantidad a eliminar excede el numero de caracteres del documento");
                }
               texto.replace(pos, cuantosQuitar, textoNuevo);
              }

             std::string Leer(size_t pos, size_t largo) const {
              if (pos > texto.size()){
                throw std::runtime_error ("La posicion no esta en el documento");
              }
              if (pos + largo > texto.size()){
                throw std::runtime_error ("La cantidad a leer excede el numero de caracteres del documento");
                }
                  return texto.substr(pos, largo);
                    }
               const std::string& ObtenerTexto() const {
                     return texto;
               }

      private:
              std::string texto;
};

inline void Deshacer(Documento& doc, const Registro& reg) {
    doc.Modificar (reg.pos, reg.despues.size(), reg.antes);
}

inline void Rehacer(Documento& doc, const Registro& reg) {
    doc.Modificar (reg.pos, reg.antes.size(), reg.despues);
}

inline Evento LeerEvento(const std::string& linea){

      std::istringstream flujo(linea);
      std::string comando;

      if (!(flujo >> comando)) {  
         throw std::runtime_error ("linea vacia");
      }

      if (comando == "UNDO") {
          Evento e;
          e.clase = TipoEvento::Undo;
          e.op = TipoOp::Insert;
          e.pos = 0;
          e.cuantos = 0;
          return e; 
      } else if (comando == "REDO") {
          Evento e;
          e.clase = TipoEvento::Redo;
          e.op = TipoOp::Insert;
          e.pos = 0;
          e.cuantos = 0;
          return e; 
      } else if (comando == "EDIT"){
            std::string tipo;
            if  (!(flujo >> tipo)){
                  throw std::runtime_error ("falta el tipo de edicion");
            }
            size_t pos;
            if  (!(flujo >> pos)){
                  throw std::runtime_error ("falta la posicion");
            }
            if (tipo == "INSERT"){
                  std::string contenido;
                  std::getline(flujo, contenido);
                  if(!contenido.empty() && contenido.front() == ' '){
                        contenido.erase(0,1);
                  }
                  Evento e;
                  e.clase = TipoEvento::Edicion;
                  e.op = TipoOp::Insert;
                  e.pos = 0;
                  e.cuantos = 0;
                  return e; 
            } else if (tipo == "DELETE"){
                  size_t cuantos;
                  if  (!(cuantos >> tipo)){
                        throw std::runtime_error ("falta la cantidad a borrar");
                  }
                  Evento e;
                  e.clase = TipoEvento::Edicion;
                  e.op = TipoOp::Delete;
                  e.pos = 0;
                  e.cuantos = 0;
                  return e; 
            } else if (tipo == "REPLACE") {
                  size_t cuantos;
                  if  (!(cuantos >> tipo)){
                        throw std::runtime_error ("falta la cantidad a remplazar");
                  }
                  std::string contenido;
                  std::getline(flujo, contenido);
                  if(!contenido.empty() && contenido.front() == ' '){
                        contenido.erase(0,1);
                  }
                  Evento e;
                  e.clase = TipoEvento::Edicion;
                  e.op = TipoOp::Replace;
                  e.pos = 0;
                  e.cuantos = 0;
                  return e; 
            } else {
                  throw std::runtime_error ("tipo de edicion desconocido:" + tipo);
            }            
      } else {
          throw std::runtime_error("Comando desconocido:" + comando);
      }
}



#endif
