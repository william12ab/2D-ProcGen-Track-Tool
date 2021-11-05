// SFML includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <list>
// The defines that state the size of the grid and the speed of the game.
#define GRID_SIZE_X 800
#define GRID_SIZE_Y 800
#define SQUARE_SIZE 10
#define FRAMES_PER_SECOND 5
#define MOVE_CHANCE 5
#define SITES 25



#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using the_clock = std::chrono::steady_clock;

// A GLOBAL!!! 
bool grid[GRID_SIZE_Y][GRID_SIZE_X] = { false };			//used to draw - check if used
UINT32 tempGrid[GRID_SIZE_Y][GRID_SIZE_X] = { 0 };			//used to store the colours 0= edge

int gridArray[GRID_SIZE_X][GRID_SIZE_Y] = { 0 };

int gridDistance[GRID_SIZE_X][GRID_SIZE_Y] = { 0 };


int xp[SITES];
int yp[SITES];

int amount_squares = 0;
/* Rule 1
 * Random Movement
 * Goes through every square that can move and randomly decides if it can
 */
int xHolder = 0;
int yHolder = 0;
int countHolder = 0;
bool foundEnd = false;
int it;
bool end = false;


std::vector<DWORD> colors_;

int numberofPos = 0;
int numberofPos1 = 0;
int numberofPos2 = 0;


std::list<int>positions;
std::list<int> positionx;
std::list<int> positiony;


void RandomPlace()
{
	for (int i = 0; i < SITES; i++)
	{
		xp[i] = rand() % GRID_SIZE_X;
		yp[i] = rand() % GRID_SIZE_Y;

		grid[xp[i]][yp[i]]=true;
	}
}

void Place()
{



	
	int place = GRID_SIZE_X / SITES;
	xp[0] = 200, yp[0] = 200;
	xp[1] = 400, yp[1] = 200;
	xp[2] = 600, yp[2] = 200;
	xp[3] = 800, yp[3] = 200;
	xp[4] = 200, yp[4] = 400;
	xp[5] = 400, yp[5] = 400;
	xp[6] = 600, yp[6] = 400;
	xp[7] = 800, yp[7] = 400;
	xp[8] = 200, yp[8] = 600;
	xp[9] = 400, yp[9] = 600;
	xp[10] = 600, yp[10] = 600;
	xp[11] = 800, yp[11] = 600;
	xp[12] = 200, yp[12] = 800;
	xp[13] = 400, yp[13] = 800;
	xp[14] = 600, yp[14] = 800;
	xp[15] = 800, yp[15] = 800;
	for (int i = 0; i < SITES; i++)
	{
		
		//xp[i] = place;
		//yp[i] = place/2;

		grid[xp[i]][yp[i]] = true;
		place+= GRID_SIZE_X / SITES;

	}

}
int distancesqrt(int x, int y, int x2, int y2)
{
	int xd = x2 - x;
	int yd = y2 - y;
	return (xd * xd) + (yd * yd);
}



int getMin(int arr[], int n)
{
	int res = arr[0];
	int elementPos = 0;
	for (int i = 1; i < n; i++)
	{

		if (arr[i] < res)
		{
			res = arr[i];
			elementPos = i;
		}
	}
	return elementPos;
}

void Zones()
{
	int d1=0;
	int incr[SITES] = { 0 };
	for (int i = 0; i < SITES; i++)
	{
		incr[i] = i;
	}
	incr[0] = 101;

	for (int j = 0; j < (GRID_SIZE_Y); j++)
	{
		for (int i = 0; i < (GRID_SIZE_X); i++)
		{
			int ind = -1, dist = INT_MAX;
			for (int p = 0; p < SITES; p++)
			{
				//d[p] = distancesqrt(xp[p], yp[p], j, i);
				d1 = distancesqrt(xp[p], yp[p], j, i);
			
				//if the distance is less than the nmax distance, so if this point is in the bounds for this site
				//change the max distance so it wont change the points site.
				if (d1<dist)
				{
					dist = d1;
					ind = p;
				}
			}
			//for each point, see if the ind value is not -1, which means that it is coloured aadn within a sites region.
			if (ind > -1)
			{
				//tempGrid[j][i] = c[ind];
				tempGrid[j][i] = incr[ind];
			}
		

		}
	}

}
void CreateColors() {
	for (size_t i = 0; i < (GRID_SIZE_X* GRID_SIZE_X); i++) {
		DWORD c = RGB(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50);
		colors_.push_back(c);
	}
}

