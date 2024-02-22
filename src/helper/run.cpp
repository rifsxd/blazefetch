#include "defines.cpp"
#include "colors.cpp"

void runProgram() {
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);

    // Check if shared memory is empty
    bool isShmEmpty = true;
    auto startTime = std::chrono::steady_clock::now();
    std::cout << yellowColor << "\n Processing buffers... ";
    std::cout.flush(); // Make sure the message is displayed immediately
    while (true) {
        for (int i = 0; i < 1024; ++i) {
            if (shm[i] != '\0') {
                isShmEmpty = false;
                break;
            }
        }
        if (!isShmEmpty) {
            break;
        }
        // Check elapsed time
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        if (elapsedTime >= 3) // At least 3 seconds have passed
            break;

        // Display loading animation
        const char loading[] = {'|', '/', '-', '\\'};
        static int loadingIdx = 0;
        std::cout << "\b" << loading[loadingIdx++] << std::flush;
        loadingIdx %= 4;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust as needed
    }
    std::cout << "\b\b\b   \b\b\b"; // Clear loading animation characters

    std::cout << "\n";

    // Display cached info
    std::cout << "\n" << shm;
    colorPallate();

    // Detach the shared memory segment
    shmdt(shm);
}