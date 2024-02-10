#include "helper.cpp"

std::string getDateInfo() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    char dateBuffer[80];
    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", localTime);
    return "\033[96m" + std::string(DATE) + " \033[0m" + std::string(dateBuffer);
}