void Borders()
{
	for (int j = 0; j < (GRID_SIZE_Y); j++)
	{
		for (int i = 0; i < (GRID_SIZE_X); i++)
		{
			if (tempGrid[j][i]!= 0 && tempGrid[j][i+1] != 0)
			{
				if (tempGrid[j][i] != tempGrid[j][i+1])
				{
					tempGrid[j][i] = 0;	
				}
			}
			if (tempGrid[j][i] != 0 && tempGrid[j+1][i ] != 0)
			{
				if (tempGrid[j][i] != tempGrid[j+1][i])
				{
					tempGrid[j][i] = 0;
				}
			}
		}

	}
}

void OutsideEdges()
{
	int y = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (GRID_SIZE_Y); j++)
		{
			tempGrid[y][j] = 90;
		}
		y = GRID_SIZE_Y-1;
	}
	int x = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (GRID_SIZE_Y); j++)
		{
			tempGrid[j][x] = 90;
		}
		x = GRID_SIZE_Y-1;
	}
}

void Drawcircle( int x0, int y0, int radius)
{
	static int x = radius;
	x = radius;
	static int y = 0;
	y = 0;
	int err = 0;

	while (x >= y)
	{
		if (x0+x<0)
		{
			//you got this
			int s = 1;
		}
		if (x0 - x < 0)
		{
			//you got this
			int s = 1;
		}
		tempGrid[x0 + x][y0 + y] = 0;		
		tempGrid[x0 + y][y0 + x] = 0;		
		tempGrid[x0 - y][y0 + x] = 0;	
		tempGrid[x0  -x][y0 + y] = 0;	
		tempGrid[x0 -x ][y0 - y] = 0;	
		tempGrid[x0 - y][y0 - x] = 0;
		tempGrid[x0 + y][y0 - x] = 0;
		tempGrid[x0 + x][y0 -y ] = 0;
		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void RemoveColour()
{
	for (int j = 0; j < (GRID_SIZE_Y); j++)
	{
		for (int i = 0; i < (GRID_SIZE_X); i++)
		{
			if (tempGrid[j][i]!=0)
			{
				tempGrid[j][i] = 0xFFFFFFFF;
			}
		}
	}
}


void findZeros()
{
	for (int j = 0; j < GRID_SIZE_X; j++)
	{
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			if(tempGrid[j][i]==0)
			{
				numberofPos += 1;
			}
		}
	}
}

void setPositions(std::vector<sf::RectangleShape> &shape, int points)
{
	shape.resize(points);
	for (int i = 0; i < shape.size(); i++)
	{
		shape.at(i).setSize(sf::Vector2f(1, 1));
		shape.at(i).setFillColor(sf::Color::Green);

	
	}
	int p = 0;
	for (int j = 0; j < GRID_SIZE_X; j++)
	{
		for (int y = 0; y < GRID_SIZE_X; y++)
		{
			if (tempGrid[j][y] == 0)
			{
				shape.at(p).setPosition(j, y);
				p++;
			}
		}
	}
	//int p = 1;

}

void findPointOnEdge(sf::RectangleShape &shape)
{
	//loop until a point is found
	//choose a random point, so in tempgrid for x and y within grid_size
	//check if its value is 0
	//if 0 exit the loop
	//display this point
	shape.setSize(sf::Vector2f(5, 5));
	bool found = false;
	while (found==false)
	{
		//range of 20-80
		int x = rand() % 20 + 80;

		//50-750
		int y = rand() %700+50;

		if (tempGrid[x][y]==0)
		{
			found = true;
			tempGrid[x][y] = 700;
			gridArray[x][y] = 0;
			shape.setPosition(x, y);
			shape.setFillColor(sf::Color::Red);
		}
	}
}

void findPointOnRight(sf::RectangleShape& shape)
{
	//loop until a point is found
	//choose a random point, so in tempgrid for x and y within grid_size
	//check if its value is 0
	//if 0 exit the loop
	//display this point
	shape.setSize(sf::Vector2f(5, 5));
	bool found = false;
	while (found == false)
	{
		//700-780
		int x = rand()%80 + 700;

		//50-750
		int y = rand()%700 + 50;

		

		if (tempGrid[x][y] == 0)
		{
			found = true;
			tempGrid[x][y] = 701;
			gridArray[x][y] = -3;
			shape.setPosition(x, y);
			shape.setFillColor(sf::Color::Red);
		}
	}
}

void initGrid()
{
	
	for (int i = 0; i < GRID_SIZE_X; i++)
	{
		for (int j = 0; j < GRID_SIZE_Y; j++)
		{
			if (tempGrid[i][j]!=0 && tempGrid[i][j]!=700 && tempGrid[i][j]!=701)
			{
				gridArray[i][j] = -2;
			}
			else if (tempGrid[i][j]==0)
			{
				gridArray[i][j] = -1;
			}
			/*
			else if (tempGrid[i][j] == 700)
			{
				gridArray[i][j] = 0;
			}
			else if (tempGrid[i][j] == 701)
			{
				gridArray[i][j] = -3; 

			}
			*/
		}
	}

	
}

void fillGrid()
{

	srand(time(NULL));
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			if (gridArray[x][y] == -1)
			{
				gridArray[x][y] = rand() % 14 + 1;			//random number between 1 and 14 to stop it being 0 and interfering with the start pos
			}
		
		}
	}


}


