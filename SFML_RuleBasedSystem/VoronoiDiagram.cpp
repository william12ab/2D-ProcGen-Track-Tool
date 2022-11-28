#include "VoronoiDiagram.h"
#include <iostream>
#include <ppl.h>


using namespace concurrency;

#include <SFML/Graphics.hpp>
#include <random>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using the_clock = std::chrono::steady_clock;
VoronoiDiagram::VoronoiDiagram()
{
	type = 1;
	grid_size_x = 0;
	num_of_points = 0;
	num_of_sites = 0;
	grid_v_1 = nullptr;
	sites_v_1 = nullptr;
	grid_distance = nullptr;
	failed_ = false;
	srand(static_cast <unsigned> (time(0)));
	max_distance_ = 0;
	site_iterator = 0;
	high_point = 0;
	high_point_v = sf::Vector2i(0, 0);
	low_point_v = sf::Vector2i(0, 0);
	found_raidus = false;
	radius_length = 0;
	do_testing_ = false;
	track_max = 0;
	track_min = 0;
	circum_points.resize(2);
	
}

VoronoiDiagram::~VoronoiDiagram()
{
	delete[] grid_v_1;
	delete[] sites_v_1;				//crashes on this line 
	delete[] grid_distance;
}

void VoronoiDiagram::InitVector(int grid_size, int num_points, int num_sites)
{
	grid_size_x = grid_size;
	grid_v_1 = new int[grid_size_x * grid_size_x];
	grid_distance = new int[grid_size_x * grid_size_x];
	sites_v_1 = new int[num_sites * 2];
}

void VoronoiDiagram::ResizeGrid(int grid_size, float scale)
{
	size_t new_size = grid_size * scale;
	int* temp_arr = new int[new_size * new_size];

	//resizing grid
	std::copy(grid_v_1, grid_v_1 + (grid_size * grid_size), temp_arr + 0);
	grid_size = new_size;
	delete[] grid_v_1;
	grid_v_1 = temp_arr;
}

void VoronoiDiagram::UpScaleGrid(int grid_size, float scale)
{
	int new_size = grid_size * scale;
	int* new_arr = new int[new_size * new_size];

	//initial positions
	parallel_for(0, grid_size, [&](int i)
		{
			for (int j = 0; j < (grid_size); j++)										//x
			{

				int x_dash = j * new_size / grid_size;
				int y_dash = i * new_size / grid_size;
				new_arr[x_dash * new_size + y_dash] = grid_v_1[i * grid_size + j];

			}
		});

	//coloums
	parallel_for(0, new_size, [&](int i)
		{
			for (int j = 0; j < (new_size - 1); j += scale)
			{
				if (j > 400)
				{
					int d = 2;
				}
				int c = grid_v_1[i / (int)scale * grid_size + j / (int)scale];
				for (int g = 0; g < scale; g++)
				{
					new_arr[(i * new_size) + (j + g)] = c;
				}
			}
		});

	//////rows - same for rows

	for (int i = 0; i < (new_size - 1); i += scale)											//y
	{
		parallel_for(0, new_size, [&](int j)
			{
				int c = grid_v_1[i / (int)scale * grid_size + j / (int)scale];
				for (int g = 0; g < scale; g++)
				{

					new_arr[(i + g) * new_size + j] = c;
				}
			});
	}

	//re sizing the grid
	ResizeGrid(grid_size, scale);

	parallel_for(0, new_size, [&](int i)
		{
			for (int j = 0; j < (new_size); j++)										//x
			{
				grid_v_1[i * new_size + j] = new_arr[i * new_size + j];

			}
		});
}

void VoronoiDiagram::RandomPlaceSites(int num_sites, int grid_size)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, grid_size);
	//loop over the number of sites and push back sites
	for (int i = 0; i < (num_sites * 2); i++)
	{
		sites_v_1[i] = distribution(generator);
		//sites_v_1[i] = rand() % grid_size;
	}
}

