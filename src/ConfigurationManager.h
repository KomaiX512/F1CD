#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "CarDesign.h"
#include <vector>
#include <string>
#include <filesystem>

using DesignProcessor = void (*)(const CarDesign&);

class ConfigurationManager {
public:
    static std::vector<std::string> getDesignFiles();
    static void processDesigns(DesignProcessor processor);
    static int countDesignFiles(const std::string& path);
    static bool designExists(const std::string& name);
    static void backupDesign(const std::string& filename);
};

#endif