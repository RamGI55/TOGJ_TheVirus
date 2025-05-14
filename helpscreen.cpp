//
// Created by hunda on 2025-05-13.
//

#include "helpscreen.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace TheVirus {
    using namespace ftxui;
    namespace {


        Element HelpPageDecorator(Element button)
        {      auto  logo = vbox({
                paragraphAlignCenter(R"("                                 ")"),
                paragraphAlignCenter(R"("  |  |   __|   |      _ \    __| ")"),
                paragraphAlignCenter(R"("  __ |   _|    |      __/  \__ \ ")"),
                paragraphAlignCenter(R"(" _| _|  ___|  ____|  _|    ____/ ")"),
                paragraphAlignCenter(R"("                                 ")")
            });

            auto description =
           vbox({
               filler(),
               paragraphAlignCenter("In-Combat Promports "),
            filler(),
            paragraphAlignCenter(R"(" enter <location> - Enter a location (e.g., 'enter tower') ")"),
            paragraphAlignCenter(R"("  look - Look around your current location. ")"),
            paragraphAlignCenter(R"("  inventory - Check your items")"),
            paragraphAlignCenter(R"("  ")"),
            paragraphAlignCenter(R"("  Remember - More you got, More you lost.")"),
               filler(),
           });

            auto document=  vbox({
                logo|flex_shrink,
                separator(),
            description | flex_grow,
                button | center,});
            return document | border;
        }
    }

void helpscreen::HelpScreen()
{
        auto screen = ScreenInteractive::TerminalOutput();
        // Create a continue button that exits the screen
        auto continue_button = Button(
            "Continue",
            screen.ExitLoopClosure(),
            ButtonOption::Animated(Color::DarkOrange)
        );

        // Create a horizontal container for the button
        auto button_container = Container::Horizontal({
            continue_button
        });

        // Apply the decorator to the button container
        auto component = button_container | HelpPageDecorator;

        // Run the screen loop - this will block until the continue button is clicked
        screen.Loop(component);

    }
}
