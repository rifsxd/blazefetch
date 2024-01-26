#include "helper.cpp"

std::string getTerminalInfo() {
    char* term = getenv("TERM");
    char* termProgram = getenv("TERM_PROGRAM");

    if (term != nullptr) {
        std::string terminal = "\033[35m" + std::string(TERM) + " \033[0m" + std::string(term);
        
        if (termProgram != nullptr) {
            terminal += "\n\033[32m╰─| \033[35m" + std::string(TERM_PROG) + " \033[0m" + std::string(termProgram);
        }

        return terminal;
    } else {
        return "\033[35m" + std::string(TERM) + " \033[0mUnknown... \033[35mTerminal information not available?!\033[0m";
    }
}