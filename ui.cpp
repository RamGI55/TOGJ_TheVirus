//
// Created by hunda on 2025-05-09.
//

#include "ui.h"

#include "dungeon.h"
#include "game.h"
#include "GameUtil.h"
#include "player.h"
#include "virus.h"

using namespace ftxui;

ftxui::Element ui::RenderDungeon(const dungeon &dungeon) {
    auto playerpos = dungeon.GetPlayerPosition();
    int width = dungeon.GetWidth();
    int height = dungeon.GetHeight();

    auto canvas = ftxui::Canvas(width, height);

    for (int y =0 ; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const auto& cell= dungeon.GetCell(x, y);

            switch (cell.Type) {
                case DungeonCells::WALL:
                    canvas.DrawBlock(x,y,ftxui::Color::GrayDark);
                    break;
                case DungeonCells::ENTRANCE:
                    canvas.DrawBlock(x,y,ftxui::Color::Blue);
                    break;
                case DungeonCells::EXIT:
                    canvas.DrawBlock(x,y,ftxui::Color::GreenLight);
                    break;
                case DungeonCells::ITEM:
                    canvas.DrawBlock(x,y,ftxui::Color::Yellow);
                    break;
                case DungeonCells::VIRUS:
                    canvas.DrawBlock(x,y,ftxui::Color::Red);
                    break;
                default:
                    canvas.DrawBlock(x,y,ftxui::Color::Black);
                    break;
            }
        }
    }

    canvas.DrawBlock(playerpos.first, playerpos.second, ftxui::Color::White);
    return ftxui::canvas(std::move(canvas));

}

ui::ui()    : InputBuffer("")
    , MessageLog()
    , CurrentGame(nullptr)
    , InputField()
    , inBattle(false)
    , CurrentVirus(nullptr) {
}

void ui::Initialise(game* gameptr) {
    InputField = Input(&InputBuffer, "Enter the Command");
    // Setup the screen
    screen = ftxui::ScreenInteractive::TerminalOutput(); // TODO::ERROR!
    // ERROR: Attempt to use deleted ScreenInteractive &ftxui::ScreenInteractive::operator=(const ScreenInteractive &)

    auto renderComponent = Renderer([this] {
        return vbox({
        RenderLocationInfo(),
        RenderPlayerInfo(),
        RenderDungeon(*CurrentGame->GetPlayer()->GetCurrentDungeon()), // TODO:: which parameter?
        RenderMessageLog(),
        RenderInputField()});
    });

    auto inputComponent = InputField;
    auto gameComponent = CatchEvent([this](const Event& event) {
        auto game = GetGame(); // TODO::ERROR.
        if (game && game->GetPlayer() && game->GetPlayer()->GetCurrentDungeon()) {
            if (event == Event::ArrowUp) {
                game->MovePlayer(0, -1);
                return true;
            }
            if (event == Event::ArrowDown) {
                game->MovePlayer(0, 1);
                return true;
            }
            if (event == Event::ArrowRight) {
                game->MovePlayer(1,0 );
                return true;
            }
            if (event == Event::ArrowLeft) {
                game->MovePlayer(-1,0 );
                return true;
            }
        }
        if (event == Event::Return) {
            if (!InputBuffer.empty()) {
                ProcessCommand(InputBuffer);
                InputBuffer.clear();
                return true;
            }
        }
        return false;
    });

    auto EnterPressed = [this] {
        if (!InputBuffer.empty()) {
            ProcessCommand(InputBuffer);
            InputBuffer.clear();
            Render();
        }
        return true;
    };

    InputField |= CatchEvent([this, EnterPressed](const Event &event) {
        if (event == Event::Return) {
            return EnterPressed();
        }
        return false;
    });

    // comabine all components


    ftxui::Component components;
    components->Add(inputComponent);
    components->Add(gameComponent); // TODO:: ERROR Cannot convert braced-init-list to parameter type Components

    auto container = Container::Vertical(components);
    screen.Loop(renderComponent | gameComponent);
}

void ui::AddMessage(const std::string &message) {
    MessageLog.push_back(message);
    // Erase the Message log over 20s
    if (MessageLog.size() > 20) {
        MessageLog.erase(MessageLog.begin());
    }
}

void ui::Render() {
}

void ui::ProcessCommand(const std::string &command) {

}

