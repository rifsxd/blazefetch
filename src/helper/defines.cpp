#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <csignal>
#include <fcntl.h>
#include <getopt.h>
#include <vector>
#include <dirent.h>
#include <unordered_map>
#include <termios.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ifaddrs.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#ifdef ICONIC
#define OS "󰍹"
#define PACKAGES "󰏓"
#define KERNEL "󰌽"
#define UPTIME "󱘿"
#define TIME "󱑁"
#define SHELL ""
#define CPU ""
#define GPU "󰿵"
#define DISK "󰋊"
#define RAM "󰇻"
#define DE "󰖲"
#define MEDIA "󰲸"
#define NETWORK "󰛳"
#define TERM ""
#define TERM_PROG ""
#else
#define OS "󰍹 OS:"
#define PACKAGES "󰏓 PACKAGES:"
#define KERNEL "󰌽 KERNEL:"
#define UPTIME "󱘿 UPTIME:"
#define TIME "󱑁 TIME:"
#define SHELL " SHELL:"
#define CPU " CPU:"
#define GPU "󰿵 GPU:"
#define DISK "󰋊 DISK:"
#define RAM "󰇻 RAM:"
#define DE "󰖲 DE:"
#define MEDIA "󰲸 MEDIA:"
#define NETWORK "󰛳 NETWORK:"
#define TERM " TERMINAL:"
#define TERM_PROG " TERMINAL_PROGRAM:"
/*
#define OS "OS:"
#define PACKAGES "PACKAGES:"
#define KERNEL "KERNEL:"
#define UPTIME "UPTIME:"
#define TIME "TIME:"
#define SHELL "SHELL:"
#define CPU "CPU:"
#define GPU "GPU:"
#define DISK "DISK:"
#define RAM "RAM:"
#define DE "DE:"
#define MEDIA "MEDIA:"
#define NETWORK "NETWORK:"
#define TERM "TERMINAL:"
#define TERM_PROG "TERMINAL_PROGRAM:"
*/
#endif

