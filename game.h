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

class virus;
class borough;
class items;
class ui;
class player;
class utils;
class game {
private:
    bool running;

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

    std::shared_ptr<player> GetPlayer() const {return Player;}
    std::shared_ptr<ui> GetUI() const {return Ui;}

private:
    void LoadLocations();
    void LoacItems();
    void LoadViruses();
    void SetupInitialGameState();

};



#endif //GAME_H
