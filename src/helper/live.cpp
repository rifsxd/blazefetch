#include "defines.cpp"
#include "modules.cpp"
#include "colors.cpp"
#include "memory.cpp"

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

    std::string distroName = getDistroInfo();
    
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
        std::string output = USER_INFO + "\n" + OS_INFO + "\n" + HOST_INFO + "\n" + PACKAGES_INFO + "\n" +
                                KERNEL_INFO + "\n" + UPTIME_INFO + "\n" + TIME_INFO + "\n" + DATE_INFO + "\n" + SHELL_INFO + "\n" +
                                CPU_INFO + "\n" + GPU_INFO + "\n" + SCREEN_INFO + "\n" + BRIGHTNESS_INFO + "\n" + DISK_INFO + "\n" +
                                RAM_INFO + "\n" + DE_INFO + "\n" + MEDIA_INFO + "\n" + VOLUME_INFO + "\n" + NETWORK_INFO + "\n" + TERM_INFO + "\n\n";

        // Update shared memory
        std::strcpy(shm, output.c_str());

        // Print cached info
        std::cout << "\n" << shm;
        
        colorPallate();
    }

    // Detach the shared memory segment
    shmdt(shm);
}