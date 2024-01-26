#include "helper.cpp"

std::string getStorageInfo() {
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        long total_space = (long)vfs.f_frsize * vfs.f_blocks;
        long used_space = (long)vfs.f_frsize * (vfs.f_blocks - vfs.f_bfree);

        return "\033[95m" + std::string(DISK) + " \033[0m" + std::to_string(used_space / (1024 * 1024)) + "MB / " + std::to_string(total_space / (1024 * 1024)) + "MB";
    } else {
        return "\033[95m" + std::string(DISK) + " \033[0mUnknown";
    }
}