#include <iostream>
#include "game.h"

int main() {
   auto game = std::make_shared<class game>();
   game->Initialise();
   game->Run();

   return 0;
}