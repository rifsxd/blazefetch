#include "helper/defines.cpp"
#include "helper/memory.cpp"
#include "helper/daemon.cpp"
#include "helper/live.cpp"
#include "helper/static.cpp"
#include "helper/minimal.cpp"
#include "helper/cute.cpp"
#include "helper/ascii.cpp"
#include "helper/help.cpp"
#include "helper/get.cpp"
#include "helper/options.cpp"
#include "helper/version.cpp"

int CommitNumbers = 5;

int main(int argc, char *argv[]) {

    // Declare the identifiers
    int runDaemonFlag               = 0;
    int showVersionFlag             = 0;
    int clearMemoryFlag             = 0;
    int removeLockFlag              = 0;
    int showHelpFlag                = 0;
    int showLiveFlag                = 0;
    int killDaemonFlag              = 0;
    int showStaticFlag              = 0;
    int showMinimalFlag             = 0;
    int showBunnyFlag               = 0;
    int showPussyFlag               = 0;
    int showASCIIFlag               = 0;
    int showTandemFlags             = 0;
    int showMinimalTandemFlags      = 0;
    int showGitFlags                = 0;

    // Check for flags
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-d") == 0 || std::strcmp(argv[i], "--daemon") == 0) {
            runDaemonFlag = 1;
        } else if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--version") == 0) {
            showVersionFlag = 1;
        } else if (std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--clear") == 0) {
            clearMemoryFlag = 1;
        } else if (std::strcmp(argv[i], "-l") == 0 || std::strcmp(argv[i], "--live") == 0) {
            showLiveFlag = 1;
        } else if (std::strcmp(argv[i], "-s") == 0 || std::strcmp(argv[i], "--static") == 0) {
            showStaticFlag = 1;
        } else if (std::strcmp(argv[i], "-m") == 0 || std::strcmp(argv[i], "--minimal") == 0) {
            showMinimalFlag = 1;
        } else if (std::strcmp(argv[i], "-b") == 0 || std::strcmp(argv[i], "--bunny") == 0) {
            showBunnyFlag = 1;
        } else if (std::strcmp(argv[i], "-p") == 0 || std::strcmp(argv[i], "--pussy") == 0) {
            showPussyFlag = 1;
        } else if (std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--ascii") == 0) {
            showASCIIFlag = 1;
        } else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            showHelpFlag = 1;
        } else if (std::strcmp(argv[i], "-k") == 0 || std::strcmp(argv[i], "--kill") == 0) {
            killDaemonFlag = 1;
        } else if (std::strcmp(argv[i], "-q") == 0 || std::strcmp(argv[i], "--query") == 0) {
            showGitFlags = 1;
        } else if ((std::strcmp(argv[i], "-r") == 0 || std::strcmp(argv[i], "--remove") == 0) && access(LOCK_FILE_PATH, F_OK) == -1) {
            std::cerr << "\nNo lock file found. Nothing to remove.\n" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Check if both -s and -a flags are present
    if (showStaticFlag && showASCIIFlag) {
        showTandemFlags = 1;
    }

    if (showMinimalFlag && showASCIIFlag) {
        showMinimalTandemFlags = 1;
    }

    // Check if the daemon is already running (excluding -v, -c, -h, -a , -s, -q, -r, and -d flags)
    if (!runDaemonFlag && !showVersionFlag && !clearMemoryFlag && !showHelpFlag && !removeLockFlag && access(LOCK_FILE_PATH, F_OK) == -1 && !showLiveFlag && !showStaticFlag && !showMinimalFlag && !showBunnyFlag && !showPussyFlag && !showASCIIFlag && !killDaemonFlag && !showGitFlags) {
        std::cerr << "\nBlaze daemon is not running. Please run 'blazefetch --daemon' to start the daemon first.\n" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::string> getInfoTypes;

     // Variable to store the specified information to fetch

    int opt;
    while ((opt = getopt_long(argc, argv, "g:q:dlsmbpavhcrk", longOptions, NULL)) != -1) {
        switch (opt) {
            
            case 'g':
                getInfoTypes.push_back(optarg);
                break;
            case 'q':
                CommitNumbers = std::atoi(optarg);
                showGitFlags = 1;
                break;
            case 'd':
                runDaemonFlag = 1;
                break;
            case 'l':
                showLiveFlag = 1;
                break;
            case 's':
                showStaticFlag = 1;
                break;
            case 'm':
                showMinimalFlag = 1;
                break;
            case 'b':
                showBunnyFlag = 1;
                break;
            case 'p':
                showPussyFlag = 1;
                break;
            case 'a':
                showASCIIFlag = 1;
                break;
            case 'v':
                showVersionFlag = 1;
                break;
            case 'h':
                showHelpFlag = 1;
                break;
            case 'c':
                clearMemoryFlag = 1;
                break;
            case 'r':
                removeLockFlag = 1;
                break;
            case 'k':
                killDaemonFlag = 1;
                break;
            default:
                showHelpFlag = 1;
                break;
        }
    }
    
    if (removeLockFlag) {
        // Check if the lock file exists before attempting to remove it
        if (access(LOCK_FILE_PATH, F_OK) == -1) {
            std::cerr << "\nNo lock file found. Nothing to remove.\n" << std::endl;
            return EXIT_FAILURE;
        }

        // Remove the lock file before exiting
        if (unlink(LOCK_FILE_PATH) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
        std::cout << "\nLock file removed successfully.\n" << std::endl;
        return EXIT_SUCCESS;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    int shmid = createSharedMemory();
    char* shm = attachSharedMemory(shmid);

    // Correct the function calls
    detachSharedMemory(shm);

    if (runDaemonFlag) {
        runDaemon();
    } else if (showVersionFlag) {
        printVersion();
    } else if (clearMemoryFlag) {
        clearStoredMemory();
    } else if (showLiveFlag) {
        runLiveProgram();
    } else if ((showStaticFlag || showTandemFlags) && (showASCIIFlag || showTandemFlags)) {
        runASCIIProgram();
        runStaticProgram();
    } else if (showStaticFlag) {
        runStaticProgram();
    } else if ((showMinimalFlag || showMinimalTandemFlags) && (showASCIIFlag || showMinimalTandemFlags)) {
        runMinimalASCIIProgram();
        runMinimalProgram();
    } else if (showMinimalFlag) {
        runMinimalProgram();
    } else if (showBunnyFlag) {
        runBunnyProgram();
    } else if (showPussyFlag) {
        runPussyProgram();
    } else if (showASCIIFlag) {
        runASCIIProgram();
    } else if (!getInfoTypes.empty()) {
        getInfoAndPrint(getInfoTypes);
    } else if (showHelpFlag) {
        printHelp();
    } else if (killDaemonFlag) {
        killBlazefetchProcess();
    } else if (showGitFlags) {
        getCommitInfo(CommitNumbers);
    } else {
        runDaemon();
    }

    return 0;
}