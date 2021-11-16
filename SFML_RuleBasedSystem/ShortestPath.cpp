#include "ShortestPath.h"
#include <iostream>
ShortestPath::ShortestPath()
{

}

//i =z/y, j=x
//if a position isnt = to zero or 700 then set it to -2 which signals a blocked path
//else set to -1 which is a possible path
void ShortestPath::Initgrid(int grid_size, int* grid, int num_points)
{
	int start = -3;
	
		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (grid[(i * grid_size) + j]==0)
				{
					grid[(i * grid_size) + j] = -1;
				}
				if (grid[(i * grid_size) + j] == 2000)
				{
					grid[(i * grid_size) + j] = 0;
				}
				for (int p = 0; p < (num_points-1); p++)
				{
				if(grid[(i * grid_size) + j] == 2001 + p)
				{
					grid[(i * grid_size) + j] = start - p;
				}
				}
				if(grid[(i * grid_size) + j]>0 )
				{
					grid[(i * grid_size) + j] = -2;
				}
			}
		}
}

/*

	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] != 0 && grid[(i * grid_size) + j] != 2000 && grid[(i * grid_size) + j] != 2001 && grid[(i * grid_size) + j] != 2002)
			{
				grid[(i * grid_size) + j] = -2;
			}
			else if (grid[(i * grid_size) + j]==0)
			{
				grid[(i * grid_size) + j] = -1;
			}
			else if (grid[(i * grid_size) + j] == 2000)
			{
				grid[(i * grid_size) + j] = 0;
			}
			else if (grid[(i * grid_size) + j] == 2001)
			{
				grid[(i * grid_size) + j] = -3;
			}
			else if (grid[(i * grid_size) + j] == 2002)
			{
				grid[(i * grid_size) + j] = -4;
			}
		}
	}
	*/
//}

//x=y/z y=x lol
void ShortestPath::PhaseOne(int grid_size, int* grid, int count_holder, bool found_end, int it, bool end, int x_holder, int y_holder, int end_n)
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
							grid[((y + 1) * grid_size) + x] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_= it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x;
							break;

						}
					}

					//checks the soth east cell
					if (y < grid_size - 1 && x < grid_size - 1)
					{
						int& south = grid[((y + 1) * grid_size) + (x+1)];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + (x+1)] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_ = it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x+1;
							break;

						}
					}

					//checks the south west
					if (y < grid_size - 1 && x > 0)
					{
						int& south = grid[((y + 1) * grid_size) + (x - 1)];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + (x - 1)] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_ = it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x - 1;
							break;

						}
					}

					//checks the "north" cell
					if (y > 0)
					{
						int& north= grid[((y - 1) * grid_size) + x];
						if (north == -1)
						{
							grid[((y - 1) * grid_size) + x] = it + 1;
							//north = it + 1;
							found_empty = true;
						}
						else if (north == end_n)
						{
							found_end= true;
							count_holder_ = it;
							x_holder_ = x;
							y_holder_= y-1;
							break;
						}
					}

					//checks the nortth east cell
					if (y > 0 && x < grid_size - 1)
					{
						int& north = grid[((y - 1) * grid_size) + (x+1)];
						if (north == -1)
						{
							grid[((y - 1) * grid_size) + (x+1)] = it + 1;
							//north = it + 1;
							found_empty = true;
						}
						else if (north == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x + 1;
							y_holder_ = y-1;
							break;
						}
					}
					//checks the nortth west cell
					if (y > 0 && x > 0)
					{
						int& north = grid[((y - 1) * grid_size) + (x - 1)];
						if (north == -1)
						{
							grid[((y - 1) * grid_size) + (x - 1)] = it + 1;
							//north = it + 1;
							found_empty = true;
						}
						else if (north == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x - 1;
							y_holder_ = y - 1;
							break;
						}
					}

					//checks the "east" wall
					if (x < grid_size - 1)
					{
						int& east = grid[(y * grid_size) + (x+1)];
						if (east == -1)
						{
							grid[(y * grid_size) + (x + 1)] = it + 1;
							//east = it + 1;
							found_empty = true;
						}
						else if (east == end_n)
						{
							found_end= true;
							count_holder_ = it;
							x_holder_= x+1;
							y_holder_ = y;
							break;
						}
					}

					//checks "west" wall
					if (x > 0)
					{
						int& west= grid[(y * grid_size) + (x - 1)];
						if (west == -1)
						{
							grid[(y * grid_size) + (x - 1)] = it + 1;
							//west = it + 1;
							found_empty = true;
						}
						else if (west == end_n)
						{
							found_end= true;
							count_holder_ = it;
							x_holder_= x-1;
							y_holder_= y ;
							break;
						}
					}
				}
			}
		}


		//need to do something with this instead of printing out a message that is not practical or good.
		if (!found_empty && !found_end)
		{
			end_ = true;
			std::cout << "There is no solution to this maze.";
			std::cout << std::endl;
			break;
		}

		it++;
	}
}


