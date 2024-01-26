#pragma once

#include "defines.cpp"

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