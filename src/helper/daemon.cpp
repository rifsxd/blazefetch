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
        if (fscanf(processCmd, "%d", &pid) != 1) {
            // Handle the case where fscanf fails to read the expected value
            perror("fscanf");
            std::cerr << redColor << "\nError reading process ID for " << PROCESS_NAME << ".\n" << std::endl;
            pid = -1;  // Set pid to an invalid value
        }

        pclose(processCmd);
    } else {
        // Handle the case where popen fails
        perror("popen");
        std::cerr << redColor << "\nError executing command to find " << PROCESS_NAME << " process.\n" << std::endl;
    }

    // Check if the process ID is valid and kill the process
    if (pid > 0) {
        if (kill(pid, SIGTERM) == 0) {
            std::cout << yellowColor << "\nTerminated " << PROCESS_NAME << " process...\n" << std::endl;
            // Optionally wait for the process to exit
            waitpid(pid, nullptr, 0);
        } else {
            perror("kill");
            std::cerr << redColor << "\nFailed to terminate " << PROCESS_NAME << " process.\n" << std::endl;
        }
    } else {
        std::cerr << yellowColor << "\n" << PROCESS_NAME << " process not found.\n" << std::endl;
    }
}

void runDaemon() {

    std::string distroName = getDistroInfo();
    
    // Set up signal handling
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    // Check if the lock file exists
    if (access(LOCK_FILE_PATH, F_OK) != -1) {
        std::cerr << yellowColor << "\nUmm... Blaze daemon is already running?!\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create the lock file
    int lockFile = open(LOCK_FILE_PATH, O_CREAT | O_WRONLY, 0644);
    if (lockFile == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Display the message only when creating the lock file for the first time
    std::cout << greenColor << "\nBlaze daemon is running in the background." << std::endl;
    std::cout << blueColor << "Use 'blazefetch' command to fetch and display system information.\n" << std::endl;
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
        std::string output = USER_INFO + "\n" + OS_INFO + "\n" + HOST_INFO + "\n" + PACKAGES_INFO + "\n" +
                                KERNEL_INFO + "\n" + UPTIME_INFO + "\n" + TIME_INFO + "\n" + DATE_INFO + "\n" + SHELL_INFO + "\n" +
                                CPU_INFO + "\n" + GPU_INFO + "\n" + SCREEN_INFO + "\n" + BRIGHTNESS_INFO + "\n" + DISK_INFO + "\n" +
                                RAM_INFO + "\n" + DE_INFO + "\n" + MEDIA_INFO + "\n" + VOLUME_INFO + "\n" + NETWORK_INFO + "\n" + TERM_INFO + "\n\n";

        // Update shared memory
        std::strcpy(shm, output.c_str());

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Detach the shared memory segment
    shmdt(shm);

    // Remove the lock file before exiting
    if (unlink(LOCK_FILE_PATH) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}