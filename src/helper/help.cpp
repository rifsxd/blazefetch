#include "defines.cpp"
#include "colors.cpp"

void printHelp() {
    std::cout << orangeColor    <<      "\nUsage: blazefetch [OPTIONS]\n\n"
              << blueColor      <<      "Options:\n"
              << blueColor      <<      "  -d, --daemon      Run as a daemon\n"
              << greenColor     <<      "  -g, --get <INFO>  Get and display specific information (e.g., OS, GPU)\n"
              << greenColor     <<      "  -l, --live        Launch live fetch instance\n"
              << blueColor      <<      "  -s, --static      Show static fetch information\n"
              << greenColor     <<      "  -a, --ascii       Show ascii art of your distro (can be used as tandem with -s, --static)\n"
              << orangeColor    <<      "  -c, --clear       Clears the stored cache from memory\n"
              << redColor       <<      "  -r, --remove      Removes the lock file incase if something went wrong\n"
              << purpleColor    <<      "  -v, --version     Show version information\n"
              << redColor       <<      "  -k, --kill        Kill the daemon process\n"
              << purpleColor    <<      "  -h, --help        Show this help message\n" << resetColor << std::endl;
}