//
// Created by user on 2025-05-15.
//

#include "gamedashboard.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
//those are the very basic core objects of the ftxui; component and elements.


namespace TheVirus
{   using namespace ftxui;
    namespace
    {
        Element GameDashboardDecorator()
        {

        };



    }

    auto dashboard = vbox({
        // this is for the dashboard features.
    });


    gamedashboard::gamedashboard()
    {
        auto screen = ScreenInteractive::TerminalOutput();

        // back button for the UI
        auto backbutton_ = Button(
        "Back",
        screen.ExitLoopClosure(),
        ButtonOption::Animated(Color::DarkOrange));

        auto helpbutton_ = Button(
            "help",
            screen.ExitLoopClosure(), // need another callback?
            ButtonOption::Animated(Color::DarkOrange));

        auto optionbutton_ = Button(
            "option",
            screen.ExitLoopClosure(),
            ButtonOption::Animated(Color::DarkOrange));

        auto buttoncontainer_ = Container::Vertical({backbutton_,helpbutton_,optionbutton_});

        // auto component = buttoncontainer_ | // put other components

        // screen.Loop(component); // final screen loop for the screen.

    }
}
