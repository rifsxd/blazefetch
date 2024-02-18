#include "helper.cpp"

bool isDdcUtilAvailable() {
    FILE *checkDdcUtil = popen("command -v ddcutil 2>/dev/null", "r");
    if (checkDdcUtil) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), checkDdcUtil) != nullptr) {
            pclose(checkDdcUtil);
            return true;
        }
        pclose(checkDdcUtil);
    }
    return false;
}

std::string getBrightnessInfo() {

    if (!isDdcUtilAvailable()) {
        return "\033[94m" + std::string(BRIGHTNESS) + " \033[0mddcutil not found";
    }

    FILE *brightnessInfo = popen("ddcutil getvcp 10 2>/dev/null | grep -oE 'current value = *[0-9]+' | grep -oE '[0-9]+'", "r");

    if (brightnessInfo) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), brightnessInfo) != nullptr) {
            buffer[strcspn(buffer, "\n")] = 0;
            pclose(brightnessInfo);
            return "\033[94m" + std::string(BRIGHTNESS) + " \033[0m" + std::string(buffer) + "%";
        } else {
            pclose(brightnessInfo);
            return "\033[94m" + std::string(BRIGHTNESS) + " \033[0mUnknown";
        }
    }

    return "\033[94m" + std::string(BRIGHTNESS) + " \033[0mUnknown";

}