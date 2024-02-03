#include "defines.cpp"
#include "modules.cpp"

void getInfoAndPrint(const std::vector<std::string>& infoTypes) {
    for (const auto& info : infoTypes) {
        if (info == "USER") {
            std::cout << getTitleInfo() << std::endl;
        } else if (info == "OS") {
            std::cout << getOsInfo() << std::endl;
        } else if (info == "PACKAGES") {
            std::cout << getPackageInfo() << std::endl;
        } else if (info == "KERNEL") {
            std::cout << getKernelInfo() << std::endl;
        } else if (info == "TIME") {
            std::cout << getTimeInfo() << std::endl;
        } else if (info == "UPTIME") {
            std::cout << getUptimeInfo() << std::endl;
        } else if (info == "SHELL") {
            std::cout << getShellInfo() << std::endl;
        } else if (info == "CPU") {
            std::cout << getCpuInfo() << std::endl;
        } else if (info == "GPU") {
            std::cout << getGpuInfo() << std::endl;
        } else if (info == "DISK") {
            std::cout << getStorageInfo() << std::endl;
        } else if (info == "RAM") {
            std::cout << getRamInfo() << std::endl;
        } else if (info == "DE") {
            std::cout << getDEInfo() << std::endl;
        } else if (info == "MEDIA") {
            std::cout << getMediaInfo() << std::endl;
        } else if (info == "NETWORK") {
            std::cout << getNetworkStatusInfo() << std::endl;
        /**/ } else if (info == "TERM") {  // ENABLE TERM
            std::cout << getTerminalInfo() << std::endl; /**/
        } else {
            std::cerr << "Invalid information type: " << info << std::endl;
        }
    }
}

void getInfoAndPrint(const std::vector<std::string>& infoTypes);