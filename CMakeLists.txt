cmake_minimum_required(VERSION 3.31)
project(TOGJ_TheVirus VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies of the project

# Add FetchContent for downloading dependencies
include(FetchContent)

#FTXUI
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# JSON parsing nlohmann/json
FetchContent_Declare(
        json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

#Add Source Files
set(SOURCES
)
add_executable(TOGJ_TheVirus main.cpp
        game.cpp
        game.h
        items.cpp
        items.h
        borough.cpp
        borough.h
        player.cpp
        player.h
        ui.cpp
        ui.h
        GameUtil.cpp
        GameUtil.h
        virus.cpp
        virus.h
        map.cpp
        map.h
        locations.cpp
        locations.h
        dungeon.cpp
        dungeon.h)

# Link libraries
target_link_libraries(TOGJ_TheVirus
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE nlohmann_json::nlohmann_json
)
#json file implant
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})