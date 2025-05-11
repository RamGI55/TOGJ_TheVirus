//
// Created by hunda on 2025-05-10.
//

#include "locations.h"

#include "dungeon.h"

locations::locations(const std::string &id, const std::string &name, const std::string &description):
id(id), name(name), description(description),
baseinfectionrate(0.f), infectionrate(0.f), isInfected(false){
}

int locations::GetInfectionrate() {
    infectionrate = baseinfectionrate;
    isInfected = (infectionrate >=1.0f);
    return static_cast<int>(infectionrate*100);
}

int locations::GetInfected() {
    return isInfected ? 1: 0;
}

void locations::UpdateInfectionrate(float delta) {
    infectionrate =std::max(0.0f, std::min(0.0f ,infectionrate+ delta));
    isInfected = (infectionrate == 1.f);
}

void locations::ResetInfectionrate() {
    infectionrate = baseinfectionrate;
    isInfected = false;
}

void const locations::GenerateDungeon() {
    // Create a new dungeon with parameters from JSON
    CurrentDungeon = std::make_shared<dungeon>(name, dungeonWidth, dungeonHeight);

    // Configure initial viruses and items count
    CurrentDungeon->SetInitialCounts(dungeonInitialViruses, dungeonInitialItems);

    // Generate the dungeon
    CurrentDungeon->GenerateDungeon();

    // Update infection rate based on the dungeon
    infectionrate = CurrentDungeon->GetInfectionRate();

}

