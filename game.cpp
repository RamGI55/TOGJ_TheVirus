//
// Created by hunda on 2025-05-09.
//

#include "game.h"
#include "borough.h"
#include "dungeon.h"
#include "items.h"
#include "ui.h"
#include "player.h"
#include "GameUtil.h"
#include "locations.h"
#include "virus.h"


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
    std::string lcCommand = GameUtil::ToLower(command);

    if (lcCommand == "quit" || lcCommand == "q" || lcCommand == "exit") {
        Quit();
        return;
    }

    std::vector<std::string> tokens = GameUtil::Split(lcCommand, ' ');

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

void game::MovePlayer(int dx, int dy) {
    if (Player && Player -> GetCurrentDungeon()) {
        // Try to move the player
        if (Player -> MoveInDungeon(dx ,dy)) {
            // movement successful

            // check if the player in the exit.
            if (Player->GetCurrentDungeon()->AtExit()) {
                const std::string& dungeonName = Player->GetCurrentDungeon()->GetName();

                char buffer[128];
                std::snprintf(buffer, sizeof(buffer), "You've exited from %s", dungeonName.c_str());
                Ui->AddMessage (buffer);
            }
            // ITEM Finding; checking the player position
            auto dungeon = Player->GetCurrentDungeon();
            auto [PlayerX, PlayerY] = dungeon->GetPlayerPosition();
            auto& cell = dungeon->GetCell(PlayerX, PlayerY);

            if (cell.Type == DungeonCells::ITEM&& cell.Item) {
                Ui->AddMessage("You found a " + cell.Item->GetName()+ "!");
                Player->AddItem(cell.Item);
                dungeon->RemoveItem(PlayerX, PlayerY);
            }

            ProcessvirusMovement();

            // Check the Virus Attraction + health Reduction
        }
    }
}

void game::ProcessvirusMovement() {
    // Get the dungeon information player in.
    auto dungeon = Player->GetCurrentDungeon();
    if (!dungeon) return;

    // Move the virus based on the player's attraction level
    dungeon->MoveViruses(Player->GetVirusattraction());

    // Check if any viruses are not adjacent to the player.
    auto [playerX, playerY] = dungeon->GetPlayerPosition();
    const int dx[] = {0, 1, 0, -1, 1, 1, -1, -1}; // 8 directions (including diagonals)
    const int dy[] = {-1, 0, 1, 0, -1, 1, 1, -1};


    for (int i = 0; i < 8; i++) {
        int nx = playerX + dx[i];
        int ny = playerY + dy[i];

        if (nx >= 0 && nx < dungeon->GetWidth() && ny < dungeon->GetHeight()) {
            auto&cell = dungeon->GetCell(nx, ny);

            if (cell.Type == DungeonCells::VIRUS && cell.VirusEntity) {
                // chance for the virust to attack based on the attraction
                int attackChance = 20 +(Player->GetVirusattraction()/5 );

                if (GameUtil::RandomInt(1, 100) <= attackChance) {
                    int damage = cell.VirusEntity->AttackPlayer()/2;

                    Player->TakeDamage(damage);

                    std::string attackMsg = cell.VirusEntity->GetName(); +  " attacks you for " +
                        std::to_string(damage) + " damage";
                }

                if (Player->IsDefeated()) {
                    Ui->AddMessage("You are dead!, GameOver");
                    // TODO:: handle the gameover event.
                }
            }
        }
    }


}

void game::ExitDungeon(bool completed) {
    // check the player is in the dungeon
    auto dungeon = Player->GetCurrentDungeon();
    if (!dungeon) return;

    auto location = Player->GetCurrentDungeon();
    if (!location) return;

    float infectionIncrease = 0.0f;

    if (completed) {
        Ui->AddMessage("You've cleared the location, but still the infection continues in the rest of locations...");
        infectionIncrease = 0.01f;
    } else {
        // Player exited without completing - larger increase
        Ui->AddMessage("You've left the dungeon without clearing all viruses. Infection is spreading faster!");
        infectionIncrease = 0.15f;
    }
    // TODO::Location/Dungeons's infectionrate must be based on the (remain virus)/(total virus)
    // location->UpdateInfectionRate(infectionIncrease);

    Player->GetCurrentBorough()->Updateinfectionrates();

    Player->SetCurrenDungeon(nullptr);

    Ui->AddMessage(location->GetName() + " infection rate: " +
    std::to_string(static_cast<int>(location->GetInfectionRate() * 100)) + "%");
    Ui->AddMessage(Player->GetCurrentBorough()->GetName() + " infection rate: " +
    std::to_string(static_cast<int>(Player->GetCurrentBorough()->GetInfectionrate() * 100)) + "%");

    /*if (location->GetInfected()) {
        Ui->AddMessage("WARNING: " + location->GetName() + " is now fully infected!");
    }*/ // TODO::Must get infected attribute and infectionrate attributes from the location - must be same as dungeon's infection rate (remain virus/total virus)

    // Check if borough is now locked down
    if (Player->GetCurrentBorough()->GetInfectionrate() >= 1.0f) {
        Ui->AddMessage("CRITICAL ALERT: " + Player->GetCurrentBorough()->GetName() +
        " has been locked down due to high infection rates!");
    }

    // TODO:: Gameoverlogic if all of borough has been lockdowned.
}

