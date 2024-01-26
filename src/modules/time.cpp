#include "helper.cpp"

std::string getTimeInfo() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    char timeBuffer[80];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    return "\033[96m" + std::string(TIME) + " \033[0m" + std::string(timeBuffer);
}