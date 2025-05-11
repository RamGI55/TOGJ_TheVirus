//
// Created by hunda on 2025-05-09.
//

#include "items.h"

items::items(const std::string &name, const std::string &description, int healthBoost, int damageBoost,
    int attractionModifier ,int actionPointBoost, bool consumable, float infectionRateImpact) : name (name), description (description), healthBoost (healthBoost),
damageBoost(damageBoost), attractionModifier(attractionModifier) , isConsumable(false), actionPointBoost(actionPointBoost),
consumable(consumable), infectionRateImpact(infectionRateImpact){
}
