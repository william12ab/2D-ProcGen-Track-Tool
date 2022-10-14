#include "DeCastelJau.h"

DeCastelJau::DeCastelJau()
{
}


DeCastelJau::~DeCastelJau()
{}

//passing in control points
void DeCastelJau::CreateCurve(std::vector<std::pair<int, int>> control_points, int grid_size, int* grid)
{
	int control_point_size = control_points.size();

	control_point_size = 4;

	std::vector<std::vector<int>> b_x;											//control point(s) x
	std::vector<std::vector<int>> b_y;											//control point(s) y

	b_x.resize(control_point_size);
	b_y.resize(control_point_size);

	b_x[0][0] = 1;
	b_x[0][1] = 2;
	b_x[0][2] = 6;
	b_x[0][3] = 8;
	
	b_y[0][0] = 1;
	b_y[0][1] = 4;
	b_y[0][2] = 4;
	b_y[0][3] = 2;
	

	//set size of bx by here using control_pooint size and set their values too
	std::vector<int> new_x;
	std::vector<int> new_y;
	


	int j_max = control_point_size;											//iterator - for the position of the c.p(s) in the loop
	float t_zero = 0.4;															//unsure
	int count = 0;																//iterator - for the position of the new points in the array(structure)
	
	for (t_zero = 0; t_zero < 1; t_zero+=0.01)
	{
		count +=1;																		//un used?

		for (int j = 2; j < (j_max+1); j++)
		{
			for (int i = 0; i < (j_max + 2)-j; i++)
			{
				b_x[j][i] = (1 - t_zero) * b_x[j-1][i] + t_zero * b_x[j-1][i+1];
				b_y[j][i] = (1 - t_zero) * b_y[j-1][i] + t_zero * b_y[j-1][i + 1];
			}
		}
		//set new points here
		//
		new_x.push_back(b_x[control_point_size][0]);
		new_y.push_back(b_y[control_point_size][0]);
	}
}

