//
// Created by hunda on 2025-05-09.
//

#ifndef BOROUGH_H
#define BOROUGH_H
#include <stdexcept>
#include <string>
#include <vector>


class locations;
class borough {

    std::string name;
    std::vector <locations> location;
    int infectionrates;

    public:
    borough(const std::string& inName);
    void AddLocation(const locations& inlocation);
    void Updateinfectionrates();

    std::string GetName() const {return name;}
    float GetInfectionrate() const {return infectionrates;}
    const std::vector<locations>& GetLocations() const {return location;}
    locations& GetLocation(int index) {
        if (index >= 0 && index < location.size()) {
            return location[index];
        }
        throw std::out_of_range("Location index out of range");
    }

};

#endif //BOROUGH_H
