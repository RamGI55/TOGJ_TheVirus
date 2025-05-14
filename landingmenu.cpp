//
// Created by hunda on 2025-05-13.
//

#include "landingmenu.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "logo.h"
#include "game.h"
#include "helpscreen.h"
#include "mainmenu.h"

namespace TheVirus{

using namespace ftxui;

    namespace{
Element IntroDecorator(Element buttons) {
    auto description = vbox({
    filler(), paragraphAlignCenter("Survival from the VIRUS"),
        paragraphAlignCenter("TO JAM Edition"),
        paragraphAlignCenter(" "),
        paragraphAlignCenter(" Press Play button to start. "),
        paragraphAlignCenter(" by RAM_G (Hyunsoo Park) "),
        filler(),
    });

    auto document = vbox({
    Logo() | flex_shrink,
    separator(),
    description | flex_grow,
    buttons | center, });
    return document | border;
    }
    }

ftxui::Component LandingMenu(std::function<void()> quit) {

 // if it has the call back function
    auto StartGame = [&] {
        mainmenu menu;
        menu.StartGame();
        quit();
    };

    auto HelpGame = [&] {
        TheVirus::helpscreen();

    };
    auto QuitGame = [&] {
        game game;
        game.Quit();
        quit();
    };

    auto ButtonStart = ButtonOption::Animated(Color::DarkOrange);
    auto ButtonHelp= ButtonOption::Animated(Color::DarkOrange);
    auto ButtonQuit = ButtonOption::Animated(Color::Red);

    auto buttons = Container ::Vertical({
        Button("Start Game", StartGame, ButtonStart),
        Button("Help", HelpGame, ButtonHelp),
        Button("Quit Game", QuitGame, ButtonQuit)});
    return buttons | IntroDecorator;
}
}
