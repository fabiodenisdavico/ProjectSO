#ifndef PROCESSI_H
#define PROCESSI_H

// ============================================================
// DICHIARAZIONI FUNZIONI PROCESSI
// ============================================================

/**
 * Funzione principale processo operatore
 * @param id ID operatore (1, 2, ...)
 * @param shm_id ID memoria condivisa
 * @param sem_id ID semaforo
 * @param msg_id ID coda messaggi
 */
void processo_operatore(int id, int shm_id, int sem_id, int msg_id);

/**
 * Funzione principale processo utente
 * @param id ID utente (1, 2, ...)
 * @param shm_id ID memoria condivisa
 * @param sem_id ID semaforo
 * @param msg_id ID coda messaggi
 */
void processo_utente(int id, int shm_id, int sem_id, int msg_id);

#endif // PROCESSI_H