#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CHIAVI IPC
// ============================================================
#define SHM_KEY 1234        // Chiave shared memory
#define SEM_KEY 5678        // Chiave semaforo
#define MSG_KEY 9012        // Chiave coda messaggi

// ============================================================
// PARAMETRI SIMULAZIONE
// ============================================================
#define NUM_OPERATORI 2     // Numero operatori
#define NUM_UTENTI 20        // Numero utenti per test

// ============================================================
// TIPI MESSAGGIO
// ============================================================
#define MSG_TYPE_ORDINE 1           // Tipo messaggio ordine
#define MSG_TYPE_RISPOSTA_BASE 100  // Base per risposte (100 + id_utente)

// ============================================================
// TEMPI SIMULAZIONE (secondi)
// ============================================================
#define TEMPO_PREPARAZIONE 15        // Tempo preparazione piatto
#define TEMPO_CONSUMAZIONE 2        // Tempo consumazione pasto
#define TEMPO_ARRIVO 1              // Intervallo arrivi utenti

// ============================================================
// STRINGHE
// ============================================================
#define MAX_PIATTO_LEN 50

typedef enum {
    CAT_PRIMI = 0,
    CAT_SECONDI = 1,
    CAT_DOLCI = 2,
    CAT_CAFFE = 3,
    CAT_CONTORNI = 4,  // ← NUOVO
    NUM_CATEGORIE = 5   // ← AGGIORNA
} Categoria;

#endif // CONFIG_H