//
// Created by hunda on 2025-05-12.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace GameStateNS {
    enum class GameState {
        MENU,
        PLAYING,
        DUNGEON,
        BATTLE
    };
}

using GameStateNS::GameState;

#endif //GAMESTATE_H