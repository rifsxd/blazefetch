// C++ Core Headers
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <ctime>
#include <fstream>
#include <csignal>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <memory>
#include <array>
#include <chrono>
#include <thread>

// Base Headers
#include <ifaddrs.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>

// Sys Headers
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>

// XOrg Headers
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

// Alsa Headers
#include <alsa/asoundlib.h>

// Curl Headers
#include <curl/curl.h>

// Fetch Titles Defines
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
#define BRIGHTNESS "󰃠"
#define DISK "󰋊"
#define RAM "󰇻"
#define DE "󰖲"
#define MEDIA "󰲸"
#define VOLUME_HIGH ""
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
#define BRIGHTNESS "󰃠 BRIGHTNESS:"
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

// Fetch Function Defines
#define ASCII_INFO getGlyphInfo()
#define OS_INFO getOsInfo()
#define USER_INFO getTitleInfo(distroName)
#define HOST_INFO getHostInfo()
#define PACKAGES_INFO getPackageInfo()
#define KERNEL_INFO getKernelInfo()
#define UPTIME_INFO getUptimeInfo()
#define TIME_INFO getTimeInfo()
#define DATE_INFO getDateInfo()
#define SHELL_INFO getShellInfo()
#define CPU_INFO getCpuInfo()
#define GPU_INFO getGpuInfo()
#define SCREEN_INFO getScreenResInfo()
#define BRIGHTNESS_INFO getBrightnessInfo()
#define DISK_INFO getStorageInfo()
#define RAM_INFO getRamInfo()
#define DE_INFO getDEInfo()
#define MEDIA_INFO getMediaInfo()
#define VOLUME_INFO getAudioVolumeInfo()
#define NETWORK_INFO getNetworkStatusInfo()
#define TERM_INFO getTerminalInfo()
#define PUSSY_GREETING getRandomPussyGreeting()
#define BUNNY_GREETING getRandomBunnyGreeting()