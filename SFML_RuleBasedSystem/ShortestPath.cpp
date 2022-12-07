#include "ShortestPath.h"
#include <algorithm>

#include <ppl.h>


using namespace concurrency;
std::vector<sf::Vector2i>ShortestPath::track_points(1);

ShortestPath::ShortestPath()
{
	found_end = false;				//start as false;
	count_holder_ = 0;
	it = 0;
	end_ = 0;					//start as false;
	x_holder_ = 0, y_holder_ = 0;
	failed_ = false;
	total_track_distance = 0;
	number_of_turns = 0;
	do_testing_ = true;
}

//i =z/y, j=x
//if a position isnt = to zero or 700 then set it to -2 which signals a blocked path
//else set to -1 which is a possible path
void ShortestPath::Initgrid(const int &grid_size, int* grid, const int &num_points)
{
	int start = -3;
	number_of_turns = 0;

	total_track_distance = 0;

	if (!old_num.empty())
	{
		old_num.clear();
	}
	if (!occurances.empty())
	{
		occurances.clear();
	}
	if (!old_occurances.empty())
	{
		old_occurances.clear();
	}
	unique_count_old_occuarances = 0;
	unique_count_occuarances = 0;

	track_points.clear();
	it = 0;
	x_holder_ = 0;
	y_holder_ = 0;
	end_ = false;
	north_site = 0;
	north_e_site = 0;
	north_w_site = 0;
	west_site = 0;
	east_site = 0;
	south_site = 0;
	south_e_site = 0;
	south_w_site = 0;
	segment_lengths_.clear();
	first_position.x= 0;
	first_position.y = 0;
	line_positions.clear();
	control_points.clear();
	angles_.clear();
	new_angles_.clear();
	int size_ = num_points - 1;
	for (int i = 0; i < grid_size; i++)
	{
				for (int j = 0; j < grid_size; j++)
				{
					if (grid[(i * grid_size) + j] == 0)
					{
						if (do_testing_)
						{
							old_num.push_back(grid[(i * grid_size) + j]);
						}
						grid[(i * grid_size) + j] = -1;			//possible path
					}
					if (grid[(i * grid_size) + j] == 2000)
					{
						if (do_testing_)
						{
							old_num.push_back(grid[(i * grid_size) + j]);
						}
						grid[(i * grid_size) + j] = 0;								//first position
					}
					for (int p = 0; p < (size_); p++)
					{
						if (grid[(i * grid_size) + j] == 2001 + p)
						{
							if (do_testing_)
							{
								old_num.push_back(grid[(i * grid_size) + j]);
							}
							grid[(i * grid_size) + j] = start - p;				//every other position
						}
					}
					if (grid[(i * grid_size) + j] > 0)
					{
						if (do_testing_)
						{
							old_num.push_back(grid[(i * grid_size) + j]);
						}
						grid[(i * grid_size) + j] = -2;										//non positions
					}
				}
	}
}


