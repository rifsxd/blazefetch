#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <csignal>
#include <fcntl.h>
#include <getopt.h>
#include <vector>
#include <dirent.h>
#include <unordered_map>
#include <termios.h>

#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ifaddrs.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#define VERSION "2.6.0"
#define SHM_SIZE 4096
#define LOCK_FILE_PATH "/tmp/blazefetch.lock"

const char* PROCESS_NAME = "blazefetch";

#ifdef ICONIC
#define OS "󰍹"
#define PACKAGES "󰏓"
#define KERNEL "󰌽"
#define UPTIME "󱘿"
#define TIME "󱑁"
#define SHELL ""
#define CPU ""
#define GPU "󰿵"
#define DISK "󰋊"
#define RAM "󰇻"
#define DE "󰖲"
#define MEDIA "󰲸"
#define NETWORK "󰛳"
#define TERM ""
#define TERM_PROG ""
#else
#define OS "󰍹 OS:"
#define PACKAGES "󰏓 PACKAGES:"
#define KERNEL "󰌽 KERNEL:"
#define UPTIME "󱘿 UPTIME:"
#define TIME "󱑁 TIME:"
#define SHELL " SHELL:"
#define CPU " CPU:"
#define GPU "󰿵 GPU:"
#define DISK "󰋊 DISK:"
#define RAM "󰇻 RAM:"
#define DE "󰖲 DE:"
#define MEDIA "󰲸 MEDIA:"
#define NETWORK "󰛳 NETWORK:"
#define TERM " TERMINAL:"
#define TERM_PROG " TERMINAL_PROGRAM:"
/* basic words
#define OS "OS:"
#define PACKAGES "PACKAGES:"
#define KERNEL "KERNEL:"
#define UPTIME "UPTIME:"
#define SHELL "SHELL:"
#define CPU "CPU:"
#define GPU "GPU:"
#define DISK "DISK:"
#define RAM "RAM:"
#define DE "DE:"
#define MEDIA "MEDIA:"
*/
#endif

bool liveMode = false;

// -------------------------------------------------------------- Info Func Start Point -------------------------------------------------------------- //

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
        return "\033[33m" + std::string(KERNEL) + " \033[0m" + std::string(unameData.sysname) + " " + unameData.release;
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

std::string getDEInfo() {
    char *waylandDisplay = getenv("WAYLAND_DISPLAY");
    char *Session = getenv("XDG_SESSION_TYPE");

    if (strstr(Session, "wayland") != nullptr) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        return "\033[38;5;93m" + std::string(DE) + " \033[0m" + (xdgDesktop ? xdgDesktop : "Unknown");
    } else if ((waylandDisplay) != nullptr) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        return "\033[38;5;93m" + std::string(DE) + " \033[0m" + (xdgDesktop ? xdgDesktop : "Unknown");
    } else if (strstr(Session, "x11") != nullptr) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        if (xdgDesktop) {
            return "\033[38;5;93m" + std::string(DE) + " \033[0m" + xdgDesktop;
        }
    }

    // Fall back to the final else block if none of the conditions are met
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
                    return "\033[38;5;93m" + std::string(DE) + " \033[0m" + wmName;
                }
            }
        }

        XCloseDisplay(display);
    }

    return "\033[38;5;93m" + std::string(DE) + " \033[0mUnknown";
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
        std::unordered_map<std::string, int> cpuModels;

        while (fgets(line, sizeof(line), cpuinfo) != NULL) {
            if (strstr(line, "model name")) {
                char *model = strchr(line, ':') + 2;
                model[strlen(model) - 1] = '\0'; // Remove the trailing newline
                cpuModels[std::string(model)]++;
            }
        }

        fclose(cpuinfo);

        if (!cpuModels.empty()) {
            std::string result = "\033[95m" + std::string(CPU) + " \033[0m";
            
            for (const auto& entry : cpuModels) {
                result += entry.first + " (x" + std::to_string(entry.second) + "), ";
            }

            // Remove the trailing comma and space
            result.pop_back();
            result.pop_back();
            
            return result;
        }
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
    FILE *mediaInfoFile = popen("playerctl metadata --format '{{artist}} - {{title}}' 2>/dev/null", "r");

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

