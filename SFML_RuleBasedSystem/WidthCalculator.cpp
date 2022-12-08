#include "WidthCalculator.h"



std::vector<int>WidthCalculator::point_inc_(1);
std::vector<int>WidthCalculator::cp_inc_(1);
WidthCalculator::WidthCalculator()
{
}

void WidthCalculator::FindMinMax(const int& layers_, int* const& noise_grid, const int& grid_size)
{
	int min_ = 100000;
	int max_ = 0;

	//finds the min and max of the terrrain
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if ((noise_grid[i * grid_size + j] / layers_) > max_)
			{
				max_ = noise_grid[i * grid_size + j] / layers_;
			}
			if ((noise_grid[i * grid_size + j] / layers_) < min_)
			{
				min_ = noise_grid[i * grid_size + j] / layers_;
			}
		}
	}
	image_max = max_;
	image_min = min_;
}

void WidthCalculator::FindTrackMinMax(const std::vector<sf::Vector2i>& track_points, const int& grid_size, const int& layers_, int* const& noise_grid)
{
	track_min = 0;
	track_max = 0;
	int min_ = 10000000;
	int max_ = 0;
	for (const sf::Vector2i& iter : track_points)
	{
		int x = iter.x;
		int y = iter.y;
		if ((noise_grid[y * grid_size + x] / layers_) > max_)
		{
			max_ = noise_grid[y * grid_size + x] / layers_;
		}
		if ((noise_grid[y * grid_size + x] / layers_) < min_)
		{
			min_ = noise_grid[y * grid_size + x] / layers_;
		}
	}
	track_max = max_, track_min = min_;
}


sf::Vector2i WidthCalculator::Lerp(const sf::Vector2i& p1, const sf::Vector2i& p2, const float& t)
{
	//auto r = p1 + ((p2 - p1) * t);
	auto a = p2 - p1;
	auto b = sf::Vector2i(a.x * t, a.y * t);
	auto c = p1 + b;
	return c;
}

float WidthCalculator::FindT(const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3)
{
	//t = (p1-p3)/(p1-p2)

	//have check somewhere to stop running if p1==p2

	auto a = p1 - p3;
	auto b = p1 - p2;

	float t = 0.0f;
	if (a.x == 0 || b.x == 0)
	{
		float c2 = ((float)a.y / (float)b.y);
		t = c2;
		if (t < 0.0f)
		{
			int gggg = 1;
			throw;
		}
	}
	else if (a.y == 0 || b.y == 0)
	{
		float c1 = ((float)a.x / (float)b.x);
		t = c1;
	}
	else
	{
		float c1 = ((float)a.x / (float)b.x);
		float c2 = ((float)a.y / (float)b.y);
		t = (c1 + c2) / 2.0f;
	}
	return t;
}



void WidthCalculator::TrackTValues(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points)
{
	int iter = 0;								//iterator for control points, chnages when a new control point is hit
	sf::Vector2i current_cp;					//stores current control point could just do control_points[iter-1]

	std::vector<float> t_values;				//stores t values - should be track_points-amount of control points					CHECK THAT IS CORRECT!!!!!!!

	for (const sf::Vector2i& i : track_points)
	{
		if (i == control_points[iter])						//if is currenctly over a control point
		{
			current_cp = i;
			iter++;
		}
		else
		{
			t_values.push_back(FindT(current_cp, control_points[iter], i));					//pushes back t value
		}
	}
}


void WidthCalculator::FindInclinePoints(const std::vector<sf::Vector2i>& vector_, const int& grid_size, const int& layers_, std::vector<int>& results_, int* const& noise_grid)
{
	int difference = 0;

	for (int i = 0; i < vector_.size() - 1;)
	{
		int x = vector_[i].x;
		int y = vector_[i].y;
		int point_a_height = noise_grid[y * grid_size + x] / layers_;
		i++;
		x = vector_[i].x, y = vector_[i].y;
		int point_b_height = noise_grid[y * grid_size + x] / layers_;
		difference = point_b_height - point_a_height;
		results_.push_back(difference);
	}
}