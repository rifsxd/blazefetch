#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <csignal>
#include <fcntl.h>

#include <sys/shm.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#define VERSION "2.0.0-TESTING"
#define SHM_SIZE 4096
#define LOCK_FILE_PATH "/tmp/blazefetch.lock"


std::string getTitleInfo();
std::string getUptimeInfo();
std::string getOsInfo();
std::string getRamInfo();
std::string getKernelInfo();
std::string getShellInfo();
std::string getWmInfo();
std::string getPackageInfo();
std::string getStorageInfo();
std::string getCpuInfo();
std::string getGpuInfo();

#ifdef ICONIC
#define OS "󰍹"
#define PACKAGES "󰏓"
#define KERNEL "󰌽"
#define UPTIME "󱑁"
#define SHELL ""
#define CPU ""
#define GPU "󰿵"
#define DISK "󰋊"
#define RAM "󰇻"
#define WM "󰖲"
#define MEDIA "󰲸"
#else
#define OS "󰍹 OS:"
#define PACKAGES "󰏓 PACKAGES:"
#define KERNEL "󰌽 KERNEL:"
#define UPTIME "󱑁 UPTIME:"
#define SHELL " SHELL:"
#define CPU " CPU:"
#define GPU "󰿵 GPU:"
#define DISK "󰋊 DISK:"
#define RAM "󰇻 RAM:"
#define WM "󰖲 WM:"
#define MEDIA "󰲸 MEDIA:"
#endif

std::string getTitleInfo() {
    char username[256];
    char hostname[256];
    if (getlogin_r(username, sizeof(username)) == 0 && gethostname(hostname, sizeof(hostname)) == 0) {
        return "\033[31m" + std::string() + "\033[31m" + username + "@" + hostname + "\033[0m";
    } else {
        return "\033[31m" + std::string() + " \033[31mUnknown\033[0m";
    }
}

std::string getOsInfo() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp) {
        int foundPrettyName = 0;
        char line[256];
        
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "PRETTY_NAME")) {
                char *name = strchr(line, '=') + 2;
                name[strlen(name) - 2] = '\0';
                fclose(fp);
                return "\033[32m" + std::string(OS) + " \033[0m" + name;
            }
        }

        fclose(fp);
        return "\033[32m" + std::string(OS) + " \033[0mUnknown";
    } else {
        return "\033[32m" + std::string(OS) + " \033[0mUnknown";
    }
}

std::string getKernelInfo() {
    struct utsname unameData;
    if (uname(&unameData) == 0) {
        return "\033[33m" + std::string(KERNEL) + " \033[0m" + unameData.release;
    } else {
        return "\033[33m" + std::string(KERNEL) + " \033[0mUnknown";
    }
}

std::string getShellInfo() {
    char *shellenv = getenv("SHELL");
    if (shellenv) {
        char *shellname = strrchr(shellenv, '/');
        if (shellname) {
            return "\033[34m" + std::string(SHELL) + " \033[0m" + (shellname + 1);
        } else {
            return "\033[34m" + std::string(SHELL) + " \033[0m" + shellenv;
        }
    } else {
        return "\033[34m" + std::string(SHELL) + " \033[0mUnknown";
    }
}

std::string getWmInfo() {
    char *waylandDisplay = getenv("WAYLAND_DISPLAY");
    if (waylandDisplay) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        return "\033[38;5;93m" + std::string(WM) + " \033[0m" + (xdgDesktop ? xdgDesktop : "Unknown");
    } else {
        Display *display = XOpenDisplay(NULL);
        if (display) {
            Window root = DefaultRootWindow(display);

            Atom netSupportingWmCheckAtom = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
            Atom netWmNameAtom = XInternAtom(display, "_NET_WM_NAME", False);
            Atom utf8StringAtom = XInternAtom(display, "UTF8_STRING", False);

            if (netSupportingWmCheckAtom != None && netWmNameAtom != None) {
                Atom actualType;
                int actualFormat;
                unsigned long nItems, bytesAfter;
                unsigned char *propValue = NULL;

                if (XGetWindowProperty(display, root, netSupportingWmCheckAtom, 0, 1, False,
                                       XA_WINDOW, &actualType, &actualFormat,
                                       &nItems, &bytesAfter, &propValue) == Success && propValue) {
                    Window supportingWmCheck = *((Window *)propValue);
                    XFree(propValue);

                    if (XGetWindowProperty(display, supportingWmCheck, netWmNameAtom, 0, 1024, False,
                                           utf8StringAtom, &actualType, &actualFormat,
                                           &nItems, &bytesAfter, &propValue) == Success && propValue) {
                        std::string wmName(reinterpret_cast<char*>(propValue));
                        XFree(propValue);
                        XCloseDisplay(display);
                        return "\033[38;5;93m" + std::string(WM) + " \033[0m" + wmName;
                    }
                }
            }

            XCloseDisplay(display);
        }
    }

    return "\033[38;5;93m" + std::string(WM) + " \033[0mUnknown";
}

