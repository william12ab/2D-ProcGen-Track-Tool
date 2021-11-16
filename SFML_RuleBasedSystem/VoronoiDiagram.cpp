#include "VoronoiDiagram.h"


VoronoiDiagram::VoronoiDiagram()
{
	//set the defaults in here
	
	grid_size_x = 0;
	num_of_points = 0;
	num_of_sites = 0;
}

void VoronoiDiagram::InitVector(int grid_size, int num_points, int num_sites)
{
	//grid_v.resize(grid_size,std::vector<int>(grid_size));
	grid_v_1 = new int[grid_size * grid_size];
	sites_v_1 = new int[num_sites*2];
	point_v_1 = new int[num_points];
}


void VoronoiDiagram::RandomPlaceSites(int num_sites, int grid_size)
{
	//loop over the number of sites and push back sites
	//create temp vector, loop over twice as this is a 2D space, x and y position.
	for (int i = 0; i < (num_sites*2); i++)
	{
		sites_v_1[i] = rand() % grid_size;
	}
}

int VoronoiDiagram::DistanceSqrt(int x, int y, int x2, int y2)
{
	int xd = x2 - x;
	int yd = y2 - y;
	return (xd * xd) + (yd * yd);
}

//the vector of ints "incr" is used in replace of the colors for the bruteforce method 
//the loop pushes back i into the vector at the amount of sites.
//each element is a sites "colour" 
//the distance is found at each site in comparison to the index of the loop in x and y direction.
//relative to the distance the cell is found of the diagram.
void VoronoiDiagram::CreateDiagram(int num_sites, int grid_size)
{
	int d = 0;											
	int* incr;
	incr = new int[num_sites];
	
	for (int i = 0; i < num_sites; i++)
	{
		incr[i] = i+1;
	}
	for (int j = 0; j < grid_size; j++)
	{
		for (int i=0;i< grid_size;i++)
		{
			int ind = -1, dist = INT_MAX;

			int s = 0;
			for (int p = 0; p < num_sites; p++)
			{
				d = DistanceSqrt(sites_v_1[s], sites_v_1[s+1], j, i);
				s += 2;
				if (d<dist)
				{
					dist = d;
					ind = p;
				}
			}
			//so if this point has a distance which all points do
			if (ind>-1)
			{
				int s = grid_v_1[(j * grid_size) + i];
				int p = incr[ind];
				grid_v_1[(j* grid_size)+i]=incr[ind];
				int w = grid_v_1[(j * grid_size) + i];
			}
		}
	}

}

//loop over all points
//the out if statements are to check if the index is still in bounds of the grid_size in x and y direction
//j =y/z, i=x
void VoronoiDiagram::SetEdges(int grid_size)
{
	for (int j = 0; j < grid_size; j++)
	{
		for (int i=0; i< grid_size; i++)
		{
			if (i+1<grid_size)
			{
				if (grid_v_1[(j * grid_size) + i] != 0 && grid_v_1[(j * grid_size) + (i + 1)] != 0)
				{
					if (grid_v_1[(j * grid_size) + i] != grid_v_1[(j * grid_size) + (i + 1)])
					{
						grid_v_1[(j * grid_size) + i] = 0;
					}
				}
			}
			if (j + 1 < grid_size)
			{
				if (grid_v_1[(j * grid_size) + i] != 0 && grid_v_1[((j + 1) * grid_size) + i] != 0)
				{
					if (grid_v_1[(j * grid_size) + i] != grid_v_1[((j + 1) * grid_size) + i])
					{
						grid_v_1[(j * grid_size) + i] = 0;
					}
				}
			}

		}
	}
}

//unused? needs changed anyway
void VoronoiDiagram::SetBorders(int grid_size)
{
	int y = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (grid_size); j++)
		{
			grid_v[y][j] = 90;
		}
		y = grid_size - 1;
	}
	int x = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (grid_size); j++)
		{
			grid_v[j][x] = 90;
		}
		x = grid_size - 1;
	}
}
void VoronoiDiagram::DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid_v_1[(i * grid_size) + j] == -2)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Green;
			}
			if (grid_v_1[(i * grid_size) + j] > 0)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
			}
		}
	}


}

void VoronoiDiagram::DrawVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j=0;j<grid_size;j++)
		{
			if (grid_v_1[(i * grid_size) + j]== -12303)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::White;
			}
			//pathway


			//start
			if (grid_v_1[(i * grid_size) + j] == -1234)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Red;
				
			}
			//end/middle
			if (grid_v_1[(i * grid_size) + j] == 0)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Red;
			}
			//end
			if (grid_v_1[(i * grid_size) + j] == -3)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Red;
			}
		}
	}
	
}

//clear the vector if empty
//find out what type and then how many points
//two different methods, loop needs to go in a loop, point to point needs to go from one side to other.
//loop needs minimun of 3 points, p2p needs 2 min.

//cirlce
//loop over num of points
//point 1
//
void VoronoiDiagram::SetPoint(int grid_size, int num_points, int type)
{


	//zero is loop, 1 is p2p
	switch (type)
	{
	case 0:
		break;

	case 1:
		int iter = grid_size / num_points;
		int start = 1;
		int position = 0;
		for (int i = 0; i < num_points; i++)
		{
			
			bool found = false;
			while (!found)
			{
				//so first is between 0 and grid_size/numpoints, second is iter and iter+iter, etc
				int x = rand()% iter+start;	
				int y = rand()% (int)(grid_size/1.1)+(grid_size/16);
				if (grid_v_1[(y * grid_size) + x] == 0)
				{
					found = true;
					
					point_v_1[position] = x;
					point_v_1[position+1] = y;
					position += 2;

					grid_v_1[(y * grid_size) + x] = 2000 + i;
		
				}
			}
			start += iter;
		}
		break;
	}

}

