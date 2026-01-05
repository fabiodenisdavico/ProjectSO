// include/piatti.h

#ifndef PIATTI_H
#define PIATTI_H

#include "strutture.h"
#include "config.h"

// Carica piatti da file
int carica_piatti(ListaPiatti *lista);

// Ottieni piatto casuale per categoria
const char* piatto_casuale_per_categoria(ListaPiatti *lista, Categoria cat);

// Stampa menu completo
void stampa_piatti(ListaPiatti *lista);

// Nomi categorie (per output)
extern const char* NOMI_CATEGORIE[NUM_CATEGORIE];

#endif