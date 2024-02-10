#include "helper.cpp"

bool isPlayerctlAvailable() {
    FILE *checkPlayerctl = popen("command -v playerctl 2>/dev/null", "r");
    if (checkPlayerctl) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), checkPlayerctl) != nullptr) {
            pclose(checkPlayerctl);
            return true;
        }
        pclose(checkPlayerctl);
    }
    return false;
}

std::string getMediaInfo() {
    if (!isPlayerctlAvailable()) {
        return "\033[94m" + std::string(MEDIA) + " \033[0mplayerctl not found";
    }

    FILE *mediaInfoFile = popen("playerctl metadata --format '{{artist}} - {{title}}' 2>/dev/null", "r");

    if (mediaInfoFile) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), mediaInfoFile) != nullptr) {
            buffer[strcspn(buffer, "\n")] = 0;
            pclose(mediaInfoFile);
            return "\033[94m" + std::string(MEDIA) + " \033[0m" + std::string(buffer);
        } else {
            pclose(mediaInfoFile);
            return "\033[94m" + std::string(MEDIA) + " \033[0mNot playing anything...";
        }
    }

    return "\033[94m" + std::string(MEDIA) + " \033[0mUnknown";
}
