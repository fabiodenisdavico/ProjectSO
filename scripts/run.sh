#!/bin/bash

# Pulisco prima eventuali risorse rimaste
./clean_ipc.sh

# Compilo
make clean
make

# Eseguo
./mensa

# Pulisco dopo
./clean_ipc.sh