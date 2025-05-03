#include "CarDesign.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <regex>
#include <algorithm>
#include <cctype>

const std::map<int, PartAttributes> FrontWing::designs = {
    {0, {10.0, 5.0, 10000.0}},  // Standard
    {1, {12.0, 4.5, 12000.0}},  // High Downforce
    {2, {8.0, 6.0, 9000.0}},    // Low Drag
    {3, {11.0, 5.2, 11000.0}},  // Balanced
    {4, {9.5, 5.8, 9500.0}}     // Experimental
};
const std::map<int, std::string> FrontWing::designNames = {
    {0, "Standard"}, {1, "High Downforce"}, {2, "Low Drag"}, {3, "Balanced"}, {4, "Experimental"}
};
FrontWing::FrontWing() : selectedDesign(0) {}
PartAttributes FrontWing::getAttributes() const { 
    return designs.at(selectedDesign) * aeroEfficiency; 
}
void FrontWing::setDesign(int designIndex) { 
    if (designs.find(designIndex) == designs.end()) throw std::invalid_argument("Invalid FrontWing design");
    selectedDesign = designIndex; 
}
int FrontWing::getSelectedDesign() const { return selectedDesign; }
std::string FrontWing::getDesignName() const { return designNames.at(selectedDesign); }

const std::map<int, PartAttributes> RearWing::designs = {
    {0, {15.0, 6.0, 15000.0}},
    {1, {18.0, 5.5, 18000.0}},
    {2, {12.0, 7.0, 13000.0}},
    {3, {16.0, 6.2, 16000.0}},
    {4, {13.5, 6.8, 14000.0}}
};
const std::map<int, std::string> RearWing::designNames = {
    {0, "Standard"}, {1, "High Downforce"}, {2, "Low Drag"}, {3, "Balanced"}, {4, "Experimental"}
};
RearWing::RearWing() : selectedDesign(0) {}
PartAttributes RearWing::getAttributes() const { 
    return designs.at(selectedDesign) * aeroEfficiency; 
}
void RearWing::setDesign(int designIndex) { 
    if (designs.find(designIndex) == designs.end()) throw std::invalid_argument("Invalid RearWing design");
    selectedDesign = designIndex; 
}
int RearWing::getSelectedDesign() const { return selectedDesign; }
std::string RearWing::getDesignName() const { return designNames.at(selectedDesign); }

const std::map<int, PartAttributes> Diffuser::designs = {
    {0, {5.0, 3.0, 8000.0}},
    {1, {6.0, 2.8, 9000.0}},
    {2, {4.0, 3.5, 7000.0}},
    {3, {5.5, 3.2, 8500.0}},
    {4, {4.5, 3.3, 7500.0}}
};
const std::map<int, std::string> Diffuser::designNames = {
    {0, "Standard"}, {1, "Aggressive"}, {2, "Minimal"}, {3, "Balanced"}, {4, "Experimental"}
};
Diffuser::Diffuser() : selectedDesign(0) {}
PartAttributes Diffuser::getAttributes() const { 
    return designs.at(selectedDesign) * aeroEfficiency; 
}
void Diffuser::setDesign(int designIndex) { 
    if (designs.find(designIndex) == designs.end()) throw std::invalid_argument("Invalid Diffuser design");
    selectedDesign = designIndex; 
}
int Diffuser::getSelectedDesign() const { return selectedDesign; }
std::string Diffuser::getDesignName() const { return designNames.at(selectedDesign); }

const std::map<int, PartAttributes> Sidepods::designs = {
    {0, {8.0, 10.0, 20000.0}},
    {1, {9.0, 9.5, 22000.0}},
    {2, {7.0, 11.0, 18000.0}},
    {3, {8.5, 10.2, 21000.0}},
    {4, {7.5, 10.5, 19000.0}}
};
const std::map<int, std::string> Sidepods::designNames = {
    {0, "Standard"}, {1, "Compact"}, {2, "Streamlined"}, {3, "Balanced"}, {4, "Experimental"}
};
Sidepods::Sidepods() : selectedDesign(0) {}
PartAttributes Sidepods::getAttributes() const { 
    return designs.at(selectedDesign) * aeroEfficiency; 
}
void Sidepods::setDesign(int designIndex) { 
    if (designs.find(designIndex) == designs.end()) throw std::invalid_argument("Invalid Sidepods design");
    selectedDesign = designIndex; 
}
int Sidepods::getSelectedDesign() const { return selectedDesign; }
std::string Sidepods::getDesignName() const { return designNames.at(selectedDesign); }

