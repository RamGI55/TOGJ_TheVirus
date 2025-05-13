#include <iostream>
#include "game.h"
#include "gametest.h"

int main() {
   auto game = std::make_shared<class game>();
   /*
   game->Initialise();
   game->Run();
   */
   TheVirus::StartGame();
   return 0;
}