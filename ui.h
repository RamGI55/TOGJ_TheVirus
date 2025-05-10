//
// Created by hunda on 2025-05-09.
//

#ifndef UI_H
#define UI_H

#pragma once
#include <string>
#include <memory>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

class ui {

private:
    std::string InputBuffer;
    std::vector<std::string> MessageLog;


    ftxui::Component InputField;
    ftxui::ScreenInteractive Screen;

public:

    void Initialise();
    void AddMessage(const std::string& message);
    void Render();
    void ProcessCommand(const std::string& command);

private:
    ftxui::Element RenderLocationInfo();
    ftxui::Element RenderPlayerInfo();
    ftxui::Element RenderMessageLog();
    ftxui::Element RenderInputField();

};



#endif //UI_H
