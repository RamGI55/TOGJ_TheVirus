//
// Created by hunda on 2025-05-09.
//

#ifndef UI_H
#define UI_H

#pragma once
#include <string>
#include <memory>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

class virus;
class dungeon;
class game;
class ui {

private:
    std::string InputBuffer;
    std::vector<std::string> MessageLog;
    std::shared_ptr<game> CurrentGame;
    std::shared_ptr<virus> CurrentVirus; // For the battle UI.
    bool inBattle = false;

    ftxui::Component InputField;
    ftxui::ScreenInteractive Screen;
    ftxui::Element RenderDungeon(const dungeon& dungeon);

public:

    void Initialise();
    void AddMessage(const std::string& message);
    void Render();
    void ProcessCommand(const std::string& command);

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
