#include "CatmullRomSpline.h"
CatmullRomSpline::CatmullRomSpline()
{
	step_size = 0.005f;
	alpha_ = 0.5f;
}
CatmullRomSpline::~CatmullRomSpline()
{

}

std::pair<int, int> CatmullRomSpline::CreatePoint(std::vector<std::pair<int, int>> control_points, bool is_looped, float t)
{
	int p0, p1, p2, p3;
	if (!is_looped)
	{
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
		if (p3>=control_points.size())
		{
			p3 = control_points.size() - 1;
		}
		if (p2 >= control_points.size())
		{
			p2 = control_points.size() - 1;
		}
		if (p1 >= control_points.size())
		{
			p1 = control_points.size() - 1;
		}
		if (p0 >= control_points.size())
		{
			p0 = control_points.size() - 1;
		}
	}
	else
	{
		p1 = (int)t;
		p2 = (p1 + 1) % control_points.size();
		p3 = (p2 + 1) % control_points.size();
		p0 = p1 >= 1 ? p1 - 1 : control_points.size() - 1;					//if p1>= 1, true= p1-1, false = size-1
	}

	t = t - (int)t;

	float step_squared = t * t;
	float step_cubed = step_squared * t;

	float q1 = -step_cubed + 2.0f * step_squared - t;
	float q2 = 3.0f * step_cubed - 5.0f * step_squared+ 2.0f;
	float q3 = -3.0f * step_cubed + 4.0f * step_squared+ t;
	float q4 = step_cubed - step_squared;

	

	float tx = 0.5f * (control_points[p0].first * q1 + control_points[p1].first * q2 + control_points[p2].first * q3 + control_points[p3].first * q4);
	float ty = 0.5f * (control_points[p0].second * q1 + control_points[p1].second * q2 + control_points[p2].second * q3 + control_points[p3].second * q4);

	return{ tx, ty };
}


void CatmullRomSpline::CreateCurve(std::vector<std::pair<int, int>> control_points, int grid_size, sf::VertexArray& vertexarray, bool is_looped)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			vertexarray[i * grid_size + j].color = sf::Color::Black;
		}
	}
	for (float t = 0; t < (float)control_points.size(); t += step_size)
	{
		std::pair<int, int> point_ = CreatePoint(control_points, is_looped,t);
		vertexarray[point_.second * grid_size + point_.first].color = sf::Color::White;
	}
}