void VoronoiDiagram::EqaullyDispursSites(int num_sites, int grid_size, int times_, int displacement)
{
	srand(time(NULL));

	int sqrt_sites = sqrt(num_sites);							//gives the sqrt of the number of sites

	int spacing_ = grid_size / sqrt_sites;						//spacing for all spaces of sites

	int x_spacing = spacing_;									//x axis spacing: |.|.|.|.|
	int y_spacing = spacing_;									//y axis spacing: that but vertical

	int x_modifier = x_spacing / 2;								//modifiers for axies, so centres the point in middle of division as seen above, other wise would be: | | | | |
	int y_modifier = y_spacing / 2;

	int site_iter = 0;											//index for sites
	for (int i = 0; i < sqrt_sites; i++)
	{
		for (int j = 0; j < sqrt_sites; j++)
		{
			sites_v_1[site_iter] = (x_spacing - x_modifier);					//sets x
			site_iter++;
			sites_v_1[site_iter] = (y_spacing - y_modifier);						//sets y
			site_iter++;
			x_spacing += spacing_;												//add to spacing
		}
		y_spacing += spacing_;													//y spacing
		x_spacing = spacing_;													//reset spacing
	}


	//setting up generator and distributor, see <random> for more info or read the part on it. this engine and distributor were selected as they work best for this.
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(-displacement, displacement);


	for (int t = 0; t < times_; t++)
	{
		for (int i = 0; i < (num_sites * 2); i++)
		{
			sites_v_1[i] += distribution(generator);
			if (sites_v_1[i] >= grid_size)									//if bigger than the max res then max res - the amount bigger than = new pos
			{
				int difference = sites_v_1[i] - grid_size;
				sites_v_1[i] = grid_size - difference;
			}
			if (sites_v_1[i] <= 0)											//if less than, new pos = amount less than but positive
			{
				sites_v_1[i] += (-sites_v_1[i] - sites_v_1[i]);
			}
			i++;
			sites_v_1[i] += distribution(generator);
			if (sites_v_1[i] >= grid_size)
			{
				int difference = sites_v_1[i] - grid_size;
				sites_v_1[i] = grid_size - difference;
			}
			if (sites_v_1[i] <= 0)
			{
				sites_v_1[i] += (-sites_v_1[i] - sites_v_1[i]);
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
void VoronoiDiagram::DiagramAMP(int num_sites, int grid_size)
{
	max_distance_ = 0;
	int* incr;
	incr = new int[num_sites];
	for (int i = 0; i < num_sites; i++)
	{
		incr[i] = i + 1;
	}

	parallel_for(0, grid_size, [&](int j)
		{
			for (int i = 0; i < grid_size; i++)
			{
				int ind = -1, dist = INT_MAX;
				int s = 0;
				int d = 0;
				for (int p = 0; p < num_sites; p++)
				{
					d = DistanceSqrt(sites_v_1[s], sites_v_1[s + 1], i, j);

					s += 2;
					if (d < dist)
					{
						dist = d;
						ind = p;
					}
				}
				//so if this point has a distance which all points do
				if (ind > -1)
				{
					int s = grid_v_1[(j * grid_size) + i];
					int p = incr[ind];
					grid_v_1[(j * grid_size) + i] = incr[ind];
					if (dist > max_distance_)
					{
						max_distance_ = dist;
					}
					grid_distance[(j * grid_size) + i] = dist;
				}
			}
		});
	delete[] incr;
}


//loop over all points
//the out if statements are to check if the index is still in bounds of the grid_size in x and y direction
//j =y/z, i=x
//at this stage: all positions are equal to a number representing a site
//so if theres 25 sites, each position will be 1-25 
void VoronoiDiagram::SetEdges(int grid_size)
{
	for (int j = 0; j < grid_size; j++)
	{
		for (int i = 0; i < grid_size; i++)
		{
			if (i + 1 < grid_size && j + 1 < grid_size)		//if in the bounds
			{
				if (grid_v_1[(j * grid_size) + i] != grid_v_1[(j * grid_size) + (i + 1)])		//if the current pos and pos 1 to the left are not the same
				{
					grid_v_1[(j * grid_size) + i] = 0;			//set to path way
					//here you could find what the sites bordering are
				}
				else if (grid_v_1[(j * grid_size) + i] != grid_v_1[((j + 1) * grid_size) + i])
				{
					grid_v_1[(j * grid_size) + i] = 0;
				}
			}
		}
	}
}

void VoronoiDiagram::SetPointModi(int& x, int&x_2, int& y, int&y_2, int grid_size, float x_v_1, float x_v_2, float y_v_1, float y_v_2)
{
	x = (grid_size * x_v_1);
	x_2 = (grid_size * x_v_2);
	y = (grid_size * y_v_1);
	y_2 = (grid_size * y_v_2);
}

void VoronoiDiagram::PlacePoint(int x, int y, int grid_size, int i, bool& found_)
{
	if (grid_v_1[(y * grid_size) + x] == 0)
	{
		found_ = true;
		grid_v_1[(y * grid_size) + x] = 2000 + i;
	}
}

void VoronoiDiagram::ThreePoints(int grid_size, int num_points, bool& b_failed, float values_[12])
{
	int x_pos_one;
	int x_pos_two;
	int y_pos_one;
	int y_pos_two;
	SetPointModi(x_pos_one, x_pos_two, y_pos_one, y_pos_two, grid_size, values_[0], values_[1], values_[2], values_[3]);
	for (int i = 0; i < num_points; i++)
	{
		bool found = false;
		int counter = 0;
		while (!found)
		{
			int x = rand() % x_pos_one + x_pos_two;
			int y = rand() % y_pos_one + y_pos_two;
			PlacePoint(x, y, grid_size, i, found);
			counter++;
			if (counter > 200)
			{
				b_failed = true;
				break;
				std::cout << "didnt set a point\n";
			}
		}
		if (i == 0)
		{
			SetPointModi(x_pos_one, x_pos_two, y_pos_one, y_pos_two, grid_size, values_[4], values_[5], values_[6], values_[7]);
		}
		if (i == 1)
		{
			SetPointModi(x_pos_one, x_pos_two, y_pos_one, y_pos_two, grid_size, values_[8], values_[9], values_[10], values_[11]);
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
void VoronoiDiagram::SetPoint(int grid_size, int num_points, int type, bool b_failed)
{
	//zero is iother, 1 is p2p,2 loop
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution((grid_size / 16), (int)(grid_size / 1.1));
	switch (type)
	{
	case 0:
	{
		float arr[12] = { 0.3f,0.05f,0.2f,0.75f, 0.2f,0.45f,0.4f,0.05f, 0.2f,0.75f,0.2f,0.75f };
		ThreePoints(grid_size, num_points, b_failed, arr);
	}
	break;
	case 1:
	{
		int iter = grid_size / num_points;
		iter -= (iter) / 2;
		int start = 1;
		int position = 0;
		for (int i = 0; i < num_points; i++)
		{
			bool found = false;
			if (start + iter > grid_size)
			{
				int difference_ = (start + iter) - grid_size;
				start -= difference_;
			}
			int counter = 0;

			while (!found)
			{
				counter++;
				//so first is between 0 and grid_size/numpoints, second is iter and iter+iter, etc
				int x = rand() % iter + start;
				int y = distribution(generator);
				PlacePoint(x, y, grid_size, i, found);
				if (counter > 200)
				{
					failed_ = true;
					break;
					std::cout << "didnt set a point\n";
				}
			}
			iter = grid_size / num_points;
			start += iter;
		}
	}
	break;
	case 2:
	{
		float arr[12] = { 0.05f,0.15f,0.2f,0.4f, 0.2f,0.4f,0.15f,0.15f, 0.05f,0.80f,0.2f,0.4f };
		ThreePoints(grid_size, num_points, b_failed, arr);
	}
	break;
	}
}

void VoronoiDiagram::SetHighPoint(int grid_size, int layers_, int* noise_grid, sf::Vector2i& high_point_v_, int& high_point_, int i, int j, int &min_height, sf::Vector2i &low_point_v_)
{
	int temp_height = noise_grid[(i * grid_size) + j] / layers_;
	if (temp_height > high_point)
	{
		high_point_ = temp_height;
		high_point_v_.x = j;
		high_point_v_.y = i;
	}
	if (temp_height< min_height)
	{
		min_height = temp_height;
		low_point_v_.x = j;
		low_point_v_.y = i;
	}
}

void VoronoiDiagram::FindMax(int grid_size, int layers_, int* noise_grid)
{
	high_point = 0;
	high_point_v = sf::Vector2i(0, 0);
	low_point_v= sf::Vector2i(0, 0);
	found_raidus = false;
	radius_length = 0;

	int min_height=10000;
	//if circles vector is empty then just find the max - meaning that no peaks exist currently so no  worries about checking for other peaks
	//if there are peaks, check if the current point is within a peak and if so dont find the max
	parallel_for(0, grid_size, [&](int i)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (!circles_.empty())
				{
					bool found_in_first_circle = false;
					bool not_found_in_circle = true;
					for (int c = 0; c < circles_.size(); c++)
					{
						int r = pow(circles_[c].r_length, 2);						//radius squared
						int a = pow((j - circles_[c].centre_x), 2);				//x part squared
						int b = pow((i - circles_[c].centre_y), 2);

						if (a + b > r)		//outside the circle, so find the max
						{
							not_found_in_circle = false;
						}
						else
						{
							found_in_first_circle = true;
						}
					}
					if (!not_found_in_circle && !found_in_first_circle)				//so if not in ANY circle 
					{
						if (!found_in_first_circle)
						{
							SetHighPoint(grid_size, layers_, noise_grid, high_point_v, high_point, i, j, min_height, low_point_v);
						}
					}
				}
				else
				{
					SetHighPoint(grid_size, layers_, noise_grid, high_point_v, high_point, i, j, min_height, low_point_v);
				}
			}
		});
}

void VoronoiDiagram::SetDirectionXY(int& signal, int& x, int& y, int a,int b, int c)
{
	signal = a;
	x = b;
	y = c;
}

void VoronoiDiagram::DirectionDecider(int grid_size, int radius_cutoff_, int layers_, int index_v, int* noise_h_m, sf::Vector2i& high_or_low, bool b_what_p)
{
	//identify where x and y are in relation to the complete image
	//so are they top left, top right, bottom left, bottom right
	//then mark that as the direction to go in 
	int signal, x_pos, y_pos;
	std::cout << "		HIGHEST point(peak): " << high_or_low.x << " " << high_or_low.y << "\n\n";
	if (high_or_low.x <= (grid_size / 2) && high_or_low.y <= (grid_size / 2))
	{
		//square 1 in diagram(top left) - going south east
		SetDirectionXY(signal, x_pos, y_pos, 1, 1, 1);
	}
	else if (high_or_low.x >= (grid_size / 2) && high_or_low.x <= (grid_size) && high_or_low.y <= (grid_size / 2))
	{
		//square 2 in diagram(top right) - going south west 
		SetDirectionXY(signal, x_pos, y_pos, 2, -1, 1);
	}
	else if (high_or_low.x >= (grid_size / 2) && high_or_low.x <= (grid_size) && high_or_low.y >= (grid_size / 2) && high_or_low.y <= grid_size)
	{
		//square 4 in diagram(bottom right) - going north west
		SetDirectionXY(signal, x_pos, y_pos, 4, -1, -1);
	}
	else if (high_or_low.x <= (grid_size / 2) && high_or_low.y >= (grid_size / 2) && high_or_low.y<= grid_size)
	{
		//square 3 in diagram(bottom left) - going north east
		SetDirectionXY(signal, x_pos, y_pos, 3, 1, -1);
	}

	temp_rad.resize(2);																		
	FindCircumPoint(grid_size, x_pos, y_pos, signal, radius_cutoff_, layers_, 1, 0, noise_h_m,circum_points[0],high_or_low, b_what_p);
	FindCircumPoint(grid_size, -x_pos, -y_pos, signal, radius_cutoff_, layers_, -1, 1, noise_h_m,circum_points[1], high_or_low, b_what_p);
	radiiDecider(index_v, high_or_low);
}

void VoronoiDiagram::SetCircumPoint(sf::Vector2i& circum_point_, int x, int y, int iterator_, int place)
{
	circum_point_ = sf::Vector2i(x, y );
	std::cout << "		POINT on circumferenece: " << x << ", " << y << "\n";
	std::cout << "		RADIUS Length: " << iterator_ << "\n";
	found_raidus = true;
	temp_rad.at(place) = (iterator_);
}

void VoronoiDiagram::SwitchPoint(int& iterator, int& y_, int& x_, int signal_, int modifier_)
{
	iterator++;
	switch (signal_)
	{
	case 1:
		y_ += (1 * modifier_), x_ += (1 * modifier_);
		break;
	case 2:
		y_ += (1 * modifier_), x_ -= (1 * modifier_);
		break;
	case 3:
		y_ -= 1, x_ += 1;
		break;
	case 4:
		y_ -= (1 * modifier_), x_ -= (1 * modifier_);
		break;
	}
}

void VoronoiDiagram::FindCircumPoint(int grid_size, int x_value_, int y_value_, int signal_, int radius_cutoff_, int layers_, int modifier_, int place, int* noise_h_m, sf::Vector2i& circum_point_, sf::Vector2i& high_or_low, bool b_what_p)
{
	int y = high_or_low.y;
	int x = high_or_low.x;
	int iterator_ = 0;
	found_raidus = false;
	do
	{
		//travelled the length of the radius then set found etc
		if (y >= 0 && x >= 0 && y <= grid_size && x <= grid_size)
		{
			switch (b_what_p)
			{
			case true:
				if ((noise_h_m[((y + y_value_) * grid_size) + (x + x_value_)] / layers_) <= (radius_cutoff_))
				{
					x += x_value_, y += y_value_;
					SetCircumPoint(circum_point_, x, y, iterator_, place);
				}
				else
				{
					SwitchPoint(iterator_, y, x, signal_, modifier_);
				}
				break;
			case false:
				if ((noise_h_m[((y + y_value_) * grid_size) + (x + x_value_)] / layers_) >= (radius_cutoff_))
				{
					x += x_value_, y += y_value_;
					SetCircumPoint(circum_point_, x, y, iterator_, place);
				}
				else
				{
					SwitchPoint(iterator_, y, x, signal_, modifier_);
				}
				break;
			}
		}
		else
		{
			SetCircumPoint(circum_point_, x, y, iterator_, place);
			std::cout << "		Point too far.\n";
		}
	} while (found_raidus != true && !failed_);
}

void VoronoiDiagram::SelectRadii(int index_v, int a, int b, sf::Vector2i& high_or_low)
{
	int p = a;
	if (temp_rad.at(a) > 200)
	{
		p = b;
	}
	radius_length = temp_rad.at(p);
	if (radius_length > 200)
	{
		radius_length = 200 - (radius_length - 200);
	}
	std::cout << "\nSelected radius: " << p << "\n";
	std::cout << "RADIUS Length: " << radius_length << "\n";
	std::cout << "Point on circumferenece: " << circum_points[p].x << ", " << circum_points[p].y << "\n\n";
	circles_[index_v].centre_x = high_or_low.x;
	circles_[index_v].centre_y = high_or_low.y;
	circles_[index_v].r_length = radius_length;
}

void VoronoiDiagram::radiiDecider(int index_v, sf::Vector2i& high_or_low)
{
	int p = 0;
	if (temp_rad.at(0) > temp_rad.at(1))					//chooses the bigger of the two but if its > 300 then chooses the smaller cos thats large
	{
		SelectRadii(index_v,0,1, high_or_low);
	}
	else
	{
		SelectRadii(index_v, 1, 0, high_or_low);
	}
}
void VoronoiDiagram::vector_all(int size)
{
	circles_.resize(size);
	circum_points.resize(2);
}


void VoronoiDiagram::TerrainSites(int num_sites, int grid_size)
{
	//loop over number of sites for x and y 
	//generate random point for site and loop over this generation until it is not within the circle

	//for every site*2 so that gives x and y pos
	//create a random coordinate
	//check if that coordinate is within ANY circle or ON ANY circle
	//if true then discard point and create new point UNTIL point is NOT in circle
	for (int i = 0; i < (num_sites * 2); i++)
	{
		bool found = false;
		while (!found)
		{
			bool false_in_first_cirlce = false;
			sites_v_1[i] = rand() % (grid_size);
			i++;
			sites_v_1[i] = rand() % (grid_size);
			for (int c = 0; c < circles_.size(); c++)
			{
				int r = pow(circles_[c].r_length, 2);						//radius squared
				int a = pow((sites_v_1[i - 1] - circles_[c].centre_x), 2);				//x part squared
				int b = pow((sites_v_1[i] - circles_[c].centre_y), 2);					//y part squared
				if (a + b < r || a + b == r)				//the circle formula - checking whether the point exist in the circle and if it does then set the iterator back to what it was and go again
				{
					found = false;
					false_in_first_cirlce = true;			//so if this is true then when it comes to circle(n+1) the found wont trigger
				}
				else
				{
					if (!false_in_first_cirlce)
					{
						found = true;		//point is not in the circle so exit loop and create new site
					}
				}
			}
			if (false_in_first_cirlce)
			{
				i--;
			}
		}
	}

	int iterator_ = 0;
	for (int i = 0; i < circles_.size(); i++)
	{
		sites_v_1[iterator_] = circles_[i].centre_x;				//setting the first sites the the centre point of the circles
		iterator_++;
		sites_v_1[iterator_] = circles_[i].centre_y;
		iterator_++;
	}
}

void VoronoiDiagram::ResetVars()
{
	high_point = 0;
	high_point_v.x = 0;
	high_point_v.y = 0;
	low_point_v.x = 0;
	low_point_v.y = 0;
	found_raidus = false;
	radius_length = 0;
	circles_.clear();
	temp_rad.clear();
}


void VoronoiDiagram::FindMinMax(int grid_size, int layers_, int* noise_grid)
{
	int min_ = 100000;
	int max_ = 0;

	//finds the min and max of the track ---when theres terrrain
	//this is better than finding the min and max of the whole image as there will be areas where the track does not pass by
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid_v_1[(i * grid_size) + j] == 0)							//diagram
			{
				if ((noise_grid[i * grid_size + j] / layers_) > max_)
				{
					max_ = noise_grid[i * grid_size + j] / layers_;
				}

				if ((noise_grid[i * grid_size + j] / layers_) > min_)
				{
					min_ = noise_grid[i * grid_size + j] / layers_;
				}
				//find max
				//find min of noise heighmap
				//noise_heightmap_[i * grid_size + j] / layers_
			}
		}
	}
	track_max = max_;
	track_min = min_;
}