void firstPhase()
{
	
	//setting the vars needed for the function. xHolder and yHolder, house the position currently being worked out. CountHolder holds the route that is being worked through, the current number that is being checked against in the statements.

	int p = 0;


	countHolder = 0;
	foundEnd = false;
	for (int i = 0; i < GRID_SIZE_X; i++)
	{
		for (int j = 0; j < GRID_SIZE_X; j++)
		{
			if (gridArray[i][j]==-3)
			{
				p += 1;
			}
		}
	}

	while (!foundEnd)			//loop whilst the end has not been found.
	{
		bool foundEmpty = false;			//set the bool to false that houses if there is an empty slot.


		for (int x = 0; x < GRID_SIZE_X && !foundEnd; x++)
		{
			for (int y = 0; y < GRID_SIZE_X; y++)
			{
				//this is so that its the current number being found to check, so zero or one. then check up,down,left,right from it.
				if (gridArray[x][y] == it)
				{

					//check the "east" cell
					if (x < GRID_SIZE_X - 1)
					{
						int& east = gridArray[x + 1][y];	//creating reference to the point, used for direction
						if (east == -1)
						{
							east = it + 1;			//
							foundEmpty = true;
						}
						else if (east == -3)
						{
							foundEnd = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							countHolder = it;		//holds the it value at -3
							xHolder = x + 1;
							yHolder = y;
							break;

						}
					}

					//checks the "west" cell
					if (x > 0)
					{
						int& west = gridArray[x - 1][y];
						if (west == -1)
						{
							west = it + 1;
							foundEmpty = true;
						}
						else if (west == -3)
						{
							foundEnd = true;
							countHolder = it;
							xHolder = x - 1;
							yHolder = y;
							break;
						}
					}

					//checks the "south" wall
					if (y < GRID_SIZE_X - 1)
					{
						int& south = gridArray[x][y + 1];
						if (south == -1)
						{
							south = it + 1;
							foundEmpty = true;
						}
						else if (south == -3)
						{
							foundEnd = true;
							countHolder = it;
							xHolder = x;
							yHolder = y + 1;
							break;
						}
					}

					//checks "north" wall
					if (y > 0)
					{
						int& north = gridArray[x][y - 1];
						if (north == -1)
						{
							north = it + 1;
							foundEmpty = true;
						}
						else if (north == -3)
						{
							foundEnd = true;
							countHolder = it;
							xHolder = x;
							yHolder = y - 1;
							break;
						}
					}

					//north east
					if ((x > 0) && (y <GRID_SIZE_X-1))
					{
						int& ne = gridArray[x - 1][y + 1];
						if (ne==-1)
						{
							ne = it + 1;
							foundEmpty = true;
						}
						else if (ne==-3)
						{
							foundEnd = true;
							countHolder = it;
							break;
						}
					}

					//north west
					if ((x > 0) && (y > 0))
					{
						int& nw = gridArray[x - 1][y - 1];
						if (nw == -1)
						{
							nw = it + 1;
							foundEmpty = true;
						}
						else if (nw == -3)
						{
							foundEnd = true;
							countHolder = it;
							break;
						}
					}

					//south east
					if ((x <GRID_SIZE_X-1 ) && (y < GRID_SIZE_X - 1))
					{
						int& se = gridArray[x + 1][y + 1];
						if (se == -1)
						{
							se = it + 1;
							foundEmpty = true;
						}
						else if (se == -3)
						{
							foundEnd = true;
							countHolder = it;
							break;
						}
					}

					//south west
					if ((x < GRID_SIZE_X - 1) && (y > 0))
					{
						int& sw = gridArray[x + 1][y - 1];
						if (sw == -1)
						{
							sw = it + 1;
							foundEmpty = true;
						}
						else if (sw == -3)
						{
							foundEnd = true;
							countHolder = it;
							break;
						}
					}

				}
			}
		}
		//breaks out when there is no solution as to not go any further,
		
		
		if (!foundEmpty && !foundEnd)
		{
			end = true;
			std::cout << "There is no solution to this maze.";
			std::cout << std::endl;
			break;
		}
		
		it++;


		
	}
}


