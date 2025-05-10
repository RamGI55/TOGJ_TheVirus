//
// Created by hunda on 2025-05-09.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <memory>
#include <string>
#include <vector>


class borough;
class items;
class player {
    std::string name;
    int health;
    int actionpoint;
    int virusattraction;
    std::vector<items> inventory;
    std::shared_ptr<borough> currentlocation;

public:
    player(const std::string &name);

    void AddItem(const items &item);
    void RemoveItem (const std::string& ItemName);
    void UpdateVirusAttraction();

    int GetVirusAttraction() const;

};



#endif //PLAYER_H
