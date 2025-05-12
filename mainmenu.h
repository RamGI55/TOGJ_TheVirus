//
// Created by hunda on 2025-05-12.
//

#ifndef MAINMENU_H
#define MAINMENU_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

class game;
class map;

class mainmenu {
private:
    game* GameInstance = nullptr;
    std::shared_ptr<map> Map;

    // Menu state
    bool menuActive = true;
    std::vector<std::string> menuItems = {"Start Game", "Help", "Quit"};
    int selectedItem = 0;

    // Callbacks
    std::function<void()> onStartGame;
    std::function<void(const std::string&)> onAddMessage;

public:
    mainmenu();
    void Initialize(game* gamePtr, std::shared_ptr<map> mapPtr);

    // Setup callbacks
    void SetOnStartGame(std::function<void()> callback) { onStartGame = callback; }
    void SetOnAddMessage(std::function<void(const std::string&)> callback) { onAddMessage = callback; }

    // Rendering
    ftxui::Element Render() const;

    // Input handling
    bool HandleInput(ftxui::Event event);
    bool HandleCommand(const std::string& command);

    // State management
    void Show() { menuActive = true; }
    void Hide() { menuActive = false; }
    bool IsActive() const { return menuActive; }

private:
    // Helper methods
    void ShowHelp();
    void StartGame();
    void ShowTitle() const;
};



#endif //MAINMENU_H
