#include "helper.cpp"
#include <string>
#include <cstdlib>
#include <iostream>
#include <X11/Xlib.h>
// #include <wayland-client-core.h>

std::string getScreenResInfo() {
    std::string resInfo;

    // Detecting the current display server
    const char *sessionType = std::getenv("XDG_SESSION_TYPE");

    // Will find a logical way to get wayland resolution info (currently using X cuz wayland has shit documentations -_-)

    if (sessionType && std::string(sessionType) == "wayland") {
        
        Display *display = XOpenDisplay(NULL);
        int screen = DefaultScreen(display);
        int screenWidth = DisplayWidth(display, screen);
        int screenHeight = DisplayHeight(display, screen);
        XCloseDisplay(display);

        return "\033[92m" + std::string(SCREEN) + " \033[0m" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + " " + "(wayland)";
    } 

    if (sessionType && std::string(sessionType) == "x11") {
        Display *display = XOpenDisplay(NULL);
        int screen = DefaultScreen(display);
        int screenWidth = DisplayWidth(display, screen);
        int screenHeight = DisplayHeight(display, screen);
        XCloseDisplay(display);

        return "\033[92m" + std::string(SCREEN) + " \033[0m" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + " " + "(x11)";
    }

    return "\033[92m" + std::string(SCREEN) + " \033[0mUnknown";;
}