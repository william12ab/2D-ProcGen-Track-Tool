#include "ShortestPath.h"
ShortestPath::ShortestPath()
{

}

//i =z/y, j=x
//if a position isnt = to zero or 700 then set it to -2 which signals a blocked path
//else set to -1 which is a possible path
void ShortestPath::Initgrid(int grid_size, int* grid)
{
	
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] != 0 && grid[(i * grid_size) + j] != 700 )
			{
				grid[(i * grid_size) + j] = -2;
			}
			else if (grid[(i * grid_size) + j])
			{
				grid[(i * grid_size) + j] = -1;
			}
		}
	}
}
void ShortestPath::PhaseOne(int grid_size, int* grid, int count_holder, bool found_end, int it, bool end)
{

}
void ShortestPath::PhaseTwo(int grid_size, int* grid, bool end)
{

}