void phaseTwo()
{

	bool foundStart = false;
	//start at end, record pos, look north,east, west, find countholder, record pos, countholder-1, look from here. until start pos.


	for (int i = 0; i < GRID_SIZE_X; i++)
	{
		for (int j = 0; j < GRID_SIZE_Y; j++)
		{
			if (gridArray[i][j]==-3)
			{
				xHolder = i;
				yHolder = j;
			}
		}
	}

	//loop whilst the bool foundStart, knowing if the start has been located and if the end has also been located as well
	while (!foundStart && !end)
	{

		if (countHolder != 0)
		{
			int& north = gridArray[xHolder - 1][yHolder];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (north == countHolder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				xHolder = xHolder - 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;

				//	break;
			}
			int& northE = gridArray[xHolder - 1][yHolder+1];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (northE == countHolder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				xHolder = xHolder - 1;
				yHolder = yHolder + 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;

				//	break;
			}
			int& northW = gridArray[xHolder - 1][yHolder-1];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (northW == countHolder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				xHolder = xHolder - 1;
				yHolder = yHolder - 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;

				//	break;
			}

			int& west = gridArray[xHolder][yHolder - 1];	//west
			if (west == countHolder)
			{
				yHolder = yHolder - 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;
				//		break;

			}

			int& east = gridArray[xHolder][yHolder + 1];		//east 
			if (east == countHolder)
			{
				yHolder = yHolder + 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;
				//			break;

			}

			int& south = gridArray[xHolder + 1][yHolder];
			if (south == countHolder)
			{
				xHolder = xHolder + 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;

			}
			int& southE = gridArray[xHolder + 1][yHolder + 1];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (southE == countHolder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				xHolder = xHolder + 1;
				yHolder = yHolder + 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;

				//	break;
			}
			int& southW = gridArray[xHolder + 1][yHolder - 1];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (southW == countHolder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				xHolder = xHolder + 1;
				yHolder = yHolder - 1;
				positionx.push_back(xHolder);
				positiony.push_back(yHolder);
				countHolder -= 1;

				//	break;
			}

		}
		if (countHolder == 0)		//check for if the start has been located in the path and output the resulting path.
		{
			foundStart = true;

			//prints out lists using iterators, looping until end pos.





			std::list<int>::iterator iter = positionx.begin();
			std::list<int>::iterator itera = positiony.begin();
			while (iter != positionx.end() && itera != positiony.end())
			{
				int x, y;
			
				x = *iter; y = *itera;
				gridArray[x][y] = 500;
				iter++;
				itera++;			//increasing the iterators for both of the lists invovled in the coordinates.
			}


	

			break;
		}
	}
}


