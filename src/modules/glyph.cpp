#include "helper.cpp"

bool isFigletAvailable() {
    FILE *checkPlayerctl = popen("command -v figlet 2>/dev/null", "r");
    if (checkPlayerctl) {
        char buffer[256];
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
    char buffer[256];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 256, pipe.get()) != nullptr) {
            result += buffer;
        }
    }
    return result;
}


std::string getDistroInfo() {
    std::ifstream file("/etc/os-release");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("PRETTY_NAME") != std::string::npos) {
                size_t start = line.find('"');
                if (start != std::string::npos) {
                    size_t end = line.find('"', start + 1);
                    if (end != std::string::npos) {
                        return line.substr(start + 1, end - start - 1);
                    }
                } else {
                    start = line.find('='); // Look for '=' if '"' is not found
                    if (start != std::string::npos) {
                        return line.substr(start + 1); // Return the content after '='
                    }
                }
            }
        }
        file.close();
    }
    return "Unknown";
}

std::string getDistroAnsiColor() {

    std::string distroAnsiColor = getDistroColorAnsiInfo();

    return distroAnsiColor;
}

std::string getDistroColor(const std::string& distroName) {

    std::string distroColor = getDistroColorInfo(distroName);
    
    return distroColor;
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

    std::string distroAnsiColor = getDistroAnsiColor();

    // If distroAnsiColor is not empty, return its value
    if (!distroAnsiColor.empty() && distroAnsiColor.find("Unknown")) {
        return distroAnsiColor;
    }

    std::string cleanedDistroName = removeParentheses(distroName);

    std::string distroConditionalColor = getDistroColor(cleanedDistroName);

    // Otherwise, return the result of getDistroColor
    return distroConditionalColor;
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

        return "\033[94m" + std::string(ASCII) + " \033[0m'figlet' not found. Falling back to default ascii art!";
    }

    std::ofstream tempFontFile("temp_font.flf");
    tempFontFile << customFont;
    tempFontFile.close();

    
    
    std::string cleanedDistroName = removeParentheses(distroName);
    std::string figletCommand = "figlet -f temp_font.flf \"" + cleanedDistroName + "\"";
    std::string conditionalDistroColor = getConditionalDistroColor(distroName);
    std::string figletOutput = execHelperGlyph(figletCommand.c_str());
    std::remove("temp_font.flf");
    std::string distroColor = conditionalDistroColor;
    return conditionalDistroColor + figletOutput  + resetColor;
}

std::string getMinimalGlyphInfo(const std::string& distroName) {

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

        return "\033[94m" + std::string(ASCII) + " \033[0m'figlet' not found. Falling back to default ascii art!";
    }

    std::ofstream tempFontFile("temp_font.flf");
    tempFontFile << customFont;
    tempFontFile.close();

    // Get the first word of the distro name
    std::istringstream iss(distroName);
    std::string firstWord;
    iss >> firstWord;
    
    std::string figletCommand = "figlet -f temp_font.flf \"" + firstWord + "\"";
    std::string conditionalDistroColor = getConditionalDistroColor(distroName);
    std::string figletOutput = execHelperGlyph(figletCommand.c_str());
    std::remove("temp_font.flf");
    std::string distroColor = conditionalDistroColor;
    return conditionalDistroColor + figletOutput  + resetColor;
}