std::string runNetworkInfoCMD(const char* command) {
    char buffer[128];
    std::string result = "";

    // Open the command for reading
    FILE* fp = popen(command, "r");
    if (fp == nullptr) {
        perror("popen");
        return result;
    }

    // Read the output a line at a time
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        result += buffer;
    }

    // Close the file pointer
    pclose(fp);

    // Remove trailing newline character
    result.erase(result.find_last_not_of("\n") + 1);

    return result;
}

std::string getNetworkStatusInfo() {
    std::string wiredInterfaceCommand = "ls /sys/class/net | grep -E '^enp[0-9]+s[0-9]+$'";
    std::string wirelessInterfaceCommand1 = "ls /sys/class/net | grep -E '^wlan[0-9]+$'";
    std::string wirelessInterfaceCommand2 = "ls /sys/class/net | grep -E '^wlp[0-9]+s[0-9]+$'";

    std::string activeInterface;

    // Check for wired interface
    activeInterface = runNetworkInfoCMD(wiredInterfaceCommand.c_str());
    if (activeInterface.empty()) {
        // Check for wireless interface using the first pattern
        activeInterface = runNetworkInfoCMD(wirelessInterfaceCommand1.c_str());

        // If the first pattern is null, try the second pattern
        if (activeInterface.empty()) {
            activeInterface = runNetworkInfoCMD(wirelessInterfaceCommand2.c_str());
        }
    }

    if (!activeInterface.empty()) {
        std::string connectionStateCommand = "nmcli dev show " + activeInterface + " | grep -E '^GENERAL.STATE' | awk '{print $2}'";
        std::string connectionState = runNetworkInfoCMD(connectionStateCommand.c_str());

        if (!connectionState.empty()) {
            int state = std::stoi(connectionState);
            if (state == 100) {
                std::string interfaceType = activeInterface.substr(0, 3); // Extract "enp" or "wlan" or "wlp"
                return "\033[94m" + std::string(NETWORK) + " \033[0mConnected (" + interfaceType + ": " + activeInterface + ")";
            } else if (state == 30) {
                return "\033[94m" + std::string(NETWORK) + " \033[0mDisconnected";
            }
        }
    }

    return "\033[94m" + std::string(NETWORK) + " \033[0mUnknown";
}

std::string getTimeInfo() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    char timeBuffer[80];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    return "\033[96m" + std::string(TIME) + " \033[0m" + std::string(timeBuffer);
}

/* ENABLE IF YOU NEED IT - ALSO ENABLE IN LINES 706 & 786 */

std::string getTerminalInfo() {
    char* term = getenv("TERM");
    char* termProgram = getenv("TERM_PROGRAM");

    if (term != nullptr) {
        std::string terminal = "\033[35m" + std::string(TERM) + " \033[0m" + std::string(term);
        
        if (termProgram != nullptr) {
            terminal += "\n\033[32m╰─| \033[35m" + std::string(TERM_PROG) + " \033[0m" + std::string(termProgram);
        }

        return terminal;
    } else {
        return "\033[35m" + std::string(TERM) + " \033[0mUnknown... \033[35mTerminal information not available?!\033[0m";
    }
}

/**/

// -------------------------------------------------------------- Info Func End Point -------------------------------------------------------------- //

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

