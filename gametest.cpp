//
// Created by hunda on 2025-05-13.
//

#include "gametest.h"


namespace TheVirus {
    using namespace ftxui;

    namespace {
        void ExecuteIntro() {
            auto screen = ScreenInteractive::Fullscreen();
            auto component = LandingMenu(screen.ExitLoopClosure());
            screen.Loop(component);
        }
    }

    void StartGame() {
        ExecuteIntro();
    }

}