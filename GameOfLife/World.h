#ifndef WORLD_H
#define WORLD_H

#include <unordered_set>

#include "Cell.h"


namespace GameOfLifeNS
{

	class World
	{
	public:
		std::unordered_set <Cell, CellHasher> LiveCells;
		World() = default;
		World(std::initializer_list<Cell> liveCells);
		bool IsEmpty() const;
	};
}

#endif //WORLD_H