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

void CatmullRomSpline::FixControlPoints(std::vector<std::pair<int, int>> & const control_points, int it_,std::pair<int,int> co)
{
	control_points[it_].first = co.first;
	control_points[it_].second = co.second;
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
	if (!is_looped)
	{
		auto it = control_points.begin();
		control_points.insert(it, control_points[0]);
	}

	for (float t = 0; t < (float)control_points.size(); t += step_size)
	{
		std::pair<int, int> point_ = CreatePoint(control_points, is_looped,t);
		vertexarray[point_.second * grid_size + point_.first].color = sf::Color::White;
	}
}



void CatmullRomSpline::DrawControlPoints(std::vector<std::pair<int, int>> control_points, int grid_size, sf::VertexArray& vertexarray)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			vertexarray[i * grid_size + j].color = sf::Color::Black;
		}
	}
	for (int i = 0; i < control_points.size(); i++)
	{
		vertexarray[control_points[i].second * grid_size + control_points[i].first].color = sf::Color::White;
	}
}

sf::Vector2f CatmullRomSpline::CentripetalCurve(float t)
{
	
	std::vector<sf::Vector2i> points;
	points.push_back(sf::Vector2i(100, 400));
	points.push_back(sf::Vector2i(200, 200));
	points.push_back(sf::Vector2i(200, 210));
	points.push_back(sf::Vector2i(400, 400));

	
	float t01 = pow(DistanceSqrt(points[0].x, points[0].y, points[1].x, points[1].y), alpha_);
	float t12 = pow(DistanceSqrt(points[1].x, points[1].y, points[2].x, points[2].y), alpha_);
	float t23 = pow(DistanceSqrt(points[2].x, points[2].y, points[3].x, points[3].y), alpha_);




	sf::Vector2f m1 = (1.0f) *((sf::Vector2f)points[2] - (sf::Vector2f)points[1] + t12 * ((sf::Vector2f)(points[1] - points[0]) / t01 - (sf::Vector2f)(points[2] - points[0]) / (t01 + t12)));
	sf::Vector2f m2 = (1.0f) *((sf::Vector2f)points[2] - (sf::Vector2f)points[1] + t12 * ((sf::Vector2f)(points[3] - points[2]) / t23 - (sf::Vector2f)(points[3] - points[1]) / (t12 + t23)));


	Segment segment;
	segment.a = 2.0f * (sf::Vector2f)(points[1] - points[2]) + m1 + m2;
	segment.b = -3.0f * (sf::Vector2f)(points[1] - points[2]) - m1 - m1 - m2;
	segment.c = m1;
	segment.d = (sf::Vector2f)points[1];

	sf::Vector2f point = segment.a * t * t * t +segment.b * t * t +segment.c * t +segment.d;
	return point;
}


void CatmullRomSpline::CreateCurve(int grid_size, sf::VertexArray& vertexarray)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			vertexarray[i * grid_size + j].color = sf::Color::Black;
		}
	}

	for (float i = 0; i < 1; i+=0.1f)
	{
		sf::Vector2f point = CentripetalCurve(i);
		sf::Vector2i point_ = (sf::Vector2i)point;
		vertexarray[ point_.y* grid_size + point_.x].color = sf::Color::White;
	}
	vertexarray[400 * grid_size + 100].color = sf::Color::Red;
	vertexarray[200 * grid_size + 200].color = sf::Color::Red;
	vertexarray[210 * grid_size + 200].color = sf::Color::Red;
	vertexarray[400 * grid_size + 400].color = sf::Color::Red;
}

int CatmullRomSpline::DistanceSqrt(int x, int y, int x2, int y2)
{
	int xd = x2 - x;
	int yd = y2 - y;
	return (xd * xd) + (yd * yd);
}
