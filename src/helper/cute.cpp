#include "defines.cpp"
#include "modules.cpp"
#include "colors.cpp"
#include "memory.cpp"

void runPussyProgram() {

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
    std::string output =    
        " \033[3m" + PUSSY_GREETING + "\n\n"
        "            " + USER_INFO + "\n"
        "   \033[38;5;94m/\\_/\\\033[0m    " + OS_INFO + "\n"
        "  \033[38;5;15m( >.< )\033[0m   " + KERNEL_INFO + "\n"
        "   \033[38;5;94m= ^ =\033[0m    " + SHELL_INFO + "\n"
        "  \033[38;5;15m~(\033[38;5;211m♥\033[\033[38;5;15m)(\033[38;5;211m♥\033[38;5;15m)   " + DE_INFO + "\n\n";

    // Update shared memory
    std::strcpy(shm, output.c_str());

    // Print cached info
    std::cout << "\n" << shm;

    colorPallateAlt();

    // Detach the shared memory segment
    shmdt(shm);
    
}

void runBunnyProgram() {

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
    std::string output =    
        " \033[3m" + BUNNY_GREETING + "\n\n"
        "            " + USER_INFO + "\n"
        "   \033[38;5;15m(\\ /)\033[0m    " + OS_INFO + "\n"
        "   \033[38;5;15m( . .)\033[0m   " + KERNEL_INFO + "\n"
        "   \033[38;5;15mc(\033[38;5;211m\"" + "\033[\033[38;5;15m)(\033[38;5;211m\"" + "\033[\033[38;5;15m)\033[0m  " + SHELL_INFO + "\n"
        "            " + DE_INFO + "\n\n";

    // Update shared memory
    std::strcpy(shm, output.c_str());

    // Print cached info
    std::cout << "\n" << shm;

    colorPallateAlt();

    // Detach the shared memory segment
    shmdt(shm);
    
}