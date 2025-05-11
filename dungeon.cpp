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
Name(name), Width (width), Height (height), PlayerX(1), PlayerY(1), ExitX(Width-2), ExitY(Height-2),InfectionRate(0.1f),initialVirusCount(0){

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
                queue.push({nx, ny});
                visited[ny][nx] = true;
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
        int x = GameUtil::RandomInt(1, Width-2);
        int y = GameUtil::RandomInt(1, Height-2);

        if ((x == PlayerX && y == PlayerY) || (x == ExitX && y == ExitY)) {
            continue;
        }
        Grid[y][x].Type = DungeonCells::WALL;
    }

    // Mark the Entrence and Exit
    Grid[PlayerY][PlayerX].Type = DungeonCells::ENTRANCE;
    Grid[ExitY][ExitX].Type = DungeonCells::EXIT;

    // Place items
    for (int i = 0; i < initialItemCount; i++) {
        int x, y;
        do {
            x = GameUtil::RandomInt(1, Width-2);
            y = GameUtil::RandomInt(1, Height-2);
        } while (Grid[y][x].Type != DungeonCells::EMPTY);

        Grid[y][x].Type = DungeonCells::ITEM;
    }

    // Place viruses
    for (int i = 0; i < initialVirusCount; i++) {
        int x, y;
        do {
            x = GameUtil::RandomInt(1, Width-2);
            y = GameUtil::RandomInt(1, Height-2);
        } while (Grid[y][x].Type != DungeonCells::EMPTY);

        Grid[y][x].Type = DungeonCells::VIRUS;
        virusPositions.push_back({x, y});
    }

    CheckThePathToExit();
}

bool dungeon::MovePlayer(int dx, int dy) {
    int newX = PlayerX + dx;
    int newY = PlayerY + dy;

    if (newX < 0 || newX >= Width || newY < 0 || newY >= Height) {
        return false;
    }
    if (Grid[newY][newX].Type == DungeonCells::WALL) {
        return false;
    }

    PlayerX = newX;
    PlayerY = newY;
    return true;
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

void dungeon::MoveViruses(int playerAttractionLevel) {
    if (PlayerX < 0 || PlayerY < 0) return;

    auto virusPositionsCopy = virusPositions;

    for (auto&pos :virusPositionsCopy) {
        int vx = pos.first;
        int vy = pos.second;
        if (Grid[vy][vx].Type == DungeonCells::VIRUS || !Grid[vy][vx].VirusEntity) {
            continue;
        }
        auto virus = Grid[vy][vx].VirusEntity;

        int moveChance = virus-> GetMoveSpeed() * 10 + (playerAttractionLevel / 10);
        if (GameUtil::RandomInt (0, 100) > moveChance) {
            continue;
        }

        int dx = 0, dy = 0;

        if (playerAttractionLevel > 30 && GameUtil::RandomInt (0, 100) < playerAttractionLevel) {
            // move toward to the player
            if (PlayerX > vx) dx = 1;
            else if (PlayerX < vx) dx = -1;

            if (PlayerX > vy) dy = 1;
            else if (PlayerY > vy) dy = -1;

            if (dx != 0 && dy != 0) {
                if (GameUtil::RandomInt (0, 1) == 0) {
                    dx = 0;
                }
                else {
                    dy =0;
                }
            }
        }
        else {
            // Random Movement
            int dir = GameUtil::RandomInt (0,3);
            switch (dir) {
                case 0: dy = -1; break; // North
                case 1: dx = 1;  break; // East
                case 2: dy = 1;  break; // South
                case 3: dx = -1; break; // West
            }
        }

        // Calcuate new position
        int newX = vx + dx;
        int newY = vy + dy;

        if (newX >= 0 && newX < Width && newY >= 0 && newY < Height
            && Grid[newY][newX].Type == DungeonCells::EMPTY) {

            // Move the virus to the new position
            Grid[newY][newX].Type = DungeonCells::VIRUS;
            Grid[newY][newX].VirusEntity = virus;
            Grid[vy][vx].Type = DungeonCells::EMPTY;
            Grid[vy][vx].VirusEntity = nullptr;

            // Update virus position in our tracking
            auto it = std::find(virusPositions.begin(), virusPositions.end(), pos);
            if (it != virusPositions.end()) {
                *it = {newX, newY};
            }
        }
    }
}

void dungeon::Addvirus(int x, int y, std::shared_ptr<virus> v) {
    if (x >= 0 && x < Width && y >= 0 && y < Height && Grid[y][x].Type == DungeonCells::EMPTY) {
        Grid[y][x].Type = DungeonCells::VIRUS;
        Grid[y][x].VirusEntity = v;
        Viruses.push_back(v);

         // Update the Infectionrate
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

        Grid[y][x].Type = DungeonCells::EMPTY;
        Grid[y][x].VirusEntity = nullptr;

        auto it = std::find(Viruses.begin(), Viruses.end(), Grid[y][x].VirusEntity);
        if (it != Viruses.end()) {
            Viruses.erase(it);
        }
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
    // TODO::Totalvirus must be stroed as the parameter, has to be randomised by the location.
    float totalViruses = static_cast<float>(initialVirusCount);
    if (totalViruses <= 0) totalViruses =1.0f;

    float remainingviruses = static_cast<float>(Viruses.size());

    InfectionRate = remainingviruses / totalViruses;
}



