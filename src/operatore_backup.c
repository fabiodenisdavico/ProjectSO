#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "config.h"
#include "strutture.h"
#include "ipc_utils.h"
#include "processi.h"

void processo_operatore(int id, int shm_id, int sem_id, int msg_id) {
    printf("[OPERATORE %d] Avvio (PID=%d)\n", id, getpid());
    
    // Attach alla shared memory
    Stats *stats = (Stats*)shm_attach(shm_id);
    if (!stats) {
        fprintf(stderr, "[OPERATORE %d] Errore attach shared memory\n", id);
        exit(1);
    }
    
    Messaggio msg;
    
    // Loop principale: riceve e processa ordini
    while (1) {
        // Riceve messaggio dalla coda (tipo 1 = ordini)
        if (msgrcv(msg_id, &msg, sizeof(msg) - sizeof(long), 
                   MSG_TYPE_ORDINE, 0) < 0) {
            perror("msgrcv");
            break;
        }
        
        // Controlla messaggio di terminazione
        if (msg.id_utente == -1) {
            printf("[OPERATORE %d] Ricevuto segnale terminazione\n", id);
            break;
        }
        
        printf("[OPERATORE %d] Ricevuto ordine da UTENTE %d: %s\n", 
               id, msg.id_utente, msg.piatto);
        
        // Simula preparazione piatto
        sleep(TEMPO_PREPARAZIONE);
        printf("[OPERATORE %d] Piatto preparato: %s\n", id, msg.piatto);
        
        // Aggiorna statistiche in sezione critica
        sem_wait(sem_id);  // <<<< LOCK
        stats->piatti_distribuiti++;
        printf("[OPERATORE %d] Statistiche: Piatti distribuiti=%d\n", 
               id, stats->piatti_distribuiti);
        sem_signal(sem_id);  // >>>> UNLOCK
        
        // Invia risposta all'utente
        Messaggio risposta;
        risposta.mtype = MSG_TYPE_RISPOSTA + msg.id_utente;  // ← CORRETTO!
        risposta.id_utente = msg.id_utente;  // ← Mantieni l'id dell'utente
        strcpy(risposta.piatto, msg.piatto);  // ← Copia il piatto vero!
        if (msgsnd(msg_id, &risposta, sizeof(risposta) - sizeof(long), 0) < 0) {
            perror("msgsnd risposta");
        } else {
            printf("[OPERATORE %d] ✓ Risposta inviata a UTENTE %d: %s\n", 
                id, msg.id_utente, risposta.piatto);
        }
        
        // Controlla se deve terminare
        sem_wait(sem_id);
        int attiva = stats->simulazione_attiva;
        sem_signal(sem_id);
        
        if (!attiva) {
            printf("[OPERATORE %d] Simulazione terminata\n", id);
            break;
        }
    }
    
    // Cleanup
    shm_detach(stats);
    printf("[OPERATORE %d] Terminato\n", id);
    exit(0);
}