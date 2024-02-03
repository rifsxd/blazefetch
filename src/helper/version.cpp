#include "defines.cpp"
#include "colors.cpp"

#define VERSION "2.8.0"
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

    char buffer[256];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != NULL)
            result += buffer;
    }
    
    result.pop_back();

    pclose(pipe);
    return result;
}

int printVersion() {

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

    std::cout << greenColor << "Blazefetch version " << VERSION << " (" << BUILD_DATE << " | " << BUILD_TIME << " | " << getUserName() << "@" << getHostName() << " | " << getKernelVersion() << ")" << "\n" << std::endl;
    std::cout << blueColor << "Copyright \u00A9 2024 RifsxD" << "\n" << std::endl;
    std::cout << cyanColor << "Blazefetch is a MIT licensed project" << resetColor << "\n" << std::endl;

    return 0;
}