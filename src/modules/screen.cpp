#include "helper.cpp"
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

        XRRScreenResources *res = XRRGetScreenResources(display, DefaultRootWindow(display));
        for (int i = 0; i < res->noutput; ++i) {
            XRROutputInfo *output_info = XRRGetOutputInfo(display, res, res->outputs[i]);
            if (output_info->connection == RR_Connected) {
                XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(display, res, output_info->crtc);
                if (crtc_info->mode != None) {
                    XRRModeInfo *mode_info = NULL;
                    for (int j = 0; j < res->nmode; ++j) {
                        if (res->modes[j].id == crtc_info->mode) {
                            mode_info = &res->modes[j];
                            break;
                        }
                    }
                    if (mode_info) {
                        int refreshRate = (int) (mode_info->dotClock / (mode_info->hTotal * mode_info->vTotal) + 1);

                        // Get the connection name (port)
                        std::string connectionName = output_info->name;

                        XRRFreeCrtcInfo(crtc_info);
                        XRRFreeOutputInfo(output_info);
                        XRRFreeScreenResources(res);
                        XCloseDisplay(display);
                        return "\033[92m" + std::string(SCREEN) + " \033[0m" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + "Px" + " @ " + std::to_string(refreshRate) + "Hz" + " * " + connectionName;
                    }
                }
                XRRFreeOutputInfo(output_info);
            }
        }
        XRRFreeScreenResources(res);
        XCloseDisplay(display);
        return "\033[92m" + std::string(SCREEN) + " \033[0m" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + "Px";
    }

    if (sessionType && std::string(sessionType) == "x11") {
        Display *display = XOpenDisplay(NULL);
        int screen = DefaultScreen(display);
        int screenWidth = DisplayWidth(display, screen);
        int screenHeight = DisplayHeight(display, screen);

        XRRScreenResources *res = XRRGetScreenResources(display, DefaultRootWindow(display));
        for (int i = 0; i < res->noutput; ++i) {
            XRROutputInfo *output_info = XRRGetOutputInfo(display, res, res->outputs[i]);
            if (output_info->connection == RR_Connected) {
                XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(display, res, output_info->crtc);
                if (crtc_info->mode != None) {
                    XRRModeInfo *mode_info = NULL;
                    for (int j = 0; j < res->nmode; ++j) {
                        if (res->modes[j].id == crtc_info->mode) {
                            mode_info = &res->modes[j];
                            break;
                        }
                    }
                    if (mode_info) {
                        int refreshRate = (int) (mode_info->dotClock / (mode_info->hTotal * mode_info->vTotal) + 1);

                        // Get the connection name (port)
                        std::string connectionName = output_info->name;

                        XRRFreeCrtcInfo(crtc_info);
                        XRRFreeOutputInfo(output_info);
                        XRRFreeScreenResources(res);
                        XCloseDisplay(display);
                        return "\033[92m" + std::string(SCREEN) + " \033[0m" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + "Px" + " @ " + std::to_string(refreshRate) + "Hz" + " * " + connectionName;
                    }
                }
                XRRFreeOutputInfo(output_info);
            }
        }
        XRRFreeScreenResources(res);
        XCloseDisplay(display);
        return "\033[92m" + std::string(SCREEN) + " \033[0m" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + "Px";
    }

    return "\033[92m" + std::string(SCREEN) + " \033[0mUnknown";
}
