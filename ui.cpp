//
// Created by hunda on 2025-05-09.
//

#include "ui.h"

using namespace ftxui;
void ui::Initialise() {

    InputField = Input(&InputBuffer, "Enter the Command");

    auto EnterPressed = [this] {
        if (!InputBuffer.empty()) {
            ProcessCommand(InputBuffer);
            InputBuffer.clear();
            Render();
        }
        return true;
    };

    InputField |= CatchEvent([this, EnterPressed](const Event &event) {
        if (event == Event::Return) {
            return EnterPressed();
        }
        return false;
    });
}

void ui::AddMessage(const std::string &message) {
    MessageLog.push_back(message);
    // Erase the Message log over 20s
    if (MessageLog.size() > 20) {
        MessageLog.erase(MessageLog.begin());
    }
}

void ui::Render() {
}

void ui::ProcessCommand(const std::string &command) {

}

ftxui::Element ui::RenderLocationInfo() {
}

ftxui::Element ui::RenderPlayerInfo() {
}

ftxui::Element ui::RenderMessageLog() {
}

ftxui::Element ui::RenderInputField() {
}
