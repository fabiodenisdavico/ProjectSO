#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "config.h"
#include "strutture.h"
#include "ipc_utils.h"
#include "processi.h"
#include <signal.h>

volatile sig_atomic_t stop = 0;

void ctrl_c_handler(int sig) {  
    stop = 1;
}


int main() {

    signal(SIGINT, ctrl_c_handler);

    printf("\n========================================\n");
    printf("  SIMULAZIONE MENSA - TEST IPC\n");
    printf("========================================\n\n");
    
    int shm_id, sem_id, msg_id;
    Stats *stats;
    
    // --------------------------------------------------------
    // 1. CREAZIONE RISORSE IPC
    // --------------------------------------------------------
    printf("[RESPONSABILE] Creazione risorse IPC...\n");
    
    // Shared Memory
    shm_id = shm_create(SHM_KEY, sizeof(Stats));
    if (shm_id < 0) {
        fprintf(stderr, "Errore creazione shared memory\n");
        exit(1);
    }
    printf("[RESPONSABILE] Shared memory creata: ID=%d\n", shm_id);
    
    // Semaforo (mutex)
    sem_id = sem_create(SEM_KEY, 1);
    if (sem_id < 0) {
        shm_destroy(shm_id);
        exit(1);
    }
    sem_init(sem_id, 1);  // Inizializza a 1 (mutex libero)
    printf("[RESPONSABILE] Semaforo creato: ID=%d\n", sem_id);
    
    // Coda messaggi
    msg_id = msg_create(MSG_KEY);
    if (msg_id < 0) {
        sem_destroy(sem_id);
        shm_destroy(shm_id);
        exit(1);
    }
    printf("[RESPONSABILE] Coda messaggi creata: ID=%d\n\n", msg_id);
    
    // --------------------------------------------------------
    // 2. INIZIALIZZAZIONE STATISTICHE
    // --------------------------------------------------------
    stats = (Stats*)shm_attach(shm_id);
    if (!stats) {
        msg_destroy(msg_id);
        sem_destroy(sem_id);
        shm_destroy(shm_id);
        exit(1);
    }
    
    memset(stats, 0, sizeof(Stats));
    stats->simulazione_attiva = 1;
    
    // --------------------------------------------------------
    // 3. CREAZIONE OPERATORI
    // --------------------------------------------------------
    printf("[RESPONSABILE] Avvio %d operatori...\n", NUM_OPERATORI);
    pid_t pid_operatori[NUM_OPERATORI];
    
    for (int i = 0; i < NUM_OPERATORI; i++) {

        if (stop) break; //Gestione Stop Fabio

        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork operatore");
            exit(1);
        }
        else if (pid == 0) {
            // PROCESSO FIGLIO: Operatore
            processo_operatore(i + 1, shm_id, sem_id, msg_id);
            // Non ritorna mai qui
        }
        else {
            // PROCESSO PADRE
            pid_operatori[i] = pid;
        }
    }
    
    sleep(1);  // Aspetta che operatori siano pronti
    
    // --------------------------------------------------------
    // 4. CREAZIONE UTENTI
    // --------------------------------------------------------
    printf("[RESPONSABILE] Generazione %d utenti...\n\n", NUM_UTENTI);
    pid_t pid_utenti[NUM_UTENTI];
    
    for (int i = 0; i < NUM_UTENTI; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork utente");
            exit(1);
        }
        else if (pid == 0) {
            // PROCESSO FIGLIO: Utente
            processo_utente(i + 1, shm_id, sem_id, msg_id);
            // Non ritorna mai qui
        }
        else {
            // PROCESSO PADRE
            pid_utenti[i] = pid;
        }
        
        sleep(1);  // Arrivi scaglionati
    }
    
    // --------------------------------------------------------
    // 5. ATTESA TERMINAZIONE UTENTI
    // --------------------------------------------------------
    printf("\n[RESPONSABILE] Attesa terminazione utenti...\n");
    for (int i = 0; i < NUM_UTENTI; i++) {
        waitpid(pid_utenti[i], NULL, 0);
    }
    printf("[RESPONSABILE] Tutti gli utenti sono usciti\n\n");
    
    // --------------------------------------------------------
    // 6. TERMINAZIONE OPERATORI
    // --------------------------------------------------------
    printf("[RESPONSABILE] Terminazione operatori...\n");
    
    // Segnala terminazione nelle statistiche
    sem_wait(sem_id);
    stats->simulazione_attiva = 0;
    sem_signal(sem_id);
    
    // Invia messaggi di terminazione agli operatori
    for (int i = 0; i < NUM_OPERATORI; i++) {
        Messaggio term;
        term.mtype = MSG_TYPE_ORDINE;
        term.id_utente = -1;  // -1 indica terminazione
        strcpy(term.piatto, "TERMINA");
        msgsnd(msg_id, &term, sizeof(term) - sizeof(long), 0);
    }
    
    // Attende terminazione operatori
    for (int i = 0; i < NUM_OPERATORI; i++) {
        waitpid(pid_operatori[i], NULL, 0);
    }
    
    // --------------------------------------------------------
    // 7. STATISTICHE FINALI
    // --------------------------------------------------------
    printf("\n========================================\n");
    printf("  STATISTICHE FINALI\n");
    printf("========================================\n");
    printf("Utenti serviti:       %d\n", stats->utenti_serviti);
    printf("Piatti distribuiti:   %d\n", stats->piatti_distribuiti);
    printf("========================================\n\n");
    
    // --------------------------------------------------------
    // 8. CLEANUP RISORSE IPC
    // --------------------------------------------------------
    printf("[RESPONSABILE] Pulizia risorse IPC...\n");
    
    shm_detach(stats);
    
    if (shm_destroy(shm_id) == 0) {
        printf("[RESPONSABILE] Shared memory rimossa\n");
    }
    
    if (sem_destroy(sem_id) == 0) {
        printf("[RESPONSABILE] Semaforo rimosso\n");
    }
    
    if (msg_destroy(msg_id) == 0) {
        printf("[RESPONSABILE] Coda messaggi rimossa\n");
    }
    
    printf("\n[RESPONSABILE] Simulazione completata!\n\n");
    
    return 0;
}