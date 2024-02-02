#include "helper.cpp"

bool isNMCLIAvailable() {
    FILE *checkNMCLI = popen("command -v nmcli 2>/dev/null", "r");
    if (checkNMCLI) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), checkNMCLI) != nullptr) {
            pclose(checkNMCLI);
            return true;
        }
        pclose(checkNMCLI);
    }
    return false;
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
    if (!isNMCLIAvailable()) {
        return "\033[94m" + std::string(NETWORK) + " \033[0mnmcli not found";
    }

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