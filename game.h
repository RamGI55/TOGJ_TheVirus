//
// Created by hunda on 2025-05-09.
//

#ifndef GAME_H
#define GAME_H
#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>

class locations;
class virus;
class borough;
class items;
class ui;
class player;
class utils;
class dungeon;
class game : public std::enable_shared_from_this<game>{
private:
    bool running;
    std::shared_ptr<game> GetPtr() {
        return shared_from_this();
    }
    std::shared_ptr<player> Player;
    std::map<std::string, std::shared_ptr<borough>> Boroughs;
    std::map<std::string, std::shared_ptr<items>> Items;
    std::map<std::string, std::shared_ptr<virus>> viruses;

    std::shared_ptr<ui> Ui;
public:
    game();

    void Initialise();
    void Run();
    void ProcessInput(const std::string& command);
    void Update();
    void SpawnViruses();

    bool isRunning() const {return running;}
    void Quit() { running = false; }

    // Movement functions
    void MovePlayer(int dx, int dy);
    void ProcessvirusMovement();
    void ExitDungeon(bool completed);
    // Infection rate functions
    void UpdateInfectionRates(float delta);

    std::shared_ptr<player> GetPlayer() const {return Player;}
    std::shared_ptr<ui> GetUI() const {return Ui;}

private:
    void StartBattle(std::shared_ptr<virus> enemy);
    void LoadLocations();
    void LoacItems();
    void LoadViruses();
    void SetupInitialGameState();
    void HandleCollision(int x, int y);

    // loading Jsons
    void LoadDataFromJson();
    void LoadBoroughsFromJson(const std::string& filename);
    void LoadLocationsFromJson(const std::string& filename);
    void LoadVirusesFromJson(const std::string& filename);
    void LoadItemsFromJson(const std::string& filename);

    void InitialisePlayerItems();

    std::shared_ptr<virus> CreateVirusFromType(const std::string& virusId);
    std::shared_ptr<items> CreateItemFromType(const std::string& itemId);
    std::shared_ptr<locations> currentLocation;

    void EnterLocation(const std::string& locationId);
    void EnterDungeon(const std::string& locationId); // Also add this
    void PopulateDungeon(std::shared_ptr<dungeon> dungeon);


};



#endif //GAME_H
