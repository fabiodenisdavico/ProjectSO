#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "ipc_utils.h"
#include "strutture.h"

// ============================================================
// MEMORIA CONDIVISA
// ============================================================

int shm_create(key_t key, size_t size) {
    int shm_id = shmget(key, size, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        return -1;
    }
    return shm_id;
}

void* shm_attach(int shm_id) {
    void *shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        return NULL;
    }
    return shm_ptr;
}

int shm_detach(void *shm_ptr) {
    if (shmdt(shm_ptr) < 0) {
        perror("shmdt");
        return -1;
    }
    return 0;
}

int shm_destroy(int shm_id) {
    if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
        perror("shmctl IPC_RMID");
        return -1;
    }
    return 0;
}

// ============================================================
// SEMAFORI
// ============================================================

int sem_create(key_t key, int num_sems) {
    int sem_id = semget(key, num_sems, IPC_CREAT | 0666);
    if (sem_id < 0) {
        perror("semget");
        return -1;
    }
    return sem_id;
}

int sem_init(int sem_id, int value) {
    union semun arg;
    arg.val = value;
    if (semctl(sem_id, 0, SETVAL, arg) < 0) {
        perror("semctl SETVAL");
        return -1;
    }
    return 0;
}

void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};  // P operation
    if (semop(sem_id, &sb, 1) < 0) {
        perror("semop wait");
        exit(1);
    }
}

void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};   // V operation
    if (semop(sem_id, &sb, 1) < 0) {
        perror("semop signal");
        exit(1);
    }
}

int sem_destroy(int sem_id) {
    if (semctl(sem_id, 0, IPC_RMID) < 0) {
        perror("semctl IPC_RMID");
        return -1;
    }
    return 0;
}

// ============================================================
// CODE MESSAGGI
// ============================================================

int msg_create(key_t key) {
    int msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id < 0) {
        perror("msgget");
        return -1;
    }
    return msg_id;
}

int msg_destroy(int msg_id) {
    if (msgctl(msg_id, IPC_RMID, NULL) < 0) {
        perror("msgctl IPC_RMID");
        return -1;
    }
    return 0;
}