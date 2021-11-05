#include "VoronoiDiagram.h"


VoronoiDiagram::VoronoiDiagram()
{
	//set the defaults in here
	
	grid_size_x = 0;
	num_of_points = 0;
	num_of_sites = 0;
}

void VoronoiDiagram::InitVector(int grid_size)
{
	grid_v.resize(grid_size,std::vector<int>(grid_size));
}


void VoronoiDiagram::RandomPlaceSites(int num_sites, int grid_size)
{
	//clear the vector if its not empty, this will "refresh" it as not to have old sites
	if (!sites_v.empty())
	{
		sites_v.clear();
	}

	//loop over the number of sites and push back sites
	//create temp vector, loop over twice as this is a 2D space, x and y position.
	for (int i = 0; i < num_sites; i++)
	{
		std::vector<int>temp_s_v;
		for (int j = 0; j < 2; j++)
		{
			temp_s_v.push_back(rand() % grid_size);
		}
		sites_v.push_back(temp_s_v);
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
	std::vector<int>incr;

	for (int i = 0; i < num_sites; i++)
	{
		incr.push_back(i);
	}
	incr.at(0) = 101;			//cant be zero as zero is for the edge i think

	for (int i = 0; i < grid_size; i++)
	{
		for (int j=0;j< grid_size;j++)
		{
			int ind = -1, dist = INT_MAX;

			for (int p = 0; p < num_sites; p++)
			{
				//might throw an error out of bounds
				for (int q = 0; q < 1; q++)
				{
					d = DistanceSqrt(sites_v[p][q], sites_v[p][q+1], j, i);
				}
				
				if (d<dist)
				{
					dist = d;
					ind = p;
				}
			}
			//so if this point has a distance which all points do
			if (ind>-1)
			{
				/*std::vector<int>temp_g_v;
				for (int j = 0; j < grid_size; j++)
				{
					temp_g_v.push_back(incr.at(ind));
				}
				grid_v.push_back(temp_g_v);*/
				grid_v[j][i] = ind;
			}
		}
	}
}

void VoronoiDiagram::SetEdges(int grid_size)
{
	for (int j = 0; j < grid_size; j++)
	{
		for (int i=0; i< grid_size; i++)
		{
			if (i<=(grid_size-2) &&j<=(grid_size-2))
			{
				if (grid_v[j][i] != 0 && grid_v[j][i + 1] != 0)
				{
					if (grid_v[j][i] != grid_v[j][i + 1])
					{
						grid_v[j][i] = 0;
					}
				}
				if (grid_v[j][i] != 0 && grid_v[j + 1][i] != 0)
				{
					if (grid_v[j][i] != grid_v[j + 1][i])
					{
						grid_v[j][i] = 0;
					}
				}
			}
		}
	}
}

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


void VoronoiDiagram::DrawVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j=0;j<grid_size;j++)
		{
			if (grid_v[i][j]==0)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(i, j);
			}
		}
	}
}

void VoronoiDiagram::SetPoint()
{

}