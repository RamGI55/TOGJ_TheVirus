//
// Created by hunda on 2025-05-09.
//

#include "ui.h"

#include "borough.h"
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

ui::ui()  {

}

ui::~ui() {
    // Ensure components are properly cleaned up
    MainContainer = nullptr;
    InputComponent = nullptr;

    // Clean up the screen
    delete screen;
    screen = nullptr;

    // Clear any other resources
    MessageLog.clear();
    CurrentVirus = nullptr;
}

void ui::Initialise(game* gameptr) {
    GameInstance = gameptr;

    MainContainer = Container::Vertical({});

    // initialise the input component
    InputComponent = Input(&InputBuffer, "Command : ");

    // Create the renderer with proper captures
    auto renderer = Renderer([this] {
        Elements elements;
        elements.push_back(RenderStatus());

        if (GameInstance && GameInstance->GetPlayer() && GameInstance->GetPlayer()->GetCurrentDungeon()) {
            elements.push_back(RenderDungeonView());
        }
        elements.push_back(
            hbox({
            text("> "), InputComponent->Render()})
            );
        return vbox(elements);
    });

    auto EventHandler = CatchEvent([this](Event event) {
        if (GameInstance && GameInstance->GetPlayer() && GameInstance->GetPlayer()->GetCurrentDungeon()) {
            if (event == Event::ArrowUp) {
                GameInstance->MovePlayer(0, -1);
                return true;
            }
            if (event == Event::ArrowDown) {
                GameInstance->MovePlayer(0, 1);
                return true;
            }
            if (event == Event::ArrowLeft) {
                GameInstance->MovePlayer(-1, 0);
                return true;
            }
            if (event == Event::ArrowRight) {
                GameInstance->MovePlayer(1, 0);
                return true;
            }
        }
        // Handle enter key for command input
        if (event == Event::Return) {
            if (!InputBuffer.empty()) {
                ProcessCommand(InputBuffer);
                InputBuffer.clear();
                return true;
            }
        }
        return false;
    });

    MainContainer |= EventHandler;
    /*MainContainer = Container::Vertical({
    Renderer(MainContainer, [&]() {
        return renderer->Render();
        })
    });*/
    MainContainer = Container::Vertical({renderer});
}

void ui::Run() {
    // Create a screen and store it as a member
    if (screen == nullptr) {
        screen = new ftxui::ScreenInteractive(ftxui::ScreenInteractive::TerminalOutput());
    }

    // Run the main loop with proper error handling
    try {
        screen->Loop(MainContainer);
    } catch (const std::exception& e) {
        std::cerr << "Exception in UI loop: " << e.what() << std::endl;
    }
}

void ui::AddMessage(const std::string &message) {
    MessageLog.push_back(message);
    // Erase the Message log over 20s
    const size_t maxMessages = 20;
    if (MessageLog.size() > maxMessages) {
        MessageLog.erase(MessageLog.begin());
    }
}

void ui::ProcessCommand(const std::string &command) {
    if (GameInstance) {
        GameInstance->ProcessInput(command);
    }
}

ftxui::Element ui::RenderMessages() const {
    Elements message_elements;
    for (const auto& message : MessageLog) {
        message_elements.push_back(text(message));
    }

    return window(text("Messages"), vbox(message_elements)) | size(HEIGHT, LESS_THAN, 10);
}

ftxui::Element ui::RenderStatus() const {

    if (!GameInstance || !GameInstance->GetPlayer()) {
        text("No player information available");
    }
    auto player = GameInstance->GetPlayer();

    Elements status_elements;
    status_elements.push_back(text("Health: " + std::to_string(player->GetHealth()) +
        "/" + std::to_string(player->GetMaxHealth())));
    status_elements.push_back(text("Action Points: " + std::to_string(player->GetActionpoints())));
    status_elements.push_back(text("Virus Attraction: " + std::to_string(player->GetVirusattraction()) + "%"));

    // Add location information if available
    if (player->GetCurrentBorough()) {
        status_elements.push_back(text("Borough: " + player->GetCurrentBorough()->GetName()));
    }
    return window(text("Status"), vbox(status_elements));
}

