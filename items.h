//
// Created by hunda on 2025-05-09.
//

#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <memory>
#include <functional>

class items {
private:
    std::string name;
    std::string description;
    int healthBoost;
    int damageBoost;
    int attractionModifier;
    int actionPointBoost;
    bool consumable;
    float infectionRateImpact;

public:

    bool isConsumable;
    items(const std::string &name, const std::string &description, int healthBoost, int damageBoost, int attractionModifier
        ,int actionPointBoost = 0, bool consumable = false, float infectionRateImpact = 0.0f);

    // Getters
    const std::string &GetName() const { return name; }
    const std::string &GetDescription() const { return description; }
    int GetHealthBoost() const { return healthBoost; }
    int GetDamageBoost() const { return damageBoost; }
    int GetAttractionModifier() const { return attractionModifier; }
    int GetActionPointBoost() const {return actionPointBoost; }
    bool IsConsumable() const { return isConsumable; }
    float GetInfectionRate() const { return infectionRateImpact; }


};



#endif //ITEMS_H
