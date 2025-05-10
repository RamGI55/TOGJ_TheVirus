#include <iostream>
#include "game.h"

int main() {
   std::shared_ptr<game> const game;
   game->Initialise();
   game->Run();

   return 0;
}