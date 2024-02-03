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