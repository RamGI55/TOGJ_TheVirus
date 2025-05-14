//
// Created by user on 2025-05-14.
//

#ifndef BOROUGHSELECTION_H
#define BOROUGHSELECTION_H
#include <ftxui/component/component.hpp>
#include <functional>
#include <memory>
#include <string>
#include <map>
#include "borough.h"

namespace TheVirus{
class boroughselection {
public:
    ftxui::Component BoroughSelectionMenu(
        const std::map<std::string,
        std::shared_ptr<borough>>& boroughs,
        std::function<void(const std::string&)> onselectedborough,
        std::function<void()> callback
        );
    };


}
#endif //BOROUGHSELECTION_H
