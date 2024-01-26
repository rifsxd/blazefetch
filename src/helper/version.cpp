#include "defines.cpp"
#include <iostream>
#include <unistd.h>

#define VERSION "2.7.1"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

std::string getUserName() {
    const char *username = std::getenv("USER");
    return (username != nullptr) ? username : "Unknown User";
}

std::string getHostName() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return hostname;
    } else {
        return "Unknown Host";
    }
}

std::string getKernelVersion() {
    FILE* pipe = popen("uname -s -r", "r");
    if (!pipe) {
        return "Unknown Kernel";
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
            result.pop_back();
    }

    pclose(pipe);
    return result;
}

int printVersion() {
    std::cout << "\nBlazefetch version " << VERSION << " (" << BUILD_DATE << " | " << BUILD_TIME << " | " << getUserName() << "@" << getHostName() << " | " << getKernelVersion() << ")" << std::endl;
    std::cout << "Copyright\u00A9 2024 RifsxD" << std::endl;
    std::cout << "Blazefetch is a MIT licensed project\n" << std::endl;
    return 0;
}