CarDesign::CarDesign() {
    aeroEfficiencies[FRONT_WING] = 1.0;
    aeroEfficiencies[REAR_WING] = 1.0;
    aeroEfficiencies[DIFFUSER] = 1.0;
    aeroEfficiencies[SIDEPODS] = 1.0;
}
CarDesign::~CarDesign() = default;
CarDesign::CarDesign(const CarDesign& other)
    : frontWing(), rearWing(), diffuser(), sidepods(), aeroEfficiencies(other.aeroEfficiencies) {
    frontWing.getPart()->setDesign(other.frontWing.getPart()->getSelectedDesign());
    rearWing.getPart()->setDesign(other.rearWing.getPart()->getSelectedDesign());
    diffuser.getPart()->setDesign(other.diffuser.getPart()->getSelectedDesign());
    sidepods.getPart()->setDesign(other.sidepods.getPart()->getSelectedDesign());
    frontWing.getPart()->adjustAeroEfficiency(other.aeroEfficiencies.at(FRONT_WING));
    rearWing.getPart()->adjustAeroEfficiency(other.aeroEfficiencies.at(REAR_WING));
    diffuser.getPart()->adjustAeroEfficiency(other.aeroEfficiencies.at(DIFFUSER));
    sidepods.getPart()->adjustAeroEfficiency(other.aeroEfficiencies.at(SIDEPODS));
}
CarDesign::CarDesign(CarDesign&& other) noexcept
    : frontWing(std::move(other.frontWing)),
      rearWing(std::move(other.rearWing)),
      diffuser(std::move(other.diffuser)),
      sidepods(std::move(other.sidepods)),
      aeroEfficiencies(std::move(other.aeroEfficiencies)) {}
