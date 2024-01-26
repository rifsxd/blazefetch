#include "helper.cpp"

std::string getKernelInfo() {
    struct utsname unameData;
    if (uname(&unameData) == 0) {
        return "\033[33m" + std::string(KERNEL) + " \033[0m" + std::string(unameData.sysname) + " " + unameData.release;
    } else {
        return "\033[33m" + std::string(KERNEL) + " \033[0mUnknown";
    }
}