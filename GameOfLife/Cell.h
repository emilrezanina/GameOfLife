#ifndef CELL_H
#define CELL_H

#include <atomic>

namespace GameOfLifeNS
{
	struct Cell
	{
		const int32_t X;
		const int32_t Y;
		Cell(int32_t x, int32_t y) : X(x), Y(y) {}

		bool operator==(const Cell& right) const
		{
			return compareTo(right);
		}

		bool operator!=(const Cell& right) const
		{
			return !compareTo(right);
		}

	private:
		bool compareTo(const Cell& right) const
		{
			return X == right.X && Y == right.Y;
		}
	};


	struct CellHasher
	{
		uint32_t operator()(const Cell& cell) const
		{
			int hash = 97;
			hash = ((hash + cell.X) << 5) - (hash + cell.X);
			hash = ((hash + cell.Y) << 5) - (hash + cell.Y);
			return hash;
		}
	};


}

#endif //CELL_H