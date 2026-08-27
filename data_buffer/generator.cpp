#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <cmath> // Necesario para std::log

struct Params {
    uint64_t seed = 42;
    long long n = 10000;
    std::string out = "paquetes_n10000.txt";
    double mean_interarrival_ms = 1.8;
    int size_min = 64;
    int size_max = 1500;
    double burst_prob = 0.0005;
    int burst_len = 50;
    double burst_interarrival_ms = 0.1;
};

static bool parse_arg(std::string_view arg, std::string_view key, std::string& value) {
    if (arg.size() > key.size() + 3 && arg.substr(0, 2) == "--" &&
        arg.substr(2, key.size()) == key && arg[2 + key.size()] == '=') {
        value = std::string(arg.substr(2 + key.size() + 1));
        return true;
    }
    return false;
}

static Params parse_params(int argc, char** argv) {
    Params p;
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == nullptr) continue;
        std::string_view arg = argv[i];
        std::string v;
        if (parse_arg(arg, "seed", v)) p.seed = std::stoull(v);
        else if (parse_arg(arg, "n", v)) p.n = std::stoll(v);
        else if (parse_arg(arg, "out", v)) p.out = v;
        else if (parse_arg(arg, "mean-interarrival", v)) p.mean_interarrival_ms = std::stod(v);
        else if (parse_arg(arg, "size-min", v)) p.size_min = std::stoi(v);
        else if (parse_arg(arg, "size-max", v)) p.size_max = std::stoi(v);
        else if (parse_arg(arg, "burst-prob", v)) p.burst_prob = std::stod(v);
        else if (parse_arg(arg, "burst-len", v)) p.burst_len = std::stoi(v);
        else if (parse_arg(arg, "burst-interarrival", v)) p.burst_interarrival_ms = std::stod(v);
    }
    return p;
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false); // Aceleración nativa de I/O
    Params p = parse_params(argc, argv);

    std::mt19937_64 rng(p.seed);
    std::uniform_int_distribution<int> distrib(1,5);
    std::uniform_int_distribution<int> size_dist(p.size_min, p.size_max);
    std::uniform_real_distribution<double> unit(0.0, 1.0);

    std::ofstream out(p.out, std::ios::binary);
    if (!out) {
        std::cerr << "No se pudo abrir el archivo de salida: " << p.out << "\n";
        return 1;
    }

    out << p.n << '\n';

    double t_ms = 0.0;
    long long remaining_burst = 0;
    long long total_burst_packets = 0;

    for (long long i = 0; i < p.n; ++i) {
        double dt;
        // Generación exponencial manual segura utilizando la distribución uniforme
        double u = unit(rng);
        if (u >= 1.0) u = 0.9999999999; // Protección matemática contra log(0)

        if (remaining_burst > 0) {
            dt = -std::log(1.0 - u) * p.burst_interarrival_ms;
            --remaining_burst;
            ++total_burst_packets;
        } else {
            dt = -std::log(1.0 - u) * p.mean_interarrival_ms;
            if (unit(rng) < p.burst_prob) {
                remaining_burst = p.burst_len;
            }
        }
        t_ms += dt;

        int size = size_dist(rng);
        long long ts_ms = static_cast<long long>(t_ms);
        out << ts_ms << ' ' << size << ' ' << distrib(rng) << '\n';
    }

    out.close();
    std::cerr << "Generado " << p.n << " paquetes en " << p.out << " (seed=" << p.seed << ")\n";
    return 0;
}