CarDesign& CarDesign::operator=(const CarDesign& other) {
    if (this != &other) {
        frontWing.getPart()->setDesign(other.frontWing.getPart()->getSelectedDesign());
        rearWing.getPart()->setDesign(other.rearWing.getPart()->getSelectedDesign());
        diffuser.getPart()->setDesign(other.diffuser.getPart()->getSelectedDesign());
        sidepods.getPart()->setDesign(other.sidepods.getPart()->getSelectedDesign());
        aeroEfficiencies = other.aeroEfficiencies;
        frontWing.getPart()->adjustAeroEfficiency(aeroEfficiencies[FRONT_WING]);
        rearWing.getPart()->adjustAeroEfficiency(aeroEfficiencies[REAR_WING]);
        diffuser.getPart()->adjustAeroEfficiency(aeroEfficiencies[DIFFUSER]);
        sidepods.getPart()->adjustAeroEfficiency(aeroEfficiencies[SIDEPODS]);
    }
    return *this;
}
CarDesign& CarDesign::operator=(CarDesign&& other) noexcept {
    if (this != &other) {
        frontWing = std::move(other.frontWing);
        rearWing = std::move(other.rearWing);
        diffuser = std::move(other.diffuser);
        sidepods = std::move(other.sidepods);
        aeroEfficiencies = std::move(other.aeroEfficiencies);
    }
    return *this;
}
PartAttributes CarDesign::getTotalAttributes() const {
    return frontWing.getPart()->getAttributes() +
           rearWing.getPart()->getAttributes() +
           diffuser.getPart()->getAttributes() +
           sidepods.getPart()->getAttributes();
}
double CarDesign::getFuelConsumption() const {
    auto attrs = getTotalAttributes();
    return 0.15 * attrs.mass + 0.25 * attrs.drag + 5.0; // Realistic fuel model
}
double CarDesign::getSpeed() const {
    auto attrs = getTotalAttributes();
    return 15000.0 / (attrs.drag + 0.05 * attrs.mass); // Speed model
}
void CarDesign::saveToFile(const std::string& filename) const {
    auto [isValid, error] = validateDesignName(filename);
    if (!isValid) {
        throw std::invalid_argument(error);
    }
    std::ofstream file("designs/" + filename + ".f1design");
    if (!file.is_open()) throw std::runtime_error("Failed to save design");
    file << "FrontWing: " << frontWing.getPart()->getSelectedDesign() << "\n";
    file << "RearWing: " << rearWing.getPart()->getSelectedDesign() << "\n";
    file << "Diffuser: " << diffuser.getPart()->getSelectedDesign() << "\n";
    file << "Sidepods: " << sidepods.getPart()->getSelectedDesign() << "\n";
    file << "FrontWingAero: " << aeroEfficiencies.at(FRONT_WING) << "\n";
    file << "RearWingAero: " << aeroEfficiencies.at(REAR_WING) << "\n";
    file << "DiffuserAero: " << aeroEfficiencies.at(DIFFUSER) << "\n";
    file << "SidepodsAero: " << aeroEfficiencies.at(SIDEPODS) << "\n";
    file.close();
}
void CarDesign::loadFromFile(const std::string& filename) {
    std::ifstream file("designs/" + filename + ".f1design");
    if (!file.is_open()) throw std::runtime_error("Failed to load design");
    std::string line;
    std::map<std::string, double> values;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        if (!(std::getline(iss, key, ':') && iss >> value)) {
            file.close();
            throw std::runtime_error("Corrupted design file");
        }
        values[key] = value;
    }
    file.close();
    try {
        if (values.find("FrontWing") != values.end()) frontWing.getPart()->setDesign(static_cast<int>(values["FrontWing"]));
        if (values.find("RearWing") != values.end()) rearWing.getPart()->setDesign(static_cast<int>(values["RearWing"]));
        if (values.find("Diffuser") != values.end()) diffuser.getPart()->setDesign(static_cast<int>(values["Diffuser"]));
        if (values.find("Sidepods") != values.end()) sidepods.getPart()->setDesign(static_cast<int>(values["Sidepods"]));
        if (values.find("FrontWingAero") != values.end()) {
            aeroEfficiencies[FRONT_WING] = values["FrontWingAero"];
            frontWing.getPart()->adjustAeroEfficiency(values["FrontWingAero"]);
        }
        if (values.find("RearWingAero") != values.end()) {
            aeroEfficiencies[REAR_WING] = values["RearWingAero"];
            rearWing.getPart()->adjustAeroEfficiency(values["RearWingAero"]);
        }
        if (values.find("DiffuserAero") != values.end()) {
            aeroEfficiencies[DIFFUSER] = values["DiffuserAero"];
            diffuser.getPart()->adjustAeroEfficiency(values["DiffuserAero"]);
        }
        if (values.find("SidepodsAero") != values.end()) {
            aeroEfficiencies[SIDEPODS] = values["SidepodsAero"];
            sidepods.getPart()->adjustAeroEfficiency(values["SidepodsAero"]);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Invalid design data");
    }
}
void CarDesign::setPartDesign(PartType type, int designIndex) {
    if (type & FRONT_WING) frontWing.getPart()->setDesign(designIndex);
    if (type & REAR_WING) rearWing.getPart()->setDesign(designIndex);
    if (type & DIFFUSER) diffuser.getPart()->setDesign(designIndex);
    if (type & SIDEPODS) sidepods.getPart()->setDesign(designIndex);
}
std::string CarDesign::getPartDesignName(PartType type) const {
    if (type == FRONT_WING) return frontWing.getPart()->getDesignName();
    if (type == REAR_WING) return rearWing.getPart()->getDesignName();
    if (type == DIFFUSER) return diffuser.getPart()->getDesignName();
    if (type == SIDEPODS) return sidepods.getPart()->getDesignName();
    return "";
}
int CarDesign::getPartDesign(PartType type) const {
    if (type == FRONT_WING) return frontWing.getPart()->getSelectedDesign();
    if (type == REAR_WING) return rearWing.getPart()->getSelectedDesign();
    if (type == DIFFUSER) return diffuser.getPart()->getSelectedDesign();
    if (type == SIDEPODS) return sidepods.getPart()->getSelectedDesign();
    return 0;
}
std::string CarDesign::getVisualRepresentation() const {
    std::stringstream ss;
    ss << "  _______ \n";
    ss << " /  ***  \\ [" << frontWing.getPart()->getDesignName() << "]\n";
    ss << "/_________\\\n";
    ss << "|  ***  | [" << sidepods.getPart()->getDesignName() << "]\n";
    ss << "|  ***  |\n";
    ss << "|_______| [" << diffuser.getPart()->getDesignName() << "]\n";
    ss << " \\  ***  / [" << rearWing.getPart()->getDesignName() << "]\n";
    ss << "  \\_____/\n";
    return ss.str();
}
std::pair<bool, std::string> CarDesign::validateDesignName(const std::string& name) {
    std::string trimmed = name;
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char c) { return !std::isspace(c); }));
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), trimmed.end());
    
    if (trimmed.empty()) {
        return {false, "Design name cannot be empty"};
    }
    if (trimmed.length() > 50) {
        return {false, "Design name exceeds 50 characters"};
    }
    std::regex validName("^[a-zA-Z0-9_-][a-zA-Z0-9_ -]*[a-zA-Z0-9_-]$");
    if (!std::regex_match(trimmed, validName)) {
        return {false, "Design name contains invalid characters (use alphanumeric, _, -, or spaces)"};
    }
    return {true, ""};
}
void CarDesign::adjustAeroEfficiency(PartType type, double factor) {
    if (type & FRONT_WING) {
        aeroEfficiencies[FRONT_WING] = factor;
        frontWing.getPart()->adjustAeroEfficiency(factor);
    }
    if (type & REAR_WING) {
        aeroEfficiencies[REAR_WING] = factor;
        rearWing.getPart()->adjustAeroEfficiency(factor);
    }
    if (type & DIFFUSER) {
        aeroEfficiencies[DIFFUSER] = factor;
        diffuser.getPart()->adjustAeroEfficiency(factor);
    }
    if (type & SIDEPODS) {
        aeroEfficiencies[SIDEPODS] = factor;
        sidepods.getPart()->adjustAeroEfficiency(factor);
    }
}
double CarDesign::getAeroEfficiency(PartType type) const {
    return aeroEfficiencies.at(type);
}