std::string getUptimeInfo() {
    FILE *uptimeFile = fopen("/proc/uptime", "r");
    if (uptimeFile) {
        double uptimeValue;
        fscanf(uptimeFile, "%lf", &uptimeValue);
        fclose(uptimeFile);

        int hours = static_cast<int>(uptimeValue / 3600);
        int minutes = static_cast<int>((uptimeValue - hours * 3600) / 60);

        return "\033[36m" + std::string(UPTIME) + " \033[0m" + std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    } else {
        return "\033[36m" + std::string(UPTIME) + " \033[0mUnknown";
    }
}

std::string getRamInfo() {
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if (meminfo) {
        long total_mem = 0, free_mem = 0, buffers = 0, cached = 0;
        char line[256];

        while (fgets(line, sizeof(line), meminfo) != NULL) {
            if (sscanf(line, "MemTotal: %ld kB", &total_mem) == 1) {
                continue;
            }
            if (sscanf(line, "MemFree: %ld kB", &free_mem) == 1) {
                continue;
            }
            if (sscanf(line, "Buffers: %ld kB", &buffers) == 1) {
                continue;
            }
            if (sscanf(line, "Cached: %ld kB", &cached) == 1) {
                continue;
            }
        }

        fclose(meminfo);

        if (total_mem > 0 && free_mem > 0) {
            long used_mem = total_mem - free_mem - buffers - cached;
            return "\033[38;5;198m" + std::string(RAM) + " \033[0m" + std::to_string(used_mem / 1024) + " MB / " + std::to_string(total_mem / 1024) + " MB";
        } else {
            return "\033[38;5;198m" + std::string(RAM) + " \033[0mUnknown";
        }
    } else {
        return "\033[38;5;198m" + std::string(RAM) + " \033[0mUnknown";
    }
}

std::string getStorageInfo() {
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        long total_space = (long)vfs.f_frsize * vfs.f_blocks;
        long used_space = (long)vfs.f_frsize * (vfs.f_blocks - vfs.f_bfree);

        return "\033[95m" + std::string(DISK) + " \033[0m" + std::to_string(used_space / (1024 * 1024)) + " MB / " + std::to_string(total_space / (1024 * 1024)) + " MB";
    } else {
        return "\033[95m" + std::string(DISK) + " \033[0mUnknown";
    }
}


std::string getLSBReleaseInfo() {
    const int bufferSize = 256;
    char buffer[bufferSize];

    FILE* lsbReleaseFile = popen("lsb_release -si 2>/dev/null", "r");
    if (lsbReleaseFile) {
        if (fgets(buffer, bufferSize, lsbReleaseFile) != nullptr) {
            // Remove newline character
            buffer[strcspn(buffer, "\n")] = 0;
        } else {
            snprintf(buffer, bufferSize, "%s", "Unknown");
        }
        pclose(lsbReleaseFile);
    } else {
        snprintf(buffer, bufferSize, "%s", "Unknown");
    }

    return std::string(buffer);
}

