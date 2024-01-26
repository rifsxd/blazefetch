#include "defines.cpp"
#include "modules.cpp"
#include "colors.cpp"

bool liveMode = false;

// Function to check if a key is pressed
bool isKeyPressed() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

void runLiveProgram() {
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Enable live mode
    liveMode = true;

    // Run the live program loop
    while (liveMode) {
        // Check for key press to exit
        if (isKeyPressed()) {
            char key = getchar();
            if (key == 'q' || key == 'Q') {
                liveMode = false;
                break;
            }
        }
        
        // Clear shared memory content
        memset(shm, 0, SHM_SIZE);

        // Run get<example>Info functions and store the output in shared memory
        std::string output = getTitleInfo() + "\n" + getOsInfo() + "\n" + getPackageInfo() + "\n" +
                              getKernelInfo() + "\n" + getUptimeInfo() + "\n" + getTimeInfo() + "\n" + getShellInfo() + "\n" +
                              getCpuInfo() + "\n" + getGpuInfo() + "\n" + getStorageInfo() + "\n" +
                              getRamInfo() + "\n" + getDEInfo() + "\n" + getMediaInfo() + "\n" + getNetworkStatusInfo() /**/ + "\n" + getTerminalInfo() /**/ + "\n\n"; // ENABLE getTerminalInfo()

        // Update shared memory
        std::strcpy(shm, output.c_str());

        // Print cached info
        std::cout << "\n" << shm;
        
        colorPallate();
    }

    // Detach the shared memory segment
    shmdt(shm);
}