//
// Created by user on 2025-05-14.
//

#include "boroughselection.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <sstream>
#include <iomanip>
#include <vector>

#include "ftxui/component/event.hpp"


namespace TheVirus
{
    using namespace ftxui;
    namespace
    {
        Element BoroughSelectDecorator (Element content)
        {return vbox({
            text("Toronto Boroughs") | bold | center,
            separator(),
            content | flex,
            text("Use arrow keys to navigate, Enter to select, ESC to go back") | center
            }) | border;
            // decorate the vbox of the ui.
        }
    }


    ftxui::Component boroughselection::BoroughSelectionMenu(
        const std::map<std::string, std::shared_ptr<borough>>& boroughs,
        std::function<void(const std::string&)> onselectedborough, std::function<void()> callback)
    {
        std::vector<std::pair<std::string, std::shared_ptr<borough>>> boroughlist;
        for (const auto& [id, borough_ptr] : boroughs) {
            boroughlist.push_back({id, borough_ptr});
        }

        // header generation
        std::vector<std::string> boroughEntries;
        std::vector<std::string> boroughIDs;


        for (const auto& [id, borough_ptr] : boroughlist)
        {   // set the infection rate by 1st,
            std::stringstream rate;
            rate << std::fixed << std::setprecision(2) << (borough_ptr->GetInfectionrate()*100) << "%";

            // add json data to the table
            boroughEntries.push_back(borough_ptr->GetName()  + " (" + rate.str() + ") ");
            boroughIDs.push_back(id);
        }

        int selected = 0;

        // create the menu for borough selection
        auto menu = Menu(&boroughEntries, &selected);

        // Event handling for the Enter Key
        menu|=CatchEvent ([&boroughIDs, &selected, onselectedborough](Event event)
        {
            if (event == Event::Return && selected >= 0 && selected <static_cast<int>(boroughIDs.size()))
            {
                onselectedborough(boroughIDs[selected]);
                return true;
            }
            return false;

        });

        // Create the back button
        auto backButton = Button ("Back", callback, ButtonOption::Animated(Color::Red));

        auto container = Container::Vertical({
        menu,
        backButton
        });

        return Renderer(container, [container, boroughlist, selected, boroughIDs]
        {
            Element content = container->Render();
            if (selected >= 0 && selected < static_cast<int>(boroughIDs.size()))
            {
                std::string id = boroughIDs[selected];
                auto it = std::find_if(boroughlist.begin(), boroughlist.end(),[&id](const auto& pair)
                { return pair.first == id; });

               if (it != boroughlist.end())
               {
                   content = vbox({
                   content,
                   separator(),
                   text("Descrption: " + it->second->GetDescription())
                   });

               }
            } return vbox({
                text("SELECT BOROUGHS") | bold | center,
                separator(),
                content,
                text("Use arrow keys to navigate, Enter to select, ESC to go back") | center
            }) | border;
             });
        }
    }