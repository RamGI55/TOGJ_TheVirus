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
    running = true;

    // Create UI
    Ui = std::make_shared<ui>();
    //Ui->SetGame(this);
    Ui->SetGame(shared_from_this()); // substitution of this in the raw pointer.

    // Load all data from JSON files
    LoadDataFromJson();

    // Create player
    Player = std::make_shared<player>("Virus Hunter");

    // Initialize player with starting items
    InitialisePlayerItems();

    // Set up initial game state - player starts in the first borough
    if (!Boroughs.empty()) {
        Player->SetCurrentBorough(Boroughs.begin()->second);
    }

    // Display welcome message
    Ui->AddMessage("THE VIRUS");
    Ui->AddMessage("Press Start to Start the Game");
    Ui->AddMessage("Type 'help' for commands");
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
        Ui->AddMessage("  enter <borough/location> - Enter a borough or location");
        Ui->AddMessage("  go <direction> - Move in a direction (north/south/east/west)");
        Ui->AddMessage("  take <item> - Pick up an item");
        Ui->AddMessage("  drop <item> - Drop an item from your inventory");
        Ui->AddMessage("  inventory - List your items");
        Ui->AddMessage("  clean <virus> - Attack a virus");
        Ui->AddMessage("  status - Display your status");
        Ui->AddMessage("  quit - Exit the game");
    }
    if (action == "enter" && tokens.size() > 1) {
        // Try to enter a borough
        auto boroughIt = Boroughs.find(GameUtil::ToLower(tokens[1]));
        if (boroughIt != Boroughs.end()) {
            Player->SetCurrentBorough(boroughIt->second);
            Ui->AddMessage("You've entered " + boroughIt->second->GetName() + ".");

            // Show available locations
            Ui->AddMessage("Available locations:");
            const auto& locations = boroughIt->second->GetLocations();
            for (const auto& location : locations) {
                Ui->AddMessage("- " + location.GetId() + ": " + location.GetName());
            }

            // Clear current location when entering new borough
            currentLocation = nullptr;
            return;
        }

        // Try to enter a location
        EnterLocation(tokens[1]);
        return;
    }

    // If we get here, the command wasn't recognized
    Ui->AddMessage("Unknown command. Type 'help' for a list of commands.");
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

    Player->SetCurrentDungeon(nullptr);

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
}

void game::LoadDataFromJson() {
    try {
        LoadBoroughsFromJson("data/boroughs.json");
        LoadLocationsFromJson("data/locations.json");
        LoadVirusesFromJson("data/viruses.json");
        LoadItemsFromJson("data/items.json");

        Ui->AddMessage("Game data loaded successfully.");
    } catch (const std::exception& e) {
        Ui->AddMessage("Error loading game data: " + std::string(e.what()));
        // Continue with defaults if possible
    }
}

void game::LoadBoroughsFromJson(const std::string &filename) {
    nlohmann::json data = GameUtil::LoadJson(filename);

    for (const auto& boroughJson : data["boroughs"]) {
        std::string name = boroughJson["name"];
        auto borough = std::make_shared<class borough>(name);

        // Store in map with lowercase key for case-insensitive lookup
        Boroughs[GameUtil::ToLower(name)] = borough;

        Ui->AddMessage("Loaded borough: " + name);
    }
}

void game::LoadLocationsFromJson(const std::string &filename) {
    nlohmann::json data = GameUtil::LoadJson(filename);

    for (const auto& locationJson : data["locations"]) {
        std::string id = locationJson["id"];
        std::string name = locationJson["name"];
        std::string description = locationJson["description"];
        std::string boroughName = locationJson["borough"];
        float baseInfectionRate = locationJson.value("baseInfectionRate", 0.3f);

        // Find the borough to add this location to
        auto boroughIt = Boroughs.find(GameUtil::ToLower(boroughName));
        if (boroughIt != Boroughs.end()) {
            // Create and add the location
            locations location(id, name, description);
            location.SetBaseInfectionRate(baseInfectionRate);

            // Set dungeon parameters if present
            if (locationJson.contains("dungeonWidth") && locationJson.contains("dungeonHeight")) {
                int dungeonWidth = locationJson["dungeonWidth"];
                int dungeonHeight = locationJson["dungeonHeight"];
                // These would be stored for when we generate the dungeon
            }

            boroughIt->second->AddLocation(location);
            Ui->AddMessage("Loaded location: " + name + " in " + boroughName);
        }
    }
}

void game::LoadVirusesFromJson(const std::string &filename) {
    nlohmann::json data = GameUtil::LoadJson(filename);

    for (const auto& virusJson : data["viruses"]) {
        std::string id = virusJson["id"];
        std::string name = virusJson["name"];
        std::string description = virusJson["description"];
        int health = virusJson["health"];
        int damage = virusJson["damage"];
        int attractionFactor = virusJson["attractionFactor"];
        int moveSpeed = virusJson["moveSpeed"];
        int criticalRate = virusJson.value("criticalRate", 5);
        float criticalMultiplier = virusJson.value("criticalMultiplier", 1.5f);

        // Create the virus
        auto virusEntity = std::make_shared<virus>(
            name, description, health, damage, attractionFactor, moveSpeed,
            criticalRate, criticalMultiplier);

        // Store in the viruses map
        viruses[id] = virusEntity;

        Ui->AddMessage("Loaded virus: " + name);
    }
}

