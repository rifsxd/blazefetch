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
#include <cstdio>
#include <memory>
#include <array>
#include <ifaddrs.h>

#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

#include <alsa/asoundlib.h>

#include <curl/curl.h>

#ifdef ICONIC
#define ASCII ""
#define USER ""
#define OS "󰍹"
#define HOST "󰒋"
#define PACKAGES "󰏓"
#define KERNEL "󰌽"
#define UPTIME "󱘿"
#define TIME "󱑁"
#define DATE ""
#define SHELL ""
#define CPU ""
#define GPU "󰿵"
#define SCREEN "󰹑"
#define DISK "󰋊"
#define RAM "󰇻"
#define DE "󰖲"
#define MEDIA "󰲸"
#define VOLUME_HIGH ":"
#define VOLUME_LOW ""
#define VOLUME_MUTED ""
#define NETWORK "󰛳"
#define TERM ""
#define TERM_PROG ""
#else
#define ASCII " ASCII:"
#define USER " USER:"
#define OS "󰍹 OS:"
#define HOST "󰒋 HOST:"
#define PACKAGES "󰏓 PACKAGES:"
#define KERNEL "󰌽 KERNEL:"
#define UPTIME "󱘿 UPTIME:"
#define TIME "󱑁 TIME:"
#define DATE " DATE:"
#define SHELL " SHELL:"
#define CPU " CPU:"
#define GPU "󰿵 GPU:"
#define SCREEN "󰹑 SCREEN:"
#define DISK "󰋊 DISK:"
#define RAM "󰇻 RAM:"
#define DE "󰖲 DE:"
#define MEDIA "󰲸 MEDIA:"
#define VOLUME_HIGH " VOLUME:"
#define VOLUME_LOW " VOLUME:"
#define VOLUME_MUTED " VOLUME:"
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

