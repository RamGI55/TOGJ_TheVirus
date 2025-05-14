//
// Created by hunda on 2025-05-13.
//

#ifndef HELPSCREEN_H
#define HELPSCREEN_H
#include <functional>

#include "ftxui/component/component_base.hpp"


namespace TheVirus{
class helpscreen {
public: //fucker please do not forget declare public:
    ftxui::Component CreateHelpScreen(std::function<void()> continuation);

    void HelpScreen();
};
}


#endif //HELPSCREEN_H
