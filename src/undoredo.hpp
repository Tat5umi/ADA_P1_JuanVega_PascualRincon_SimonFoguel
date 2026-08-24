#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include <string>
#include <stdexcept>
#include <iostream>

enum class TipoOp {Insert, Delete, Replace};
struct Registro{
TipoOp tipo;
size_t pos;
std::string antes;
std::string despues;
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
               const std::string& Obtenertexto() const {
                     return texto;
               }

      private:
              std::string texto;
};

Void Deshacer{Documento& doc, const Registro& reg} {
    doc.Modificar (reg.pos, reg.despues.size(), reg.antes);
}

Void Rehacer{Documento& doc, const Registro& reg} {
    doc.Modificar (reg.pos, reg.antes.size(), reg.despues);
#endif
