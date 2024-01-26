#include "helper.cpp"

std::string getCpuInfo() {
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo) {
        char line[256];
        std::unordered_map<std::string, int> cpuModels;

        while (fgets(line, sizeof(line), cpuinfo) != NULL) {
            if (strstr(line, "model name")) {
                char *model = strchr(line, ':') + 2;
                model[strlen(model) - 1] = '\0'; // Remove the trailing newline
                cpuModels[std::string(model)]++;
            }
        }

        fclose(cpuinfo);

        if (!cpuModels.empty()) {
            std::string result = "\033[95m" + std::string(CPU) + " \033[0m";
            
            for (const auto& entry : cpuModels) {
                result += entry.first + " (x" + std::to_string(entry.second) + "), ";
            }

            // Remove the trailing comma and space
            result.pop_back();
            result.pop_back();
            
            return result;
        }
    }

    return "\033[95m" + std::string(CPU) + " \033[0mUnknown";
}