void game::LoadItemsFromJson(const std::string &filename) {
    nlohmann::json data = GameUtil::LoadJson(filename);

    for (const auto& itemJson : data["items"]) {
        std::string id = itemJson["id"];
        std::string name = itemJson["name"];
        std::string description = itemJson["description"];
        int healthBoost = itemJson.value("healthBoost", 0);
        int damageBoost = itemJson.value("damageBoost", 0);
        int attractionModifier = itemJson.value("attractionModifier", 0);
        int actionPointBoost = itemJson.value("actionPointBoost", 0);
        bool consumable = itemJson.value("consumable", true);
        float infectionRateImpact = itemJson.value("infectionRateImpact", 0.0f);

        // Create the item
        auto item = std::make_shared<items>(
            name, description, healthBoost, damageBoost, attractionModifier,
            actionPointBoost, consumable, infectionRateImpact);

        // Store in items map
        Items[id] = item;

        Ui->AddMessage("Loaded item: " + name);
    }
}

void game::InitialisePlayerItems() {
    // Give player starting items (3 health potions and 3 action point potions)
    for (int i = 0; i < 3; i++) {
        if (Items.find("healthkit") != Items.end()) {
            Player->AddItem(Items["healthkit"]);
        }

        if (Items.find("actionpotion") != Items.end()) {
            Player->AddItem(Items["actionpotion"]);
        }
    }
}

std::shared_ptr<virus> game::CreateVirusFromType(const std::string &virusId) {
    auto it = viruses.find(virusId);
    if (it != viruses.end()) {
        // Clone the virus to create a new instance
        auto original = it->second;
        return std::make_shared<virus>(
            original->GetName(), original->GetDescription(), //TODO:: Error Marked
            original->GetHealth(), original->GetDamage(),
            original->GetAttractionFactor(), original->GetMoveSpeed(),
            original->GetCriticalRate(), original->GetCriticalMultiplier()
        );
    }

    // If not found, return a basic virus
    return std::make_shared<virus>("Unknown Virus", "A mysterious virus.",
    10, 2, 1, 1, 5, 1.5f);
}

std::shared_ptr<items> game::CreateItemFromType(const std::string &itemId) {
    auto it = Items.find(itemId);
    if (it != Items.end()) {
        // Return the item directly - items can be shared
        return it->second;
    }

    // If not found, return a basic item
    return std::make_shared<items>("Unknown Item", "A mysterious item.",
    0, 0, 0, 0, true, 0.0f);
}

void game::EnterLocation(const std::string &locationId) {
    // Check if player is in a borough
    auto currentBorough = Player->GetCurrentBorough();
    if (!currentBorough) {
        Ui->AddMessage("You must enter a borough first.");
        return;
    }

    // Get available locations in this borough
    const auto& locationsVector = currentBorough->GetLocations();

    // Find the location with matching ID
    for (int i = 0; i < locationsVector.size(); i++) {
        const locations& loc = locationsVector[i];
        if (loc.GetId() == locationId) {
            // Found the location
            Ui->AddMessage("You've entered " + loc.GetName() + ".");
            Ui->AddMessage(loc.GetDescription());

            // Get a non-const reference to the location
            locations& location = currentBorough->GetLocation(i);

            // Store current location
            currentLocation = std::make_shared<locations>(location);

            // Generate a new dungeon for this location
            location.GenerateDungeon();

            // Populate the dungeon with items and viruses
            PopulateDungeon(location.GetDungeon());

            // Set the player's current dungeon
            Player->SetCurrentDungeon(location.GetDungeon());

            // Inform the player they've entered the dungeon
            Ui->AddMessage("You've entered the dungeon at " + location.GetName() + ".");
            Ui->AddMessage("Use arrow keys to move. Watch out for viruses!");

            return;
        }
    }

    // If we get here, the location wasn't found
    Ui->AddMessage("Cannot find location: " + locationId);
}

void game::PopulateDungeon(std::shared_ptr<dungeon> dungeon) {
    if (!dungeon) return;

    // Get all cells with items and viruses
    for (int y = 0; y < dungeon->GetHeight(); y++) {
        for (int x = 0; x < dungeon->GetWidth(); x++) {
            auto& cell = dungeon->GetCell(x, y);

            if (cell.Type == DungeonCells::ITEM && !cell.Item) {
                // Choose a random item type from our loaded items
                std::vector<std::string> itemTypes;
                for (const auto& [id, item] : Items) {
                    itemTypes.push_back(id);
                }

                if (!itemTypes.empty()) {
                    int randomIndex = GameUtil::RandomInt(0, itemTypes.size() - 1);
                    std::string randomItemType = itemTypes[randomIndex];

                    // Use the item from our loaded items
                    dungeon->AddItem(x, y, Items[randomItemType]);
                }
            }
            else if (cell.Type == DungeonCells::VIRUS && !cell.VirusEntity) {
                // Choose a random virus type, with boss being rare
                std::vector<std::string> virusTypes;
                for (const auto& [id, virusEntity] : viruses) {
                    if (id != "boss") {  // Add regular viruses more often
                        virusTypes.push_back(id);
                    }
                }

                // Small chance to add a boss
                if (GameUtil::RandomInt(1, 100) <= 10 && viruses.find("boss") != viruses.end()) {
                    virusTypes.push_back("boss");
                }

                if (!virusTypes.empty()) {
                    int randomIndex = GameUtil::RandomInt(0, virusTypes.size() - 1);
                    std::string randomVirusType = virusTypes[randomIndex];

                    // Use the virus template from our loaded viruses
                    // Create a clone since each virus needs its own health
                    auto originalVirus = viruses[randomVirusType];
                    auto newVirus = std::make_shared<virus>(
                        originalVirus->GetName(),
                        originalVirus->GetDescription(),
                        originalVirus->GetHealth(),
                        originalVirus->GetDamage(),
                        originalVirus->GetAttractionFactor(),
                        originalVirus->GetMoveSpeed(),
                        originalVirus->GetCriticalRate(),
                        originalVirus->GetCriticalMultiplier()
                    );

                    dungeon->Addvirus(x, y, newVirus);
                }
            }
        }
    }
}



