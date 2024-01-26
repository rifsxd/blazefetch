#include "helper.cpp"

std::string getUptimeInfo() {
    FILE *uptimeFile = fopen("/proc/uptime", "r");
    if (uptimeFile) {
        double uptimeValue;
        fscanf(uptimeFile, "%lf", &uptimeValue);
        fclose(uptimeFile);

        int hours = static_cast<int>(uptimeValue / 3600);
        int minutes = static_cast<int>((uptimeValue - hours * 3600) / 60);

        return "\033[36m" + std::string(UPTIME) + " \033[0m" + std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    } else {
        return "\033[36m" + std::string(UPTIME) + " \033[0mUnknown";
    }
}