#include "helper.cpp"

std::string getShellInfo() {
    char *shellenv = getenv("SHELL");
    if (shellenv) {
        char *shellname = strrchr(shellenv, '/');
        if (shellname) {
            return "\033[34m" + std::string(SHELL) + " \033[0m" + (shellname + 1);
        } else {
            return "\033[34m" + std::string(SHELL) + " \033[0m" + shellenv;
        }
    } else {
        return "\033[34m" + std::string(SHELL) + " \033[0mUnknown";
    }
}