//you could remove the north and all that stuff from here and just check the position directly 
//fills the data structure with numbers that equal a distance from the starting point, increasing in distance apart.
//runs until the end position has been found or if it cant find the end position, run a number of times then exit the loop
//stores the end position for reference in phasse 2
void ShortestPath::PhaseOne(const int& grid_size, int* grid, int end_n, int start_p, int end_p)
{
	it = 0;
	x_holder_ = 0;
	y_holder_ = 0;
	end_ = false;
	count_holder_ = 0;
	found_end = false;
	while (!found_end)
	{
		bool found_empty = false;
		for (int y = 0; y < grid_size && !found_end; y++)
		{
			for (int x = 0; x < grid_size; x++)
			{
				if (grid[(y * grid_size) + x] == it)
				{
					if (y > 0)
					{
						int& north = grid[((y - 1) * grid_size) + x];
						if (north == -1)
						{
							grid[((y - 1) * grid_size) + x] = it + 1;
							found_empty = true;
						}
						else if (north == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x;
							y_holder_ = y - 1;
							break;
						}
					}

					//checks the nortth east cell
					if (y > 0 && x < grid_size - 1)
					{
						int& north = grid[((y - 1) * grid_size) + (x + 1)];
						if (north == -1)
						{
							grid[((y - 1) * grid_size) + (x + 1)] = it + 1;
							//north = it + 1;
							found_empty = true;
						}
						else if (north == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x + 1;
							y_holder_ = y - 1;
							break;
						}
					}

					//checks the "east" wall
					if (x < grid_size - 1)
					{
						int& east = grid[(y * grid_size) + (x + 1)];
						if (east == -1)
						{
							grid[(y * grid_size) + (x + 1)] = it + 1;
							//east = it + 1;
							found_empty = true;
						}
						else if (east == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x + 1;
							y_holder_ = y;
							break;
						}
					}

					//check the "south" cell, 
					if (y < grid_size - 1)
					{
						int& south = grid[((y + 1) * grid_size) + x];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + x] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_ = it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x;
							break;
						}
					}

					//checks the soth east cell
					if (y < grid_size - 1 && x < grid_size - 1)
					{
						int& south = grid[((y + 1) * grid_size) + (x + 1)];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + (x + 1)] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_ = it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x + 1;
							break;
						}
					}

					//checks the south west
					if (y < grid_size - 1 && x > 0)
					{
						int& south = grid[((y + 1) * grid_size) + (x - 1)];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + (x - 1)] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_ = it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x - 1;
							break;
						}
					}
					//checks "west" wall
					if (x > 0)
					{
						int& west = grid[(y * grid_size) + (x - 1)];
						if (west == -1)
						{
							grid[(y * grid_size) + (x - 1)] = it + 1;
							//west = it + 1;
							found_empty = true;
						}
						else if (west == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x - 1;
							y_holder_ = y;
							break;
						}
					}
					//checks the "north" cell

					//checks the nortth west cell
					if (y > 0 && x > 0)
					{
						int& north = grid[((y - 1) * grid_size) + (x - 1)];
						if (north == -1)
						{
							grid[((y - 1) * grid_size) + (x - 1)] = it + 1;
							//north = it + 1;
							found_empty = true;
						}
						else if (north == end_n)
						{
							found_end = true;
							count_holder_ = it;
							x_holder_ = x - 1;
							y_holder_ = y - 1;
							break;
						}
					}
				}
			}
		}

		//just in case it cant find the end position and needs to break out of the loop so that it doesnt get stuck in it
		if (it > 1000)
		{
			std::cout << "hit the break in phase 1\n";
			failed_ = true;
			break;
		}
		it++;
	}
}

//passes in the current point, finds it and then changes it to the new number
void ShortestPath::ChangePoint(const int &grid_size, int* grid, int point, int new_point)
{
	if (!failed_)
	{
		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (grid[(i * grid_size) + j] == point)
				{
					grid[(i * grid_size) + j] = new_point;
				}
			}
		}
	}

}

void ShortestPath::PrintOutStartEnd(const int& grid_size, int* const& grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] == 0)
			{
				std::cout << "first x " << j << " y " << i; std::cout << std::endl;
				start_p.x = j;
				start_p.y = i;
			}
			if (grid[(i * grid_size) + j] == -3)
			{
				std::cout << "second x " << j << " y " << i;
				std::cout << std::endl;
			}
			if (grid[(i * grid_size) + j] == -4)
			{
				end_p.x = j;
				end_p.y = i;
				std::cout << "third x " << j << " y " << i; 
				std::cout << std::endl;				
			}
			if (grid[(i * grid_size) + j] == -5)
			{
				std::cout << "fourth x " << j << " y " << i;
				std::cout << std::endl;
			}	
			if (grid[(i * grid_size) + j] == -6)
			{
				std::cout << "fith x " << j << " y " << i;
				std::cout << std::endl;
			}
		}
	}
}

//removes all numbers found by the initial phase, so that the method can run again and be used to find another path
void ShortestPath::CleanGrid(const int &grid_size, int* grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] > 0)
			{
				grid[(i * grid_size) + j] = -1;
			}
		}
	}
}

int ShortestPath::DistanceSqrt(int x, int y, int x2, int y2)
{
	int xd = x2 - x;
	int yd = y2 - y;
	return sqrt((xd * xd) + (yd * yd));
}


