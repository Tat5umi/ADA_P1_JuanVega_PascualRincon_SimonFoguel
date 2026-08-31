// Experimentacion del Problema 1 (Undo/Redo) - Seccion 9 del enunciado.
// Mide el tiempo de procesamiento para n en {1000, 10000, 100000, 1000000},
// con 5 repeticiones por tamano, sobre las dos representaciones de pila.
// Reloj: std::chrono::steady_clock (monotono). Se reporta en ms con 2 decimales.

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include "undoredo.hpp"

const int REPETICIONES = 5;

struct Medicion {
  double media;
  double desviacion;
  int efectivos;
  int noOps;
};

template <typename Pila>
Medicion medir(const Vector<Evento>& eventos){
  double tiempos[REPETICIONES];
  int efectivos = 0;
  int noOps = 0;

  for (int r = 0; r < REPETICIONES; ++r){
    Motor<Pila> motor;

    auto inicio = std::chrono::steady_clock::now();
    motor.Procesar(eventos);
    auto fin = std::chrono::steady_clock::now();

    tiempos[r] = std::chrono::duration<double, std::milli > (fin - inicio).count();
    efectivos = motor.Efectivos();
    noOps = motor.NoOps();
      }

  double suma = 0.0;
  for (int r = 0; r < REPETICIONES; ++r){
    suma += tiempos[r];
  }
  double media = suma / REPETICIONES;

  double acumulado = 0.0;
  for (int r = 0; r < REPETICIONES; ++r){
    double d = tiempos[r] - media;
    acumulado += d * d;
  }
  double desviacion = std::sqrt(acumulado / (REPETICIONES -1));

  Medicion m;
  m.media = media;
  m. desviacion = desviacion;
  m.efectivos = efectivos;
  m.noOps = noOps;
  return m;
}

void imprimirFila(int n, const std::string& pila, const Medicion& m){
  std::cout << std::setw(9) << n
  << std::setw(10) << pila
  << std::setw(12) << std::fixed << std::setprecision(8) << m.media
  << std::setw(12) << m.desviacion
  << std::setw(12) << m.efectivos
  << std::setw(10) << m.noOps << "\n";
}

int main(){
  const int tamanos[] = {1000, 10000, 100000, 1000000};
  std::cout << std::setw(9) << "n"
  << std::setw(10) << "pila"
  << std::setw(12) << "media_ms"
  << std::setw(12) << "desv_ms"
  << std::setw(12) << "efectivos"
  << std::setw(10) << "no_ops" << "\n";

  for (int t = 0; t < 4; ++t){
    int n = tamanos[t];
    std::string ruta = "eventos_n" + std::to_string(n) + ".txt";

    try {
      ResultadoLectura datos = LeerArchivo(ruta);

      imprimirFila(n, "arreglo", medir<ArrayStack<Registro>>(datos.eventos));
      imprimirFila(n, "lista", medir<LinkedStack<Registro>>(datos.eventos));
   
    } catch (const std::exception& error) {
      std::cerr << "error con " << ruta << ": " << error.what() << "\n";
    }
  }
  return 0;
}
