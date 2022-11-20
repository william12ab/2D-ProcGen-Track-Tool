#include "CatmullRomSpline.h"
CatmullRomSpline::CatmullRomSpline()
{
	step_size = 0.005f;
	alpha_ = 0.5f;
}
CatmullRomSpline::~CatmullRomSpline()
{

}

int* CatmullRomSpline::GetIndices(bool is_looped, float t, int size_)
{
	int p0, p1, p2, p3;
	if (!is_looped)
	{
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
		if (p3 >= size_)
		{
			p3 = size_ - 1;
		}
		if (p2 >= size_)
		{
			p2 = size_ - 1;
		}
		if (p1 >= size_)
		{
			p1 = size_ - 1;
		}
		if (p0 >= size_)
		{
			p0 = size_ - 1;
		}
	}
	else
	{
		p1 = (int)t;
		p2 = (p1 + 1) % size_;
		p3 = (p2 + 1) % size_;
		p0 = p1 >= 1 ? p1 - 1 : size_ - 1;					//if p1>= 1, true= p1-1, false = size-1
	}
	int arr[4] = { p0,p1,p2,p3 };
	return arr;
}

sf::Vector2i CatmullRomSpline::CreatePoint(std::vector<sf::Vector2i> control_points, bool is_looped, float t)
{
	int p0, p1, p2, p3;
	auto arr_ = GetIndices(is_looped, t, control_points.size());

	p0 = arr_[0];
	p1 = arr_[1];
	p2 = arr_[2];
	p3 = arr_[3];
	t = t - (int)t;

	float step_squared = t * t;
	float step_cubed = step_squared * t;

	float q1 = -step_cubed + 2.0f * step_squared - t;
	float q2 = 3.0f * step_cubed - 5.0f * step_squared+ 2.0f;
	float q3 = -3.0f * step_cubed + 4.0f * step_squared+ t;
	float q4 = step_cubed - step_squared;

	float tx = 0.5f * (control_points[p0].x * q1 + control_points[p1].x * q2 + control_points[p2].x * q3 + control_points[p3].x * q4);
	float ty = 0.5f * (control_points[p0].y * q1 + control_points[p1].y * q2 + control_points[p2].y * q3 + control_points[p3].y * q4);

	return{ (int)tx, (int)ty };
}

void CatmullRomSpline::FixControlPoints(std::vector<sf::Vector2i> & const control_points, int it_, sf::Vector2i co)
{
	control_points[it_].x = co.x;
	control_points[it_].y = co.y;
}

void CatmullRomSpline::CreateCurve(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray& vertexarray, bool is_looped)
{
	ResetArray(vertexarray, grid_size);
	if (!is_looped)
	{
		auto it = control_points.begin();
		control_points.insert(it, control_points[0]);
	}

	for (float t = 0; t < (float)control_points.size(); t += step_size)
	{
		sf::Vector2i point_ = CreatePoint(control_points, is_looped,t);
		vertexarray[point_.y * grid_size + point_.x].color = sf::Color::White;
	}
}



void CatmullRomSpline::DrawControlPoints(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray& vertexarray)
{
	ResetArray(vertexarray, grid_size);
	for (int i = 0; i < control_points.size(); i++)
	{
		vertexarray[control_points[i].y * grid_size + control_points[i].x].color = sf::Color::White;
	}
}

sf::Vector2f CatmullRomSpline::CentripetalCurve(float t, std::vector<sf::Vector2i> control_points, int j, bool is_looped)
{
	//indices
	int p0, p1, p2, p3;
	if (!is_looped)
	{
		p1 = j + ((int)t + 1);
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
		if (j>=control_points.size()-3)
		{
			for (int i = 0; i < 3; i++)
			{
				auto second_ = control_points[control_points.size() - 1];
				second_.x += 5, second_.y += 5;
				control_points.push_back(second_);
			}
		}
	}
	else
	{
		p1 = j+ (int)t;
		p2 = (p1 + 1) % control_points.size();
		p3 = (p2 + 1) % control_points.size();
		p0 = p1 >= 1 ? p1 - 1 : control_points.size() - 1;					//if p1>= 1, true= p1-1, false = size-1
	}
	
	float t01 = pow(DistanceSqrt(control_points[p0].x, control_points[p0].y, control_points[p1].x, control_points[p1].y), alpha_);
	float t12 = pow(DistanceSqrt(control_points[p1].x, control_points[p1].y, control_points[p2].x, control_points[p2].y), alpha_);
	float t23 = pow(DistanceSqrt(control_points[p2].x, control_points[p2].y, control_points[p3].x, control_points[p3].y), alpha_);

	sf::Vector2f m1 = (1.0f) *((sf::Vector2f)control_points[p2] - (sf::Vector2f)control_points[p1] + t12 * ((sf::Vector2f)(control_points[p1] - control_points[p0]) / t01 - (sf::Vector2f)(control_points[p2] - control_points[p0]) / (t01 + t12)));
	sf::Vector2f m2 = (1.0f) *((sf::Vector2f)control_points[p2] - (sf::Vector2f)control_points[p1] + t12 * ((sf::Vector2f)(control_points[p3] - control_points[p2]) / t23 - (sf::Vector2f)(control_points[p3] - control_points[p1]) / (t12 + t23)));


	Segment segment;
	segment.a = 2.0f * (sf::Vector2f)(control_points[p1] - control_points[p2]) + m1 + m2;
	segment.b = -3.0f * (sf::Vector2f)(control_points[p1] - control_points[p2]) - m1 - m1 - m2;
	segment.c = m1;
	segment.d = (sf::Vector2f)control_points[p1];

	sf::Vector2f point = segment.a * t * t * t +segment.b * t * t +segment.c * t +segment.d;
	return point;
}


void CatmullRomSpline::CreateCurve(int grid_size, sf::VertexArray& vertexarray, std::vector<sf::Vector2i> control_points, bool is_looped)
{
	ResetArray(vertexarray, grid_size);
	if (!is_looped)
	{
		
		if (alpha_==0.5f)
		{
			auto first_ = control_points[0];
			auto second_ = control_points[control_points.size() - 1];

			auto it = control_points.begin();
			first_.x -= 5, first_.y -= 5;
			control_points.insert(it, first_);

			second_.x += 5, second_.y += 5;
			control_points.push_back(second_);
		}
		else
		{
			auto it = control_points.begin();
			control_points.insert(it, control_points[0]);
		}
	}


	for (int j = 0; j < control_points.size(); j++)
	{
		
		for (float i = 0; i < 1; i += step_size)
		{
			sf::Vector2f point = CentripetalCurve(i, control_points,j, is_looped);
			sf::Vector2i point_ = (sf::Vector2i)point;
			vertexarray[point_.y * grid_size + point_.x].color = sf::Color::White;
		}
	}
}

int CatmullRomSpline::DistanceSqrt(int x, int y, int x2, int y2)
{
	int xd = x2 - x;
	int yd = y2 - y;
	return sqrt((xd * xd) + (yd * yd));
}


void CatmullRomSpline::ResetArray(sf::VertexArray& vertexarray, int grid_size)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			vertexarray[i * grid_size + j].color = sf::Color::Black;
		}
	}
}