ftxui::Element ui::RenderBattleUI(std::shared_ptr<virus> enemy) {
    CurrentVirus = enemy;
    inBattle = true;
    auto player = CurrentGame->GetPlayer();

    // Battle Display
    std::string playerHealth = "HP: " + std::to_string(player->GetHealth()); + "/" +
        std::to_string(player->GetMaxHealth());
    std::string playerAP = "AP: "+ std::to_string(player->GetHealth()); + "/10"; // Max AP is hardcoded.. for now...
    std::string enemyHealth = "HP: " + std::to_string(enemy->GetHealth());

    // Create player and enemy status displays
    Elements playerStatus = {
    text("Player: ")| bold,
    text(playerHealth),
    text(playerAP),
    text("Virus Attraction: " + std::to_string(player->GetVirusattraction()) + "%")};

    Elements enemyStatus = {
        text("Enemy: " + enemy->GetName()) | bold | color(Color::Red),
        text(enemyHealth),
        text(enemy->GetDescription())
    };


    // Battle buttons
    auto attackButton = Button("Attack", [this]() {
        if (CurrentGame && CurrentGame->GetPlayer() && CurrentVirus) {
            bool defeated = CurrentGame->GetPlayer()->AttackVirus(CurrentVirus);
            if (defeated) {
                AddMessage("You defeated the " + CurrentVirus->GetName() + "!");
                inBattle = false;
            } else {
                AddMessage("You attacked the " + CurrentVirus->GetName() + "!");
                // Enemy's turn would be processed in the actual implementation
            }
        }
    });

    auto itemButton = Button("Use Item", [this]() {
        AddMessage("Choose an item to use:");
        // Show item selection in the actual implementation
    });

    auto runButton = Button("Run", [this]() {
        AddMessage("Attempting to escape...");
        // Run logic would be processed in the actual implementation
    });
    // Arrange the battle UI

    ftxui::Elements buttonElements;
    buttonElements.push_back(attackButton|border);
    buttonElements.push_back(itemButton | border);
    buttonElements.push_back(runButton | border);
    return vbox({
        hbox({
            vbox(playerStatus) | flex,
            vbox(enemyStatus) | flex
        }),
        separator(),
        hbox(buttonElements) | center // TODO ERROR ::Cannot convert braced-init-list to parameter type Elements
    }) | border | center;

}

void ui::ShowBattleButtons(std::shared_ptr<virus> enemy) {
    // Set up clickable buttons for battle
    std::vector<Component> battleButtons;

    // Attack button
    auto attackButton = Button("Attack", [this, enemy]() {
        if (CurrentGame && CurrentGame->GetPlayer()) {
            bool defeated = CurrentGame->GetPlayer()->AttackVirus(enemy);
            if (defeated) {
                AddMessage("You defeated the " + enemy->GetName() + "!");
                inBattle = false;
                // Remove enemy from dungeon
            } else {
                AddMessage("You attacked the " + enemy->GetName() + "!");
                // Process enemy turn
            }
        }
    });
    battleButtons.push_back(attackButton);

    // Items button
    auto itemsButton = Button("Items", [this]() {
        // Show items menu
        AddMessage("Choose an item:");
        // Implementation for selecting and using items
    });
    battleButtons.push_back(itemsButton);

    // Run button
    auto runButton = Button("Run", [this, enemy]() {
        int escapeChance = 50 - enemy->GetAttractionFactor() * 5;
        if (GameUtil::RandomInt(1, 100) <= escapeChance) {
            AddMessage("You successfully escaped!");
            inBattle = false;
        } else {
            AddMessage("You failed to escape!");
            // Process enemy turn
        }
    });
    battleButtons.push_back(runButton);

    // Create a container with the buttons
    auto buttonContainer = Container::Horizontal(battleButtons);

    // Render the button container
    // In a real implementation, this would be added to the screen
}

std::string ui::GetInput() {
    // Block until user enters a command
    std::string input;

    // Set up a temporary input component
    auto inputComp = Input(&input, "");
    auto enterPressed = [&input]() {
        return !input.empty();
    };

    // Add event handling for the Enter key
    inputComp |= CatchEvent([&enterPressed](const Event& event) {
        if (event == Event::Return) {
            return enterPressed();
        }
        return false;
    });

    // Render the component and wait for input
    auto document = vbox({
        text("Enter your choice:"),
        inputComp->Render()
    });

    auto screen = Screen::Create(Terminal::Size());
    screen.Clear();
    document.Render(screen);
    screen.Print();

    // In a real implementation, this would handle input events until Enter is pressed

    return input;
}


ftxui::Element ui::RenderLocationInfo() {
}

ftxui::Element ui::RenderPlayerInfo() {
}

ftxui::Element ui::RenderMessageLog() {
}

ftxui::Element ui::RenderInputField() {
}
