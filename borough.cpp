//
// Created by hunda on 2025-05-09.
//

#include "borough.h"
#include <iostream>
#include "locations.h"
#include <numeric>

void borough::AddLocation(const std::shared_ptr<locations> &inlocation) {
    if (!inlocation) {
        std::cerr <<"Warning: Attempted to add null location to borough " << name << std::endl;
        return;

    }

    location.push_back(inlocation);
    Updateinfectionrates();

    std::cerr << "Added location " << inlocation->GetName() << " to borough " << name << std::endl;
}

void borough::Updateinfectionrates() {
    if (location.empty()) {
        infectionrates = 0.0f;
        return;
    }

    float totalinfection = 0.0f;
    // Debug output
    std::cerr << "Updating infection rates for borough: " << name << std::endl;

    for (auto& loc : location) {
        if (loc) {
            float locRate = loc->GetInfectionrate() / 100.0f; // Convert from percentage
            totalinfection += locRate;
            std::cerr << "  Location: " << loc->GetName() << " infection rate: " << locRate << std::endl;
        } else {
            std::cerr << "  Warning: Null location in borough " << name << std::endl;
        }
    }

    infectionrates = totalinfection / static_cast<float>(location.size());
    std::cerr << "  Borough: " << name << " total infection rate: " << infectionrates << std::endl;
}
