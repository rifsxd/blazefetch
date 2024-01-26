#include "defines.cpp"

void printHelp() {
    std::cout << "\nUsage: blazefetch [OPTIONS]\n"
              << "Options:\n"
              << "  -d, --daemon      Run as a daemon\n"
              << "  -g, --get <INFO>  Get and display specific information (e.g., OS, GPU)\n"
              << "  -l, --live        Launch live fetch instance\n"
              << "  -c, --clear       Clears the stored cache from memory\n"
              << "  -r, --remove      Removes the lock file incase if something went wrong\n"
              << "  -v, --version     Show version information\n"
              << "  -k, --kill        Kill the daemon process\n"
              << "  -h, --help        Show this help message\n\n";
}