#include "World.h"

GameOfLifeNS::World::World(std::initializer_list<Cell> liveCells)
{
	LiveCells.insert(liveCells.begin(), liveCells.end());
}

bool GameOfLifeNS::World::IsEmpty() const
{
	return LiveCells.empty();
}