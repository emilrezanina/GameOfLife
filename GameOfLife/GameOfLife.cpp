#include "GameOfLife.h"

namespace GameOfLifeNS
{
	namespace
	{
#define ABOVE(cell) Cell(cell.X, cell.Y - 1)
#define ABOVE_LEFT(cell) Cell(cell.X - 1, cell.Y - 1)
#define ABOVE_RIGHT(cell) Cell(cell.X + 1, cell.Y - 1)
#define BELOW(cell) Cell(cell.X, cell.Y + 1)
#define BELOW_LEFT(cell) Cell(cell.X - 1, cell.Y + 1)
#define BELOW_RIGHT(cell) Cell(cell.X + 1, cell.Y + 1)
#define LEFT(cell) Cell(cell.X - 1, cell.Y)
#define RIGHT(cell) Cell(cell.X + 1, cell.Y)

		std::vector<Cell> getNeighbor(const Cell& cell)
		{
			size_t NeighborCount = 8;
			std::vector<Cell> Neighbor = std::vector<Cell>();
			Neighbor.reserve(NeighborCount);
			Neighbor.push_back(ABOVE(cell));
			Neighbor.push_back(ABOVE_LEFT(cell));
			Neighbor.push_back(ABOVE_RIGHT(cell));
			Neighbor.push_back(BELOW(cell));
			Neighbor.push_back(BELOW_LEFT(cell));
			Neighbor.push_back(BELOW_RIGHT(cell));
			Neighbor.push_back(LEFT(cell));
			Neighbor.push_back(RIGHT(cell));
			return Neighbor;
		}

		inline bool checkCellOnCoordinates(const World& oldWorld, Cell cell)
		{
			return std::find(oldWorld.LiveCells.begin(), oldWorld.LiveCells.end(), cell) != oldWorld.LiveCells.end();
		}

		size_t getNeightbornLevel(const World& oldWorld, const Cell& cell)
		{
			std::vector<Cell> Neighbor = getNeighbor(cell);
			return std::count_if(Neighbor.begin(), Neighbor.end(), [&](const Cell& cell) { return checkCellOnCoordinates(oldWorld, cell); });
		}

		void copyAliveCellsByRules(const World& oldWorld, World& newWorld)
		{
			std::copy_if(oldWorld.LiveCells.begin(), oldWorld.LiveCells.end(), std::inserter(newWorld.LiveCells, newWorld.LiveCells.begin()), [&](const Cell& cell)
			{
				size_t neightbornLevel = getNeightbornLevel(oldWorld, cell);
				return neightbornLevel == 2 || neightbornLevel == 3;
			});
		}

		class ReviveZombiePredicate
		{
			const World& _world;
		public:
			ReviveZombiePredicate(const World& world) : _world(world) {}
			bool operator()(const Cell& cell) const
			{
				return !checkCellOnCoordinates(_world, cell) && getNeightbornLevel(_world, cell) == 3;
			}
		};

		std::unordered_set<Cell, CellHasher> getNewAliveCellsFromDeadCells(const World& world)
		{
			std::unordered_set<Cell, CellHasher> newAliveCells;
			for (Cell cell : world.LiveCells)
			{
				std::vector<Cell> Neighbor = getNeighbor(cell);
				std::copy_if(Neighbor.begin(), Neighbor.end(),
					std::inserter<std::unordered_set<Cell, CellHasher> >(newAliveCells, newAliveCells.begin()), ReviveZombiePredicate(world));
			}
			return newAliveCells;
		}

		void copyNewAliveCells(const World& oldWord, World& newWorld)
		{
			auto newAliveCells = getNewAliveCellsFromDeadCells(oldWord);
			std::copy(newAliveCells.begin(), newAliveCells.end(), std::inserter(newWorld.LiveCells, newWorld.LiveCells.begin()));
		}
	}

	World worldEvolution(const World& oldWorld)
	{
		World newWorld;
		copyAliveCellsByRules(oldWorld, newWorld);
		copyNewAliveCells(oldWorld, newWorld);
		return newWorld;
	}
}