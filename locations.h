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


public:
    locations(const std::string& id, const std::string& name, const std::string& description);
    int GetInfectionrate();
    int GetInfected();
    void UpdateInfectionrate(float delta);
    void ResetInfectionrate();
    std::shared_ptr<dungeon> GetDungeon() const {return CurrentDungeon;}
    void const GenerateDungeon();

};



#endif //LOCATIONS_H
