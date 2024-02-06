#include "helper.cpp"

std::string execHelperTitle(const char* cmd) {
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return "ERROR";
    }
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string getTitleInfo() {
    std::string username = execHelperTitle("whoami");
    std::string hostname = execHelperTitle("hostname");

    // Remove newline characters from the end of each string
    if (!username.empty() && !hostname.empty()) {
        username.erase(username.find_last_not_of("\n") + 1);
        hostname.erase(hostname.find_last_not_of("\n") + 1);
        return "\033[31m" + username + "@" + hostname + "\033[0m";
    } else {
        return "\033[31mUnknown\033[0m";
    }
}