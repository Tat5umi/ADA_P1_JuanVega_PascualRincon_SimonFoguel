// Generador de eventos sinteticos para el Problema 1 (Undo/Redo).
// Semilla fija: 6012009 (fecha 06/01/2009). Documentada en el README.
// Distribucion objetivo: 60% INSERT, 15% DELETE, 15% UNDO, 10% REDO.
// El generador simula las dos pilas del motor para conocer en todo momento
// el largo real del documento. Sin esa simulacion, los UNDO/REDO desplazan
// el largo estimado y casi la mitad de las posiciones caen fuera de rango.
// Solo se guarda el delta de longitud de cada operacion, no su contenido.

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

const unsigned int SEMILLA = 6012009;

void generar(int n, const std::string& ruta){
  std::ofstream salida(ruta);
  if (!salida){
    std::cerr <<"no se pudo crear " << ruta << "\n";
    return;
  }
  
  std::mt19937  rng(SEMILLA);
  std::uniform_int_distribution<int> dadoAccion(1,100);
  std::uniform_int_distribution<int> dadoLetra(1,25);

  salida <<"# generado con semilla " << SEMILLA << ", n = " << "\n";

  long long  largo = 0;
  std::vector<long long> undo;
  std::vector<long long> redo;

  for (int i = 0; i < n; ++i){
    int accion = dadoAccion(rng);

    if(accion <= 60){
      std::uniform_int_distribution<long long> dadoPos(0, largo);
      long long pos = dadoPos(rng)
      char letra = 'a' + dadoLetra(rng);
      salida <<"EDIT INSERT " << pos << " " << letra << "\n";
      largo += 1;
      undo.push_back(+1);
      redo.clear();

    } else if (accion <= 75 && largo > 0)  
      std::uniform_int_distribution<long long> dadoPos(0, largo - 1);
      long long pos = dadoPos(rng)
      salida <<"EDIT DELETE " << pos << " " << " 1\n";
      largo -= 1;
      undo.push_back(-1);
      redo.clear();

    } else if (accion <= 90){
      salida <<"UNDO\n";
      if (!undo.empty()){
        long long delta = undo.back();
        undo.pop_back();
        largo -= delta;
        redo.push_back(delta);
      }

    } else {
      salida <<"REDO\n";
      if (!redo.empty()){
        long long delta = redo.back();
        redo.pop_back();
        largo += delta;
        undo.push_back(delta);
      }
    }
  }
  std::cout <<"generado " << ruta << " con " << n << "eventos\n";
}

int main(){
  generar(1000, "eventos_n1000.txt");
  generar(10000, "eventos_n10000.txt");
  generar(100000, "eventos_n100000.txt");
  generar(1000000, "eventos_n1000000.txt");
}

        
