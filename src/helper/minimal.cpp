#include "defines.cpp"
#include "modules.cpp"
#include "colors.cpp"
#include "memory.cpp"

void runMinimalProgram() {

    std::string distroName = getDistroInfo();
    
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Clear shared memory content
    memset(shm, 0, SHM_SIZE);

    // Run get<example>Info functions and store the output in shared memory
    std::string output =    USER_INFO + "\n" + OS_INFO + "\n" + KERNEL_INFO + "\n" +
                            UPTIME_INFO +  + "\n" + SHELL_INFO + "\n" + DE_INFO + "\n\n";

    // Update shared memory
    std::strcpy(shm, output.c_str());

    // Print cached info
    std::cout << "\n" << shm;

    colorPallate();

    // Detach the shared memory segment
    shmdt(shm);
    
}