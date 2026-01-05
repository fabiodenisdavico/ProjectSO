// src/piatti.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "strutture.h"

// ============================================================
// NOMI CATEGORIE (per output)
// ============================================================
const char* NOMI_CATEGORIE[NUM_CATEGORIE] = {
    "PRIMI",
    "SECONDI",
    "CONTORNI",    // ← AGGIUNTO
    "DOLCI",
    "CAFFE"
};

// ============================================================
// CONVERTI STRINGA -> CATEGORIA
// ============================================================
int string_to_categoria(const char *str) {
    if (strcmp(str, "PRIMI") == 0) return CAT_PRIMI;
    if (strcmp(str, "SECONDI") == 0) return CAT_SECONDI;
    if (strcmp(str, "CONTORNI") == 0) return CAT_CONTORNI;  // ← AGGIUNTO
    if (strcmp(str, "DOLCI") == 0) return CAT_DOLCI;
    if (strcmp(str, "CAFFE") == 0) return CAT_CAFFE;
    return -1;
}

// ============================================================
// CARICA PIATTI DA FILE (FORMATO PIPE)
// ============================================================
int carica_piatti(ListaPiatti *lista) {
    FILE *file = fopen(FILE_PIATTI, "r");
    
    // Inizializza contatori
    for (int i = 0; i < NUM_CATEGORIE; i++) {
        lista->num_piatti[i] = 0;
    }
    
    if (file == NULL) {
        fprintf(stderr, "[ERRORE] Impossibile aprire %s\n", FILE_PIATTI);
        
        // Piatti di default
        strcpy(lista->piatti[CAT_PRIMI][0], "Pasta al pomodoro");
        lista->num_piatti[CAT_PRIMI] = 1;
        
        strcpy(lista->piatti[CAT_SECONDI][0], "Cotoletta");
        lista->num_piatti[CAT_SECONDI] = 1;
        
        strcpy(lista->piatti[CAT_CONTORNI][0], "Insalata");  // ← AGGIUNTO
        lista->num_piatti[CAT_CONTORNI] = 1;
        
        strcpy(lista->piatti[CAT_DOLCI][0], "Tiramisù");
        lista->num_piatti[CAT_DOLCI] = 1;
        
        strcpy(lista->piatti[CAT_CAFFE][0], "Caffè");
        lista->num_piatti[CAT_CAFFE] = 1;
        
        printf("[WARNING] Uso piatti di default\n");
        return -1;
    }
    
    char buffer[512];
    int totale_piatti = 0;
    
    // Leggi riga per riga
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strlen(buffer) == 0) continue;
        
        // Estrai categoria (primo token)
        char *token = strtok(buffer, "|");
        if (token == NULL) continue;
        
        int cat = string_to_categoria(token);
        if (cat == -1) {
            fprintf(stderr, "[WARNING] Categoria non valida: %s\n", token);
            continue;
        }
        
        // Estrai tutti i piatti
        while ((token = strtok(NULL, "|")) != NULL) {
            if (lista->num_piatti[cat] < MAX_PIATTI_PER_CATEGORIA) {
                strcpy(lista->piatti[cat][lista->num_piatti[cat]], token);
                lista->num_piatti[cat]++;
                totale_piatti++;
            }
        }
    }
    
    fclose(file);
    
    printf("[INFO] Caricati %d piatti da %s\n", totale_piatti, FILE_PIATTI);
    return 0;
}

// ============================================================
// OTTIENI PIATTO CASUALE PER CATEGORIA
// ============================================================
const char* piatto_casuale_per_categoria(ListaPiatti *lista, Categoria cat) {
    if (lista->num_piatti[cat] == 0) {
        return "Non disponibile";
    }
    
    int indice = rand() % lista->num_piatti[cat];
    return lista->piatti[cat][indice];
}

// ============================================================
// STAMPA MENU COMPLETO
// ============================================================
void stampa_piatti(ListaPiatti *lista) {
    printf("\n========================================\n");
    printf("      MENU DEL GIORNO\n");
    printf("========================================\n");
    
    for (int cat = 0; cat < NUM_CATEGORIE; cat++) {
        printf("\n--- %s (%d) ---\n", NOMI_CATEGORIE[cat], lista->num_piatti[cat]);
        
        for (int i = 0; i < lista->num_piatti[cat]; i++) {
            printf("  • %s\n", lista->piatti[cat][i]);
        }
    }
    
    printf("\n========================================\n\n");
}