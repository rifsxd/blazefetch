#include "helper.cpp"

std::string getRamInfo() {
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if (meminfo) {
        long total_mem = 0, free_mem = 0, buffers = 0, cached = 0;
        char line[256];

        while (fgets(line, sizeof(line), meminfo) != NULL) {
            if (sscanf(line, "MemTotal: %ld kB", &total_mem) == 1) {
                continue;
            }
            if (sscanf(line, "MemFree: %ld kB", &free_mem) == 1) {
                continue;
            }
            if (sscanf(line, "Buffers: %ld kB", &buffers) == 1) {
                continue;
            }
            if (sscanf(line, "Cached: %ld kB", &cached) == 1) {
                continue;
            }
        }

        fclose(meminfo);

        if (total_mem > 0 && free_mem > 0) {
            long used_mem = total_mem - free_mem - buffers - cached;
            return "\033[38;5;198m" + std::string(RAM) + " \033[0m" + std::to_string(used_mem / 1024) + "MB / " + std::to_string(total_mem / 1024) + "MB";
        } else {
            return "\033[38;5;198m" + std::string(RAM) + " \033[0mUnknown";
        }
    } else {
        return "\033[38;5;198m" + std::string(RAM) + " \033[0mUnknown";
    }
}