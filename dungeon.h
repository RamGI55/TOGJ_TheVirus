//
// Created by hunda on 2025-05-10.
//

#ifndef DUNGEON_H
#define DUNGEON_H

#include <string>
#include <vector>
#include <memory>



class virus;
class items;
struct DungeonCells {
    enum Celltypes {
        EMPTY,
        WALL,
        ENTRANCE,
        EXIT,
        ITEM,
        VIRUS
    };

    Celltypes Type;
    std::shared_ptr<items> Item;
    std::shared_ptr<virus> VirusEntity;
};
class dungeon {
private:
    std::string Name;
    int Width;
    int Height;
    std::vector<std::vector<DungeonCells>> Grid;
    int PlayerX, PlayerY;
    int ExitX, ExitY;
    float InfectionRate; // this must be same as the location's infection rate. dungeon == location.
    int initialVirusCount = 5;// Default values
    int initialItemCount = 3;

    std::vector<std::shared_ptr<items>> DungeonItems;
    std::vector<std::shared_ptr<virus>> Viruses;
    std::vector<std::pair<int, int>> virusPositions;
public:
    dungeon (const std::string& name, int width = 25, int height =25);


    void CheckThePathToExit();

    void GenerateDungeon();
    bool MovePlayer(int dx, int dy);
    bool AtExit() const;

    // accessors
    const std::string& GetName() const {return Name;}
    int GetWidth() const {return Width;}
    int GetHeight() const {return Height;}
    float GetInfectionRate() const {return InfectionRate;}


    DungeonCells& GetCell(int x, int y);
    const DungeonCells& GetCell(int x, int y) const;


    void MoveViruses(int playerAttractionLevel);
    void Addvirus(int x, int y, std::shared_ptr<virus> v);
    void AddItem(int x, int y, std::shared_ptr<items> item);
    void Removevirus (int x, int y);
    void RemoveItem(int x, int y);
    std::vector<std::pair<int, int>> GetVirusPositions() const {return virusPositions; }

    std::pair<int, int> GetPlayerPosition() const {return std::make_pair(PlayerX, PlayerY);}

    void UpdateInfectionRate();

    void SetInitialCounts(int viruses, int items) {
        initialVirusCount = viruses;
        initialItemCount = items;
    }
private:

public:


};



#endif //DUNGEON_H
