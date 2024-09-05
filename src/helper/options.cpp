#include "defines.cpp"

const struct option longOptions[] = {
    {"daemon", no_argument, nullptr, 'd'},
    {"version", no_argument, nullptr, 'v'},
    {"live", no_argument, nullptr, 'l'},
    {"static", no_argument, nullptr, 's'},
    {"minimal", no_argument, nullptr, 'm'},
    {"pussy", no_argument, nullptr, 'p'},
    {"bunny", no_argument, nullptr, 'b'},
    {"ascii", no_argument, nullptr, 'a'},
    {"get", required_argument, nullptr, 'g'},
    {"clear", no_argument, nullptr, 'c'},
    {"remove", no_argument, nullptr, 'r'},
    {"help", no_argument, nullptr, 'h'},
    {"kill", no_argument, nullptr, 'k'},
    {"query", required_argument, nullptr, 'q'},
    {nullptr, 0, nullptr, 0} // End of the array
};