#include "helper.cpp"

std::string getUptimeInfo() {
    FILE *uptimeFile = fopen("/proc/uptime", "r");
    if (uptimeFile) {
        double uptimeValue;
        if (fscanf(uptimeFile, "%lf", &uptimeValue) != 1) {
            // Handle fscanf error, e.g., print an error message or return a default value
            fclose(uptimeFile);
            return "\033[36m" + std::string(UPTIME) + " \033[0mError reading uptime";
        }
        fclose(uptimeFile);

        int hours = static_cast<int>(uptimeValue / 3600);
        int minutes = static_cast<int>((uptimeValue - hours * 3600) / 60);

        return "\033[36m" + std::string(UPTIME) + " \033[0m" + std::to_string(hours) + "H " + std::to_string(minutes) + "M";
    } else {
        return "\033[36m" + std::string(UPTIME) + " \033[0mUnknown";
    }
}
