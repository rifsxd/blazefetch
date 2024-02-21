#include "defines.cpp"

const struct option longOptions[] = {
    {"daemon", no_argument, NULL, 'd'},
    {"version", no_argument, NULL, 'v'},
    {"live", no_argument, NULL, 'l'},
    {"static", no_argument, NULL, 's'},
    {"minimal", no_argument, NULL, 'm'},
    {"pussy", no_argument, NULL, 'm'},
    {"bunny", no_argument, NULL, 'm'},
    {"ascii", no_argument, NULL, 'a'},
    {"get", required_argument, NULL, 'g'},
    {"clear", no_argument, NULL, 'c'},
    {"remove", no_argument, NULL, 'r'},
    {"help", no_argument, NULL, 'h'},
    {"kill", no_argument, NULL, 'k'},
    {"query", required_argument, NULL, 'q'},
    {NULL, 0, NULL, 0} // End of the array
};