#include "helper.cpp"

std::string getOsInfo() {
    // Check if BLZ_OS environment variable is set
    const char* blz_os = std::getenv("BLZ_OS");
    if (blz_os != nullptr && *blz_os != '\0') {
        return "\033[32m" + std::string(OS) + " \033[0m" + std::string(blz_os);
    }

    // BLZ_OS not set or empty, fallback to reading from /etc/os-release
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
                    start = line.find('=');
                    if (start != std::string::npos) {
                        return "\033[32m" + std::string(OS) + " \033[0m" + line.substr(start + 1);
                    }
                }
            }
        }
        file.close();
    }

    // Neither BLZ_OS nor /etc/os-release could be read, return unknown
    return "\033[32m" + std::string(OS) + " \033[0mUnknown";
}
