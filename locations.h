//
// Created by hunda on 2025-05-10.
//

#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <string>
#include <map>
#include <vector>
#include <memory>

#define MAP_X_MAX 50
#define MAP_Y_MAX 50


class dungeon;
class virus;
class items;

class locations {

    std::string id;
    std::string name;
    std::string description;
    float baseinfectionrate;
    float infectionrate;
    bool isInfected;
    std::shared_ptr<dungeon> CurrentDungeon;

    // Dungeon parameters
    int dungeonWidth = 20;      // Default values
    int dungeonHeight = 20;
    int dungeonInitialViruses = 5;
    int dungeonInitialItems = 3;


public:
    locations(const std::string& id, const std::string& name, const std::string& description);
    int GetInfectionrate();
    int GetInfected();
    void UpdateInfectionrate(float delta);
    void ResetInfectionrate();
    std::shared_ptr<dungeon> GetDungeon() const {return CurrentDungeon;}
    void const GenerateDungeon();

    std::string GetDescription() const {return description;}
    std::string GetName() const {return name;}
    std::string GetId() const {return id;}

    void SetBaseInfectionRate(float base) {baseinfectionrate = base;}
    void SetDungeonParameters(int width, int height, int initialViruses, int initialItems) {
        dungeonWidth = width;
        dungeonHeight = height;
        dungeonInitialViruses = initialViruses;
        dungeonInitialItems = initialItems;
    }
};


#endif //LOCATIONS_H
