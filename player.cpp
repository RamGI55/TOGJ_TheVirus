//
// Created by hunda on 2025-05-09.
//

#include "player.h"

#include <algorithm>

#include "dungeon.h"
#include "GameUtil.h"
#include "items.h"
#include "virus.h"

player::player(const std::string &name) : name(name), maxhealth(150), health(150), actionpoints (10), virusattraction(0),
criticalrate(10), criticalmultiplier(2.0f){
    UpdateVirusAttraction();
}

void player::AddItem(const std::shared_ptr<items> item) {
    inventory.push_back(item);
    UpdateVirusAttraction();
}

void player::RemoveItem(const std::string &ItemName) {
    auto it = std::find_if(inventory.begin(), inventory.end(),[&ItemName]
        (const std::shared_ptr<items> item) {return item->GetName() == ItemName; });
    if (it != inventory.end()) {
        inventory.erase(it);
        UpdateVirusAttraction();
    }
}

void player::UpdateVirusAttraction() {
    for (const std::shared_ptr<items> item : inventory) {
        virusattraction+= item->GetAttractionModifier();
        virusattraction-= item->GetDamageBoost()*2;
    }
    virusattraction += inventory.size()*1.5;
    virusattraction = std::min(100, virusattraction);
}

bool player::UseActionPoints(int amount) {
    if (actionpoints >= amount) {
        actionpoints -= amount;
        return true;
    }
    return false;
}

void player::RestoreActionPoints(int amount) {
    actionpoints = std::min(10, actionpoints + amount);
}

void player::UseItem(const std::string &ItemName) {
    auto it = std::find_if(inventory.begin(), inventory.end(),
        [&ItemName](const std::shared_ptr<items> item) {
            return item->GetName() == ItemName;
        });
    if (it != inventory.end()) {
        auto item = * it;
        health = std::min(maxhealth, health+item->GetHealthBoost());
        actionpoints = std::min(10, actionpoints + item->GetDamageBoost());

        if (item->isConsumable) {
            inventory.erase(it);
            UpdateVirusAttraction();
        }
    }
}

bool player::AttackVirus(std::shared_ptr<virus> targetVirus) {
    if (!UseActionPoints(1)) {
        return false;
    }
    int damage = 5;

    for (const auto& item : inventory) {
        damage += item->GetDamageBoost();
    }

    bool isCritical = CriticalHits();
    if (isCritical) {
        damage = static_cast<int>(damage * criticalmultiplier);
    }

    targetVirus->TakeDamage(damage);
    return targetVirus->IsDefeated(); // ???

}

bool player::CriticalHits() const {
    return GameUtil::RandomInt(1, 100) <= criticalrate;
}

void player::TakeDamage(int amount) {
    health = std::max(0, health - amount);
}

bool player::MoveInDungeon(int dx, int dy) {
    if (!currentDungeon) {
        return false;
    }

    return currentDungeon->MovePlayer(dx, dy);
}






