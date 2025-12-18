#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <sys/types.h>

// ============================================================
// FUNZIONI MEMORIA CONDIVISA
// ============================================================

/**
 * Crea segmento di memoria condivisa
 * @param key Chiave IPC
 * @param size Dimensione in bytes
 * @return ID memoria condivisa, -1 in caso di errore
 */
int shm_create(key_t key, size_t size);

/**
 * Attacca memoria condivisa al processo
 * @param shm_id ID memoria condivisa
 * @return Puntatore alla memoria, NULL in caso di errore
 */
void* shm_attach(int shm_id);

/**
 * Distacca memoria condivisa dal processo
 * @param shm_ptr Puntatore alla memoria
 * @return 0 successo, -1 errore
 */
int shm_detach(void *shm_ptr);

/**
 * Rimuove memoria condivisa dal sistema
 * @param shm_id ID memoria condivisa
 * @return 0 successo, -1 errore
 */
int shm_destroy(int shm_id);

// ============================================================
// FUNZIONI SEMAFORI
// ============================================================

/**
 * Crea set di semafori
 * @param key Chiave IPC
 * @param num_sems Numero semafori nel set
 * @return ID semaforo, -1 in caso di errore
 */
int sem_create(key_t key, int num_sems);

/**
 * Inizializza valore semaforo
 * @param sem_id ID semaforo
 * @param value Valore iniziale
 * @return 0 successo, -1 errore
 */
int sem_init(int sem_id, int value);

/**
 * Operazione P (wait) su semaforo
 * @param sem_id ID semaforo
 */
void sem_wait(int sem_id);

/**
 * Operazione V (signal) su semaforo
 * @param sem_id ID semaforo
 */
void sem_signal(int sem_id);

/**
 * Rimuove semaforo dal sistema
 * @param sem_id ID semaforo
 * @return 0 successo, -1 errore
 */
int sem_destroy(int sem_id);

// ============================================================
// FUNZIONI CODE MESSAGGI
// ============================================================

/**
 * Crea coda di messaggi
 * @param key Chiave IPC
 * @return ID coda messaggi, -1 in caso di errore
 */
int msg_create(key_t key);

/**
 * Rimuove coda messaggi dal sistema
 * @param msg_id ID coda messaggi
 * @return 0 successo, -1 errore
 */
int msg_destroy(int msg_id);

#endif // IPC_UTILS_H