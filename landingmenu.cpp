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
#include "boroughselection.h"

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

ftxui::Component LandingMenu(std::shared_ptr<game> gameInstance, std::function<void()> quit) {

 // if it has the call back function
    auto StartGame = [gameInstance, quit] {
       auto screen = ftxui::ScreenInteractive::TerminalOutput();

        boroughselection boroughselector;

        // Show the borough selection screen
        auto menu = boroughselector.BoroughSelectionMenu(
            gameInstance->GetBoroughs(),  // Replace with your game's borough data
            [&screen](const std::string& borough_id) {
                // When a borough is selected
                screen.Exit();
                // Here you would handle going to the location selection screen
            },
            [&screen]() {
                // When the back button is pressed
                screen.Exit();
            }
        );

        // Run the borough selection screen
        screen.Loop(menu);
        quit();
    };

    auto HelpGame = [&] {
        helpscreen helpscreen;
        helpscreen.HelpScreen();

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
