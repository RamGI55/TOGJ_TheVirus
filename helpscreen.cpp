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

        Element HelpPage(Element button)
        {
            auto description =
           vbox({
            paragraphAlignCenter(R"(" ")"),
            paragraphAlignCenter(R"(" enter <location> - Enter a location (e.g., 'enter tower') ")"),
            paragraphAlignCenter(R"("  look - Look around your current location. ")"),
            paragraphAlignCenter(R"("  inventory - Check your items")"),
            paragraphAlignCenter(R"("  ")"),
            paragraphAlignCenter(R"("  Remember - More you got, More you lost.")"),
           });

            auto document=  vbox({
            description | flex_grow,
                button | center,});
        }
    }


ftxui::Component HelpMainPage(std::function<void()> continuation) {

        auto Continue = [&]
        {
            continuation();
        };

        auto ButtonContinue = ButtonOption::Animated(Color::DarkOrange);

        auto button = Container ::Horizontal({
        Button("Continue", Continue, ButtonContinue)});
        return button | HelpPage;

    }
}
