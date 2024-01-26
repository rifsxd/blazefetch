#include "helper.cpp"

std::string getLSBReleaseInfo() {
    const int bufferSize = 256;
    char buffer[bufferSize];

    FILE* lsbReleaseFile = popen("lsb_release -si 2>/dev/null", "r");
    if (lsbReleaseFile) {
        if (fgets(buffer, bufferSize, lsbReleaseFile) != nullptr) {
            // Remove newline character
            buffer[strcspn(buffer, "\n")] = 0;
        } else {
            snprintf(buffer, bufferSize, "%s", "Unknown");
        }
        pclose(lsbReleaseFile);
    } else {
        snprintf(buffer, bufferSize, "%s", "Unknown");
    }

    return std::string(buffer);
}

std::string getPackageInfo() {
    std::string lsbaltInfo = getLSBReleaseInfo();
    std::string packageInfo;

    int pkgCount = 0;

    // Check the Linux distribution using os-release
    FILE* osReleaseFile = popen("cat /etc/os-release | grep '^ID_LIKE=' | cut -d'=' -f2 && cat /etc/os-release | grep '^ID=' | cut -d'=' -f2", "r");
    if (osReleaseFile) {
        char distro[64];
        if (fgets(distro, sizeof(distro), osReleaseFile) != nullptr) {
            // Remove newline character
            distro[strcspn(distro, "\n")] = 0;

            // Get package info based on the distribution
            if (strstr(distro, "arch") != nullptr) {
                // Count Arch Linux packages
                FILE* archFile = popen("pacman -Qq", "r");
                if (archFile) {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), archFile) != nullptr) {
                        pkgCount++;
                    }
                    pclose(archFile);

                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (pacman)";
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else if (strstr(distro, "fedora") != nullptr || strstr(distro, "opensuse") != nullptr || strstr(distro, "suse") != nullptr || strstr(distro, "rhel") != nullptr) {
                // Count RPM packages
                FILE* rpmFile = popen("rpm -qa", "r");
                if (rpmFile) {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), rpmFile) != nullptr) {
                        pkgCount++;
                    }
                    pclose(rpmFile);

                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (rpm)";
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else if (strstr(distro, "nixos") != nullptr) {
                // Count NixOS packages
                FILE* nixFile = popen("echo $(( $(nix-store -qR /run/current-system/sw | wc -l) + $(nix-store -qR /etc/profiles/per-user/$USER | wc -l) ))", "r");
                if (nixFile) {
                    char buffer[256];
                    if (fgets(buffer, sizeof(buffer), nixFile) != nullptr) {
                        pkgCount = atoi(buffer);
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (nix)";
                    } else {
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                    }
                    pclose(nixFile);
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else if (strstr(distro, "debian") != nullptr || strstr(distro, "ubuntu") != nullptr || strstr(distro, "linuxmint") != nullptr || strstr(distro, "popos") != nullptr) {
                // Count Debian/Ubuntu packages
                FILE* debFile = popen("dpkg -l | grep ^ii | wc -l", "r");
                if (debFile) {
                    char buffer[256];
                    if (fgets(buffer, sizeof(buffer), debFile) != nullptr) {
                        pkgCount = atoi(buffer);
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0m" + std::to_string(pkgCount) + " (deb)";
                    } else {
                        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                    }
                    pclose(debFile);
                } else {
                    packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
                }
            } else {
                packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
            }
        } else {
            packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
        }
        pclose(osReleaseFile);
    } else {
        packageInfo = "\033[38;5;208m" + std::string(PACKAGES) + " \033[0mUnknown";
    }

    return packageInfo;
}