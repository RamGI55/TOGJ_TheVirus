//
// Created by hunda on 2025-05-09.
//

#ifndef UI_H
#define UI_H

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <functional>

class virus;
class dungeon;
class game;

class ui {

private:

    game * GameInstance = nullptr;
    std::vector<std::string> MessageLog;
    std::string InputBuffer;

    ftxui::Component InputComponent;
    ftxui::Component MainContainer;

    std::shared_ptr<game> CurrentGame; // DEPCRATED
    // BattleState
    std::shared_ptr<virus> CurrentVirus; // For the battle UI.
    bool inBattle = false;

public:
    ui();
    void Initialise(game* gameptr);
    void Run();

    // Message handling
    void AddMessage(const std::string& message);

    void Render(); // DECRPTED
    // Command Processing
    void ProcessCommand(const std::string& command);

    // Rendering Methods
    ftxui::Element RenderMessages() const;
    ftxui::Element RenderStatus() const;
    ftxui::Element RenderDungeonView();

    // Battle UI
    void StartBattle (std::shared_ptr<virus> enemy);
    void EndBattle();

    // input helpers
    std::string GetInputBlocking(const std::string& input);

    ftxui::Element RenderBattleUI(std::shared_ptr<virus> enemy);
    void ShowBattleButtons(std::shared_ptr<virus> enemy);
    std::string GetInput();
    void SetGame(std::shared_ptr<game> game) {CurrentGame = game;}
    std::shared_ptr<game> GetGame() const {return CurrentGame;}

private:
    ftxui::Element RenderLocationInfo();
    ftxui::Element RenderPlayerInfo();
    ftxui::Element RenderMessageLog();
    ftxui::Element RenderInputField();


};



#endif //UI_H