ftxui::Element ui::RenderDungeonView() {
    if (!GameInstance || !GameInstance->GetPlayer() || !GameInstance->GetPlayer()->GetCurrentDungeon()) {
        return text("No dungeon to display");
    }

    auto dungeon_ptr = GameInstance->GetPlayer()->GetCurrentDungeon();
    auto player_pos = dungeon_ptr->GetPlayerPosition();
    int width = dungeon_ptr->GetWidth();
    int height = dungeon_ptr->GetHeight();

    // Create a canvas for the dungeon (doubled size for better visibility)
    auto c = Canvas(width * 2, height * 2);

    // Draw each cell of the dungeon
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto& cell = dungeon_ptr->GetCell(x, y);

            // Choose color based on cell type
            Color color;
            switch (cell.Type) {
                case DungeonCells::WALL:
                    color = Color::GrayDark;
                    break;
                case DungeonCells::ENTRANCE:
                    color = Color::Blue;
                    break;
                case DungeonCells::EXIT:
                    color = Color::GreenLight;
                    break;
                case DungeonCells::ITEM:
                    color = Color::Yellow;
                    break;
                case DungeonCells::VIRUS:
                    color = Color::Red;
                    break;
                default:
                    color = Color::Black;
                    break;
            }

            // Draw a 2x2 block of colored points for each cell
            for (int dy = 0; dy < 2; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    c.DrawPoint(x * 2 + dx, y * 2 + dy, true, color);
                }
            }
        }
    }

    // Draw the player
    for (int dy = 0; dy < 2; ++dy) {
        for (int dx = 0; dx < 2; ++dx) {
            c.DrawPoint(player_pos.first * 2 + dx, player_pos.second * 2 + dy, true, Color::White);
        }
    }

    // Return the canvas
    return window(text("Dungeon"), canvas(c));
}

void ui::StartBattle(std::shared_ptr<virus> enemy) {
    inBattle = true;
    CurrentVirus = enemy;

    AddMessage("Battle started with " + enemy->GetName() + "!");
    AddMessage(enemy->GetDescription());
}

void ui::EndBattle() {
    inBattle = false;
    CurrentVirus = nullptr;

    AddMessage("Battle ended!");
}

std::string ui::GetInputBlocking(const std::string &input) {
    AddMessage(input);

    // In a real implementation, you'd use a modal dialog or similar
    // For now, we'll just return a placeholder
    return "1";  // Default to the first option
}

std::string ui::GetInput() {
    // Create a simple input dialog
    std::string result;
    bool input_done = false;

    // Add a prompt to the message log
    AddMessage("Enter your choice: ");

    // Create a temporary input component
    auto temp_input = Input(&result, "");

    // Create a button to confirm input
    auto confirm_button = Button("Confirm", [&]() {
        input_done = true;
    });

    // Create a container for these components
    auto container = Container::Vertical({
        temp_input,
        confirm_button
    });

    // Setup a modal dialog
    auto screen = ScreenInteractive::TerminalOutput();

    // Create an exit flag for the modal
    bool modal_exit = false;

    // Set up a special renderer for the modal
    auto modal_renderer = Renderer(container, [&]() {
        return vbox({
            text("Enter your choice:"),
            temp_input->Render(),
            confirm_button->Render()
        }) | border;
    });

    // Set up an event handler to exit when done
    container |= CatchEvent([&](Event event) {
        if (event == Event::Return) {
            input_done = true;
            modal_exit = true;
            return true;
        }
        if (input_done) {
            modal_exit = true;
            return true;
        }
        return false;
    });

    // Run the modal until input is done
    screen.Loop(modal_renderer);

    return result;
}
