#include "helper.cpp"

bool isUtilsAvailable() {
    FILE *checkNMCLI = popen("command -v glxinfo lspci 2>/dev/null", "r");
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

std::string xGpuInfoHelper(const char *str) {
    std::string result;
    const char *start = strchr(str, '[');
    
    while (start != nullptr) {
        const char *end = strchr(start + 1, ']');
        
        if (end == nullptr) {
            break;
        }

        std::string extractedWord(start + 1, end - start - 1);

        // Remove '/' only when it appears right before and after a word
        size_t pos = extractedWord.find('/');
        while (pos != std::string::npos) {
            if ((pos > 0 && pos < extractedWord.length() - 1) && 
                (extractedWord[pos - 1] != ' ' || extractedWord[pos + 1] != ' ')) {
                // Remove '/' and concatenate the words on both sides
                extractedWord.erase(pos, 1);
            }
            pos = extractedWord.find('/', pos + 1);
        }

        result += extractedWord + " ";
        start = strchr(end + 1, '[');
    }
    
    // Remove double spaces
    size_t doubleSpacePos = result.find("  ");
    while (doubleSpacePos != std::string::npos) {
        result.erase(doubleSpacePos, 1);
        doubleSpacePos = result.find("  ");
    }

    if (!result.empty()) {
        result.pop_back(); // Remove the trailing space
    }

    return result.empty() ? "Unknown" : result;
}

std::string getVramInfo() {
    FILE* pipe = popen("glxinfo | grep 'Video memory' | awk '{print $3}'", "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    const int bufferSize = 256; // Adjust the buffer size based on expected output size
    char buffer[bufferSize];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // Remove newline characters from the result
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

    pclose(pipe);

    return result.empty() ? "Unknown" : result;
}

std::string getGpuInfo() {

    if (!isUtilsAvailable()) {
        return "\033[94m" + std::string(GPU) + " \033[0m Utils (glxinfo & lspci) not found";
    }

    // FILE *lspci = popen("echo '01:00.0 VGA compatible controller: NVIDIA Corporation GP104 [GeForce GTX 1080] (rev a1)'", "r"); // Debugging NVIDIA GPU
    // FILE *lspci = popen("echo '00:02.0 VGA compatible controller: Intel Corporation HD Graphics 620 (rev 02)'", "r"); // Debugging Intel GPU
    // FILE *lspci = popen("echo '0d:00.0 VGA compatible controller: Advanced Micro Devices, Inc. [AMD/ATI] Ellesmere [Radeon RX 590] (rev e7)'", "r"); // Debugging AMD GPU
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

                // Extract GPU vendor (Nvidia, Intel, AMD)
                std::string vendor;
                if (strstr(gpuName, "NVIDIA")) {
                    vendor = "NVIDIA";
                } else if (strstr(gpuName, "Intel")) {
                    vendor = "Intel";
                } else if (strstr(gpuName, "AMD")) {
                    vendor = "AMD";
                } else {
                    vendor = "Unknown";
                }

                std::string vraminfo = getVramInfo();

                // Extract content inside square brackets []
                std::string gpuDetails = xGpuInfoHelper(gpuName);

                std::string gpuDetailsIntel = gpuName;

                // Remove duplicate vendor words
                size_t pos = gpuDetails.find(vendor);
                if (pos != std::string::npos) {
                    gpuDetails.erase(pos, vendor.length());
                }

                // If the vendor is Intel, use the unmodified GPU name
                if (vendor == "Intel") {

                    // Remove duplicate vendor words
                    size_t pos = gpuDetailsIntel.find(vendor);
                    if (pos != std::string::npos) {
                        gpuDetailsIntel.erase(pos, vendor.length() + 1);
                    }

                    // Find and remove content within parentheses
                    size_t start = gpuDetailsIntel.find("(");
                    size_t end = gpuDetailsIntel.find(")");
                    if (start != std::string::npos && end != std::string::npos && start < end) {
                        gpuDetailsIntel.erase(start, end - start + 1);
                    }

                    // Remove the word "corporation" if it exists
                    size_t corpPos = gpuDetailsIntel.find("Corporation");
                    if (corpPos != std::string::npos) {
                        // Erase the word and the following space
                        gpuDetailsIntel.erase(corpPos, std::string("Corporation").length() + 1);
                    }

                    pclose(lspci);
                    return "\033[96m" + std::string(GPU) + " \033[0m" + vendor + " " + gpuDetailsIntel + " " + "(" + vraminfo + ")";
                }

                pclose(lspci);
                return "\033[96m" + std::string(GPU) + " \033[0m" + vendor + " " + gpuDetails + " " + "(" + vraminfo + ")";
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