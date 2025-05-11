//
// Created by hunda on 2025-05-09.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <memory>
#include <string>
#include <vector>

#define MAX_HEALTH 150;

class virus;
class dungeon;
class borough;
class items;
class location;
class player {
    std::string name;
    int maxhealth;
    int health;
    int actionpoints;
    int virusattraction;
    int criticalrate;
    float criticalmultiplier;
    std::vector<std::shared_ptr<items>> inventory;
    std::shared_ptr<borough> currentBorough;
    std::shared_ptr<dungeon> currentDungeon;
    std::shared_ptr<location> currentLocation;

public:
    player(const std::string &name);

    // Accessors
    const std::string &getName() const {return name;}
    int GetMaxHealth() const {return maxhealth;}
    int GetHealth() const {return health;}
    int GetActionpoints() const {return actionpoints;}
    int GetVirusattraction() const {return virusattraction;}
    int GetCriticalrate() const {return criticalrate;}
    float GetCriticalMultiplier() const {return criticalmultiplier;}

    // Inventory Management
    void AddItem(std::shared_ptr<items> item);
    void RemoveItem (const std::string& ItemName);
    std::vector<std::shared_ptr<items>> GetInventory() const {return inventory;}

    // Game Mechanics
    void UpdateVirusAttraction();

    bool UseActionPoints(int amount);
    void RestoreActionPoints(int amount);

    void UseItem(const std::string &ItemName);
    bool AttackVirus(std::shared_ptr<virus> targetVirus);
    bool CriticalHits() const;
    void TakeDamage(int amount);
    bool IsDefeated() const{ return health <= 0;}

    // Movement and Locations
    void SetCurrentBorough(std::shared_ptr<borough> borough) {currentBorough = borough;}
    std::shared_ptr<class borough>GetCurrentBorough() const {return currentBorough;}
    void SetCurrentDungeon(std::shared_ptr<dungeon> dungeon){currentDungeon = dungeon;}
    std::shared_ptr<dungeon> GetCurrentDungeon() const {return currentDungeon;}

    bool MoveInDungeon(int dx, int dy);

    void GetCurrentLocation(std::shared_ptr<location> location){currentLocation = location;}
};



#endif //PLAYER_H
