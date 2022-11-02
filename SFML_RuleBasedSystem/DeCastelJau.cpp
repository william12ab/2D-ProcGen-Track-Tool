#include "DeCastelJau.h"
#include <iomanip>
DeCastelJau::DeCastelJau()
{
	step_size = 0.01;
}


DeCastelJau::~DeCastelJau()
{}

//passing in control points
void DeCastelJau::CreateCurve(std::vector<std::pair<int, int>> control_points, int grid_size, int* grid, sf::VertexArray& vertexarray)
{
	int control_point_size = control_points.size();

	//test code
	//control_point_size = 4;

	std::vector<std::vector<float>> b_x;											//control point(s) x
	std::vector<std::vector<float>> b_y;											//control point(s) y

	b_x.resize(control_point_size);
	b_y.resize(control_point_size);


	for (int i = 0; i < control_point_size; i++)
	{
		b_x[0].push_back(control_points[i].first);
		b_y[0].push_back(control_points[i].second);
	}
	for (int i = 1; i < control_point_size; i++)
	{
		for (int o = 0; o < control_point_size; o++)
		{
			b_x[i].push_back(0);
			b_y[i].push_back(0);
		}
	}

	//test code
	//b_x[0] = { 1,2,8,12 };
	//b_y[0] = { 1,7,6,2 };
	
	//


	//set size of bx by here using control_pooint size and set their values too
	std::vector<float> new_x;
	std::vector<float> new_y;
	
	

	int j_max = control_point_size-1;											//iterator - for the position of the c.p(s) in the loop1
	float t_zero = 0;															//unsure
	int count = 0;																//iterator - for the position of the new points in the array(structure)
	
	for (t_zero = 0; t_zero < 1; t_zero+= step_size)
	{
		count +=1;																		//un used?

		for (int j = 1; j <= (j_max); j++)
		{
			for (int i = 0; i < (j_max + 1)-j; i++)
			{
				b_x[j][i] = (1 - t_zero) * b_x[j-1][i] + t_zero * b_x[j-1][i+1];
				int a = b_x[j - 1][i];
				b_y[j][i] = (1 - t_zero) * b_y[j-1][i] + t_zero * b_y[j-1][i + 1];
			}
		}
		//set new points here
		//
		new_x.push_back(b_x[j_max][0]);
		new_y.push_back(b_y[j_max][0]);
	}
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			vertexarray[i * grid_size + j].color = sf::Color::Black;
		}
	}

	for (int i = 0; i < new_x.size(); i++)
	{
		//grid[((int)new_y[i] * grid_size) + (int)new_x[i]] = -5;
		vertexarray[((int)new_y[i] * grid_size) + (int)new_x[i]].color = sf::Color::White;
	}
}