void ShortestPath::ChangePoint(int grid_size, int* grid, int point, int new_point)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] == point)
			{
				grid[(i * grid_size) + j] = new_point;
			}
		}
	}
}

void ShortestPath::PrintOutStartEnd(int grid_size, int* grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] == -3)
			{
				int s = 2;
			}
			if (grid[(i * grid_size) + j] == -4)
			{
				int s = 2;
			}
		}
	}
}

//from phase one you use the x and y holder vars and count holder and end
void ShortestPath::PhaseTwo(int grid_size, int* grid, bool end, int x_holder, int y_holder, int count_holder, int end_n)
{
	bool found_start = false;

	//not needed becasues now we get it from the other function so remove this when you have a chance
	////i =y/z, j=x
	//for (int i = 0; i < grid_size; i++)
	//{
	//	for (int j = 0; j < grid_size; j++)
	//	{
	//		if (grid[(i * grid_size) + j] == -3)
	//		{
	//			x_holder = j;
	//			y_holder = i;
	//			break;											//?
	//		}
	//	}
	//}

	while (!found_start && !end)
	{

		if (count_holder != 0)
		{
			int& north = grid[((y_holder - 1) * grid_size) + x_holder];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (north == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder -= 1;
				//positionx.push_back(xHolder);
				//positiony.push_back(yHolder);
				count_holder -= 1;
				grid[(y_holder * grid_size) + x_holder]=-12303;
			}
			int& northE = grid[((y_holder - 1) * grid_size) + (x_holder + 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (northE == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder -= 1;
				x_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;

				//	break;
			}
			int& northW = grid[((y_holder - 1) * grid_size) + (x_holder - 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (northW == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder -= 1;
				x_holder -= 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;

				//	break;
			}

			int& west = grid[(y_holder * grid_size) + (x_holder - 1)];	//west
			if (west == count_holder)
			{
				x_holder -= 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				//		break;

			}

			int& east = grid[(y_holder * grid_size) + (x_holder + 1)];		//east 
			if (east == count_holder)
			{
				x_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				//			break;

			}

			int& south = grid[((y_holder + 1) * grid_size) + x_holder];
			if (south == count_holder)
			{
				y_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;

			}
			int& southE = grid[((y_holder + 1) * grid_size) + (x_holder + 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (southE == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				x_holder += 1;
				y_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;

				//	break;
			}
			int& southW = grid[((y_holder + 1) * grid_size) + (x_holder - 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (southW == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder += 1;
				x_holder -= 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;

				//	break;
			}

		/*	if (count_holder!=north&& count_holder != northE && count_holder != northW && count_holder != south && count_holder != southE && count_holder != southW)
			{
				break;
			}*/
		}
		
		if (count_holder<=end_n)
		{
			found_start = true;
			end = true;
		}
	}
}