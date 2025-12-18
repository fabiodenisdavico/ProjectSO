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

void processo_utente(int id, int shm_id, int sem_id, int msg_id) {
    printf("[UTENTE %d] Arrivo in mensa (PID=%d)\n", id, getpid());
    
    // Attach alla shared memory
    Stats *stats = (Stats*)shm_attach(shm_id);
    if (!stats) {
        fprintf(stderr, "[UTENTE %d] Errore attach shared memory\n", id);
        exit(1);
    }
    
    // Simula arrivo
    sleep(TEMPO_ARRIVO);
    
    // Prepara e invia ordine
    Messaggio ordine;
    ordine.mtype = MSG_TYPE_ORDINE;
    ordine.id_utente = id;
    sprintf(ordine.piatto, "Pasta al pomodoro");
    
    printf("[UTENTE %d] Invio ordine: %s\n", id, ordine.piatto);
    
    if (msgsnd(msg_id, &ordine, sizeof(ordine) - sizeof(long), 0) < 0) {
        perror("msgsnd ordine");
        shm_detach(stats);
        exit(1);
    }
    
    // Attende risposta
    Messaggio risposta;
    printf("[UTENTE %d] Attendo preparazione...\n", id);
    
    long tipo_risposta = MSG_TYPE_RISPOSTA_BASE + id;
    if (msgrcv(msg_id, &risposta, sizeof(risposta) - sizeof(long), 
               tipo_risposta, 0) < 0) {
        perror("msgrcv risposta");
        shm_detach(stats);
        exit(1);
    }
    
    printf("[UTENTE %d] Ricevuto piatto da OPERATORE %d!\n", 
           id, risposta.id_utente);
    
    // Aggiorna statistiche
    sem_wait(sem_id);  // <<<< LOCK
    stats->utenti_serviti++;
    printf("[UTENTE %d] Consumo pasto... (Utenti serviti: %d)\n", 
           id, stats->utenti_serviti);
    sem_signal(sem_id);  // >>>> UNLOCK
    
    // Simula consumazione
    sleep(TEMPO_CONSUMAZIONE);
    printf("[UTENTE %d] Uscita dalla mensa\n", id);
    
    // Cleanup
    shm_detach(stats);
    exit(0);
}