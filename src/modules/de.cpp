#include "helper.cpp"

std::string getDEInfo() {
    char *waylandDisplay = getenv("WAYLAND_DISPLAY");
    char *Session = getenv("XDG_SESSION_TYPE");

    if (strstr(Session, "wayland") != nullptr) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        return "\033[38;5;93m" + std::string(DE) + " \033[0m" + (xdgDesktop ? xdgDesktop : "Unknown") + + " (" + Session + ")";
    } else if ((waylandDisplay) != nullptr) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        return "\033[38;5;93m" + std::string(DE) + " \033[0m" + (xdgDesktop ? xdgDesktop : "Unknown") + " (" + Session + ")";
    } else if (strstr(Session, "x11") != nullptr) {
        char *xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
        if (xdgDesktop) {
            return "\033[38;5;93m" + std::string(DE) + " \033[0m" + xdgDesktop + " (" + Session + ")";
        }
    }

    // Fall back to the final else block if none of the conditions are met
    Display *display = XOpenDisplay(NULL);
    if (display) {
        Window root = DefaultRootWindow(display);

        Atom netSupportingWmCheckAtom = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
        Atom netWmNameAtom = XInternAtom(display, "_NET_WM_NAME", False);
        Atom utf8StringAtom = XInternAtom(display, "UTF8_STRING", False);

        if (netSupportingWmCheckAtom != None && netWmNameAtom != None) {
            Atom actualType;
            int actualFormat;
            unsigned long nItems, bytesAfter;
            unsigned char *propValue = NULL;

            if (XGetWindowProperty(display, root, netSupportingWmCheckAtom, 0, 1, False,
                                   XA_WINDOW, &actualType, &actualFormat,
                                   &nItems, &bytesAfter, &propValue) == Success && propValue) {
                Window supportingWmCheck = *((Window *)propValue);
                XFree(propValue);

                if (XGetWindowProperty(display, supportingWmCheck, netWmNameAtom, 0, 1024, False,
                                       utf8StringAtom, &actualType, &actualFormat,
                                       &nItems, &bytesAfter, &propValue) == Success && propValue) {
                    std::string wmName(reinterpret_cast<char*>(propValue));
                    XFree(propValue);
                    XCloseDisplay(display);
                    return "\033[38;5;93m" + std::string(DE) + " \033[0m" + wmName + " (" + Session + ")";
                }
            }
        }

        XCloseDisplay(display);
    }

    return "\033[38;5;93m" + std::string(DE) + " \033[0mUnknown";
}