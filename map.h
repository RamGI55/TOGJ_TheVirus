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

struct MapBorough{
    float x, y;
    std::string name;
    std::string color;
    std::string stage;

    MapBorough(const nlohmann::json& j) {
        // Use value() method with defaults for optional properties
        x = j.value("x", 20.0f);  // Default x position
        y = j.value("y", 10.0f);  // Default y position
        name = j["name"];  // Name is required - THIS IS THE PROBLEM
        color = j.value("color", "white");  // Default color
        stage = j.value("stage", "normal");  // Default stage

    }
};
struct MapLocation {
    std::string id;
    std::string name;
    std::string description;
    float x, y;
    Color color;
    bool highlighted = false;
};

class map {
private:
    std::vector<MapBorough> boroughs;
    std::vector<MapLocation> locations;
    std::string selectedLocation;
    bool showLegend = true;

public:
    map();
    ~map() = default;

    // Initialization
    void LoadFromJson(const std::string& boroughsFile, const std::string& locationsFile);

    // Rendering
    ftxui::Element Render() const;
    ftxui::Element RenderLegend() const;
    ftxui::Element LocationDot(const std::string& name, Color c, bool highlighted) const;

    // Location management
    void HighlightLocation(const std::string& locationId);
    void ClearHighlights();
    const std::vector<MapLocation>& GetLocations() const { return locations; }
    MapLocation* GetLocation(const std::string& locationId);

    // Options
    void SetShowLegend(bool show) { showLegend = show; }
    bool IsShowingLegend() const { return showLegend; }

private:
    // Helper methods
    void InitializeDefaultLocations();
    Color GetColorFromString(const std::string& colorName) const;

};



#endif //MAP_H
