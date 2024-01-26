#include "helper.cpp"

std::string getOsInfo() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp) {
        int foundPrettyName = 0;
        char line[256];
        
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "PRETTY_NAME")) {
                char *name = strchr(line, '=') + 2;
                name[strlen(name) - 2] = '\0';
                fclose(fp);
                return "\033[32m" + std::string(OS) + " \033[0m" + name;
            }
        }

        fclose(fp);
        return "\033[32m" + std::string(OS) + " \033[0mUnknown";
    } else {
        return "\033[32m" + std::string(OS) + " \033[0mUnknown";
    }
}