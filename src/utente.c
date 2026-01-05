// src/utente.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include "config.h"
#include "strutture.h"
#include "ipc_utils.h"
#include "piatti.h"

void processo_utente(int id, int shm_id, int sem_id, int msg_id) {
    srand(time(NULL) + id * 1000);

    // Attach alla shared memory
    Stats *stats = (Stats*)shm_attach(shm_id);
    if (!stats) {
        fprintf(stderr, "[UTENTE %d] Errore attach shared memory\n", id);
        exit(1);
    }

    ListaPiatti menu;
    carica_piatti(&menu);

    printf("[UTENTE %d] Arrivo in mensa\n", id);

    Messaggio ordine, risposta;

    // 1. PRIMO
    const char *primo = piatto_casuale_per_categoria(&menu, CAT_PRIMI);
    printf("[UTENTE %d] Ordino PRIMO: %s\n", id, primo);
    ordine.mtype = MSG_TYPE_ORDINE;
    ordine.id_utente = id;
    strcpy(ordine.piatto, primo);
    msgsnd(msg_id, &ordine, sizeof(ordine) - sizeof(long), 0);
    msgrcv(msg_id, &risposta, sizeof(risposta) - sizeof(long), 
           MSG_TYPE_RISPOSTA + id, 0);
    printf("[UTENTE %d] Ricevuto: %s\n", id, risposta.piatto);

    // 2. SECONDO
    const char *secondo = piatto_casuale_per_categoria(&menu, CAT_SECONDI);
    printf("[UTENTE %d] Ordino SECONDO: %s\n", id, secondo);
    ordine.mtype = MSG_TYPE_ORDINE;
    ordine.id_utente = id;
    strcpy(ordine.piatto, secondo);
    msgsnd(msg_id, &ordine, sizeof(ordine) - sizeof(long), 0);
    msgrcv(msg_id, &risposta, sizeof(risposta) - sizeof(long), 
           MSG_TYPE_RISPOSTA + id, 0);
    printf("[UTENTE %d] Ricevuto: %s\n", id, risposta.piatto);

    // 3. CONTORNO
    const char *contorno = piatto_casuale_per_categoria(&menu, CAT_CONTORNI);
    printf("[UTENTE %d] Ordino CONTORNO: %s\n", id, contorno);
    ordine.mtype = MSG_TYPE_ORDINE;
    ordine.id_utente = id;
    strcpy(ordine.piatto, contorno);
    msgsnd(msg_id, &ordine, sizeof(ordine) - sizeof(long), 0);
    msgrcv(msg_id, &risposta, sizeof(risposta) - sizeof(long), 
           MSG_TYPE_RISPOSTA + id, 0);
    printf("[UTENTE %d] Ricevuto: %s\n", id, risposta.piatto);

    // 4. DOLCE
    const char *dolce = piatto_casuale_per_categoria(&menu, CAT_DOLCI);
    printf("[UTENTE %d] Ordino DOLCE: %s\n", id, dolce);
    ordine.mtype = MSG_TYPE_ORDINE;
    ordine.id_utente = id;
    strcpy(ordine.piatto, dolce);
    msgsnd(msg_id, &ordine, sizeof(ordine) - sizeof(long), 0);
    msgrcv(msg_id, &risposta, sizeof(risposta) - sizeof(long), 
           MSG_TYPE_RISPOSTA + id, 0);
    printf("[UTENTE %d] Ricevuto: %s\n", id, risposta.piatto);

    // 5. CAFFE
    const char *caffe = piatto_casuale_per_categoria(&menu, CAT_CAFFE);
    printf("[UTENTE %d] Ordino CAFFE: %s\n", id, caffe);
    ordine.mtype = MSG_TYPE_ORDINE;
    ordine.id_utente = id;
    strcpy(ordine.piatto, caffe);
    msgsnd(msg_id, &ordine, sizeof(ordine) - sizeof(long), 0);
    msgrcv(msg_id, &risposta, sizeof(risposta) - sizeof(long), 
           MSG_TYPE_RISPOSTA + id, 0);
    printf("[UTENTE %d] Ricevuto: %s\n", id, risposta.piatto);

    // CONSUMO
    printf("[UTENTE %d] Sto consumando il pasto completo...\n", id);
    sleep(TEMPO_CONSUMO);
    printf("[UTENTE %d] Finito! Esco dalla mensa\n", id);

    // ✅ Aggiorna statistiche: utente servito
    sem_wait(sem_id);
    stats->utenti_serviti++;
    sem_signal(sem_id);

    // Cleanup
    shm_detach(stats);

    // ✅ FONDAMENTALE: Termina correttamente il processo figlio
    exit(0);
}