void game::StartBattle(std::shared_ptr<virus> enemy) {
    Ui->AddMessage("Battle started with " + enemy->GetName() + "!" );
    Ui->AddMessage(enemy->GetDescription());

    bool battleInProgress = true;
    bool playerTurn = true; // TODO:: if player has the action point.

    while (battleInProgress && running) {
        if (playerTurn) {
            Ui->AddMessage ("Player Turn. Player Health : " + std::to_string(Player->GetHealth()) + " | Action Points: "
                + std::to_string(Player->GetActionpoints()));
            Ui->AddMessage ("Enemy: " +enemy->GetName() + " | Health : " + std::to_string(enemy->GetHealth()));

            // Show battle options
            Ui->AddMessage("[1] Attack - Use 1 action point to attack");
            Ui->AddMessage("[2] Use Item - Use an item from inventory");
            Ui->AddMessage("[3] Run - Try to escape the battle");

            // Get the player choice
            std::string choice = Ui->GetInput();

            if (choice == "1") {
                // Attack
                bool defeated = Player->AttackVirus(enemy);
                if (defeated) {
                    Ui->AddMessage("You are defeated the " + enemy->GetName() + "!");
                    battleInProgress = false;

                    // Remove the virus in the dungeon
                    auto dungeon = Player->GetCurrentDungeon();
                    auto virusPositions = dungeon->GetVirusPositions();
                    for (const auto& pos : virusPositions) {
                        auto& cell = dungeon->GetCell(pos.first, pos.second);
                        if (cell.VirusEntity) {
                            dungeon->Removevirus(pos.first, pos.second);
                            break;
                        }
                    }

                }else {
                    Ui->AddMessage("You attacked the " + enemy->GetName() + "!");
                    playerTurn = false; // Switch to enemy turn
                }
            } else if (choice == "2") {
                // Using Item
                Ui->AddMessage ("Choose an item to use:");
                const auto& inventory = Player->GetInventory();
                if (inventory.empty()) {
                    Ui->AddMessage("Invetory is Empty!");
                } else {
                    for (size_t i = 0; i < inventory.size(); i++) {
                        Ui->AddMessage("[" + std::to_string(i+1) + "] " + inventory[i]->GetName() +
                            " - " + inventory [i]->GetDescription());
                    }

                    std::string itemChoice = Ui->GetInput();
                    int itemIndex = std::stoi(itemChoice) -1;

                    if (itemIndex >= 0 && itemIndex < inventory.size()) {
                        std::string itemName = inventory[itemIndex]->GetName();
                        Player->UseItem(itemName);
                        Ui->AddMessage("You Used " + itemName + ".");
                    }   else {
                        Ui->AddMessage("Invaild Item Selection.");
                    }
                }
            } else if (choice == "3") {
                int EscapeChance = 50 - enemy->GetAttractionFactor() * 5;
                if (GameUtil::RandomInt(1, 100) <= EscapeChance) {
                    Ui->AddMessage("You've escaped from the virus.");
                    battleInProgress = false;
                }else {
                    Ui->AddMessage("Failed to escape.");
                    playerTurn = false;
                }
            }
            else {
                Ui->AddMessage("Invalid choice.");
            }
        }else {
            Ui->AddMessage("Enemy's Turn..");

            int damage = enemy->AttackPlayer();
            bool isCritical = enemy->isCriticalhit();

            std::string attackMsg = "The " + enemy->GetName() + " attacks you for " + std::to_string(damage) + " damage.";
            if (isCritical) {
                attackMsg += "CRITICAL HIT!";
            }
            Ui->AddMessage(attackMsg);

            Player->TakeDamage(damage);

            if (Player->IsDefeated()) {
                Ui->AddMessage("You have been defeated by the "+ enemy->GetName() + "!");
                battleInProgress = false;
                // TODO:: handle the game over state
            }
            else {
                playerTurn = true;
            }
            // Special enemy Abilities
        }
    }
}

void game::LoadLocations() {
    auto OldToronto = std::make_shared<borough>("Old Toronto");
    OldToronto->AddLocation(locations("Tower", "CN Tower", "Toronto's Landmark tower is now the biggest abandoned towner, now."));
    OldToronto->AddLocation(locations ("harbour", "Harbourfront", "The most iconic waterfront area is now the hotzone of the virus."));
    OldToronto->AddLocation(locations("financial", "Financial District", "Heart of the Canadian Financial now turns to the downward spiral."));
    OldToronto->AddLocation (locations("Square","Yonge-Dundas Square", "The most vibrent place in the Toronto attracts the virus from somewhere"));
    OldToronto->AddLocation(locations ("Kenshington","Kenshington Market-Chinatown", "Vibrant cultural district is now turned to the dead street"));
    OldToronto->AddLocation(locations("Kingwest", "King St. West", "The biggest clubgoers place turns infection red zone."));

    Boroughs["OldToronto"] = OldToronto;

    for (locations location : OldToronto->GetLocations()) {
        location.GenerateDungeon();
    }
}

void game::LoacItems() {
}

void game::LoadViruses() {
}

void game::SetupInitialGameState() {
}

void game::HandleCollision(int x, int y) {
    auto dungeon = Player->GetCurrentDungeon();
    if (!dungeon) return;

    auto& cell = dungeon->GetCell(x, y);

    switch (cell.Type) {
        case DungeonCells::VIRUS:
            if (cell.VirusEntity) {
                // Start battle with virus
                StartBattle(cell.VirusEntity);
            }
            break;

        case DungeonCells::ITEM:
            if (cell.Item) {
                Ui->AddMessage("You found a " + cell.Item->GetName() + "!");
                Player->AddItem(cell.Item);
                dungeon->RemoveItem(x, y);
            }
            break;

        case DungeonCells::EXIT:
            Ui->AddMessage("You've reached the exit!");
            ExitDungeon(true);
            break;

        default:
            break;
}
