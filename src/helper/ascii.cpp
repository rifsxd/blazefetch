#include "defines.cpp"
#include "modules.cpp"
#include "memory.cpp"

void runASCIIProgram() {
    
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Clear shared memory content
    memset(shm, 0, SHM_SIZE);

    // Run and store other info in shared memory
    std::string output = getGlyphInfo() + "\n";

    // Update shared memory
    std::strcpy(shm, output.c_str());

    // Print cached info
    std::cout << "\n" << shm;

    // Detach the shared memory segment
    shmdt(shm);
    
}