void clearStoredMemory() {
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Clear the shared memory content
    memset(shm, 0, SHM_SIZE);

    // Detach the shared memory segment
    shmdt(shm);

    std::cout << "\nStored blaze memory has been cleared.\n" << std::endl;
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

void printHelp() {
    std::cout << "\nUsage: blazefetch [OPTIONS]\n"
              << "Options:\n"
              << "  -d, --daemon      Run as a daemon\n"
              << "  -g, --get <INFO>  Get and display specific information (e.g., OS, GPU)\n"
              << "  -l, --live		  Launch live fetch instance\n"
              << "  -c, --clear       Clears the stored cache from memory\n"
              << "  -r, --remove      Removes the lock file incase if somethign went wrong\n"
              << "  -v, --version     Show version information\n"
              << "  -k, --kill        Kill the daemon process\n"
              << "  -h, --help        Show this help message\n\n";
}

void getInfoAndPrint(const std::vector<std::string>& infoTypes) {
    for (const auto& info : infoTypes) {
        if (info == "USER") {
            std::cout << getTitleInfo() << std::endl;
        } else if (info == "OS") {
            std::cout << getOsInfo() << std::endl;
        } else if (info == "PACKAGES") {
            std::cout << getPackageInfo() << std::endl;
        } else if (info == "KERNEL") {
            std::cout << getKernelInfo() << std::endl;
        } else if (info == "TIME") {
            std::cout << getTimeInfo() << std::endl;
        } else if (info == "UPTIME") {
            std::cout << getUptimeInfo() << std::endl;
        } else if (info == "SHELL") {
            std::cout << getShellInfo() << std::endl;
        } else if (info == "CPU") {
            std::cout << getCpuInfo() << std::endl;
        } else if (info == "GPU") {
            std::cout << getGpuInfo() << std::endl;
        } else if (info == "DISK") {
            std::cout << getStorageInfo() << std::endl;
        } else if (info == "RAM") {
            std::cout << getRamInfo() << std::endl;
        } else if (info == "DE") {
            std::cout << getDEInfo() << std::endl;
        } else if (info == "MEDIA") {
            std::cout << getMediaInfo() << std::endl;
        } else if (info == "NETWORK") {
            std::cout << getNetworkStatusInfo() << std::endl;
        /**/ } else if (info == "TERM") {  // ENABLE TERM
            std::cout << getTerminalInfo() << std::endl; /**/
        } else {
            std::cerr << "Invalid information type: " << info << std::endl;
        }
    }
}

const struct option longOptions[] = {
    {"daemon", no_argument, NULL, 'd'},
    {"version", no_argument, NULL, 'v'},
    {"get", required_argument, NULL, 'g'},
    {"clear", no_argument, NULL, 'c'},
    {"remove", no_argument, NULL, 'r'},
    {"help", no_argument, NULL, 'h'},
    {"kill", no_argument, NULL, 'k'},
    {NULL, 0, NULL, 0} // End of the array
};

void getInfoAndPrint(const std::vector<std::string>& infoTypes);

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

int main(int argc, char *argv[]) {
    // Declare the missing identifiers
    int runDaemonFlag = 0;
    int showVersionFlag = 0;
    int clearMemoryFlag = 0;
    int removeLockFlag = 0;
    int showHelpFlag = 0;
    int showLiveFlag = 0;
    int killDaemonFlag = 0;

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
        } else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            showHelpFlag = 1;
        } else if (std::strcmp(argv[i], "-k") == 0 || std::strcmp(argv[i], "--kill") == 0) {
            killDaemonFlag = 1;
        } else if ((std::strcmp(argv[i], "-r") == 0 || std::strcmp(argv[i], "--remove") == 0) && access(LOCK_FILE_PATH, F_OK) == -1) {
            std::cerr << "\nNo lock file found. Nothing to remove.\n" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Check if the daemon is already running (excluding -v, -c, -h, --remove, and --daemon flags)
    if (!runDaemonFlag && !showVersionFlag && !clearMemoryFlag && !showHelpFlag && !removeLockFlag && access(LOCK_FILE_PATH, F_OK) == -1 && !showLiveFlag && !killDaemonFlag) {
        std::cerr << "\nBlaze daemon is not running. Please run 'blazefetch --daemon' to start the daemon first.\n" << std::endl;
        return EXIT_FAILURE;
    }

    const char* getInfo = nullptr; // Variable to store the specified information to fetch

    std::vector<std::string> getInfoTypes;

    int opt;
    while ((opt = getopt_long(argc, argv, "dgl:vhcrk", longOptions, NULL)) != -1) {
        switch (opt) {
            case 'd':
                runDaemonFlag = 1;
                break;
            case 'g':
                getInfoTypes.push_back(optarg);
                break;
            case 'l':
                showLiveFlag = 1;
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

    if (showVersionFlag) {
        std::cout << "\nBlazefetch version " << VERSION << std::endl;
        std::cout << "Copyright\u00A9 2023 RifsxD" << std::endl;
        std::cout << "Blazefetch is a MIT licensed project\n" << std::endl;
        return 0;
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
        std::cout << "Blazefetch version " << VERSION << std::endl;
    } else if (clearMemoryFlag) {
        clearStoredMemory();
    } else if (showLiveFlag) {
        runLiveProgram();
    } else if (!getInfoTypes.empty()) {
        getInfoAndPrint(getInfoTypes);
    } else if (showHelpFlag) {
        printHelp();
    } else if (killDaemonFlag) {
        killBlazefetchProcess();
    } else {
        runProgram();
    }

    return 0;
}