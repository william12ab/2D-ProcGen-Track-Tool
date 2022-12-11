#include "WidthCalculator.h"
std::vector<int>WidthCalculator::point_inc_(1);
std::vector<int>WidthCalculator::cp_inc_(1);
std::vector<sf::Vector2f>WidthCalculator::normalised_opposite_direction(1);
std::vector<sf::Vector2i> WidthCalculator::max_width_directions(1);
std::vector<float> WidthCalculator::t_values(1);

WidthCalculator::WidthCalculator()
{
	image_max = 0;
	image_min = 0;
	track_max = 0;
	track_min = 0;
	average_length = 0;
	default_width = 0;
	max_width_right = 0;
	max_width_left = 0;
	min_width = 0;
	track_surface = 0;
}

int WidthCalculator::DistanceSqrt(int x, int y, int x2, int y2)
{
	int xd = x2 - x;
	int yd = y2 - y;
	return sqrt((xd * xd) + (yd * yd));
}


void WidthCalculator::Clear()
{
	image_max = 0;
	image_min = 0;
	track_max = 0;
	track_min = 0;
	normalised_opposite_direction.clear();
	point_inc_.clear();
	cp_inc_.clear();
	max_width_directions.clear();
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

	for (const sf::Vector2i& i : track_points)
	{
		if (i == control_points[iter])						//if is currenctly over a control point
		{
			current_cp = i;
			iter++;
			t_values.push_back(FindT(current_cp, control_points[iter], i));
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
		if (difference<0)
		{
			difference *= -1;
		}
		results_.push_back(difference);
	}
}


//gets the normalised direction vector between two control points.
void WidthCalculator::FindDirectionBetweenCP(const std::vector<sf::Vector2i>& control_points)
{
	sf::Vector2f dir_, dir_norm_, dir_opp_, dir_norm_opp_;
	//getting positions front back

	for (int i = 0; i < control_points.size()-1;i++)
	{
		auto p1 = control_points[i];					//control points
		auto p2 = control_points[i+1];

		auto p3 = sf::Vector2i(p1.x, p2.y);				//opposite of control points kinda
		auto p4 = sf::Vector2i(p2.x, p1.y);

		//directional vector
		dir_ = sf::Vector2f(p2 - p1);
		dir_opp_ = sf::Vector2f(p4 - p3);

		//normalising vector
		dir_norm_.x = dir_.x / sqrt(pow(dir_.x, 2) + pow(dir_.y, 2));
		dir_norm_.y = dir_.y / sqrt(pow(dir_.x, 2) + pow(dir_.y, 2));
		dir_norm_opp_.x = dir_opp_.x / sqrt(pow(dir_opp_.x, 2) + pow(dir_opp_.y, 2));
		dir_norm_opp_.y = dir_opp_.y / sqrt(pow(dir_opp_.x, 2) + pow(dir_opp_.y, 2));

		//doesnt work if the line is | or -, you know what i mean 
		if (dir_norm_==sf::Vector2f(0.0,1.0)||dir_norm_==sf::Vector2f(0.0,-1.0))
		{
			dir_norm_opp_ = sf::Vector2f(1, 0);
		}
		if (dir_norm_ == sf::Vector2f(1.0, 0.0) || dir_norm_ == sf::Vector2f(1.0, 0.0))
		{
			dir_norm_opp_ = sf::Vector2f(0,1);
		}
		normalised_opposite_direction.push_back(dir_norm_opp_);
	}
}

void WidthCalculator::FindMaxWidth(int& max_width_d,int &x, int &y, const int&iter, int* const& noise_grid, const int& grid_size, const int& layers_, const sf::Vector2i& i, const int &mody)
{
	for (float adder = 0; adder < 10; adder += 0.1)
	{
		x += (mody*normalised_opposite_direction[iter - 1].x) * adder;									//CHECK THIS VALUE THE *10 PART
		y += (mody * normalised_opposite_direction[iter - 1].x) * adder;
		int height_this_way = noise_grid[i.y * grid_size + i.x] / layers_ - noise_grid[y * grid_size + x] / layers_;
		if (height_this_way < 0)
		{
			height_this_way *= -1;
		}
		if (height_this_way > 10)																	//TEMP VALYE
		{
			max_width_d = DistanceSqrt(i.x, i.y, x, y);
			adder = 10;
		}
	}
}


void WidthCalculator::FindRelatedHeight(int* const& noise_grid, const int& grid_size, const int& layers_, const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points)
{
	int iter = 0;
	sf::Vector2i current_cp=sf::Vector2i(0,0);
	//find the height in the direction of the vector calculated above.
	//for all track points.
	for (const sf::Vector2i& i : track_points)
	{
		if (i == control_points[iter])						//if is currenctly over a control point
		{
			current_cp = i;
			iter++;								//use iter and iter-1 to get the direction
		}
		int x = i.x;
		int y = i.y;
		FindMaxWidth(max_width_right, x, y, iter, noise_grid, grid_size, layers_, i, 1);
		x = i.x;
		y = i.y;
		FindMaxWidth(max_width_left, x, y, iter, noise_grid, grid_size, layers_, i, -1);
		max_width_directions.emplace_back(max_width_left, max_width_right);
	}
}


void WidthCalculator::CompareHeights(const int& max_, const int& min_)
{
	if (max_>180 && min_ >180)
	{
		//this is high
		default_width -= 1;
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}
	if (max_ < 180 && min_ < 180)
	{
		//this is low
		default_width += 1;
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
	int difference_ = max_ - min_;
	if (difference_>150)
	{
		//big differnce	so height chnages a lot
		default_width -= 1;
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}
	if (difference_<100)
	{
		//small difference
		default_width += 1;
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
}

void WidthCalculator::CheckPoints(const std::vector<int>& inc_, const int&iter, const int&height_diff)
{
	if (inc_[iter] > height_diff)
	{
		//high incline
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}
	else
	{
		//low incline
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
}

void WidthCalculator::CheckLength(const std::vector<int>& lengths_, const int &it)
{
	if (lengths_[it]> average_length+(average_length/2))		//high length
	{
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
	else if (lengths_[it] < average_length - (average_length / 2))		//low length
	{
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}
	else                     //avr length
	{
		//do nothing. . . 
	}
}


void WidthCalculator::CheckTValues(const int& i)
{
	if (t_values[i]>=0.0 && t_values[i]<=0.14)
	{
		//exit
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
	if (t_values[i] >= 0.9 && t_values[i] <= 0.97)
	{
		//entry
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}
	if (t_values[i] >= 0.98)
	{
		//apex
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}
	if (t_values[i]>0.38 &&t_values[i]<0.62)
	{
		//middle
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
}

void WidthCalculator::CheckAngle(const int &angle_)
{
	//closer to 0 = easier
	//closer to 90 =harder
	auto a = angle_ * 90;
	a /= 100;							//gives %age
	if (a<50)
	{
		width_m.modi_left += 0.01;
		width_m.modi_right += 0.01f;
	}
	else
	{
		width_m.modi_left -= 0.01;
		width_m.modi_right -= 0.01f;
	}}

void WidthCalculator::TrackLoop(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points, const std::vector<sf::Vector2i>& points_pos, const std::vector<int>& lengths_, const std::vector<int> angles_)
{
	int iter_points=0;
	auto next_point=sf::Vector2i(0,0);
	auto current_point = points_pos[iter_points];

	int iter_control_points = 0;
	auto next_control_point = sf::Vector2i(0, 0);
	auto current_control_point = points_pos[iter_points];
	auto count = 0;
	for (const sf::Vector2i& i : track_points)
	{
		if (i == points_pos[iter_points])			//finds what point the trackpoint is on
		{
			CheckPoints(point_inc_, iter_points, 60);
			current_point = i;
			iter_points++;							
			if (iter_points < points_pos.size())
			{
				next_point = points_pos[iter_points];
			}
		}
		if (i == control_points[iter_control_points])			//finds what point the trackpoint is on
		{
			CheckPoints(cp_inc_, iter_control_points, 30);
			if (iter_control_points<control_points.size())
			{
				CheckLength(lengths_, iter_control_points);
				if (iter_control_points < (control_points.size()-2))
				{
					CheckAngle(angles_[iter_control_points]);
				}
			}
			current_control_point = i;
			iter_control_points++;
			if (iter_control_points < control_points.size())
			{
				next_control_point = control_points[iter_control_points];
			}
		}

		CheckTValues(count);
		count++;

		//now use width modifiers to calc the width
	}
}

void WidthCalculator::FindWidth(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points, const std::vector<sf::Vector2i>& points_pos, const std::vector<int>& lengths_, const std::vector<int> angles_)
{
	width_m.max_m_left = 1.50f;
	width_m.max_m_right = 1.50f;
	width_m.min_m_left = 0.5f;
	width_m.min_m_right = 0.5f;
	width_m.modi_left = 1.0f;
	width_m.modi_right = 1.0f;
	min_width = 1.0f;

	track_surface = 1;

	auto sum_=0;
	for (size_t i = 0; i < lengths_.size(); i++)
	{
		sum_ += lengths_[i];
	}
	average_length = sum_ / lengths_.size();

	switch (track_surface)
	{
	case 1:						//dirt		//not fixed
		default_width = 2;
		CompareHeights(track_max,track_min);
		TrackLoop(track_points,control_points, points_pos,lengths_, angles_);
		break;
	case 0:						//tarmac		//fixed width unless constraint
		default_width = 3;
		CompareHeights(track_max, track_min);
		break;
	}

}