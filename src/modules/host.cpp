#include "helper.cpp"


std::string getHostInfo() {
    std::ifstream manufacturerFile("/sys/devices/virtual/dmi/id/board_vendor");
    std::ifstream productFile("/sys/devices/virtual/dmi/id/product_name");
    std::string manufacturer, product;

    if (manufacturerFile.is_open() && productFile.is_open()) {
        std::getline(manufacturerFile, manufacturer);
        std::getline(productFile, product);
        manufacturerFile.close();
        productFile.close();
        return "\033[92m" + std::string(HOST) + " \033[0m" + manufacturer + " @ " + product;
    } else {
        return "\033[92m" + std::string(HOST) + " \033[0m" + " Unknown";
    }
}