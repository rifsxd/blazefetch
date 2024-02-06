#include "helper.cpp"

bool isFigletAvailable() {
    FILE *checkPlayerctl = popen("command -v figlet 2>/dev/null", "r");
    if (checkPlayerctl) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), checkPlayerctl) != nullptr) {
            pclose(checkPlayerctl);
            return true;
        }
        pclose(checkPlayerctl);
    }
    return false;
}

std::string execHelperGlyph(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != nullptr) {
            result += buffer;
        }
    }
    return result;
}


std::string getDistoInfo() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp) {
        int foundPrettyName = 0;
        char line[256];
        
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "PRETTY_NAME")) {
                char *name = strchr(line, '=') + 2;
                name[strlen(name) - 2] = '\0';
                fclose(fp);
                return  name;
            }
        }

        fclose(fp);
        return "Unknown";
    } else {
        return "Unknown";
    }
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

std::string getDistroColorInfo() {
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

std::string getDistroAnsiColor() {

    std::string distroAnsiColor = getDistroColorInfo();

    return distroAnsiColor;
}


std::string getDistroColor(const std::string& distroName) {

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

std::string removeParentheses(const std::string& input) {
    std::string result;
    for (char ch : input) {
        if (ch != '(' && ch != ')') {
            result += ch;
        }
    }
    return result;
}

std::string getConditionalDistroColor(const std::string& distroName) {

    std::string distroAnsiColor = getDistroColorInfo();

    // If distroAnsiColor is not empty, return its value
    if (!distroAnsiColor.empty() && distroAnsiColor.find("Unknown")) {
        return distroAnsiColor;
    }

    std::string cleanedDistroName = removeParentheses(distroName);

    std::string distroColor = getDistroColor(cleanedDistroName);

    // Otherwise, return the result of getDistroColor
    return distroColor;
}


std::string getGlyphInfo(const std::string& distroName) {

    if (!isFigletAvailable()) {
        
        
        std::cout << yellowColor << R"(
 ██▓     ██▓ ███▄    █  █    ██ ▒██   ██▒
▓██▒    ▓██▒ ██ ▀█   █  ██  ▓██▒▒▒ █ █ ▒░
▒██░    ▒██▒▓██  ▀█ ██▒▓██  ▒██░░░  █   ░
▒██░    ░██░▓██▒  ▐▌██▒▓▓█  ░██░ ░ █ █ ▒ 
░██████▒░██░▒██░   ▓██░▒▒█████▓ ▒██▒ ▒██▒
░ ▒░▓  ░░▓  ░ ▒░   ▒ ▒ ░▒▓▒ ▒ ▒ ▒▒ ░ ░▓ ░
░ ░ ▒  ░ ▒ ░░ ░░   ░ ▒░░░▒░ ░ ░ ░░   ░▒ ░
  ░ ░    ▒ ░   ░   ░ ░  ░░░ ░ ░  ░    ░  
    ░  ░ ░           ░    ░      ░    ░  
                                         
        )" << std::endl;

        return "\033[94m" + std::string(ASCII) + " \033[0mfiglet not found. Falling back to default ascii art!";
    }

    std::ofstream tempFontFile("temp_font.flf");
    tempFontFile << customFont;
    tempFontFile.close();

    
    
    std::string cleanedDistroName = removeParentheses(distroName);
    std::string figletCommand = "figlet -f temp_font.flf \"" + cleanedDistroName + "\"";
    std::string conditionalDistroColor = getConditionalDistroColor(distroName);
    std::string figletOutput = execHelperGlyph(figletCommand.c_str());
    std::remove("temp_font.flf");
    std::string distroColor = getDistroColor(cleanedDistroName);
    return conditionalDistroColor + figletOutput  + resetColor;
}
