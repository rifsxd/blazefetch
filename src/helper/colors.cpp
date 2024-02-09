#pragma once

#include "defines.cpp"

const std::string resetColor = "\033[0m";
const std::string redColor = "\033[1;31m";
const std::string orangeColor = "\033[1;33m";
const std::string yellowColor = "\033[1;93m";
const std::string greenColor = "\033[1;32m";
const std::string cyanColor = "\033[1;36m";
const std::string blueColor = "\033[1;34m";
const std::string purpleColor = "\033[1;35m";
const std::string pinkColor = "\033[1;35m";
const std::string brownColor = "\033[1;33m";
const std::string grayColor = "\033[1;37m";
const std::string lightBlueColor = "\033[1;94m";
const std::string lightGreenColor = "\033[1;92m";

void colorPallate() {

    std::cout << "";
    for (int i = 0; i < 8; i++) {
        std::cout << "\033[4" << i << "m   ";
    }
    std::cout << "\033[0m\n";
    for (int i = 0; i < 8; i++) {
        std::cout << "\033[10" << i << "m   ";
    }
    std::cout << "\033[0m\n\n";

}

std::string generateAnsiColor(const char* colorValue) {
    // Split RGB values
    int r, g, b;
    if (sscanf(colorValue, "38;2;%d;%d;%d", &r, &g, &b) == 3) {
        // Convert RGB values to ANSI color escape code
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    } else {
        // Return default color if parsing fails
        return "Unknown";
    }
}

std::string getDistroColorAnsiInfo() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp) {
        int foundAnsiColor = 0;
        char line[256];
        
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "ANSI_COLOR")) {
                char *colorValue = strchr(line, '=') + 2;
                colorValue[strlen(colorValue) - 2] = '\0';
                
                // Process colorValue and convert it to ANSI color escape code
                std::string ansiColor = generateAnsiColor(colorValue);
                
                fclose(fp);
                return ansiColor;
            }
        }

        fclose(fp);
        return "Unknown";  // Return default color if not found
    } else {
        return "Unknown";
    }
}

std::string getDistroColorInfo(const std::string& distroName) {

    char firstChar = distroName.empty() ? ' ' : std::toupper(distroName[0]);

    

    switch (firstChar) {
        case 'A':
            return redColor;
        case 'B':
            return orangeColor;
        case 'C':
            return redColor;
        case 'D':
            return greenColor;
        case 'E':
            return cyanColor;
        case 'F':
            return blueColor;
        case 'G':
            return purpleColor;
        case 'H':
            return pinkColor;
        case 'I':
            return brownColor;
        case 'J':
            return grayColor;
        case 'K':
            return lightBlueColor;
        case 'L':
            return lightGreenColor;
        case 'M':
            return redColor; 
        case 'N':
            return orangeColor; 
        case 'O':
            return yellowColor; 
        case 'P':
            return greenColor; 
        case 'Q':
            return cyanColor; 
        case 'R':
            return blueColor; 
        case 'S':
            return purpleColor; 
        case 'T':
            return pinkColor; 
        case 'U':
            return brownColor; 
        case 'V':
            return grayColor; 
        case 'W':
            return lightBlueColor; 
        case 'X':
            return lightGreenColor; 
        case 'Y':
            return redColor; 
        case 'Z':
            return orangeColor; 
        default:
            return resetColor; // Default to reset color if no match is found
    }
}