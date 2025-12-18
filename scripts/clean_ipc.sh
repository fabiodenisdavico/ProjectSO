#!/bin/bash

echo "Pulisco risorse IPC..."

# Rimuovo shared memory
ipcrm -M 1234 2>/dev/null

# Rimuovo semaforo  
ipcrm -S 5678 2>/dev/null

# Rimuovo coda messaggi
ipcrm -Q 9012 2>/dev/null

echo "Fatto!"