void ShortestPath::SetNESW(const int &grid_size, std::vector<int>& occ)
{
	north_site = old_num[((y_holder_ - 1) * grid_size) + (x_holder_)];
	north_e_site = old_num[((y_holder_ - 1) * grid_size) + (x_holder_ + 1)];
	north_w_site = old_num[((y_holder_ - 1) * grid_size) + (x_holder_ - 1)];
	west_site = old_num[(y_holder_ * grid_size) + (x_holder_ - 1)];
	east_site = old_num[(y_holder_ * grid_size) + (x_holder_ + 1)];
	south_site = old_num[((y_holder_ + 1) * grid_size) + (x_holder_)];
	south_e_site = old_num[((y_holder_ + 1) * grid_size) + (x_holder_ + 1)];
	south_w_site = old_num[((y_holder_ + 1) * grid_size) + (x_holder_ - 1)];

	occ.push_back(north_site);
	occ.push_back(north_e_site);
	occ.push_back(north_w_site);
	occ.push_back(west_site);
	occ.push_back(east_site);
	occ.push_back(south_site);
	occ.push_back(south_e_site);
	occ.push_back(south_w_site);
}


void ShortestPath::EraseVector(std::vector<int>& occ, int& unique)
{
	occ.erase(std::remove(occ.begin(), occ.end(), 0), occ.end());
	occ.erase(std::remove(occ.begin(), occ.end(), 2000), occ.end());
	occ.erase(std::remove(occ.begin(), occ.end(), 2001), occ.end());
	occ.erase(std::remove(occ.begin(), occ.end(), 2002), occ.end());
	occ.erase(std::remove(occ.begin(), occ.end(), 2003), occ.end());
	occ.erase(std::remove(occ.begin(), occ.end(), 2004), occ.end());

	std::sort(occ.begin(), occ.end());
	unique = std::unique(occ.begin(), occ.end()) - occ.begin();
}

void ShortestPath::FindCompassPoss(int compass, int* grid, int grid_size, int& how_many, int& track_d, int x, int y)
{
	if (compass == count_holder_)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
	{
		y_holder_ += y;
		x_holder_ += x;
		count_holder_ -= 1;
		grid[(y_holder_ * grid_size) + x_holder_] = -12303;
		track_points.push_back(sf::Vector2i(x_holder_, y_holder_));
		how_many++;
		track_d++;
	}
}

