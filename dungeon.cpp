//
// Created by hunda on 2025-05-10.
//

#include "dungeon.h"
#include "virus.h"
#include "items.h"
#include <algorithm>
#include <queue>
#include <utility>
#include "GameUtil.h"

dungeon::dungeon(const std::string &name, int width, int height):
Name(name), Width (width), Height (height), PlayerX(1), PlayerY(1), ExitX(Width-2), ExitY(Height-2),InfectionRate(0.1f){

    Grid.resize(height, std::vector<DungeonCells>(width));

    GenerateDungeon();
}

void dungeon::CheckThePathToExit() {

    // Using BFS to check the Exit is available, start from the enterence then search the shortest path to vaild.
    std::vector<std::vector<bool>> visited(Height, std::vector<bool>(Width, false));
    std::queue<std::pair<int, int>> queue;

    queue.push({PlayerX, PlayerY});
    visited[PlayerY][PlayerX] = true;

    bool PathFound = false;

    while (!queue.empty() && !PathFound) {
        auto [x ,y] = queue.front();
        queue.pop();
        if (x == ExitX && y == ExitY) {
            PathFound = true;
            break;
        }

        const int dx [] = {-1, 0, 1, 0};
        const int dy [] = {0, -1, 0, 1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < Width && ny >= 0 && ny < Height &&
                Grid [ny][nx].Type != DungeonCells::WALL && !visited[ny][nx]){
                queue.push({nx, ny}); visited[ny][nx] = true;
            }
        }
    }

    if (!PathFound) {
        int x = PlayerX;
        int y = PlayerY;

        while (x!= ExitX) {
            x+= (x < ExitX) ? 1 : -1;
            Grid [y][x].Type = DungeonCells::EMPTY;
        }

        while (y != ExitY) {
            y+= (y < ExitY) ? 1 : -1;
            Grid [y][x].Type = DungeonCells::EMPTY;
        }
    }
}

void dungeon::GenerateDungeon() {
    for (int y = 0; y < Height ; y ++) {
        for (int x = 0; x < Width; x ++) {
            if ( x == 0 || y == 0 || x == Width-1 || y == Height - 1) {
                Grid[y][x].Type = DungeonCells::WALL;
            }
        }
    }

    for (int i = 0; i < Width*Height/ 10 ; i++) {
        int x = GameUtil::RandomIntger(1, Width-2);
        int y = GameUtil::RandomIntger(1, Height-2);

        if ((x == PlayerX && y == PlayerY) || (x == ExitX && y == ExitY)) {
            continue;
        }
        Grid[y][x].Type = DungeonCells::WALL;
    }

    // Mark the Entrence and Exit
    Grid[PlayerY][PlayerX].Type = DungeonCells::ENTRANCE;
    Grid[ExitY][ExitX].Type = DungeonCells::EXIT;

    // place the items - must change the hardcoded number 3.
    for (int i = 0; i< 3; i++) {
        int x, y;
        do {
            x = GameUtil::RandomIntger(1, Width-2);
            y = GameUtil::RandomIntger(1, Height-2);
        }
        while (Grid[y][x].Type == DungeonCells::EMPTY);
        Grid[y][x].Type = DungeonCells::ITEM;
    }

    // Place the viruses - must change the hard coded number 5
    for (int i = 0; i< 3 ; i++) {
        int x, y;
        do {
            x = GameUtil::RandomIntger(1, Width-2);
            y = GameUtil::RandomIntger(1, Height-2);
        } while (Grid[y][x].Type == DungeonCells::EMPTY);

        Grid[y][x].Type = DungeonCells::VIRUS;
        // VIRUS Entity is in here.
    }

    CheckThePathToExit();
}

bool dungeon::AtExit() const {
    return PlayerX == ExitX && PlayerY == ExitY;
}

DungeonCells & dungeon::GetCell(int x, int y) {
    return Grid [y][x];
}

const DungeonCells & dungeon::GetCell(int x, int y) const {
    return Grid [y][x];
}

void dungeon::Addvirus(int x, int y, std::shared_ptr<virus> v) {
     if (x >= 0 && x < Width && y >= 0 && y < Height) {
         Grid[y][x].Type = DungeonCells::VIRUS;
         Grid[y][x].VirusEntity = v;
         Viruses.push_back(v);
         UpdateInfectionRate();
     }
}

void dungeon::AddItem(int x, int y, std::shared_ptr<items> i) {
    if (x >= 0 && x < Width && y >= 0 && y < Height) {
        Grid[y][x].Type = DungeonCells::ITEM;
        Grid[y][x].Item = i;
        DungeonItems.push_back(i);
    }
}

void dungeon::Removevirus(int x, int y) {
    if (x >= 0 && x < Width && y >= 0 && y < Height && Grid[y][x].Type == DungeonCells::VIRUS) {
        auto it = std::find(Viruses.begin(), Viruses.end(), Grid[y][x].VirusEntity);
        if (it != Viruses.end()) {
            Viruses.erase(it);
        }

        Grid[y][x].Type = DungeonCells::EMPTY;
        Grid[y][x].VirusEntity = nullptr;

        UpdateInfectionRate();
    }
}

void dungeon::RemoveItem(int x, int y) {
    if (x >= 0 && x < Width && y >= 0 && y < Height && Grid[y][x].Type == DungeonCells::ITEM) {

        auto it = std::find(DungeonItems.begin(), DungeonItems.end(), Grid[y][x].Item);
        if (it != DungeonItems.end()) {
            DungeonItems.erase(it);
        }
        Grid[y][x].Type = DungeonCells::EMPTY;
        Grid[y][x].Item = nullptr;

    }
}

void dungeon::UpdateInfectionRate() {

    // TotalVirus / RemainViruses
    // Totalvirus must be stroed as the parameter, has to be randomised by the location.
}



