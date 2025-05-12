//
// Created by hunda on 2025-05-10.
//

#include "map.h"
#include "GameUtil.h"

map::map() {
    // Initialize with default locations
    InitializeDefaultLocations();
}

void map::LoadFromJson(const std::string& boroughsFile, const std::string& locationsFile) {
    try {
        // Load boroughs
        auto boroughsJson = GameUtil::LoadJson(boroughsFile);
        boroughs.clear();

        for (const auto& boroughData : boroughsJson["boroughs"]) {
            MapBorough b(boroughData);
            boroughs.push_back(b);
        }

        // Load locations
        auto locationsJson = GameUtil::LoadJson(locationsFile);
        locations.clear();

        for (const auto& locationData : locationsJson["locations"]) {
            MapLocation loc;
            loc.id = locationData["id"];
            loc.name = locationData["name"];
            loc.description = locationData["description"];

            // Position is based on dungeonWidth and dungeonHeight as a rough guide
            // We'll map these to our visualization space
            float dungeonWidth = locationData.value("dungeonWidth", 20.0f);
            float dungeonHeight = locationData.value("dungeonHeight", 20.0f);

            // Calculate position based on some heuristic
            // This is just an example - you'd want to have actual coordinates in your JSON
            loc.x = 10.0f + (dungeonWidth / 40.0f) * 30.0f;
            loc.y = 5.0f + (dungeonHeight / 40.0f) * 15.0f;

            // Set color based on infection rate
            float infectionRate = locationData.value("baseInfectionRate", 0.3f);
            if (infectionRate < 0.3f) loc.color = Color::Green;
            else if (infectionRate < 0.5f) loc.color = Color::Yellow;
            else if (infectionRate < 0.7f) loc.color = Color::Orange1;
            else loc.color = Color::Red;

            locations.push_back(loc);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading map data: " << e.what() << std::endl;

        // Fall back to default locations if loading fails
        InitializeDefaultLocations();
    }
}

ftxui::Element map::Render() const {
    // Create ASCII art map of Toronto
    std::vector<std::string> mapLines = {
        "╔════════════════════════════════════════════════╗",
        "║                 OLD TORONTO                    ║",
        "║                                                ║",
        "║       Kensington                               ║",
        "║          •                                     ║",
        "║                    Financial                   ║",
        "║                      •       Yonge-Dundas      ║",
        "║                                •               ║",
        "║    King St                                     ║",
        "║      •             CN Tower                    ║",
        "║                       •                        ║",
        "║                                                ║",
        "║                                                ║",
        "║                   Harbourfront                 ║",
        "║                       •                        ║",
        "║                                                ║",
        "║                                                ║",
        "║~~~~~~~~~~~~~~~~ LAKE ONTARIO ~~~~~~~~~~~~~~~~~~║",
        "║                                                ║",
        "╚════════════════════════════════════════════════╝"
    };

    // Convert the map to FTXUI elements
    Elements mapElements;
    for (const auto& line : mapLines) {
        mapElements.push_back(text(line));
    }

    // Combine map with legend if needed
    if (showLegend) {
        return vbox({
            window(text("Toronto Map"), vbox(mapElements)),
            RenderLegend()
        });
    } else {
        return window(text("Toronto Map"), vbox(mapElements));
    }
}

ftxui::Element map::RenderLegend() const {
    Elements legendElements;

    legendElements.push_back(text("Locations:"));

    // Create rows of locations
    Elements row1, row2, row3;
    if (locations.size() >= 1) {
        row1.push_back(LocationDot(locations[0].name, locations[0].color, locations[0].highlighted));
    }
    if (locations.size() >= 2) {
        row1.push_back(text("  "));
        row1.push_back(LocationDot(locations[1].name, locations[1].color, locations[1].highlighted));
    }

    if (locations.size() >= 3) {
        row2.push_back(LocationDot(locations[2].name, locations[2].color, locations[2].highlighted));
    }
    if (locations.size() >= 4) {
        row2.push_back(text("  "));
        row2.push_back(LocationDot(locations[3].name, locations[3].color, locations[3].highlighted));
    }

    if (locations.size() >= 5) {
        row3.push_back(LocationDot(locations[4].name, locations[4].color, locations[4].highlighted));
    }
    if (locations.size() >= 6) {
        row3.push_back(text("  "));
        row3.push_back(LocationDot(locations[5].name, locations[5].color, locations[5].highlighted));
    }

    // Add the rows to the legend
    legendElements.push_back(hbox(row1));
    legendElements.push_back(hbox(row2));
    legendElements.push_back(hbox(row3));

    // Add infection rate meaning
    legendElements.push_back(text("Infection Rate:"));
    legendElements.push_back(
        hbox({
            text("•") | color(Color::Green), text(" Low"),
            text("  "),
            text("•") | color(Color::Yellow), text(" Medium"),
            text("  "),
            text("•") | color(Color::Red), text(" High")
        })
    );

    return window(text("Legend"), vbox(legendElements));
}

ftxui::Element map::LocationDot(const std::string& name, Color c, bool highlighted) const {
    if (highlighted) {
        return hbox({
            text("["),
            text("•") | color(c),
            text("]"),
            text(" " + name) | bold
        });
    } else {
        return hbox({
            text("•") | color(c),
            text(" " + name)
        });
    }
}

void map::HighlightLocation(const std::string& locationId) {
    for (auto& loc : locations) {
        loc.highlighted = (loc.id == locationId);
    }
    selectedLocation = locationId;
}

void map::ClearHighlights() {
    for (auto& loc : locations) {
        loc.highlighted = false;
    }
    selectedLocation.clear();
}

MapLocation* map::GetLocation(const std::string& locationId) {
    for (auto& loc : locations) {
        if (loc.id == locationId) {
            return &loc;
        }
    }
    return nullptr;
}

void map::InitializeDefaultLocations() {
    locations.clear();

    // Define default locations in case JSON loading fails
    locations.push_back({"tower", "CN Tower", "Toronto's landmark tower", 20, 12, Color::Cyan});
    locations.push_back({"harbour", "Harbourfront", "The waterfront area", 25, 15, Color::Blue});
    locations.push_back({"financial", "Financial District", "Heart of Canadian finance", 22, 10, Color::Green});
    locations.push_back({"square", "Yonge-Dundas Square", "Vibrant city center", 28, 8, Color::Yellow});
    locations.push_back({"kensington", "Kensington Market", "Cultural district", 15, 7, Color::Red});
    locations.push_back({"kingwest", "King St. West", "Entertainment district", 12, 11, Color::Magenta});
}

Color map::GetColorFromString(const std::string& colorName) const {
    if (colorName == "red") return Color::Red;
    if (colorName == "green") return Color::Green;
    if (colorName == "blue") return Color::Blue;
    if (colorName == "yellow") return Color::Yellow;
    if (colorName == "magenta") return Color::Magenta;
    if (colorName == "cyan") return Color::Cyan;
    return Color::White; // Default
}
