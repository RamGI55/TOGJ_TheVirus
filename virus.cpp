//
// Created by hunda on 2025-05-09.
//

#include "virus.h"

#include "GameUtil.h"

virus::virus(std::string name, std::string description, int health, int damage, int attractionFactor, int moveSpeed,
int criticalRate, float criticalMultiplayer) :
name(name), description(description), health(health), damage(damage),attractionFactor(attractionFactor),
moveSpeed(moveSpeed), criticalRate(criticalRate), criticalMultiplier(criticalMultiplayer)
{
}

void virus::TakeDamage(int amount) {
    health = std::max(0, health - amount);
}

int virus::AttackPlayer() const {
    int baseDamage = damage;
    if (isCriticalhit()) {
        baseDamage = static_cast<int>(baseDamage * criticalMultiplier);
    }
    return baseDamage;
}

void virus::RegenerateHealth(int amount) {
    health += 2;
}

bool virus::isCriticalhit() const {
    return GameUtil::RandomInt (1, 100) <= criticalRate;
}
