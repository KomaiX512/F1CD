#include "ConfigurationManager.h"
#include <filesystem>
#include <algorithm>
#include <sys/stat.h>

namespace fs = std::filesystem;

void ensureDesignsDirectory() {
    const std::string designsPath = "designs";
    if (!fs::exists(designsPath)) {
        try {
            fs::create_directory(designsPath);
            chmod(designsPath.c_str(), 0755); // Set permissions to rwxr-xr-x
        } catch (const fs::filesystem_error& e) {
            // Log error but don't throw; handled by callers
        }
    }
}

std::vector<std::string> ConfigurationManager::getDesignFiles() {
    ensureDesignsDirectory();
    std::vector<std::string> files;
    try {
        for (const auto& entry : fs::directory_iterator("designs")) {
            if (entry.path().extension() == ".f1design") {
                files.push_back(entry.path().stem().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        // Return empty list instead of throwing
    }
    std::sort(files.begin(), files.end());
    return files;
}

void ConfigurationManager::processDesigns(DesignProcessor processor) {
    CarDesign design;
    for (const auto& file : getDesignFiles()) {
        try {
            design.loadFromFile(file);
            processor(design);
        } catch (const std::exception& e) {
            // Skip corrupted files
        }
    }
}

int ConfigurationManager::countDesignFiles(const std::string& path) {
    ensureDesignsDirectory();
    int count = 0;
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                count += countDesignFiles(entry.path().string());
            } else if (entry.path().extension() == ".f1design") {
                count++;
            }
        }
    } catch (const fs::filesystem_error& e) {
        // Return 0 instead of throwing
        return 0;
    }
    return count;
}

bool ConfigurationManager::designExists(const std::string& name) {
    ensureDesignsDirectory();
    return fs::exists("designs/" + name + ".f1design");
}

void ConfigurationManager::backupDesign(const std::string& filename) {
    ensureDesignsDirectory();
    if (designExists(filename)) {
        try {
            fs::copy_file("designs/" + filename + ".f1design",
                          "designs/" + filename + ".bak",
                          fs::copy_options::overwrite_existing);
        } catch (const fs::filesystem_error& e) {
            // Log error but don't throw
        }
    }
}