void Dijkstra()
{
	
	int Distance = 0;
	bool finished = false;
	int largeh = 5000;
	//sets the function vars involved

	int startPX = 0;
	int startPY = 0;
	int endPX = 0;
	int endPY= 0;
	for (int x = 0; x < GRID_SIZE_X; x++)			//fills the pos equal to 0 to the 'infinite' number described
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			if (gridArray[x][y] == 0)
			{
				//gridDistance[x][y] = largeh;
				startPX = x;
				startPY = y;
			}
			if (gridArray[x][y] == -3)
			{
				//gridDistance[x][y] = largeh;
				endPX = x;
				endPY = y;
			}
		}
	}





	while (!finished)			//while the finishing position hasnt been found, check south,east, southeast for smaller distance and add to distance total if it is smaller, if outside boundaries, set to largeh to avoid going over again, if -3 which is the end, finish and will go on to output the result.
	{
		int& north = gridArray[startPX - 1][startPY];
		if (north == -2)
		{
			gridDistance[startPX - 1][startPY] = largeh + 1;
		}
		else if (north == -3)
		{
			finished = true;

		}
		else if (north < largeh - 1)
		{
			gridDistance[startPX - 1][startPY] = north;

			//startPX += 1;
		}

		int& northEast = gridArray[startPX - 1][startPY+1];
		if (northEast == -2)
		{
			gridDistance[startPX - 1][startPY+1] = largeh + 1;
		}
		else if (northEast == -3)
		{
			finished = true;

		}
		else if (northEast < largeh - 1)
		{
			gridDistance[startPX - 1][startPY+1] = northEast;

			//startPX += 1;
		}





		int& south = gridArray[startPX + 1][startPY];
		if (south == -2)
		{
			gridDistance[startPX + 1][startPY] = largeh + 1;
		}
		else if (south == -3)
		{
			finished = true;

		}
		else if (south < largeh - 1)
		{
			gridDistance[startPX + 1][startPY] = south;

			//startPX += 1;
		}

		int& east = gridArray[startPX][startPY + 1];
		if (east == -2)
		{
			gridDistance[startPX][startPY + 1] = largeh + 1;
		}
		else if (east == -3)
		{
			finished = true;

		}
		else if (east < largeh - 1)
		{
			gridDistance[startPX][startPY + 1] = east;
			//startPX += 1;
		}

		int& southEast = gridArray[startPX + 1][startPY + 1];
		if (southEast == -2)
		{
			gridDistance[startPX + 1][startPY + 1] = largeh + 1;
		}
		else if (southEast == -3)
		{
			finished = true;

		}
		else if (southEast < largeh - 1)
		{
			gridDistance[startPX + 1][startPY + 1] = southEast;
		}

		//checking the distances, if not -2 then calc distance, if -2 skip and end, returning no solution
		if ((south != -2) || (east != -2) || (southEast != -2) || (north!=-2)||(northEast!=2))
		{
			//north vs south, east, southeast, northeast
			if ((gridDistance[startPX - 1][startPY] <= gridDistance[startPX+1][startPY]) && (gridDistance[startPX -1][startPY] <= gridDistance[startPX][startPY + 1]) && (gridDistance[startPX - 1][startPY] <= gridDistance[startPX+1][startPY + 1]) && (gridDistance[startPX - 1][startPY] <= gridDistance[startPX-1][startPY + 1]))		//check for south
			{
				Distance += gridDistance[startPX - 1][startPY];
				startPX -= 1;
			}
			//east se, north, ne
			if ((gridDistance[startPX + 1][startPY] <= gridDistance[startPX][startPY + 1]) && (gridDistance[startPX + 1][startPY] <= gridDistance[startPX + 1][startPY + 1]) && (gridDistance[startPX + 1][startPY] <= gridDistance[startPX - 1][startPY]) && (gridDistance[startPX + 1][startPY] <= gridDistance[startPX- 1][startPY + 1]))		//check for south
			{
				Distance += gridDistance[startPX + 1][startPY];
				startPX += 1;
			}
			//south, se, n,ne
			else if ((gridDistance[startPX][startPY + 1] <= gridDistance[startPX + 1][startPY]) && (gridDistance[startPX][startPY + 1] <= gridDistance[startPX + 1][startPY + 1]) && (gridDistance[startPX][startPY + 1] <= gridDistance[startPX - 1][startPY]) && (gridDistance[startPX][startPY + 1] <= gridDistance[startPX - 1][startPY + 1]))		//check for east
			{
				Distance += gridDistance[startPX][startPY + 1];
				startPY += 1;
			}
			//s,se,e,n,ne
			else if ((gridDistance[startPX + 1][startPY + 1] <= gridDistance[startPX + 1][startPY]) && (gridDistance[startPX + 1][startPY + 1] <= gridDistance[startPX][startPY + 1]) && (gridDistance[startPX + 1][startPY + 1] <= gridDistance[startPX-1][startPY]) && (gridDistance[startPX + -1][startPY] <= gridDistance[startPX-1][startPY + 1]))		//check for south east, checks both at once
			{
				Distance += gridDistance[startPX + 1][startPY + 1];
				startPX += 1;
				startPY += 1;
			}
			//ne
			if ((gridDistance[startPX - 1][startPY+1] <= gridDistance[startPX + 1][startPY]) && (gridDistance[startPX - 1][startPY+1] <= gridDistance[startPX][startPY + 1]) && (gridDistance[startPX - 1][startPY+1] <= gridDistance[startPX + 1][startPY + 1]) && (gridDistance[startPX - 1][startPY+1] <= gridDistance[startPX - 1][startPY]))		//check for south
			{
				Distance += gridDistance[startPX - 1][startPY+1];
				startPX -= 1;
				startPY += 1;
			}
		}
		else
		{
			std::cout << "no Solution \n";			//incase of 'barriers', -2 in this case, output no solution possible.



			return;

		}


		//ending the loop
		if ((startPX==endPX) && (startPY == endPY))			//if the current position being checked is equal to the end position defined by the users input then output the distance and set the bool to true to exit the loop.
		{
			finished = true;
			std::cout << "The shortest distance is: " << Distance;

		}

	}
}


