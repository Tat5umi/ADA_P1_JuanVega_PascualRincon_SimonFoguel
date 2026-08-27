#!/usr/bin/env bash
# generate_all.sh
set -euo pipefail
cd "$(dirname "$0")"

SEED=42
# Compilación segura sin optimizaciones que corrompan la memoria del entorno
g++ -std=c++17 -o generator generator.cpp

for N in 1000 10000 100000 1000000; do
    ./generator --n=$N --seed=$SEED --out=paquetes_n$N.txt
done

echo "Listo. Semilla usada: $SEED"
