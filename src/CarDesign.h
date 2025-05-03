#ifndef CARDESIGN_H
#define CARDESIGN_H

#include "CarPart.h"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <utility>

enum PartType {
    NONE = 0,
    FRONT_WING = 1 << 0,
    REAR_WING = 1 << 1,
    DIFFUSER = 1 << 2,
    SIDEPODS = 1 << 3
};

template<typename T>
class PartManager {
public:
    PartManager() : part(std::make_unique<T>()) {}
    T* getPart() { return part.get(); }
    const T* getPart() const { return part.get(); }
private:
    std::unique_ptr<T> part;
};

class FrontWing : public AeroPart {
public:
    FrontWing();
    PartAttributes getAttributes() const override;
    void setDesign(int designIndex) override;
    int getSelectedDesign() const override;
    std::string getDesignName() const override;
    std::string getPartType() const override { return "FrontWing"; }
private:
    int selectedDesign;
    static const std::map<int, PartAttributes> designs;
    static const std::map<int, std::string> designNames;
};

class RearWing : public AeroPart {
public:
    RearWing();
    PartAttributes getAttributes() const override;
    void setDesign(int designIndex) override;
    int getSelectedDesign() const override;
    std::string getDesignName() const override;
    std::string getPartType() const override { return "RearWing"; }
private:
    int selectedDesign;
    static const std::map<int, PartAttributes> designs;
    static const std::map<int, std::string> designNames;
};

class Diffuser : public AeroPart {
public:
    Diffuser();
    PartAttributes getAttributes() const override;
    void setDesign(int designIndex) override;
    int getSelectedDesign() const override;
    std::string getDesignName() const override;
    std::string getPartType() const override { return "Diffuser"; }
private:
    int selectedDesign;
    static const std::map<int, PartAttributes> designs;
    static const std::map<int, std::string> designNames;
};

class Sidepods : public AeroPart {
public:
    Sidepods();
    PartAttributes getAttributes() const override;
    void setDesign(int designIndex) override;
    int getSelectedDesign() const override;
    std::string getDesignName() const override;
    std::string getPartType() const override { return "Sidepods"; }
private:
    int selectedDesign;
    static const std::map<int, PartAttributes> designs;
    static const std::map<int, std::string> designNames;
};

class CarDesign {
public:
    CarDesign();
    ~CarDesign();
    CarDesign(const CarDesign& other);
    CarDesign(CarDesign&& other) noexcept;
    CarDesign& operator=(const CarDesign& other);
    CarDesign& operator=(CarDesign&& other) noexcept;
    PartAttributes getTotalAttributes() const;
    double getFuelConsumption() const;
    double getSpeed() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void setPartDesign(PartType type, int designIndex);
    std::string getPartDesignName(PartType type) const;
    int getPartDesign(PartType type) const;
    std::string getVisualRepresentation() const;
    static std::pair<bool, std::string> validateDesignName(const std::string& name);
    void adjustAeroEfficiency(PartType type, double factor);
    double getAeroEfficiency(PartType type) const;
private:
    PartManager<FrontWing> frontWing;
    PartManager<RearWing> rearWing;
    PartManager<Diffuser> diffuser;
    PartManager<Sidepods> sidepods;
    std::map<PartType, double> aeroEfficiencies;
};

#endif