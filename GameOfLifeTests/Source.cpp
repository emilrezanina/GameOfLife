#include "gtest/gtest.h"

#include "World.h"
#include "Cell.h"
#include "GameOfLife.h"

using namespace GameOfLifeNS;

TEST(WorldCreation, NewWorldWithoutSeedIsEmpty)
{
	auto world = World();
	ASSERT_TRUE(world.IsEmpty());
}

TEST(WorldCreation, NewWorldWithSeedIsntEmpty)
{
	World world = { Cell(0, 0), Cell(1, 0) };
	ASSERT_FALSE(world.IsEmpty());
}

TEST(WorldTick, WorldWithOneLiveCellWillBeEmptyAfterTick)
{
	World oldWorld = { Cell(0, 0) };
	World newWorld = worldEvolution(oldWorld);
	ASSERT_TRUE(newWorld.IsEmpty());
}

TEST(WorldTick, AliveCellWithTwoAliveNeightbornWillBeAliveAfterTick)
{
	Cell testedCell = GameOfLifeNS::Cell(0, 0);
	World oldWorld = { testedCell, Cell(0, 1), Cell(1, 0) };
	World newWorld = worldEvolution(oldWorld);
	ASSERT_TRUE(std::find(newWorld.LiveCells.begin(), newWorld.LiveCells.end(), testedCell) != newWorld.LiveCells.end());
}

TEST(WorldTick, AliveCellWithLessThanTwoAliveNeighbornWillBeDeadAfterTick)
{
	Cell testedCell = Cell(0, 0);
	World oldWorld = { testedCell, Cell(0, 1), Cell(0, 2) };
	World newWorld = worldEvolution(oldWorld);
	ASSERT_FALSE(std::find(newWorld.LiveCells.begin(), newWorld.LiveCells.end(), testedCell) != newWorld.LiveCells.end());
}

TEST(WorldTick, AliveCellsWithMoreThanThreeAliveNeighbornWillBeDeadAfterTick)
{
	Cell testedCell = Cell(0, 0);
	World oldWorld = { testedCell, Cell(0, 1), Cell(1, 0), Cell(1, 1), Cell(-1, 0) };
	World newWorld = worldEvolution(oldWorld);
	ASSERT_FALSE(std::find(newWorld.LiveCells.begin(), newWorld.LiveCells.end(), testedCell) != newWorld.LiveCells.end());
}

TEST(WorldTick, DeadCellsWithThreeAliveNeighbornWillBeAliveAfterTick)
{
	Cell testedCell = Cell(0, 0);
	World oldWorld = { Cell(0, 1), Cell(1, 0), Cell(1, 1) };
	World newWorld = worldEvolution(oldWorld);
	ASSERT_TRUE(std::find(newWorld.LiveCells.begin(), newWorld.LiveCells.end(), testedCell) != newWorld.LiveCells.end());
}

TEST(WorldTick, DeadCellsWhichCanBeRevivedMultipleTimesInOneTickAreOnlyOnceInNewAliveCells)
{
	std::unordered_set <Cell, CellHasher> expectedLiveCells = { Cell(1, 0), Cell(1, 1) };
	World oldWorld = { Cell(0, 1), Cell(2, 0), Cell(2, 1) };
	World newWorld = worldEvolution(oldWorld);
	ASSERT_EQ(expectedLiveCells, newWorld.LiveCells);
}