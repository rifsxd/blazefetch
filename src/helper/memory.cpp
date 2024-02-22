#pragma once

#include "defines.cpp"
#include "colors.cpp"

#define SHM_SIZE 1024

int getExistingSharedMemory() {
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Try to get the existing shared memory segment
    int shmid = shmget(key, 1024, 0644 | IPC_CREAT);

    if (shmid == -1) {
        perror("shmget");
        // Return -1 to indicate that the shared memory doesn't exist
        return -1;
    }

    return shmid;
}

int createSharedMemory() {
    // Attempt to get existing shared memory
    int shmid = getExistingSharedMemory();

    // If shared memory doesn't exist, create a new one
    if (shmid == -1) {
        shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0644);
        if (shmid == -1) {
            perror("shmget");
            exit(EXIT_FAILURE);
        }
    }

    return shmid;
}

char* attachSharedMemory(int shmid) {
    char* shm = (char*)shmat(shmid, NULL, 0);
    if (shm == (char*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shm;
}

void detachSharedMemory(char* shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void removeSharedMemory(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void clearStoredMemory() {
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Clear the shared memory content
    memset(shm, 0, SHM_SIZE);

    // Detach the shared memory segment
    shmdt(shm);

    std::cout << orangeColor << "\nStored blaze memory has been cleared.\n" << std::endl;
}