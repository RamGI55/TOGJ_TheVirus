//
// Created by hunda on 2025-05-09.
//

#include "game.h"
#include "borough.h"
#include "items.h"
#include "ui.h"
#include "player.h"
#include "Utils.h"


game::game() :running(false), Ui(nullptr)  {
}

void game::Initialise() {

}

void game::Run() {

    Ui->Initialise();
    Ui->AddMessage("THE VIRUS");
    Ui->AddMessage("Press Start to Start the Game");
    Ui->AddMessage("Type 'help' for commands");

    Ui->Render();
}

void game::ProcessInput(const std::string &command) {
    std::string lcCommand = Utils::ToLower(command);

    if (lcCommand == "quit" || lcCommand == "q" || lcCommand == "exit") {
        Quit();
        return;
    }

    std::vector<std::string> tokens = Utils::Split(lcCommand, ' ');

    if (tokens.empty()) {
        Ui->AddMessage("Please enter the command");
        return;
    }

    std::string action = tokens[0];

    if (action == "help") {
        Ui->AddMessage("Available commands:");
        Ui->AddMessage("  look - Look around the current location");
        Ui->AddMessage("  go <direction> - Move in a direction");
        Ui->AddMessage("  take <item> - Pick up an item");
        Ui->AddMessage("  drop <item> - Drop an item from your inventory");
        Ui->AddMessage("  inventory - List your items");
        Ui->AddMessage("  clean <virus> - Attack a virus");
        Ui->AddMessage("  status - Display your status");
        Ui->AddMessage("  quit - Exit the game");

    }
}

void game::Update() {
    SpawnViruses();
}

void game::SpawnViruses() {

}

void game::LoadLocations() {
}

void game::LoacItems() {
}

void game::LoadViruses() {
}

void game::SetupInitialGameState() {
}
