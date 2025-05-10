//
// Created by hunda on 2025-05-09.
//

#include "borough.h"
#include "locations.h"
#include <numeric>

void borough::AddLocation(const locations &inlocation) {
    location.push_back(inlocation);
    Updateinfectionrates();
}

void borough::Updateinfectionrates() {
    if (location.empty()) {
        infectionrates = 0.0f;
        return;
    }

    float totalinfection = 0.0f;
    for (auto& location: location){
        totalinfection += location.GetInfectionrate();
    }

    infectionrates = totalinfection / static_cast<float>(location.size());
}
