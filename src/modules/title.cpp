#include "helper.cpp"

std::string getDistroAnsiColorV2() {

    std::string distroAnsiColor = getDistroColorAnsiInfo();

    return distroAnsiColor;
}

std::string getDistroColorV2(const std::string& distroName) {

    std::string distroColor = getDistroColorInfo(distroName);
    
    return distroColor;
}

std::string removeParenthesesV2(const std::string& input) {
    std::string result;
    for (char ch : input) {
        if (ch != '(' && ch != ')') {
            result += ch;
        }
    }
    return result;
}

std::string getConditionalDistroColorV2(const std::string& distroName) {

    std::string distroAnsiColor = getDistroAnsiColorV2();

    // If distroAnsiColor is not empty, return its value
    if (!distroAnsiColor.empty() && distroAnsiColor.find("Unknown")) {
        return distroAnsiColor;
    }

    std::string cleanedDistroName = removeParenthesesV2(distroName);

    std::string distroConditionalColor = getDistroColorV2(cleanedDistroName);

    // Otherwise, return the result of getDistroColor
    return distroConditionalColor;
}

std::string execHelperTitle(const char* cmd) {
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return "ERROR";
    }
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string getTitleInfo(const std::string& distroName) {
    std::string username = execHelperTitle("whoami");
    std::string hostname = execHelperTitle("hostname");

    std::string conditionalDistroColor = getConditionalDistroColorV2(distroName);

    // Remove newline characters from the end of each string
    if (!username.empty() && !hostname.empty()) {
        username.erase(username.find_last_not_of("\n") + 1);
        hostname.erase(hostname.find_last_not_of("\n") + 1);
        return conditionalDistroColor + username + "\033[0m" + "@" + conditionalDistroColor + hostname + "\033[0m";
    } else {
        return "\033[31mUnknown\033[0m";
    }
}