std::string getPackageInfo() {
    std::string lsbaltInfo = getLSBReleaseInfo();
    std::string packageInfo;

    int pkgCount = 0;

    // Check the Linux distribution using os-release
    FILE* osReleaseFile = popen("cat /etc/os-release | grep '^ID_LIKE=' | cut -d'=' -f2 && cat /etc/os-release | grep '^ID=' | cut -d'=' -f2", "r");
    if (osReleaseFile) {
        char distro[64];
        if (fgets(distro, sizeof(distro), osReleaseFile) != nullptr) {
            // Remove newline character
            distro[strcspn(distro, "\n")] = 0;

            // Get package info based on the distribution
            if (strstr(distro, "arch") != nullptr) {
                // Count Arch Linux packages
                FILE* archFile = popen("pacman -Qq", "r");
                if (archFile) {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), archFile) != nullptr) {
                        pkgCount++;
                    }
                    pclose(archFile);

                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (pacman)";
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else if (strstr(distro, "fedora") != nullptr || strstr(distro, "opensuse") != nullptr || strstr(distro, "suse") != nullptr || strstr(distro, "rhel") != nullptr) {
                // Count RPM packages
                FILE* rpmFile = popen("rpm -qa", "r");
                if (rpmFile) {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), rpmFile) != nullptr) {
                        pkgCount++;
                    }
                    pclose(rpmFile);

                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (rpm)";
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else if (strstr(distro, "nixos") != nullptr) {
                // Count NixOS packages
                FILE* nixFile = popen("echo $(( $(nix-store -qR /run/current-system/sw | wc -l) + $(nix-store -qR /etc/profiles/per-user/$USER | wc -l) ))", "r");
                if (nixFile) {
                    char buffer[256];
                    if (fgets(buffer, sizeof(buffer), nixFile) != nullptr) {
                        pkgCount = atoi(buffer);
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (nix)";
                    } else {
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                    }
                    pclose(nixFile);
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else if (strstr(distro, "debian") != nullptr || strstr(distro, "ubuntu") != nullptr || strstr(distro, "linuxmint") != nullptr || strstr(distro, "popos") != nullptr) {
                // Count Debian/Ubuntu packages
                FILE* debFile = popen("dpkg -l | grep ^ii | wc -l", "r");
                if (debFile) {
                    char buffer[256];
                    if (fgets(buffer, sizeof(buffer), debFile) != nullptr) {
                        pkgCount = atoi(buffer);
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (deb)";
                    } else {
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                    }
                    pclose(debFile);
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else {
                packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
            }
        } else {
            packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
        }
        pclose(osReleaseFile);
    } else {
        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
    }

    return packageInfo;
}

std::string getCpuInfo() {
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo) {
        char line[256];
        int cpuCount = 0;

        while (fgets(line, sizeof(line), cpuinfo) != NULL) {
            if (strstr(line, "model name")) {
                cpuCount++;
                char *model = strchr(line, ':') + 2;
                model[strlen(model) - 1] = '\0'; // Remove the trailing newline
                fclose(cpuinfo);
                return "\033[95m" + std::string(CPU) + " \033[0m" + std::string(model) + " (x" + std::to_string(cpuCount) + ")";
            }
        }

        fclose(cpuinfo);
    }

    return "\033[95m" + std::string(CPU) + " \033[0mUnknown";
}

std::string getGpuInfo() {
    FILE *lspci = popen("lspci | grep -i vga", "r");
    if (lspci) {
        char line[256];

        if (fgets(line, sizeof(line), lspci) != NULL) {
            // Remove unwanted prefix
            char *gpuName = strstr(line, ": ");
            if (gpuName) {
                gpuName += 2;  // Move past the ": "
                // Remove newline character
                gpuName[strcspn(gpuName, "\n")] = 0;
                pclose(lspci);
                return "\033[96m" + std::string(GPU) + " \033[0m" + std::string(gpuName);
            } else {
                pclose(lspci);
                return "\033[96m" + std::string(GPU) + " \033[0mUnknown";
            }
        } else {
            pclose(lspci);
            return "\033[96m" + std::string(GPU) + " \033[0mUnknown";
        }
    }

    return "\033[96m" + std::string(GPU) + " \033[0mUnknown";
}

std::string getMediaInfo() {
    // You may need to replace this command with the one suitable for your media player
    FILE *mediaInfoFile = popen("playerctl metadata --format '{{artist}} - {{title}}'", "r");

    if (mediaInfoFile) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), mediaInfoFile) != nullptr) {
            // Remove newline character
            buffer[strcspn(buffer, "\n")] = 0;
            pclose(mediaInfoFile);
            return "\033[94m" + std::string(MEDIA) + " \033[0m" + std::string(buffer);
        } else {
            pclose(mediaInfoFile);
            return "\033[94m" + std::string(MEDIA) + " \033[0mNot playing anything...";
        }
    }

    return "\033[94m" + std::string(MEDIA) + " \033[0mUnknown";
}

void colorPallate() {

    std::cout << "";
    for (int i = 0; i < 8; i++) {
        std::cout << "\033[4" << i << "m   ";
    }
    std::cout << "\033[0m\n";
    for (int i = 0; i < 8; i++) {
        std::cout << "\033[10" << i << "m   ";
    }
    std::cout << "\033[0m\n\n";

}

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

int createSharedMemory() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
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
        // Run get<example>Info functions and store the output in shared memory
        std::string output = getTitleInfo() + "\n" + getOsInfo() + "\n" + getPackageInfo() + "\n" +
                            getKernelInfo() + "\n" + getUptimeInfo() + "\n" + getShellInfo() + "\n" +
                            getCpuInfo() + "\n" + getGpuInfo() + "\n" + getStorageInfo() + "\n" +
                            getRamInfo() + "\n" + getWmInfo() + "\n" + getMediaInfo() + "\n\n";

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

int main(int argc, char *argv[]) {
    // Declare the missing identifiers
    int runDaemonFlag = 0;
    int showVersionFlag = 0;

    // Check for flags
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-d") == 0 || std::strcmp(argv[i], "--daemon") == 0) {
            runDaemonFlag = 1;
        } else if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--version") == 0) {
            showVersionFlag = 1;
        }
    }

    // Check if the daemon is already running
    if (!runDaemonFlag && access(LOCK_FILE_PATH, F_OK) == -1) {
        std::cerr << "\nBlaze daemon is not running. Please run 'blazefetch --daemon' to start the daemon first.\n" << std::endl;
        return EXIT_FAILURE;
    }

    if (showVersionFlag) {
        std::cout << "\nBlazefetch version " << VERSION << std::endl;
        std::cout << "Copyright\u00A9 2023 RifsxD" << std::endl;
        std::cout << "Blazefetch is a MIT licensed project\n" << std::endl;
        return 0;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    int shmid = createSharedMemory();
    char* shm = attachSharedMemory(shmid);

    // Correct the function calls
    detachSharedMemory(shm);

    if (runDaemonFlag) {
        runDaemon();
    } else {
        runProgram();
    }

    return 0;
}