//
// Created by hunda on 2025-05-09.
//

#ifndef BOROUGH_H
#define BOROUGH_H
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>


class locations;
class borough {

    std::string name;
    std::string description;
    float lockdownThreshold;
    std::vector <std::shared_ptr<locations>> location;
    int infectionrates;

    public:
    borough(const std::string& inName,
        const std::string& inDescription = "A borough of Toronto", float inLockdownThreshold = 1.0f)
        : name(inName),
        description(inDescription),
        lockdownThreshold(inLockdownThreshold),
        infectionrates(0.0f) {
    }

    void AddLocation(const std::shared_ptr<locations>& inlocation);
    void Updateinfectionrates();

    std::string GetName() const {return name;}
    std::string GetDescription() const {return description;}
    float GetInfectionrate() const {return infectionrates;}
    const std::vector<std::shared_ptr<locations>>& GetLocations() const {return location;}

    std::shared_ptr<locations> GetLocation(int index) {
        if (index >= 0 && index < location.size()) {
            return location[index];
        }
        throw std::out_of_range("Location index out of range");
    }

    // Setters (if needed)
    void SetName(const std::string& inName) { name = inName; }
    void SetDescription(const std::string& inDescription) { description = inDescription; }
    void SetLockdownThreshold(float threshold) { lockdownThreshold = threshold; }


};

#endif //BOROUGH_H
