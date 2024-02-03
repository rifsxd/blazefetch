#include "helper.cpp"

std::string exec(const char* cmd) {
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
        return "\033[0mUnknown";
    } else {
        return "\033[0mUnknown";
    }
}

std::string getDistroColor(const std::string& distroName) {
    char firstChar = distroName.empty() ? ' ' : std::toupper(distroName[0]);

    switch (firstChar) {
        case 'A':
            return redColor;
        case 'B':
            return orangeColor;
        case 'C':
            return blueColor;
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

std::string getGlyphInfo(const std::string& distroName) {

    std::ofstream tempFontFile("temp_font.flf");
    tempFontFile << customFont;
    tempFontFile.close();
    
    std::string cleanedDistroName = removeParentheses(distroName);
    std::string figletCommand = "figlet -f temp_font.flf \"" + cleanedDistroName + "\"";
    std::string figletOutput = exec(figletCommand.c_str());
    std::remove("temp_font.flf");
    std::string distroColor = getDistroColor(cleanedDistroName);
    return distroColor + figletOutput  + resetColor;
}
