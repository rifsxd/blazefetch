#include "defines.cpp"
#include "colors.cpp"

void printHelp() {
    std::cout << orangeColor    <<      "\nUsage: blazefetch [OPTIONS]\n\n"
              << blueColor      <<      "Options:\n"
              << blueColor      <<      "  -d, --daemon             Run as a daemon\n"
              << greenColor     <<      "  -g, --get <MODULE>       Get and display specific information (example: OS, GPU, etc)\n"
              << greenColor     <<      "  -l, --live               Launch live fetch instance\n"
              << blueColor      <<      "  -s, --static             Show static fetch information\n"
              << blueColor      <<      "  -m, --minimal            Show Minimal fetch information\n"
              << greenColor     <<      "  -a, --ascii              Show ascii art of your distro\n"
              << orangeColor    <<      "  -c, --clear              Clears the stored cache from memory\n"
              << redColor       <<      "  -r, --remove             Removes the lock file incase if something went wrong\n"
              << purpleColor    <<      "  -v, --version            Show version information\n"
              << redColor       <<      "  -k, --kill               Kill the daemon process\n"
              << blueColor      <<      "  -q, --query <NUMBER>     Query git logs and commits\n"
              << purpleColor    <<      "  -h, --help               Show this help message\n\n"
              << blueColor      <<      "Combined Options:\n"
              << blueColor      <<      "  -a -m, --ascii --minimal    Show distro ascii art combined with minimal fetch info\n"
              << greenColor      <<      "  -a -s, --ascii --static     Show distro ascii art combined with static fetch info\n" << resetColor << std::endl;   
}