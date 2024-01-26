#include "helper.cpp"



std::string formatFreq(const std::string& freq) {
    std::istringstream ss(freq);
    long long freqValue;
    ss >> freqValue;

    // Check if the conversion was successful
    if (ss.fail()) {
        return "Unknown";
    }

    // Convert to GHz and format with two decimal places
    double freqGHz = static_cast<double>(freqValue) / 1e6;  // Changed from 1e9 to 1e6
    std::stringstream formattedFreq;
    formattedFreq << std::fixed << std::setprecision(2) << freqGHz;

    return "(" + formattedFreq.str() + "GHz" + ")";
}

// Function to get the maximum CPU frequency for all CPUs
std::string getMaxCpuFreq() {
    const std::string cpuPath = "/sys/devices/system/cpu/";
    const std::string scalingMaxFreqFile = "cpufreq/scaling_max_freq";

    std::vector<std::string> cpuDirectories;

    // Find all CPU directories
    for (int i = 0; ; ++i) {
        std::stringstream cpuDir;
        cpuDir << cpuPath << "cpu" << i;

        std::ifstream cpuDirCheck(cpuDir.str());
        if (!cpuDirCheck) {
            // No more CPUs found
            break;
        }

        cpuDirectories.push_back(cpuDir.str());
    }

    std::string overallMaxFreq;

    // Iterate over all CPU directories
    for (const std::string& cpuDir : cpuDirectories) {
        std::ifstream maxFreqFile(cpuDir + "/" + scalingMaxFreqFile);
        std::string maxFreq;

        if (maxFreqFile.is_open()) {
            maxFreqFile >> maxFreq;
            maxFreqFile.close();

            // Choose the highest frequency among all CPUs
            if (overallMaxFreq.empty() || maxFreq > overallMaxFreq) {
                overallMaxFreq = maxFreq;
            }
        }
    }

    if (!overallMaxFreq.empty()) {
        return formatFreq(overallMaxFreq);
    }

    return "(Unknown)";
}

std::string getCpuInfo() {
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo) {
        char line[256];
        std::unordered_map<std::string, int> cpuModels;

        std::string cpufreq = getMaxCpuFreq();

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

            for (const auto &entry : cpuModels) {
                result += entry.first + " (x" + std::to_string(entry.second) + "), ";
            }

            // Remove the trailing comma and space
            result.pop_back();
            result.pop_back();

            // Add maximum CPU frequency
            result += " @ " + cpufreq;

            return result;
        }
    }

    return "\033[95m" + std::string(CPU) + " \033[0mUnknown";
}