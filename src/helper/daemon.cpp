#include "defines.cpp"
#include "modules.cpp"
#include "memory.cpp"

#define LOCK_FILE_PATH "/tmp/blazefetch.lock"

const char* PROCESS_NAME = "blazefetch";

void signalHandler(int signum) {
    // Handle signals, e.g., clean up and exit
    if (signum == SIGTERM || signum == SIGINT) {
        // Remove the lock file before exiting
        if (unlink(LOCK_FILE_PATH) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
}

void daemonize() {
    // Fork off the parent process
    pid_t pid = fork();

    // An error occurred
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // On success: The child process becomes the session leader
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // Fork off for the second time
    pid = fork();

    // An error occurred
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the current working directory to root
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    // Redirect standard input, output, and error streams
    open("/dev/null", O_RDWR); // stdin
    open("/dev/null", O_RDWR); // stdout
    open("/dev/null", O_RDWR); // stderr
}



// Function to send SIGTERM signal to the blazefetch process
void killBlazefetchProcess() {
    pid_t pid = -1;

    // Use system command to find the blazefetch process ID
    std::string cmd = "pgrep -o " + std::string(PROCESS_NAME);
    FILE* processCmd = popen(cmd.c_str(), "r");

    if (processCmd != nullptr) {
        fscanf(processCmd, "%d", &pid);
        pclose(processCmd);
    }

    // Check if the process ID is valid and kill the process
    if (pid > 0) {
        if (kill(pid, SIGTERM) == 0) {
            std::cout << "\nTerminated " << PROCESS_NAME << " process...\n" << std::endl;
            // Optionally wait for the process to exit
            waitpid(pid, nullptr, 0);
        } else {
            perror("kill");
            std::cerr << "\nFailed to terminate " << PROCESS_NAME << " process.\n" << std::endl;
        }
    } else {
        std::cerr << "\n" << PROCESS_NAME << " process not found.\n" << std::endl;
    }
}

void runDaemon() {
    // Set up signal handling
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    // Check if the lock file exists
    if (access(LOCK_FILE_PATH, F_OK) != -1) {
        std::cerr << "\nUmm... Blaze daemon is already running?!\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create the lock file
    int lockFile = open(LOCK_FILE_PATH, O_CREAT | O_WRONLY, 0644);
    if (lockFile == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Display the message only when creating the lock file for the first time
    std::cout << "\nBlaze daemon is running in the background." << std::endl;
    std::cout << "Use 'blazefetch' command to fetch and display system information.\n" << std::endl;
    // Daemonize the process
    daemonize();

    // Set up signal handling
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Create (or get) a shared memory segment
    int shmid = shmget(key, 1024, 0644 | IPC_CREAT);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Run the daemon loop
    while (true) {

        // Clear shared memory content
        memset(shm, 0, SHM_SIZE);

        // Run get<example>Info functions and store the output in shared memory
        std::string output = getTitleInfo() + "\n" + getOsInfo() + "\n" + getPackageInfo() + "\n" +
                            getKernelInfo() + "\n" + getUptimeInfo() + "\n" + getTimeInfo() + "\n" + getShellInfo() + "\n" +
                            getCpuInfo() + "\n" + getGpuInfo() + "\n" + getStorageInfo() + "\n" +
                            getRamInfo() + "\n" + getDEInfo() + "\n" + getMediaInfo() + "\n" + getNetworkStatusInfo() /**/ + "\n" + getTerminalInfo() /**/ + "\n\n"; // ENABLE getTerminalInfo()

        // Update shared memory
        std::strcpy(shm, output.c_str());

        // Sleep for 1 seconds
        sleep(1);
    }

    // Detach the shared memory segment
    shmdt(shm);

    // Remove the shared memory segment (optional, depending on your requirements)
    shmctl(shmid, IPC_RMID, NULL);
    // Remove the lock file before exiting
    if (unlink(LOCK_FILE_PATH) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}