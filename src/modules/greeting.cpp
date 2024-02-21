#include "helper.cpp"

std::string getRandomPussyGreeting() {
    std::string greetings[] = {
        "\033[92m Meow! =>.<=\033[0m",
        "\033[92m Hello there! :D\033[0m",
        "\033[92m Greetings! :)\033[0m",
        "\033[92m Hey! >-<\033[0m",
        "\033[92m Hiya! ;>\033[0m",
        "\033[92m Yo! :3\033[0m",
        "\033[92m Hi! ^_^\033[0m",
        "\033[92m Hewwo! <3\033[0m"
    };
    return greetings[rand() % (sizeof(greetings) / sizeof(greetings[0]))];
}

std::string getRandomBunnyGreeting() {
    std::string greetings[] = {
        "\033[92m Purr! ( . .)\033[0m",
        "\033[92m Hello there! :D\033[0m",
        "\033[92m Greetings! :)\033[0m",
        "\033[92m Hey! >-<\033[0m",
        "\033[92m Hiya! ;>\033[0m",
        "\033[92m Yo! :3\033[0m",
        "\033[92m Hi! ^_^\033[0m",
        "\033[92m Hewwo! <3\033[0m"
    };

    return greetings[rand() % (sizeof(greetings) / sizeof(greetings[0]))];
}