#include "ShortestPath.h"
#include <algorithm>

ShortestPath::ShortestPath()
{
	found_end = false;				//start as false;
	count_holder_ = 0;
	it = 0;
	end_ = 0;					//start as false;
	x_holder_=0, y_holder_=0;
	failed_ = false;
	total_track_distance = 0;
	number_of_turns = 0;

	do_testing_ = false;
}

//i =z/y, j=x
//if a position isnt = to zero or 700 then set it to -2 which signals a blocked path
//else set to -1 which is a possible path
void ShortestPath::Initgrid(int grid_size, int* grid, int num_points)
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

	north_site=0;
	north_e_site = 0;
	north_w_site = 0;
	west_site = 0;
	east_site = 0;
	south_site = 0;
	south_e_site = 0;
	south_w_site = 0;
	segment_lengths_.clear();
	first_position.first = 0;
	first_position.second = 0;
	line_positions.clear();
	angles_.clear();
	
		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (grid[(i * grid_size) + j]==0)
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
				for (int p = 0; p < (num_points-1); p++)
				{
				if(grid[(i * grid_size) + j] == 2001 + p)
				{
					if (do_testing_)
					{
						old_num.push_back(grid[(i * grid_size) + j]);
					}
					grid[(i * grid_size) + j] = start - p;				//every other position
				}
				}
				if(grid[(i * grid_size) + j]>0 )
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
void ShortestPath::PhaseOne(int grid_size, int* grid, int count_holder, bool found_end, int it, bool end, int x_holder, int y_holder, int end_n, bool b_failed_, int start_p, int end_p)
{
	count_holder = 0;
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
						int& south = grid[((y+1) * grid_size) + x];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + x] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_= it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x;
							break;

						}
					}

					//checks the soth east cell
					if (y < grid_size - 1 && x < grid_size - 1)
					{
						int& south = grid[((y + 1) * grid_size) + (x+1)];	//creating reference to the point, used for direction
						if (south == -1)
						{
							grid[((y + 1) * grid_size) + (x+1)] = it + 1;
							//south = it + 1;			//
							found_empty = true;
						}
						else if (south == end_n)
						{
							found_end = true;		//if the east cell is -3 then this is the end cell. set the bool to true, exiits loop
							count_holder_ = it;		//holds the it value at -3
							y_holder_ = y + 1;
							x_holder_ = x+1;
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
		if (it>1000)
		{
			std::cout << "hit the break in phase 1\n";
			failed_ = true;
			break;
		}

		it++;
	}
}

//passes in the current point, finds it and then changes it to the new number
void ShortestPath::ChangePoint(int grid_size, int* grid, int point, int new_point)
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

void ShortestPath::PrintOutStartEnd(int grid_size, int* grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] == 0)
			{
				int s = 2;
				std::cout << "first x "<< j <<" y " << i; std::cout << std::endl;
				start_p.first = j;
				start_p.second = i;
			}
			if (grid[(i * grid_size) + j] == -3)
			{
				int s = 2;
				std::cout << "second x " << j << " y " << i; std::cout << std::endl;
				//end_p.first = j;
				//end_p.second = i;
			}
			if (grid[(i * grid_size) + j] == -4)
			{
				int s = 0;
				end_p.first = j;
				end_p.second = i;
				std::cout << "third x " << j << " y " <<i; std::cout << std::endl;
			}
		}
	}
}

//removes all numbers found by the initial phase, so that the method can run again and be used to find another path
void ShortestPath::CleanGrid(int grid_size, int* grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j]>0)
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

