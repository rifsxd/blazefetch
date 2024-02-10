#include "helper.cpp"

bool isAlsaAvailable() {
    FILE *checkALSA = popen("command -v amixer 2>/dev/null", "r");
    if (checkALSA) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), checkALSA) != nullptr) {
            pclose(checkALSA);
            return true;
        }
        pclose(checkALSA);
    }
    return false;
}

// Function to get the current audio volume
std::string getAudioVolumeInfo() {

    if (!isAlsaAvailable()) {
        return "\033[31m" + std::string(VOLUME_MUTED) + " \033[0m'amixer' not found";
    }	

    std::string command = "amixer sget Master | grep 'Front Left:' | awk -F'[][]' '{ print $2 }'";
    std::string result;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Error: Unable to open pipe for command execution";
    }

    char buffer[256];
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != nullptr) {
            result += buffer;
        }
    }

    pclose(pipe);

    // Remove newline characters from the result
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

    std::stringstream ss(result);
    int volume;
    ss >> volume;

    std::string volumeString;
    if (volume == 0)
        volumeString = "\033[31m" + std::string(VOLUME_MUTED) + " " + "\033[0m" + "Muted!";
    else {
        // Assuming 100 as the maximum volume value
        int percent = (volume * 100) / 100; // Change denominator to reflect your max volume
        if (percent < 50)
            volumeString = "\033[33m" + std::string(VOLUME_LOW) + "\033[0m";
        else
            volumeString = "\033[32m" + std::string(VOLUME_HIGH) + "\033[0m";
        volumeString += " " + std::to_string(percent) + "%";
    }

    return "\033[33m" + volumeString;
}