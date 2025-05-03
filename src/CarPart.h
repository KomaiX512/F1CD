#ifndef CARPART_H
#define CARPART_H

#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>

struct PartAttributes {
    double drag{0.0};
    double mass{0.0};
    double cost{0.0};
    PartAttributes operator+(const PartAttributes& other) const {
        return {drag + other.drag, mass + other.mass, cost + other.cost};
    }
    PartAttributes operator*(double scalar) const {
        return {drag * scalar, mass * scalar, cost * scalar};
    }
    bool operator<(const PartAttributes& other) const {
        return cost < other.cost;
    }
    bool operator==(const PartAttributes& other) const {
        return drag == other.drag && mass == other.mass && cost == other.cost;
    }
};

class CarPart {
public:
    virtual ~CarPart() = default;
    virtual PartAttributes getAttributes() const = 0;
    virtual void setDesign(int designIndex) = 0;
    virtual int getSelectedDesign() const = 0;
    virtual std::string getDesignName() const = 0;
    virtual std::string getPartType() const = 0;
};

class AeroPart : public CarPart {
protected:
    double aeroEfficiency{1.0};
public:
    virtual void adjustAeroEfficiency(double factor) {
        aeroEfficiency = std::clamp(factor, 0.5, 1.5);
    }
};

#endif