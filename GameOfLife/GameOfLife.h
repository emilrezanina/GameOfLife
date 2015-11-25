#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "World.h"

#include <iterator>
#include <algorithm>
#include <numeric>

namespace GameOfLifeNS
{
	World worldEvolution(const World& oldWorld);
}

#endif //GAME_OF_LIFE_H