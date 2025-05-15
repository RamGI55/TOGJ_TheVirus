//
// Created by user on 2025-05-15.
//

#ifndef GAMEDASHBOARD_H
#define GAMEDASHBOARD_H

#include "ftxui/component/component_base.hpp"

// shows the list of the borough
// when select the boroughs -> go to the location list, which already made

//  ---------------------------------------------
//  |                   |                       |
//  |                   |         map           |
//  |       LIST        |      SIMPLYFIED       |
//  |        of         | --------------------- |
//  |      BOROUGH      |                       |
//  |                   |  INFECTION RATE GRAPH |
//  |                   |    BY BOROUGH/TOTAL   |
//  |-------------------------------------------|
//  | > command input                           |
//  | button (Continue/Exit)                    |
//  ---------------------------------------------

namespace TheVirus {
class gamedashboard {

    ftxui::Component CreateDashboard(std::function<void()> continuation);

    gamedashboard();

};

}
#endif //GAMEDASHBOARD_H
