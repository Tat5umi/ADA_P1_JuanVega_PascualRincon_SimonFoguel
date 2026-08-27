#include <iostream>
#include <fstream>
#include <utility>
#include <ctime>
#include <ratio>
#include <chrono>
#include "Vector.hpp"
#include "queue_circular.hpp"
#include "queue_list.hpp"

int C;
int T;
int L;
int R;
int SAMPLE_TIME = 50;

struct sample{
    int marca_tiempo;
    int aceptados;
    int rechazados_lleno;
    int rechazados_tasa;
    int ocupacion_buff;
};

Vector<int> data;

void readData(){

    std::ifstream archivo("../data_buffer/paquetes_n1000000.txt"); 

    if (!archivo.is_open()) {
        std::cerr << "file couldn't be opened" << std::endl;
    }

    int n;
    archivo>>n;

    int time, size;

    while(archivo>>time>>size){
        data.push_back(time);
        data.push_back(size);
    }

}

void writeData(Vector<sample>& samples, int file){

    std::cout<<"in writeData()"<<std::endl;

    std::string file_loc = "../graphs/experiment_" + std::to_string(file) + ".csv";

    std::ofstream archivo(file_loc);

    if (!archivo.is_open()){
        std::cerr<<"unable to open output file";
    }

    std::cout<<"writing to: "<<file_loc<<std::endl;

    archivo<<"time,aceptados,rechazados_lleno,rechazados_tasa,ocupacion_buffer\n";

    for (auto itr = samples.begin(); itr != samples.end(); ++itr){
        archivo<<(*itr).marca_tiempo<<',';
        archivo<<(*itr).aceptados<<',';
        archivo<<(*itr).rechazados_lleno<<',';
        archivo<<(*itr).rechazados_tasa<<',';
        archivo<<(*itr).ocupacion_buff<<'\n';
    }
}

bool purgeExpired(int time, LinkedQueue<int>& times){
    int dt = time - T;
    while(!times.empty() && times.front() <= dt){
        times.dequeue();
    }
    return times.size() < L;
}

void processData(int c, int t, int l, int r, int file){
    
    std::cout<<"in processData()"<<std::endl;

    C = c;
    T = t;
    L = l;
    R = r;

    Vector<sample> samples;

    CircularQueue<double> buffer(C);
    LinkedQueue<int> timestamps;

    
    int aceptados = 0;
    int rechazados_lleno = 0;
    int rechazados_tasa = 0;
    int ultimo_muestreo = -SAMPLE_TIME;
    

    using namespace std::chrono;
    steady_clock::time_point t1 = steady_clock::now();

    int time;
    double bytes;
    for (int i = 0; i < data.size(); i+=2){
        time = data[i]; bytes = data[i+1];

        while(!buffer.empty() && buffer.front() <= time){
            buffer.dequeue();
        }

        if (!buffer.full() && purgeExpired(time, timestamps)){
            buffer.enqueue(std::max(static_cast<double>(time), buffer.back()) + bytes/R);
            timestamps.enqueue(time);
            ++aceptados;
        } else if (buffer.full()){
            ++rechazados_lleno;
        } else {
            ++rechazados_tasa;
        }

        if (time - ultimo_muestreo >= SAMPLE_TIME){
            samples.push_back({time, aceptados, rechazados_lleno, rechazados_tasa, buffer.size()});
            ultimo_muestreo = time;
            
        }
        
    }
    
    std::cout<<"aceptados: "<<aceptados<<std::endl;
    std::cout<<"rechazados lleno: "<<rechazados_lleno<<std::endl;
    std::cout<<"rechazados tasa: "<<rechazados_tasa<<std::endl;
    

    steady_clock::time_point t2 = steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
    std::cout<<"Tiempo ejecucion: "<<duration.count()<<std::endl;

    writeData(samples, file);
}

void processDataQueue(){

    std::cout<<"in processDataQueue()"<<std::endl;

    LinkedQueue<double> buffer;
    LinkedQueue<int> timestamps;
    
    using namespace std::chrono;
    steady_clock::time_point t1 = steady_clock::now();

    int time;
    double bytes;
    for (int i = 0; i < data.size(); i+=2){
        time = data[i]; bytes = data[i+1];
        
        while(!buffer.empty() && buffer.front() <= time){
            buffer.dequeue();
        }

        if (buffer.size() < C && purgeExpired(time, timestamps)){
            buffer.enqueue(std::max(static_cast<double>(time), buffer.empty()? 0 : buffer.back()) + bytes/R);
            timestamps.enqueue(time);
        } 
    }

    steady_clock::time_point t2 = steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
    std::cout<<"Tiempo ejecucion: "<<duration.count()<<std::endl;

}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream archivo("../data_buffer/parameters.txt");

    if(!archivo.is_open()){
        return 1;
    }

    int c, t, l, r;
    int i = 1;

    readData();

    
    
    while(archivo>>c>>t>>l>>r){
        processData(c, t, l, r, i);
        processDataQueue();
        ++i;
    }
    

    std::cout<<"process completed"<<std::endl;


    return 0;
}