//same here remove the north and all that
//from phase one you use the x and y holder vars and count holder and end
void ShortestPath::PhaseTwo(const int &grid_size, int* grid,int end_n)
{
	std::vector<sf::Vector2i> temp_vec_c_p;
	bool found_start = false;
	first_position.x = x_holder_;
	first_position.y = y_holder_;
	temp_vec_c_p.emplace_back(first_position.x, first_position.y);

	while (!found_start && !end_)
	{
		int how_many = 0;						//checks for error
		if (count_holder_ != 0)				//only run if track successfully made
		{
			if (unique_count_occuarances != 3 && unique_count_occuarances != 1 && unique_count_occuarances != 4)		//so if the previous position is not 1 or 3 then find new but if not dont. 
			{
				if (unique_count_occuarances > 4)					//test code
				{
					int ur = 23;
				}
				old_occurances.clear();										//clear old ones
				SetNESW(grid_size, old_occurances);
			}
			int& north = grid[((y_holder_ - 1) * grid_size) + x_holder_];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& northE = grid[((y_holder_ - 1) * grid_size) + (x_holder_ + 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& northW = grid[((y_holder_ - 1) * grid_size) + (x_holder_ - 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& east = grid[(y_holder_ * grid_size) + (x_holder_ + 1)];		//east 
			int& south = grid[((y_holder_ + 1) * grid_size) + x_holder_];
			int& southE = grid[((y_holder_ + 1) * grid_size) + (x_holder_ + 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& southW = grid[((y_holder_ + 1) * grid_size) + (x_holder_ - 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& west = grid[(y_holder_ * grid_size) + (x_holder_ - 1)];	//west

			FindCompassPoss(north, grid, grid_size, how_many, total_track_distance, 0, -1);
			FindCompassPoss(northE, grid, grid_size, how_many, total_track_distance, 1, -1);	//x and then y
			FindCompassPoss(east, grid, grid_size, how_many, total_track_distance, 1, 0);	//x and then y
			FindCompassPoss(south, grid, grid_size, how_many, total_track_distance, 0, 1);	//x and then y
			FindCompassPoss(southE, grid, grid_size, how_many, total_track_distance, 1, 1);	//x and then y
			FindCompassPoss(southW, grid, grid_size, how_many, total_track_distance, -1, 1);	//x and then y
			FindCompassPoss(west, grid, grid_size, how_many, total_track_distance, -1, 0);	//x and then y
			FindCompassPoss(northW, grid, grid_size, how_many, total_track_distance, -1, -1);	//x and then y

			if (y_holder_ == 0 || x_holder_ == 0)
			{
				failed_ = true;
				break;
			}

			//this finds the sites in a 9x9 position from the current point
			SetNESW(grid_size, occurances);
			EraseVector(old_occurances, unique_count_old_occuarances);
			EraseVector(occurances, unique_count_occuarances);
			//test code
			if (unique_count_old_occuarances > 4 || unique_count_occuarances > 4)
			{
				int hee = 23;
			}

			//if all of them are == to 2 then check for corner 
			if (unique_count_occuarances == 2 && unique_count_old_occuarances == 2)
			{
				//find the min and max values to see if there is a difference in either one which signals a change in the sites bordering the location
				auto it = minmax_element(std::begin(occurances), std::end(occurances));
				auto it_old = minmax_element(std::begin(old_occurances), std::end(old_occurances));

				if (*it.first != *it_old.first || *it.second != *it_old.second)						//checks if theres a difference hence a new cell.
				{
					number_of_turns++;
					segment_lengths_.push_back(DistanceSqrt(first_position.x, first_position.y, x_holder_, y_holder_));			//finds length of segment
					line_positions.emplace_back(first_position.x, first_position.y);											//pushes back first position of segment
					line_positions.emplace_back(x_holder_, y_holder_);															//pushes second position
					temp_vec_c_p.emplace_back(x_holder_, y_holder_);															//current c.p
					first_position.x = x_holder_;
					first_position.y= y_holder_;
				}
				old_occurances.clear();										//clear the vectors so that when it comes to checking a new poosition theres nothjing there
				occurances.clear();
			}
			else
			{
				occurances.clear();						//only clear the new positions sites not the old if there are not 2 sites in it - you want to preserve the old position because this is where a change begins	
			}

			if (how_many == 0)
			{
				break;
				std::cout << "hit how many phase 2\n";
			}
		}

		if (count_holder_ <= end_n)
		{
			segment_lengths_.push_back(DistanceSqrt(x_holder_, y_holder_, first_position.x, first_position.y));			//finds the length of the final segment 
			line_positions.emplace_back(first_position.x, first_position.y);					//final segment coords
			line_positions.emplace_back(x_holder_, y_holder_);									//last coord
			temp_vec_c_p.emplace_back(x_holder_, y_holder_);						//last c.p
			number_of_segments = segment_lengths_.size();
			found_start = true;
			end_ = true;
		}
	}

	//re orderers the vector to follow thje correct order - also fixes issue with multiple points.
	std::reverse(temp_vec_c_p.begin(), temp_vec_c_p.end());
	std::reverse(line_positions.begin(), line_positions.end());
	std::reverse(segment_lengths_.begin(), segment_lengths_.end());
	for (int i = 0; i < temp_vec_c_p.size(); i++)
	{
		control_points.emplace_back(temp_vec_c_p[i]);
	}
}

void ShortestPath::SortControlPoints()
{
	for (int i = 0; i < control_points.size() - 1; i++)
	{
		if (control_points[i].x == control_points[i + 1].x && control_points[i].y == control_points[i + 1].y)
		{
			control_points.erase(control_points.begin() + i);
		}
	}
}

void ShortestPath::ScaleControlPoints(float scale)
{
	for (int i = 0; i < control_points.size(); i++)
	{
		control_points.at(i).x *= scale;
		control_points.at(i).y *= scale;
	}
}


void ShortestPath::OrderControlPoints()
{
	for (int i = 0; i < control_points.size(); i++)
	{
		std::cout << " x " << control_points.at(i).x << " y " << control_points.at(i).y << "\n";
	}

}

void ShortestPath::SegmentAngles()
{
	//theta  = inverse tan((m2-m1)/(1+m2*m1)) where m1=(y2-y1)/(x2-x1), m2=(y4-y3)/(x4-x3) where x2,y2==x3,y3
	//for each turn?
	int line_iterator = 0;
	for (int i = 0; i < number_of_turns; i++)
	{
		float m1 = 0;				//gradient
		float m2 = 0;				//gradient
		float y1, y2, y3, y4;
		float x1, x2, x3, x4;
	//	if (segment_lengths_[i] > 5)
		//{
			y1 = line_positions[line_iterator].y;
			y2 = line_positions[line_iterator + 1].y;
			y3 = y2;
			y4 = line_positions[line_iterator + 3].y;
			x1 = line_positions[line_iterator].x;
			x2 = line_positions[line_iterator + 1].x;
			x3 = x2;
			x4 = line_positions[line_iterator + 3].x;

			if ((y2 - y1) == 0 || (x2 - x1) == 0)								//if the result of y2-1 or x2-x1 is going to be 0 - set the gradient(m1) to 0 because you cant divide by 0
			{
				m1 = 0;
				if ((y4 - y3) == 0 || (x4 - x3) == 0)					//check if m2 is going to be 0 and set to 0
				{
					m2 = 0;
				}
				else
				{
					m2 = (y4 - y3) / (x4 - x3);							//or just calculate the m normally
				}
			}
			else if ((y4 - y3) == 0 || (x4 - x3) == 0)					//same here
			{
				m2 = 0;
				if ((y2 - y1) == 0 || (x2 - x1) == 0)
				{
					m1 = 0;
				}
				else
				{
					m1 = (y2 - y1) / (x2 - x1);
				}
			}
			else
			{
				m1 = (y2 - y1) / (x2 - x1);								//or calculat normally		
				m2 = (y4 - y3) / (x4 - x3);


			}
			float angle = atanf((m1 - m2) / (1 + (m2 * m1)));			//angle in rads

			angle = angle * (180.0 / 3.141592653589793238463);			//angle as degrees because its easier to understand
			angles_.push_back(180.0f - angle);								//have to find the angle at this point instead
			float new_angle = 360 - angles_.back();
			float final_angle = new_angle - 180;
			if (final_angle < 0)
			{
				final_angle *= -1;
			}
			new_angles_.push_back(final_angle);
		//}
		/*else
		{
			int dfdfgdgd = 1;
		}*/
		line_iterator += 2;			//go to next set of lines
	}

}

void ShortestPath::FindDirection()
{

}

void ShortestPath::WriteToFile(int track_max, int track_min)
{
	SegmentAngles();
	std::ofstream results_;

	std::string s = std::to_string(number);

	char const* c = ".txt";
	s += c;
	char const* pchar = s.c_str();
	results_.open(pchar);
	number++;

	//write like this
	results_ << "total length: " << total_track_distance << "\n";
	results_ << "number of turns: " << number_of_turns << "\n";
	results_ << "number of segments: " << segment_lengths_.size() << "\n";
	results_ << "shortest distance(euclidean distance): " << DistanceSqrt(start_p.x, start_p.y, end_p.x, end_p.y) << "\n";
	results_ << "\n";
	for (int i = 0; i < segment_lengths_.size(); i++)
	{
		results_ << "length " << i + 1 << ": " << segment_lengths_[i] << "\n";
	}
	results_ << "\n";
	for (int i = 0; i < angles_.size(); i++)
	{
		results_ << "angle " << i + 1 << ": " << angles_[i] << "\n";
	}
	results_ << "\n";
	for (int i = 0; i < angles_.size(); i++)
	{
		results_ << "angles for turn " << i + 1 << ": " << new_angles_[i] << "\n";
	}
	results_ << "\n";
	for (int i = 0; i < control_points.size(); i++)
	{
		results_ << "Control-points " << i + 1 << ": (" << control_points[i].x << ", " << control_points[i].y<<")\n";
	}
	results_.close();
}


sf::Vector2i ShortestPath::Lerp(const sf::Vector2i& p1, const sf::Vector2i& p2, const float& t)
{
	//auto r = p1 + ((p2 - p1) * t);
	auto a = p2 - p1;
	auto b = sf::Vector2i(a.x * t,a.y*t);
	auto c = p1 + b;
	return c;
}

float ShortestPath::FindT(const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3)
{
	//t = (p1-p3)/(p1-p2)

	//have check somewhere to stop running if p1==p2

	auto a = p1 - p3;
	auto b = p1 - p2;

	float t = 0.0f;
	if (a.x==0||b.x==0)
	{
		float c2 = ((float)a.y / (float)b.y);
		t = c2;
	}
	else if (a.y==0||b.y==0)
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