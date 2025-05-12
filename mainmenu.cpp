//
// Created by hunda on 2025-05-12.
//

#include "mainmenu.h"
#include "game.h"
#include "map.h"
#include "GameUtil.h"

using namespace ftxui;

mainmenu::mainmenu() {
}

void mainmenu::Initialize(game *gamePtr, std::shared_ptr<map> mapPtr) {
    GameInstance = gamePtr;
    Map = mapPtr;

    // Add default callback implementations
    if (!onAddMessage) {
        onAddMessage = [](const std::string&) {};
    }

    if (!onStartGame) {
        onStartGame = [] {};
    }

    // Change to add button options:
    auto buttonOption = ButtonOption();
    buttonOption.Border();
    buttonOption.Animated(); // Add animation to make clicks visible

    startButton = Button("Start Game", [this]() {
        StartGame();
    }, buttonOption);

    helpButton = Button("Help", [this]() {
        ShowHelp();
    }, buttonOption);

    quitButton = Button("Quit", [this]() {
        if (GameInstance) {
            GameInstance->Quit();
        }
    }, buttonOption);

    // Create a container for the buttons
    menuContainer = Container::Vertical({
        startButton,
        helpButton,
        quitButton
    });

    // Enable mouse interaction for the menu container
    menuContainer |= CatchEvent([this](Event event) {
        // Handle mouse click events
        if (event.is_mouse()) {
            return false; // Return false to let the event propagate to the buttons
        }
        return false;
    });
}



ftxui::Element mainmenu::Render() const {
    if (!menuActive) {
        return ftxui::text("");  // Don't render if not active
    }

    Elements menuElements;

    // Add title
    menuElements.push_back(
        vbox({
            text("╔═══════════════════════════════════════════╗"),
            text("║              THE VIRUS                    ║"),
            text("║       A Virus Cleansing Adventure         ║"),
            text("║               TOJam Edition               ║"),
            text("╚═══════════════════════════════════════════╝")
        }) | bold | color(Color::Cyan)
    );


    /*
    // Add menu items
    Elements items;
    for (size_t i = 0; i < menuItems.size(); i++) {
        Element item = text(menuItems[i]);

        // Highlight selected item
        if (i == static_cast<size_t>(selectedItem)) {
            item = item | bold | color(Color::Green);
            item = hbox({text("> "), item, text(" <")});
        } else {
            item = hbox({text("  "), item, text("  ")});
        }

        items.push_back(item);
    }
    */

    // Add menu container with buttons
    menuElements.push_back(window(text("Menu"), menuContainer->Render()));

    // Add instructions
    menuElements.push_back(
        vbox({
            text("Click a button or use arrow keys to navigate, Enter to select"),
            text("Or type a command: 'start', 'help', 'quit'")
        }) | color(Color::GrayLight)
    );

    return vbox(menuElements);
}

bool mainmenu::HandleInput(ftxui::Event event) {
    if (!menuActive) {
        return false;
    }

    // Handle arrow key navigation
    if (event == Event::ArrowUp) {
        selectedItem = std::max(0, selectedItem - 1);
        return true;
    }

    if (event == Event::ArrowDown) {
        selectedItem = std::min(static_cast<int>(menuItems.size() - 1), selectedItem + 1);
        return true;
    }
    // Handle selection
    if (event == Event::Return) {
        return true;  // Let the button handle it
    }

    return false;

    /*// Handle selection
    if (event == Event::Return) {
        switch (selectedItem) {
            case 0: // Start Game
                StartGame();
                break;
            case 1: // Help
                ShowHelp();
                break;
            case 2: // Quit
                if (GameInstance) {
                    GameInstance->Quit();
                }
                break;
        }
        return true;
    }

    return false;*/
}

bool mainmenu::HandleCommand(const std::string &command) {
    if (!menuActive) {
        return false;
    }

    std::string lcCommand = GameUtil::ToLower(command);

    if (lcCommand == "start") {
        StartGame();
        return true;
    }

    if (lcCommand == "help") {
        ShowHelp();
        return true;
    }

    if (lcCommand == "quit" || lcCommand == "exit") {
        if (GameInstance) {
            GameInstance->Quit();
        }
        return true;
    }

    return false;
}

void mainmenu::ShowHelp() {
    onAddMessage("Command List:");
    onAddMessage("  start - Begin your virus-fighting mission");
    onAddMessage("  help  - Show this help message");
    onAddMessage("  quit  - Exit the game");
    onAddMessage("");
    onAddMessage("Once in game:");
    onAddMessage("  enter <location> - Enter a location (e.g., 'enter tower')");
    onAddMessage("  look - Look around your current location");
    onAddMessage("  inventory - Check your items");
    onAddMessage("");
    onAddMessage("  Remember - More you got, More you lost.");
}

void mainmenu::StartGame() {
    onAddMessage("DEBUG: StartGame called");
    onAddMessage("Starting the game...");
    onAddMessage("Your mission is to cleanse the virus from key locations in Toronto.");

    // Hide the menu
    menuActive = false;

    // Call the callback
    if (onStartGame) {
        onStartGame();
    }
}

void mainmenu::ShowTitle() const {
    if (onAddMessage) {
        onAddMessage("THE VIRUS");
        onAddMessage("A Virus Cleansing Adventure");
        onAddMessage("Type 'help' for commands or 'start' to begin");
    }
}
