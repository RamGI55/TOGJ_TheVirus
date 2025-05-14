//
// Created by hunda on 2025-05-13.
//

#include "gametest.h"

#include "game.h"


namespace TheVirus {
    using namespace ftxui;

    namespace {
        void ExecuteIntro(std::shared_ptr<game> gameinstance) {
            auto screen = ScreenInteractive::Fullscreen();
            auto component = LandingMenu(gameinstance, screen.ExitLoopClosure());
            screen.Loop(component);
        }
    }

    void StartGame() {
        auto gameInstance_ = std::make_shared<game>();
        gameInstance_->Initialise();
        ExecuteIntro(gameInstance_);


        }
    }

