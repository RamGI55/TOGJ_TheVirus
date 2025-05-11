//
// Created by hunda on 2025-05-09.
//

#ifndef VIRUS_H
#define VIRUS_H

#include <string>
#include <memory>
#include <vector>


class virus {
private:
    std::string name;
    std::string description;
    int health;
    int damage;
    int attractionFactor;
    int moveSpeed;
    int criticalRate;
    float criticalMultiplier;

public:
    virus(std::string name, std::string description, int health, int damage, int attractionFactor, int moveSpeed
        , int criticalRate = 5, float criticalMultiplayer = 1.5f);

    // Getters
    std::string GetName() const {return name;}
    std::string GetDescription() const {return description;}
    int GetHealth() const {return health;}
    int GetDamage() const {return damage;}
    int GetAttractionFactor() const {return attractionFactor;}
    int GetMoveSpeed() const {return moveSpeed;}
    int GetCriticalRate() const {return criticalRate;}
    float GetCriticalMultiplier() const {return criticalMultiplier;}

    // Game Mechanics
    void TakeDamage(int amount);
    bool IsDefeated() const {return health <= 0;}
    int AttackPlayer() const;
    void RegenerateHealth(int amount);
    bool isCriticalhit() const;



};



#endif //VIRUS_H
