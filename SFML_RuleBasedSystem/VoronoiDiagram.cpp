#include "VoronoiDiagram.h"
#include <iostream>


VoronoiDiagram::VoronoiDiagram()
{
	type = 1;
	grid_size_x = 0;
	num_of_points = 0;
	num_of_sites = 0;
	grid_v_1 = nullptr;
	sites_v_1 = nullptr;
}

VoronoiDiagram::~VoronoiDiagram()
{
	delete[] grid_v_1;
	delete[] sites_v_1;
}

void VoronoiDiagram::InitVector(int grid_size, int num_points, int num_sites)
{
	grid_size_x = grid_size;
	grid_v_1 = new int[grid_size_x * grid_size_x];
	sites_v_1 = new int[num_sites*2];
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


void VoronoiDiagram::EqualDSites(int num_sites, int grid_size)
{
	long long int site_iterator = 0;
	bool loop_cond = false;


	
	sites_v_1[site_iterator] = 0;
	site_iterator++;
	sites_v_1[site_iterator] = 0;
	site_iterator++;

	
	sites_v_1[site_iterator] = 0;
	site_iterator++;
	sites_v_1[site_iterator] = (grid_size-1);
	site_iterator++;


	
	sites_v_1[site_iterator] = grid_size;
	site_iterator++;
	sites_v_1[site_iterator] = (grid_size)-1;
	site_iterator++;

	
	sites_v_1[site_iterator] = grid_size;
	site_iterator++;
	sites_v_1[site_iterator] = (grid_size - 1);
	site_iterator++;
	
	grid_v_1[0] = 64;//bl
	grid_v_1[grid_size- 1] = 64;//br
	grid_v_1[grid_size * (grid_size)-1] = 64;//tr
	grid_v_1[grid_size * (grid_size - 1)] = 64;//tl


	for (int sideLength = grid_size - 1; sideLength >= 256; sideLength /= 2)
	{
		int halfSide = sideLength / 2;

		//diamond
		for (int y = 0; y < (grid_size - 1); y += sideLength)
		{
			for (int x = 0; x < (grid_size - 1); x += sideLength)
			{
				sites_v_1[site_iterator] = (x + halfSide);
				site_iterator++;
				sites_v_1[site_iterator] = y + halfSide;
				site_iterator++;
				grid_v_1[((y+halfSide) * grid_size) + (x+halfSide)] = 64;
			}
		}



			//square
		for (int j = 0; j < (grid_size ); j += halfSide)
		{
			for (int i = (j + halfSide) % sideLength; i < (grid_size ); i += sideLength)
			{

				sites_v_1[site_iterator] = i;
				site_iterator++;
				sites_v_1[site_iterator] = j;
				site_iterator++;
				grid_v_1[(j * grid_size) + i] = 64;
			}

		}

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
	delete[] incr;

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



void VoronoiDiagram::DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid_v_1[(i * grid_size) + j] == -1)
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
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
				
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
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
			}


			if (grid_v_1[(i * grid_size) + j] == 64)
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
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
	{
		int x_pos_one = (grid_size * 0.08);
		int x_pos_two = (grid_size * 0.02);
		int y_pos_one = (grid_size * 0.15);
		int y_pos_two = (grid_size * 0.80);
		for (int i = 0; i < num_points; i++)
		{
			bool found = false;
			int counter = 0;
			while (!found)
			{
				int x = rand() % x_pos_one + x_pos_two;
				int y = rand() % y_pos_one + y_pos_two;
				if (grid_v_1[(y * grid_size) + x] == 0)
				{
					found = true;
					grid_v_1[(y * grid_size) + x] = 2000 + i;
				}
				counter++;
				if (counter>200)
				{
					break;
				}
			}
			std::cout <<"counter: "<< counter<<std::endl;
			if (i==0)
			{
				x_pos_one = (grid_size * 0.10);
				x_pos_two = (grid_size * 0.45);
				y_pos_one = (grid_size * 0.08);
				y_pos_two = (grid_size * 0.02);
			}
			if (i == 1)
			{
				x_pos_one = (grid_size * 0.15);
				x_pos_two = (grid_size * 0.80);
				y_pos_one = (grid_size * 0.15);
				y_pos_two = (grid_size * 0.80);
			}
		}
	}
		break;

	case 1:
	{
		int iter = grid_size / num_points;
		int start = 1;
		int position = 0;
		for (int i = 0; i < num_points; i++)
		{

			bool found = false;
			while (!found)
			{
				//so first is between 0 and grid_size/numpoints, second is iter and iter+iter, etc
				int x = rand() % iter + start;
				int y = rand() % (int)(grid_size / 1.1) + (grid_size / 16);
				if (grid_v_1[(y * grid_size) + x] == 0)
				{
					found = true;
					grid_v_1[(y * grid_size) + x] = 2000 + i;

				}
			}
			start += iter;
		}
	}
		break;
	case 2:
	{
		int x_pos_one = (grid_size * 0.08);
		int x_pos_two = (grid_size * 0.02);
		int y_pos_one = (grid_size * 0.10);
		int y_pos_two = (grid_size * 0.45);
		for (int i = 0; i < num_points; i++)
		{
			bool found = false;
			int counter = 0;
			while (!found)
			{
				int x = rand() % x_pos_one + x_pos_two;
				int y = rand() % y_pos_one + y_pos_two;
				if (grid_v_1[(y * grid_size) + x] == 0)
				{
					found = true;
					grid_v_1[(y * grid_size) + x] = 2000 + i;
				}
				counter++;
				if (counter > 200)
				{
					break;
				}
			}
			std::cout << "counter: " << counter << std::endl;
			if (i == 0)
			{
				x_pos_one = (grid_size * 0.10);
				x_pos_two = (grid_size * 0.45);
				y_pos_one = (grid_size * 0.10);
				y_pos_two = (grid_size * 0.20);
			}
			if (i == 1)
			{
				x_pos_one = (grid_size * 0.15);
				x_pos_two = (grid_size * 0.80);
				y_pos_one = (grid_size * 0.10);
				y_pos_two = (grid_size * 0.65);
			}
		}
	}
		break;
	}

}

