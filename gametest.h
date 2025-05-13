//
// Created by hunda on 2025-05-13.
//

#ifndef GAMETEST_H
#define GAMETEST_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>

namespace TheVirus {
    void StartGame();

    ftxui::Component LandingMenu(std::function <void()> quit);


}
#endif //GAMETEST_H
