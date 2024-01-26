#include "helper.cpp"

std::string getMediaInfo() {
    // You may need to replace this command with the one suitable for your media player
    FILE *mediaInfoFile = popen("playerctl metadata --format '{{artist}} - {{title}}' 2>/dev/null", "r");

    if (mediaInfoFile) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), mediaInfoFile) != nullptr) {
            // Remove newline character
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