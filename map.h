//
// Created by hunda on 2025-05-10.
//

#ifndef MAP_H
#define MAP_H
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component_options.hpp>
#include <vector>
#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

using namespace ftxui;
struct borough{
    float x, y;
    std::string name;
    std::string color;
    std::string stage;

    borough(const nlohmann::json& j) {
        x = j["x"];
        y = j["y"];
        name = j["name"];
        color = j["color"];

    }
};


class map {


};



#endif //MAP_H
