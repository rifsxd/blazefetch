#include "defines.cpp"
#include "modules.cpp"
#include "colors.cpp"
#include "memory.cpp"

void runStaticProgram() {

    std::string distroName = getDistroInfo();
    
    // Create a key for shared memory
    key_t key = ftok("/tmp", 'R');

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0644);

    // Attach the shared memory segment
    char *shm = (char *)shmat(shmid, (void *)0, 0);
        
    // Clear shared memory content
    memset(shm, 0, SHM_SIZE);

    // Run get<example>Info functions and store the output in shared memory
    std::string output = getTitleInfo(distroName) + "\n" + getOsInfo() + "\n" + getHostInfo() + "\n" + getPackageInfo() + "\n" +
                            getKernelInfo() + "\n" + getUptimeInfo() + "\n" + getTimeInfo() + "\n" + getShellInfo() + "\n" +
                            getCpuInfo() + "\n" + getGpuInfo() + "\n" + getScreenResInfo() + "\n" + getStorageInfo() + "\n" +
                            getRamInfo() + "\n" + getDEInfo() + "\n" + getMediaInfo() + "\n" + getNetworkStatusInfo() + "\n" + getTerminalInfo() + "\n\n";

    // Update shared memory
    std::strcpy(shm, output.c_str());

    // Print cached info
    std::cout << "\n" << shm;
    
    colorPallate();

    // Detach the shared memory segment
    shmdt(shm);
    
}