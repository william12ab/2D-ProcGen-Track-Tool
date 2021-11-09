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

//x=y/z y=x lol
void ShortestPath::PhaseOne(int grid_size, int* grid, int count_holder, bool found_end, int it, bool end, int x_holder, int y_holder)
{
	count_holder = 0;
	found_end = false;

	while (!found_end)
	{
		bool found_empty = false;


		for (int y = 0; y < grid_size && !found_end; y++)
		{
			for (int x = 0; x < grid_size; x++)
			{
				if (grid[(y * grid_size) + x] == it)
				{
					//check the "south" cell, 
					if (y < grid_size - 1)
					{
						int& south = grid[((y+1) * grid_size) + x];	//creating reference to the point, used for direction
						if (south == -1)
						{
							south = it + 1;			//
							found_empty = true;
						}
						else if (south == -3)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder= it;		//holds the it value at -3
							y_holder = y + 1;
							x_holder = x;
							break;

						}
					}

					//checks the "north" cell
					if (y > 0)
					{
						int& north= grid[((y - 1) * grid_size) + x];
						if (north == -1)
						{
							north = it + 1;
							found_empty = true;
						}
						else if (north == -3)
						{
							found_end= true;
							count_holder= it;
							x_holder= y - 1;
							y_holder= y;
							break;
						}
					}

					//checks the "east" wall
					if (x < grid_size - 1)
					{
						int& east = grid[(y * grid_size) + (x+1)];
						if (east == -1)
						{
							east = it + 1;
							found_empty = true;
						}
						else if (east == -3)
						{
							found_end= true;
							count_holder= it;
							x_holder= x+1;
							y_holder = y;
							break;
						}
					}

					//checks "north" wall
					if (x > 0)
					{
						int& west= grid[(y * grid_size) + (x - 1)];
						if (west == -1)
						{
							west = it + 1;
							found_empty = true;
						}
						else if (west == -3)
						{
							found_end= true;
							count_holder= it;
							x_holder= x-1;
							y_holder= y ;
							break;
						}
					}
				}
			}
		}
	}
}
void ShortestPath::PhaseTwo(int grid_size, int* grid, bool end)
{

}