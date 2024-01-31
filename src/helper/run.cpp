#include "defines.cpp"
#include "colors.cpp"

void runProgram() {
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Display cached info
    std::cout << "\n" << shm;

    colorPallate();

    // Detach the shared memory segment
    shmdt(shm);
}