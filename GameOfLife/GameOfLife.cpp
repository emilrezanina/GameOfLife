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

		std::vector<Cell> getNeightborns(const Cell& cell)
		{
			size_t neightbornsCount = 8;
			std::vector<Cell> neightborns = std::vector<Cell>();
			neightborns.reserve(neightbornsCount);
			neightborns.push_back(ABOVE(cell));
			neightborns.push_back(ABOVE_LEFT(cell));
			neightborns.push_back(ABOVE_RIGHT(cell));
			neightborns.push_back(BELOW(cell));
			neightborns.push_back(BELOW_LEFT(cell));
			neightborns.push_back(BELOW_RIGHT(cell));
			neightborns.push_back(LEFT(cell));
			neightborns.push_back(RIGHT(cell));
			return neightborns;
		}

		inline bool checkCellOnCordinates(const World& oldWorld, Cell cell)
		{
			return std::find(oldWorld.LiveCells.begin(), oldWorld.LiveCells.end(), cell) != oldWorld.LiveCells.end();
		}

		size_t getNeightbornLevel(const World& oldWorld, const Cell& cell)
		{
			std::vector<Cell> neightborns = getNeightborns(cell);
			return std::count_if(neightborns.begin(), neightborns.end(), [&](const Cell& cell) { return checkCellOnCordinates(oldWorld, cell); });
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
				return !checkCellOnCordinates(_world, cell) && getNeightbornLevel(_world, cell) == 3;
			}
		};

		std::unordered_set<Cell, CellHasher> getNewAliveCellsFromDeadCells(const World& world)
		{
			std::unordered_set<Cell, CellHasher> newAliveCells;
			for (Cell cell : world.LiveCells)
			{
				std::vector<Cell> neightborns = getNeightborns(cell);
				std::copy_if(neightborns.begin(), neightborns.end(),
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