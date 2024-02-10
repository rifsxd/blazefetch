#include "helper.cpp"

bool isLessAvailable() {
    FILE *checkLess = popen("command -v less 2>/dev/null", "r");
    if (checkLess) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), checkLess) != nullptr) {
            pclose(checkLess);
            return true;
        }
        pclose(checkLess);
    }
    return false;
}

// Callback function to write the response from the HTTP request
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    response->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void printPrettyCommitInfo(const std::string& json, int commitLimit) {
    // Parse the JSON response and print it in a structured format
    std::ostringstream output;
    output << "\033[0;32m" << "Latest commit info:" << "\033[0m" << std::endl; // Set color to green

    // Start parsing JSON
    output << "-------------------------" << std::endl;
    size_t pos = 0;
    int count = 0;
    while ((pos = json.find("\"sha\":", pos)) != std::string::npos && count < commitLimit) {
        pos = json.find(":", pos);
        size_t start = json.find("\"", pos + 1);
        size_t end = json.find("\"", start + 1);
        std::string sha = json.substr(start + 1, end - start - 1);

        pos = json.find("\"author\":", end);
        pos = json.find(":", pos);
        start = json.find("\"name\":", pos);
        start = json.find("\"", start + 1);
        end = json.find("\"", start + 1);
        std::string author = json.substr(start + 1, end - start - 1);

        pos = json.find("\"message\":", end);
        pos = json.find(":", pos + 1);
        start = json.find("\"", pos + 1);
        end = json.find("\"", start + 1);
        std::string message = json.substr(start + 1, end - start - 1);

        output << "SHA: \033[0;36m" << sha << "\033[0m" << std::endl; // Set color to cyan
        output << "Author: \033[0;35m" << author << "\033[0m" << std::endl; // Set color to magenta
        output << "Message: \033[0;33m" << message << "\033[0m" << std::endl; // Set color to yellow
        output << "-------------------------" << std::endl;

        // Move to the next commit
        pos = end;
        count++;
    }

    // Display the output in less mode
    std::string lessCommand = "LESS=-R less";
    FILE* lessPipe = popen(lessCommand.c_str(), "w");
    if (lessPipe != nullptr) {
        fwrite(output.str().c_str(), 1, output.str().size(), lessPipe);
        pclose(lessPipe);
    } else {
        std::cerr << "Failed to open less." << std::endl;
    }
}

void getCommitInfo(int commitLimit) {

    if (!isLessAvailable()) {
        std::cerr << "\n" << "\033[94m" << " ? ERROR ? " << " \033[0m'less' not found to view git commit logs." << "\033[94m" << " ? ERROR ? " << " \033[0m" << "\n" << std::endl;
        return; // Return early if less is not available
    }

    std::string url = "https://api.github.com/repos/rifsxd/blazefetch/commits";
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Blazefetch"); // Set the User-Agent header

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch commit info: " << curl_easy_strerror(res) << std::endl;
            return;
        }
    } else {
        std::cerr << "Failed to initialize curl." << std::endl;
        return;
    }

    printPrettyCommitInfo(response, commitLimit); // Call printPrettyCommitInfo with commit limit
}