void findNumbers()
{
	for (int j = 0; j < GRID_SIZE_X; j++)
	{
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			if ((gridArray[j][i]!=-1) && (gridArray[j][i] != -2))
			{
				numberofPos1 += 1;
			}
		}
	}
}

void findPath()
{
	for (int j = 0; j < GRID_SIZE_X; j++)
	{
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			if ((gridArray[j][i] == 500) )
			{
				numberofPos2 += 1;
			}
		}
	}
}



void pathway(std::vector<sf::RectangleShape>& shape, int points)
{
	shape.resize(points);
	for (int i = 0; i < shape.size(); i++)
	{
		shape.at(i).setSize(sf::Vector2f(5, 5));
		shape.at(i).setFillColor(sf::Color::Blue);
	}
	int p = 0;
	for (int j = 0; j < GRID_SIZE_X; j++)
	{
		for (int y = 0; y < GRID_SIZE_X; y++)
		{
			if ((gridArray[j][y] != -1) && (gridArray[j][y] != -2))
			{
				shape.at(p).setPosition(j, y);
				p++;
			}
		}
	}
}

void pathway2(std::vector<sf::RectangleShape>& shape, int points)
{
	shape.resize(points);
	for (int i = 0; i < shape.size(); i++)
	{
		shape.at(i).setSize(sf::Vector2f(5, 5));
		shape.at(i).setFillColor(sf::Color::Yellow);
	}
	int p = 0;
	for (int j = 0; j < GRID_SIZE_X; j++)
	{
		for (int y = 0; y < GRID_SIZE_X; y++)
		{
			if ((gridArray[j][y] == 500))
			{
				shape.at(p).setPosition(j, y);
				p++;
			}
		}
	}
}



