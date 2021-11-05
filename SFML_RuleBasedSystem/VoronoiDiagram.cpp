#include "VoronoiDiagram.h"


VoronoiDiagram::VoronoiDiagram()
{
	//set the defaults in here
}

void VoronoiDiagram::RandomPlaceSites()
{
	//clear the vector if its not empty, this will "refresh" it as not to have old sites
	if (!sites_v.empty())
	{
		sites_v.clear();
	}

	//loop over the number of sites and push back sites
	//create temp vector, loop over twice as this is a 2D space, x and y position.
	for (int i = 0; i < num_of_sites; i++)
	{
		std::vector<int>temp_s_v;
		for (int j = 0; j < 2; j++)
		{
			temp_s_v.push_back(rand() % grid_size_x);
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


void VoronoiDiagram::CreateDiagram()
{
	int d = 0;											
	std::vector<int>incr;

	for (int i = 0; i < num_of_sites; i++)
	{
		incr.push_back(i);
	}
	incr.at(0) = 101;			//cant be zero as zero is for the edge i think

	for (int i = 0; i < grid_size_x; i++)
	{
		for (int j=0;j<grid_size_x;j++)
		{
			int ind = -1, dist = INT_MAX;

			for (int p = 0; p < num_of_sites; p++)
			{
				//
				d = DistanceSqrt(sites_v[0][p],sites_v[p][0],j,i);
				if (d<dist)
				{
					dist = d;
					ind = p;
				}
			}
			if (ind>-1)
			{
				
			}
		}
	}
}

void VoronoiDiagram::SetEdges()
{

}

void VoronoiDiagram::SetBorders()
{

}

void VoronoiDiagram::SetPoint()
{

}