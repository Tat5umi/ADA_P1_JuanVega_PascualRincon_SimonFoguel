#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include <string>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include "Vector.hpp"
#include "stack_array.hpp"
#include "stack_list.hpp"

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
                throw std::runtime_error ("la cantidad a eliminar excede el numero de caracteres del documento");
                }
               texto.replace(pos, cuantosQuitar, textoNuevo);
              }

             std::string Leer(size_t pos, size_t largo) const {
              if (pos > texto.size()){
                throw std::runtime_error ("la posicion no esta en el documento");
              }
              if (pos + largo > texto.size()){
                throw std::runtime_error ("la cantidad a leer excede el numero de caracteres del documento");
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
                  throw std::runtime_error ("posicion invalida o ausente");
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
                  e.pos = pos;
                  e.cuantos = 0;
                  e.contenido = contenido;
                  return e; 
            } else if (tipo == "DELETE"){
                  size_t cuantos;
                  if  (!(flujo >> cuantos)){
                        throw std::runtime_error ("falta la cantidad a borrar");
                  }
                  Evento e;
                  e.clase = TipoEvento::Edicion;
                  e.op = TipoOp::Delete;
                  e.pos = pos;
                  e.cuantos = cuantos;
                  return e; 
            } else if (tipo == "REPLACE") {
                  size_t cuantos;
                  if  (!(flujo >> cuantos)){
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
                  e.pos = pos;
                  e.cuantos = cuantos;
                  e.contenido = contenido;
                  return e; 
            } else {
                  throw std::runtime_error ("tipo de edicion desconocido: " + tipo);
            }            
      } else {
          throw std::runtime_error("comando desconocido: " + comando);
      }
}

struct ResultadoLectura {
      Vector<Evento> eventos;
      Vector<std::string> errores;
};

 inline ResultadoLectura LeerArchivo(const std::string& ruta){
       std::ifstream archivo(ruta);
       if (!archivo){
            throw std::runtime_error("no se puede abrir el archivo: " + ruta); 
       }
       ResultadoLectura resultado;
       std::string linea;
       int numero = 0;
       while (std::getline(archivo, linea)){
             ++numero;
             if (linea.empty() || linea.front() == '#' ){
                  continue;
             }
             try {
                   resultado.eventos.push_back(LeerEvento(linea));
             } catch (const std::exception& error) {
                   resultado.errores.push_back("linea " + std::to_string(numero) + ": " + error.what());
             }
        }
        return resultado;
 }

template <typename Pila>
class Motor {
public:
      void AplicarEdicion(const Evento& evento){
            if (evento.cuantos == 0 && evento.contenido.empty()){
                  log.push_back("edicion nula, se ignora");
                  return;
                        }
            std::string antes = doc.Leer(evento.pos, evento.cuantos);
            doc.Modificar (evento.pos, evento.cuantos, evento.contenido);
            undo.push (Registro{evento.op, evento.pos, antes, evento.contenido});
            redo.clear();
            }

     void HacerUndo(){
           if(undo.empty()){
                 log.push_back("Pila vacia, UNDO sin efecto");
                 return;
           }
           Registro reg = undo.top();
           undo.pop();
           Deshacer(doc, reg);
           redo.push(reg);
           log.push_back("UNDO aplicado");
     }

     void HacerRedo(){
           if(redo.empty()){
                 log.push_back("Pila vacia, REDO sin efecto");
                 return;
           }
           Registro reg = redo.top();
           redo.pop();
           Rehacer(doc, reg);
           undo.push(reg);
           log.push_back("REDO aplicado");
     }

     void Procesar(const Vector<Evento>& eventos){
           for (int i = 0; i < eventos.size(); i++){
                 if (eventos[i].clase == TipoEvento::Edicion){
                       try{
                             AplicarEdicion(eventos[i]);
                       } catch (const std::exception& error) {
                             log.push_back ("evento " + std::to_string(i+1) + ": " + error.what());
                       }
                 } else if (eventos[i].clase == TipoEvento::Undo){
                       HacerUndo();
                 } else {
                       HacerRedo();
                 }
           }
     }

     const std::string& TextoFinal() const {return doc.ObtenerTexto();}
     const Vector<std::string>& Log() const {return log;}
     int TamUndo() const {return undo.size();}
     int TamRedo() const {return redo.size();}

private: 
      Documento doc;
      Pila undo;
      Pila redo;
      Vector<std::string> log;
};

#endif
