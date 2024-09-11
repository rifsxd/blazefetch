#include "defines.cpp"
#include "colors.cpp"

#define PROGRAM_NAME "Blazefetch"
#define VERSION "2.9.17"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__
#define COPYRIGHT_YEAR "2023"
#define COPYRIGHT_OWNER "RifsxD"

// Macro defined during build process
#ifndef BUILD_ARCH
#define BUILD_ARCH "Unknown Architecture"
#endif

std::string getArchInfo() {
    return BUILD_ARCH;
}

// New function to get compiler information
std::string getCompilerInfo() {
    #if defined(__clang__)
        return "Clang " + std::string(__clang_version__);
    #elif defined(__GNUC__) || defined(__GNUG__)
        return "GCC " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__);
    #else
        return "Unknown Compiler";
    #endif
}

int printVersion() {

    std::string compilerInfo = getCompilerInfo();

    std::string archInfo = getArchInfo();

    const std::string linkSource = "\033]8;;https://github.com/rifsxd/blazefetch\033\\";

    const std::string linkCopyright = "\033]8;;https://raw.githubusercontent.com/rifsxd/blazefetch/main/LICENSE\033\\";

    const std::string linkEnd = "\033]8;;\033\\";

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

    std::cout << greenColor << linkSource << PROGRAM_NAME << linkEnd << " version " << VERSION << "\n" << std::endl;
    std::cout << yellowColor << "Build metadata - "  << BUILD_TIME << " | " << BUILD_DATE << " | " << compilerInfo << " | " << "Arch " << archInfo << "\n" << std::endl;
    std::cout << blueColor << linkCopyright << "Copyright \u00A9 " << COPYRIGHT_YEAR << " " << COPYRIGHT_OWNER << " " << "(MIT)" << linkEnd << resetColor << "\n" << std::endl;

    return 0;
}