int main()
{
	// Seed the random number generator
	srand((unsigned)std::time(NULL));

	// Create the window and UI bar on the right
	const sf::Vector2f gridSize(GRID_SIZE_X*SQUARE_SIZE, GRID_SIZE_Y*SQUARE_SIZE);
	sf::RenderWindow window(sf::VideoMode(800,800), "SFML_RuleBasedSystem", sf::Style::Close);



	// Create the font
	sf::Font font;
	font.loadFromFile("DefaultAriel.ttf");

	// Create the start/stop text and position+rotate it

	// Flag for when the game plays
	bool event_playing = false;

	// Clock for timing
	sf::Clock clock;
	float elapsed = 0.0f;


	int counter = 0;

	std::vector<sf::RectangleShape> shape;

	std::vector<sf::RectangleShape> shape2;

	sf::RectangleShape shape3;
	sf::RectangleShape shape4;

	std::vector<sf::RectangleShape> pathShape;
	std::vector<sf::RectangleShape> pathShape2;



	
	//creates the sites
	RandomPlace();



	//actual voronoi construction, stored in tempGrid[][]
	Zones();


	//changes the tempgrid so that it has just the borders now.
	Borders();

	//changes the edges of the diagram to be not part of the diagram
	OutsideEdges();

	//render work
	findZeros();
	setPositions(shape, numberofPos);


	//choosing end and start points
	findPointOnEdge(shape3);
	findPointOnRight(shape4);


	initGrid();
	the_clock::time_point startTime = the_clock::now();
	firstPhase();
		the_clock::time_point end = the_clock::now();

	auto timetaken = duration_cast<milliseconds> (end - startTime).count();

	findNumbers();
	pathway(pathShape, numberofPos1);



	/*
	phaseTwo();

	findPath();
	pathway2(pathShape2, numberofPos2);
	*/
	
	// While the window is open, update
	while (window.isOpen())
	{
		// Parse events
		sf::Event sf_event;
		while (window.pollEvent(sf_event)) {
			// Close the window when the close button is pressed
			if (sf_event.type == sf::Event::Closed) {
				window.close();
			}
			
		}

		// Update clock. If enough time has elapsed, perform an update
		elapsed += clock.restart().asSeconds();
		if (elapsed > (1.f / (float)FRAMES_PER_SECOND))
		{
			elapsed = 0.0f;
			if (event_playing)
			{
				counter++;
				//run_rules();
				if (counter >= 5)
				{
					//randPlace();

				}
			}
		}

		//used to regen the vd
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			RandomPlace();
			Zones();
			Borders();
			OutsideEdges();
			findZeros();
			setPositions(shape, numberofPos);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			RandomPlace();
			Zones();
			Borders();
			OutsideEdges();
			findZeros();
			setPositions(shape2, numberofPos);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			findPointOnEdge(shape3);
			findPointOnRight(shape4);
		
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
 			initGrid();
			//fillGrid();
			firstPhase();
			findNumbers();
			pathway(pathShape, numberofPos1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			
			phaseTwo();
			findPath();
			pathway2(pathShape2, numberofPos2);
		}
		// Clear window, draw everything, display window
		window.clear();
		
		for (int i = 0; i < shape.size(); i++)
		{
			window.draw(shape.at(i));
		}
		for (int i = 0; i < shape2.size(); i++)
		{
			window.draw(shape2.at(i));
		}

		for (int i = 0; i < pathShape.size(); i++)
		{
			window.draw(pathShape.at(i));
		}

		for (int i = 0; i < pathShape2.size(); i++)
		{
			window.draw(pathShape2.at(i));
		}
		window.draw(shape3);
		window.draw(shape4);
		window.display();
	}


	return 0;
}