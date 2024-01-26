#include "helper.cpp"

std::string getTitleInfo() {
    char username[256];
    char hostname[256];
    if (getlogin_r(username, sizeof(username)) == 0 && gethostname(hostname, sizeof(hostname)) == 0) {
        return "\033[31m" + std::string() + "\033[31m" + username + "@" + hostname + "\033[0m";
    } else {
        return "\033[31m" + std::string() + " \033[31mUnknown\033[0m";
    }
}