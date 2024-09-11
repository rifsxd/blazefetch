#include "defines.cpp"
#include "colors.cpp"

#define VERSION "2.9.15"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

// Macro defined during build process
#ifndef BUILD_ARCH
#define BUILD_ARCH "Unknown Architecture"
#endif

std::string execHelperVersion(const char* cmd) {
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

std::string getUserHostInfo() {
    std::string username = execHelperVersion("whoami");
    std::string hostname = execHelperVersion("hostname");

    // Remove newline characters from the end of each string
    if (!username.empty() && !hostname.empty()) {
        username.erase(username.find_last_not_of("\n") + 1);
        hostname.erase(hostname.find_last_not_of("\n") + 1);
        return username + "@" + hostname;
    } else {
        return "Unknown user@host";
    }
}

std::string getKernelVersion() {
    FILE* pipe = popen("uname -s -r", "r");
    if (!pipe) {
        return "Unknown Kernel";
    }

    char buffer[256];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != nullptr)
            result += buffer;
    }
    
    result.pop_back();

    pclose(pipe);
    return result;
}

std::string getArchInfo() {
    return BUILD_ARCH;
}

// New function to get compiler information
std::string getCompilerInfo() {
    #if defined(__clang__)
        return "Clang " + std::string(__clang_version__);
    #elif defined(__GNUC__) || defined(__GNUG__)
        return "GCC " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__);
    #elif defined(_MSC_VER)
        return "MSVC " + std::to_string(_MSC_VER);
    #else
        return "Unknown Compiler";
    #endif
}

int printVersion() {

    std::string hostusername = getUserHostInfo();

    std::string kernelversion = getKernelVersion();

    std::string compilerInfo = getCompilerInfo();

    std::string archInfo = getArchInfo();

    std::cout << "\n";
    std::cout << redColor << R"(
 ▄▄▄▄    ██▓    ▄▄▄      ▒███████▒▓█████   █████▒▓█████▄▄▄█████▓ ▄████▄   ██░ ██ 
▓█████▄ ▓██▒   ▒████▄    ▒ ▒ ▒ ▄▀░▓█   ▀ ▓██   ▒ ▓█   ▀▓  ██▒ ▓▒▒██▀ ▀█  ▓██░ ██▒
▒██▒ ▄██▒██░   ▒██  ▀█▄  ░ ▒ ▄▀▒░ ▒███   ▒████ ░ ▒███  ▒ ▓██░ ▒░▒▓█    ▄ ▒██▀▀██░
▒██░█▀  ▒██░   ░██▄▄▄▄██   ▄▀▒   ░▒▓█  ▄ ░▓█▒  ░ ▒▓█  ▄░ ▓██▓ ░ ▒▓▓▄ ▄██▒░▓█ ░██ 
░▓█  ▀█▓░██████▒▓█   ▓██▒▒███████▒░▒████▒░▒█░    ░▒████▒ ▒██▒ ░ ▒ ▓███▀ ░░▓█▒░██▓
░▒▓███▀▒░ ▒░▓  ░▒▒   ▓▒█░░▒▒ ▓░▒░▒░░ ▒░ ░ ▒ ░    ░░ ▒░ ░ ▒ ░░   ░ ░▒ ▒  ░ ▒ ░░▒░▒
▒░▒   ░ ░ ░ ▒  ░ ▒   ▒▒ ░░░▒ ▒ ░ ▒ ░ ░  ░ ░       ░ ░  ░   ░      ░  ▒    ▒ ░▒░ ░
 ░    ░   ░ ░    ░   ▒   ░ ░ ░ ░ ░   ░    ░ ░       ░    ░      ░         ░  ░░ ░
 ░          ░  ░     ░  ░  ░ ░       ░  ░           ░  ░        ░ ░       ░  ░  ░
      ░                  ░                                      ░                                                      
)" << std::endl;

    std::cout << greenColor << "Blazefetch version " << VERSION << "\n" << std::endl;
    std::cout << yellowColor << "Build metadata - "  << BUILD_TIME << " | " << BUILD_DATE << " | " << hostusername << " | " << kernelversion << " | " << compilerInfo << " | " << "Arch " << archInfo << "\n" << std::endl;
    std::cout << blueColor << "Copyright \u00A9 2024 RifsxD" << "\n" << std::endl;
    std::cout << cyanColor << "Blazefetch is a MIT licensed project" << resetColor << "\n" << std::endl;

    return 0;
}