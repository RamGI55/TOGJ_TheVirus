//
// Created by hunda on 2025-05-13.
//

#include "helpscreen.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace TheVirus {

    namespace {
        using namespace ftxui;
        Element helpscreen() {
            return vbox({
            text(R"(" ")"),
            text(R"(" enter <location> - Enter a location (e.g., 'enter tower') ")"),
            text(R"("  look - Look around your current location. ")"),
            text(R"("  inventory - Check your items")"),
            text(R"("  ")"),
            text(R"("  Remember - More you got, More you lost.")"),});

        }

        Element Decorate(Element inner) {
            return vbox({
            helpscreen(),
            inner,});
        }
    }

ftxui::Component HelpScreen(std::function<void()> continuation) {
        auto component = Button ("Continue", continuation, ButtonOption::Animated());
        return component | Decorate;
    }
}