//same here remove the north and all that
//from phase one you use the x and y holder vars and count holder and end
void ShortestPath::PhaseTwo(int grid_size, int* grid, bool end, int x_holder, int y_holder, int count_holder, int end_n)
{
	bool found_start = false;

	if (do_testing_)
	{
		first_position.first = x_holder;
		first_position.second = y_holder;
	}
	while (!found_start && !end)
	{
		int how_many = 0;
		if (count_holder != 0)
		{
			if (do_testing_)
			{
				if (unique_count_occuarances == 2 || unique_count_occuarances == 0)		//so if the previous position is not 1 or 3 then find new but if not dont. 
				{
					north_site = old_num[((y_holder - 1) * grid_size) + (x_holder)];
					north_e_site = old_num[((y_holder - 1) * grid_size) + (x_holder + 1)];
					north_w_site = old_num[((y_holder - 1) * grid_size) + (x_holder - 1)];
					west_site = old_num[(y_holder * grid_size) + (x_holder - 1)];
					east_site = old_num[(y_holder * grid_size) + (x_holder + 1)];
					south_site = old_num[((y_holder + 1) * grid_size) + (x_holder)];
					south_e_site = old_num[((y_holder + 1) * grid_size) + (x_holder + 1)];
					south_w_site = old_num[((y_holder + 1) * grid_size) + (x_holder - 1)];

					old_occurances.push_back(north_site);
					old_occurances.push_back(north_e_site);
					old_occurances.push_back(north_w_site);
					old_occurances.push_back(west_site);
					old_occurances.push_back(east_site);
					old_occurances.push_back(south_site);
					old_occurances.push_back(south_e_site);
					old_occurances.push_back(south_w_site);


				}
			}

			int& north = grid[((y_holder - 1) * grid_size) + x_holder];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& northE = grid[((y_holder - 1) * grid_size) + (x_holder + 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& east = grid[(y_holder * grid_size) + (x_holder + 1)];		//east 
			int& south = grid[((y_holder + 1) * grid_size) + x_holder];
			int& southE = grid[((y_holder + 1) * grid_size) + (x_holder + 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& southW = grid[((y_holder + 1) * grid_size) + (x_holder - 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			int& west = grid[(y_holder * grid_size) + (x_holder - 1)];	//west
			int& northW = grid[((y_holder - 1) * grid_size) + (x_holder - 1)];		//setting a reference that is used which holds the north position in the gridArray which will incriment if the value is equal to the value in the countHolder var, the current path of the route.
			if (north == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder -= 1;
				count_holder -= 1;
				grid[(y_holder * grid_size) + x_holder]=-12303;
				how_many++;
				total_track_distance++;
			}
			else if (northE == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder -= 1;
				x_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				how_many++;
				total_track_distance++;
			}
			else if (east == count_holder)
			{
				x_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				//			break;
				how_many++;
				total_track_distance++;
			}
			else if (south == count_holder)
			{
				y_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				how_many++;
				total_track_distance++;
			}
			else if (southE == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				x_holder += 1;
				y_holder += 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				how_many++;
				total_track_distance++;
			}
			else if (southW == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder += 1;
				x_holder -= 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				how_many++;
				total_track_distance++;
			}
			else if (west == count_holder)
			{
				x_holder -= 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				total_track_distance++;
				how_many++;
			}
			else if (northW == count_holder)		//incrimenting the coordinate, pushing back into the list to display the path, incrimenting the current route.
			{
				y_holder -= 1;
				x_holder -= 1;
				grid[(y_holder * grid_size) + x_holder] = -12303;
				count_holder -= 1;
				how_many++;
				total_track_distance++;
			}


			if (y_holder==0 ||x_holder==0)
			{
				failed_ = true;
				break;
			}
			if (do_testing_)
			{


				north_e_site = old_num[((y_holder - 1) * grid_size) + (x_holder + 1)];
				north_w_site = old_num[((y_holder - 1) * grid_size) + (x_holder - 1)];
				west_site = old_num[(y_holder * grid_size) + (x_holder - 1)];
				east_site = old_num[(y_holder * grid_size) + (x_holder + 1)];
				south_site = old_num[((y_holder + 1) * grid_size) + (x_holder)];
				south_e_site = old_num[((y_holder + 1) * grid_size) + (x_holder + 1)];
				south_w_site = old_num[((y_holder + 1) * grid_size) + (x_holder - 1)];

				occurances.push_back(north_site);
				occurances.push_back(north_e_site);
				occurances.push_back(north_w_site);
				occurances.push_back(west_site);
				occurances.push_back(east_site);
				occurances.push_back(south_site);
				occurances.push_back(south_e_site);
				occurances.push_back(south_w_site);



				//this removes all numbers that arent site numbers - eg possible tracks or start/end positions
				old_occurances.erase(std::remove(old_occurances.begin(), old_occurances.end(), 0), old_occurances.end());
				old_occurances.erase(std::remove(old_occurances.begin(), old_occurances.end(), 2000), old_occurances.end());
				old_occurances.erase(std::remove(old_occurances.begin(), old_occurances.end(), 2001), old_occurances.end());
				old_occurances.erase(std::remove(old_occurances.begin(), old_occurances.end(), 2002), old_occurances.end());
				old_occurances.erase(std::remove(old_occurances.begin(), old_occurances.end(), 2003), old_occurances.end());
				old_occurances.erase(std::remove(old_occurances.begin(), old_occurances.end(), 2004), old_occurances.end());

				occurances.erase(std::remove(occurances.begin(), occurances.end(), 0), occurances.end());
				occurances.erase(std::remove(occurances.begin(), occurances.end(), 2000), occurances.end());
				occurances.erase(std::remove(occurances.begin(), occurances.end(), 2001), occurances.end());
				occurances.erase(std::remove(occurances.begin(), occurances.end(), 2002), occurances.end());
				occurances.erase(std::remove(occurances.begin(), occurances.end(), 2003), occurances.end());
				occurances.erase(std::remove(occurances.begin(), occurances.end(), 2004), occurances.end());

				//sorts in order and finds how many sites
				std::sort(occurances.begin(), occurances.end());
				unique_count_occuarances = std::unique(occurances.begin(), occurances.end()) - occurances.begin();

				std::sort(old_occurances.begin(), old_occurances.end());
				unique_count_old_occuarances = std::unique(old_occurances.begin(), old_occurances.end()) - old_occurances.begin();

				//if all of them are == to 2 then check for corner - could this not just be if ==2?
				if (unique_count_occuarances == 2 && unique_count_old_occuarances == 2)
				{
					//find the min and max values to see if there is a difference in either one which signals a change in the sites bordering the location
					auto it = minmax_element(std::begin(occurances), std::end(occurances));
					auto it_old = minmax_element(std::begin(old_occurances), std::end(old_occurances));


					if (*it.first != *it_old.first || *it.second != *it_old.second)
					{
						number_of_turns++;

						segment_lengths_.push_back(DistanceSqrt(first_position.first, first_position.second, x_holder, y_holder));
						line_positions.emplace_back(first_position.first, first_position.second);
						line_positions.emplace_back(x_holder, y_holder);
						first_position.first = x_holder;
						first_position.second = y_holder;

					}
					old_occurances.clear();										//clear the vectors so that when it comes to checking a new poosition theres nothjing there
					occurances.clear();
				}
				else
				{
					occurances.clear();						//only clear the new positions sites not the old if there are not 2 sites in it - you want to preserve the old position because this is where a change begins	
				}
			}
			if (how_many == 0)
			{
				break;
				std::cout << "hit how many phase 2\n";
			}
		}
		
		

		if (count_holder<=end_n)
		{
			if (do_testing_)
			{
				segment_lengths_.push_back(DistanceSqrt(x_holder, y_holder, first_position.first, first_position.second));			//finds the length of the final segment 
				line_positions.emplace_back(first_position.first, first_position.second);
				line_positions.emplace_back(x_holder, y_holder);

				number_of_segments = segment_lengths_.size();
			}
			found_start = true;
			end = true;
		}
	}
}

void ShortestPath::SegmentAngles()
{
	//theta  = inverse tan((m2-m1)/(1+m2*m1)) where m1=(y2-y1)/(x2-x1), m2=(y4-y3)/(x4-x3) where x2,y2==x3,y3
	//for each turn?
	int line_iterator = 0;
	for (int i = 0; i < number_of_turns; i++)
	{
		float m1 = 0;
		float m2 = 0;
		float y1, y2, y3, y4;
		float x1, x2, x3, x4;
		y1 = line_positions[line_iterator].second;
		y2 = line_positions[line_iterator + 1].second;
		y3 = y2;
		y4 = line_positions[line_iterator + 3].second;
		x1 = line_positions[line_iterator].first;
		x2 = line_positions[line_iterator + 1].first;
		x3 = x2;
		x4 = line_positions[line_iterator + 3].first;

		if ((y2 - y1)==0 || (x2 - x1)==0)								//if the result of y2-1 or x2-x1 is going to be 0 - set the gradient(m1) to 0 because you cant divide by 0
		{
			m1 = 0;
			if ((y4 - y3) == 0 || (x4 - x3) == 0)					//check if m2 is going to be 0 and set to 0
			{
				m2 = 0;
			}
			else
			{
				m2 = (y4 - y3) / (x4- x3);							//or just calculate the m normally
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
		angles_.push_back(180.0f-angle);								//have to find the angle at this point instead
		
		line_iterator += 2;			//go to next set of lines
	}

}



void ShortestPath::WriteToFile(int track_max, int track_min)
{
	SegmentAngles();
	std::ofstream results_;

	std::string s = std::to_string(number);
	
	char const* c = ".txt";
	s += c;
	char const* pchar = s.c_str();
	results_.open(pchar );
	number++;

	


	//write like this
	results_<< "total length: "<< total_track_distance<< "\n";
	results_ << "number of turns: " << number_of_turns << "\n";
	results_ << "number of segments: " << segment_lengths_.size() << "\n";
	results_ << "shortest distance(euclidean distance): " << DistanceSqrt(start_p.first, start_p.second, end_p.first, end_p.second) << "\n";
	results_ << "\n";
	for (int i = 0; i < segment_lengths_.size(); i++)
	{
		results_ << "length " << i+1 << ": " << segment_lengths_[i] << "\n";
	}
	results_ << "\n";
	for (int i = 0; i < number_of_turns; i++)
	{
		results_ << "angle " << i+1 << ": " << angles_[i]<<"\n";
	}
	results_.close();
}
