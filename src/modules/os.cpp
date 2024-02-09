#include "helper.cpp"

std::string getOsInfo() {
    std::ifstream file("/etc/os-release");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("PRETTY_NAME") != std::string::npos) {
                size_t start = line.find('"');
                if (start != std::string::npos) {
                    size_t end = line.find('"', start + 1);
                    if (end != std::string::npos) {
                        return "\033[32m" + std::string(OS) + " \033[0m" + line.substr(start + 1, end - start - 1);
                    }
                } else {
                    start = line.find('='); // Look for '=' if '"' is not found
                    if (start != std::string::npos) {
                        return "\033[32m" + std::string(OS) + " \033[0m" + line.substr(start + 1); // Return the content after '='
                    }
                }
            }
        }
        file.close();
    }
    return "\033[32m" + std::string(OS) + " \033[0mUnknown";
}