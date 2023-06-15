#include "VoronoiDiagram.h"
#include <iostream>
#include <ppl.h>
using namespace concurrency;

#include <SFML/Graphics.hpp>


using std::chrono::duration_cast;
using std::chrono::milliseconds;
using the_clock = std::chrono::steady_clock;
std::vector<VoronoiDiagram::peaks_> VoronoiDiagram::circles_(1);
std::vector<sf::Vector2i> VoronoiDiagram::point_pos(1);
std::vector<VoronoiDiagram::peaks_> VoronoiDiagram::all_circles_vector(1);
VoronoiDiagram::VoronoiDiagram() {
	type = 1;
	grid_size_x = 0;
	num_of_points = 0;
	num_of_sites = 0;
	grid_v_1 = nullptr;
	grid_chunk_1 = nullptr;
	grid_chunk_2 = nullptr;
	grid_chunk_3 = nullptr;
	sites_v_1 = nullptr;
	grid_distance = nullptr;
	grid_distance_c_1 = nullptr;
	grid_distance_c_2 = nullptr;
	grid_distance_c_3 = nullptr;
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
	circum_points.resize(2);
	stop_high_ = false;
	stop_low_ = false;
	max_value_height = -1000000;
	point_pos.clear();
}

VoronoiDiagram::~VoronoiDiagram() {
	delete[] grid_v_1;
	delete[] grid_chunk_1;
	delete[] grid_chunk_2;
	delete[] grid_chunk_3;
	delete[] grid_distance;
	delete[] grid_distance_c_1;
	delete[] grid_distance_c_2;
	delete[] grid_distance_c_3;
	delete[] sites_v_1;
}
void VoronoiDiagram::ResetSitesForChunking(const int& num_of_sites_param) {
	delete[] sites_v_1;
	sites_v_1 = new int[num_of_sites_param * 2];
}

void VoronoiDiagram::InitVector(const int& grid_size, const int& num_points, const int& num_sites)
{
	grid_size_x = grid_size;
	grid_v_1 = new int[grid_size_x * grid_size_x];
	grid_chunk_1 = new int[grid_size_x * grid_size_x];
	grid_chunk_2 = new int[grid_size_x * grid_size_x];
	grid_chunk_3 = new int[grid_size_x * grid_size_x];
	grid_distance = new int[grid_size_x * grid_size_x];
	grid_distance_c_1 = new int[grid_size_x * grid_size_x];
	grid_distance_c_2 = new int[grid_size_x * grid_size_x];
	grid_distance_c_3 = new int[grid_size_x * grid_size_x];
	sites_v_1 = new int[num_sites * 2];
	InsertChunks();
	peak_.point = sf::Vector2i(0, 0);
	peak_.r_length = 0;
}

void VoronoiDiagram::SetForChunks() {
	max_distance_ = 0;
	site_iterator = 0;
	high_point = 0;
	high_point_v = sf::Vector2i(0, 0);
	low_point_v = sf::Vector2i(0, 0);
	found_raidus = false;
	radius_length = 0;
	circum_points.resize(2);
	stop_high_ = false;
	stop_low_ = false;
	max_value_height = -1000000;
	point_pos.clear();
}

void VoronoiDiagram::ResizeGrid(float scale)
{
	size_t new_size = grid_size_x * scale;
	int* temp_arr = new int[new_size * new_size];

	//resizing grid
	std::copy(grid_vector[0], grid_vector[0] + (grid_size_x * grid_size_x), temp_arr + 0);
	grid_size_x = new_size;
	delete[] grid_vector[0];
	grid_vector[0] = temp_arr;
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
				new_arr[x_dash * new_size + y_dash] = grid_vector[0][i * grid_size + j];

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
				int c = grid_vector[0][i / (int)scale * grid_size + j / (int)scale];
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
				int c = grid_vector[0][i / (int)scale * grid_size + j / (int)scale];
				for (int g = 0; g < scale; g++)
				{
					new_arr[(i + g) * new_size + j] = c;
				}
			});
	}

	//re sizing the grid
	ResizeGrid(scale);

	parallel_for(0, new_size, [&](int i)
		{
			for (int j = 0; j < (new_size); j++)										//x
			{
				grid_vector[0][i * new_size + j] = new_arr[i * new_size + j];
			}
		});
}

void VoronoiDiagram::RandomPlaceSites() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, grid_size_x);
	//loop over the number of sites and push back sites
	for (int i = 0; i < (num_of_sites * 2); i++)
	{
		sites_v_1[i] = distribution(generator);
		//sites_v_1[i] = rand() % grid_size;
	}
}

void VoronoiDiagram::EqaullyDispursSites(const int& times_, const int& displacement)
{
	srand(time(NULL));

	int sqrt_sites = sqrt(num_of_sites);							//gives the sqrt of the number of sites

	int spacing_ = grid_size_x / sqrt_sites;						//spacing for all spaces of sites

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
		for (int i = 0; i < (num_of_sites * 2); i++)
		{
			sites_v_1[i] += distribution(generator);
			if (sites_v_1[i] >= grid_size_x)									//if bigger than the max res then max res - the amount bigger than = new pos
			{
				int difference = sites_v_1[i] - grid_size_x;
				sites_v_1[i] = grid_size_x - difference;
			}
			if (sites_v_1[i] <= 0)											//if less than, new pos = amount less than but positive
			{
				sites_v_1[i] += (-sites_v_1[i] - sites_v_1[i]);
			}
			i++;
			sites_v_1[i] += distribution(generator);
			if (sites_v_1[i] >= grid_size_x)
			{
				int difference = sites_v_1[i] - grid_size_x;
				sites_v_1[i] = grid_size_x - difference;
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
void VoronoiDiagram::DiagramAMP(const int& chunk_index) {
	int local_num_sites = num_of_sites;
	int local_grid_size = grid_size_x;
	if (local_is_chunking) {
		local_num_sites = num_of_sites * 4;
		local_grid_size = grid_size_x * 2;
		full_grid_chunking = new int[local_grid_size* local_grid_size];
	}

	max_distance_ = 0;
	int* incr;
	incr = new int[local_num_sites];
	for (int i = 0; i < local_num_sites; i++) {
		incr[i] = i + 1;
	}
	//parallel_for(0, local_grid_size, [&](int j){
	for (int j = 0; j < local_grid_size; j++) {
		for (int i = 0; i < local_grid_size; i++) {
			int ind = -1, dist = INT_MAX;
			int s = 0;
			int d = 0;
			for (int p = 0; p < local_num_sites; p++) {
				d = DistanceSqrt(sites_v_1[s], sites_v_1[s + 1], i, j);
				s += 2;
				if (d < dist) {
					dist = d;
					ind = p;
				}
			}
			//so if this point has a distance which all points do
			if (ind > -1) {
				//grid_vector[chunk_index][(j * local_grid_size) + i] = incr[ind];
				if (dist > max_distance_) {
					max_distance_ = dist;
				}
				//distance_grid_vector[chunk_index][(j * local_grid_size) + i] = dist;

				if (local_is_chunking) {
					if (i < 400 && j < 400) {
						//in [0]
						grid_vector[0][(j * grid_size_x) + i] = incr[ind];
						distance_grid_vector[0][(j * grid_size_x) + i] = dist;
					}
					else if (i >= 400 && j < 400) {
						//in [1]
						grid_vector[1][(j * grid_size_x) + (i - 400)] = incr[ind];
						distance_grid_vector[1][(j * grid_size_x) + (i - 400)] = dist;
					}
					else if (i < 400 && j >= 400) {
						//in [2]
						grid_vector[2][((j - 400) * grid_size_x) + i] = incr[ind];
						distance_grid_vector[2][((j - 400) * grid_size_x) + i] = dist;
					}
					else if (i >= 400 && j >= 400) {
						//in [3]
						grid_vector[3][((j - 400) * grid_size_x) + (i - 400)] = incr[ind];
						distance_grid_vector[3][((j - 400) * grid_size_x) + (i - 400)] = dist;
					}

					full_grid_chunking[(j * local_grid_size) + i] = incr[ind];
				}
				else {
					grid_vector[chunk_index][(j * grid_size_x) + i] = incr[ind];
					distance_grid_vector[chunk_index][(j * grid_size_x) + i] = dist;
				}

			}
		}
		//});
	}
	delete[] incr;
}

//loop over all points
//the out if statements are to check if the index is still in bounds of the grid_size in x and y direction
//j =y/z, i=x
//at this stage: all positions are equal to a number representing a site
//so if theres 25 sites, each position will be 1-25 
void VoronoiDiagram::SetEdges(const int& chunk_index) {
	if (local_is_chunking){
		int local_grid_size = grid_size_x * 2;
		for (int j = 0; j < local_grid_size; j++) {
			for (int i = 0; i < local_grid_size; i++) {
				if (i + 1 < local_grid_size) {		//if in the bounds
					if (full_grid_chunking[(j * local_grid_size) + i] != full_grid_chunking[(j * local_grid_size) + (i + 1)]) {		//if the current pos and pos 1 to right are not the same
						full_grid_chunking[(j * local_grid_size) + i] = 0;			//set to path way
						//here you could find what the sites bordering are
					}
				}
				if (j + 1 < local_grid_size) {
					if (full_grid_chunking[(j * local_grid_size) + i] != full_grid_chunking[((j + 1) * local_grid_size) + i]) {		//if current pos and pos one down are not the same
						full_grid_chunking[(j * local_grid_size) + i] = 0;
					}
				}
			}
		}
	}
	else {
		for (int j = 0; j < grid_size_x; j++) {
			for (int i = 0; i < grid_size_x; i++) {
				if (i + 1 < grid_size_x) {		//if in the bounds
					if (grid_vector[chunk_index][(j * grid_size_x) + i] != grid_vector[chunk_index][(j * grid_size_x) + (i + 1)]) {		//if the current pos and pos 1 to right are not the same
						grid_vector[chunk_index][(j * grid_size_x) + i] = 0;			//set to path way
						//here you could find what the sites bordering are
					}
				}
				if (j + 1 < grid_size_x) {
					if (grid_vector[chunk_index][(j * grid_size_x) + i] != grid_vector[chunk_index][((j + 1) * grid_size_x) + i]) {		//if current pos and pos one down are not the same
						grid_vector[chunk_index][(j * grid_size_x) + i] = 0;
					}
				}
			}
		}
	}
}

void VoronoiDiagram::SetPointModi(int& x, int& x_2, int& y, int& y_2, const float& x_v_1, const float& x_v_2, const float& y_v_1, const float& y_v_2){
	x = (grid_size_x * x_v_1);
	x_2 = (grid_size_x * x_v_2);
	y = (grid_size_x * y_v_1);
	y_2 = (grid_size_x * y_v_2);
}

void VoronoiDiagram::PlacePoint(int x, int y, int i, bool& found_, const int& chunk_index) {
	if (grid_vector[chunk_index][(y * grid_size_x) + x] == 0) {
		found_ = true;
		grid_vector[chunk_index][(y * grid_size_x) + x] = 2000 + i;
		point_pos.push_back(sf::Vector2i(x, y));
	}
}

void VoronoiDiagram::ThreePoints(const float values_[12], const int& chunk_index)
{
	int x_pos_one;
	int x_pos_two;
	int y_pos_one;
	int y_pos_two;
	SetPointModi(x_pos_one, x_pos_two, y_pos_one, y_pos_two, values_[0], values_[1], values_[2], values_[3]);
	for (int i = 0; i < num_of_points; i++)
	{
		bool found = false;
		int counter = 0;
		while (!found)
		{
			int x = rand() % x_pos_one + x_pos_two;
			int y = rand() % y_pos_one + y_pos_two;
			PlacePoint(x, y, i, found, chunk_index);
			counter++;
			if (counter > 200)
			{
				failed_ = true;
				found = true;
				break;
				std::cout << "didnt set a point\n";
			}
		}
		if (i == 0)
		{
			SetPointModi(x_pos_one, x_pos_two, y_pos_one, y_pos_two, values_[4], values_[5], values_[6], values_[7]);
		}
		if (i == 1)
		{
			SetPointModi(x_pos_one, x_pos_two, y_pos_one, y_pos_two, values_[8], values_[9], values_[10], values_[11]);
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
void VoronoiDiagram::SetPoint(int type, const int& chunk_index) {
	point_pos.clear();
	//zero is iother, 1 is p2p,2 loop
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution((grid_size_x / 16), (int)(grid_size_x / 1.1));
	switch (type) {
	case 0: {
		float arr[12] = { 0.3f,0.05f,0.2f,0.75f, 0.2f,0.45f,0.4f,0.05f, 0.2f,0.75f,0.2f,0.75f };
		ThreePoints(arr, chunk_index);
	}
		  break;
	case 1: {
		SetPointDefault(chunk_index,generator,distribution,0);
	}
		  break;
	case 2: {
		float arr[12] = { 0.05f,0.15f,0.2f,0.4f, 0.2f,0.4f,0.15f,0.15f, 0.05f,0.80f,0.2f,0.4f };
		ThreePoints(arr, chunk_index);
	}  break;
	case 3:{
		if (chunk_index==0){
			SetPointDefault(chunk_index, generator, distribution, 1);
		}
		SetPointHeightExtented(chunk_index, generator, distribution);
		break;
	}	}
}
void VoronoiDiagram::SetPointDefault(const int& chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_, const int& used_if_chunked) {
	int iter = grid_size_x / num_of_points;//200
	iter -= (iter) / 2;//100
	int start = 1;
	int position = 0;
	for (int i = 0; i < (num_of_points- used_if_chunked); i++) {
		bool found = false;
		if (start + iter > grid_size_x) {//iif out of bounds
			int difference_ = (start + iter) - grid_size_x;
			start -= difference_;
		}
		int counter = 0;

		while (!found) {
			counter++;
			//so first is between 0 and grid_size/numpoints, second is iter and iter+iter, etc
			int x = rand() % iter + start;
			int y = dist_(gen_);
			PlacePoint(x, y, i, found, chunk_index);//if point generated lies on grid, add to points vector, change grid array to point position, found = true;
			if (counter > 200) {
				failed_ = true;
				break;
				std::cout << "didnt set a point\n";
			}
		}
		iter = grid_size_x / num_of_points;//200
		start += iter;//201
	}
}
void VoronoiDiagram::SetPointHeightExtented(const int& chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_) {
	bool found_ = false;
	int counter_ = 0;
	int x = 0;
	int y = 0;
	switch (chunk_index)
	{
	case 0: {
		x = grid_size_x - 1;
		y = dist_(gen_);
		SetPointOnEdgeHeight(found_, counter_, chunk_index, gen_, dist_, x, y);
		break;
	}
	case 1: {
		point_pos.push_back(sf::Vector2i(0,last_point_pos.y));
		grid_vector[chunk_index][(last_point_pos.y * grid_size_x) + 0] = 2000 + 0;
		SetPointInMiddle(found_, counter_, x, y, chunk_index);
		found_ = false;
		SetPointOnEdgeHeight(found_, counter_, chunk_index, gen_, dist_, x, y);
		break;
	}
	case 2: {
		point_pos.push_back(sf::Vector2i(last_point_pos.x, 0));
		grid_vector[chunk_index][(0 * grid_size_x) + last_point_pos.x] = 2000 + 0;
		SetPointInMiddle(found_, counter_, x, y, chunk_index);
		found_ = false;
		SetPointOnEdgeHeight(found_, counter_, chunk_index, gen_, dist_, x, y);
		break;
	}	
	case 3: {
		point_pos.push_back(sf::Vector2i(399, last_point_pos.y));
		grid_vector[chunk_index][(last_point_pos.y * grid_size_x) + 399] = 2000 + 0;
		SetPointInMiddle(found_, counter_, x, y, chunk_index);
		found_ = false;
		SetPointOnEdgeHeight(found_, counter_, chunk_index, gen_, dist_, x, y);
		break;
	}
	}
}

void VoronoiDiagram::SetPointOnEdgeHeight(bool &found_, int&counter_, const int&chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_, int& x_pos_changed, int& y_pos_changed) {
	while (!found_) {
		counter_++;
		//so first is between 0 and grid_size/numpoints, second is iter and iter+iter, etc
		switch (chunk_index)
		{
		case 0: {
			x_pos_changed = grid_size_x - 1;
			y_pos_changed = dist_(gen_);
			break;
		}
		case 1: {
			x_pos_changed = rand() % (grid_size_x- point_pos[point_pos.size() - 1].x -10)+ point_pos[point_pos.size()-1].x;
			y_pos_changed = grid_size_x - 1;
			break;
		}
		case 2: {
			x_pos_changed = 0;
			y_pos_changed = dist_(gen_);
			break;
		}
		case 3: {
			x_pos_changed = dist_(gen_);
			y_pos_changed = dist_(gen_);
			break;
		}
		}
		PlacePoint(x_pos_changed, y_pos_changed, 2, found_, chunk_index);//if point generated lies on grid, add to points vector, change grid array to point position, found = true;
		if (counter_ > 200) {
			failed_ = true;
			counter_ = 0;
			break;
			std::cout << "didnt set a point\n";
		}
	}
	if (found_){
		last_point_pos = point_pos[point_pos.size() - 1];
	}
}

void VoronoiDiagram::SetPointInMiddle(bool& found_, int& counter_, int& x_pos_changed, int& y_pos_changed, const int& chunk_index) {
	while (!found_) {
		counter_++;
		int lims_start=grid_size_x*0.25f; int lims_end=grid_size_x*0.25f;
		x_pos_changed = rand() % lims_end + lims_start;
		y_pos_changed = rand() % lims_end + lims_start;//will select a point roughly in the middle between 1/4 and 3/4 of total grid_size, so square that size of orignal square.
		PlacePoint(x_pos_changed, y_pos_changed, 1, found_, chunk_index);//if point generated lies on grid, add to points vector, change grid array to point position, found = true;
		if (counter_ > 200) {
			failed_ = true;
			break;
			std::cout << "didnt set a point\n";
		}
	}
}

void VoronoiDiagram::SetHighPoint(const int& layers_, int* const& noise_grid, sf::Vector2i& high_point_v_, int& high_point_, const int& i, const int& j, int& min_height, sf::Vector2i& low_point_v_) {
	int temp_height = noise_grid[(i * grid_size_x) + j] / layers_;
	if (temp_height > high_point_) {
		high_point_ = temp_height;
		high_point_v_.x = j;
		high_point_v_.y = i;
	}
	if (temp_height < min_height) {
		min_height = temp_height;
		low_point_v_.x = j;
		low_point_v_.y = i;
	}
}

void VoronoiDiagram::ArePointsFound(const int& high_point, const int& low_point) {
	if (high_point < 210) {
		stop_high_ = true;
		std::cout << "high condition\n";
	}
	if (low_point > 25) {
		std::cout << "low condition\n";
		stop_low_ = true;
	}
}

void VoronoiDiagram::FindMax(const int& layers_, int* const& noise_grid, const ranges& init) {
	high_point = 0;
	high_point_v = sf::Vector2i(max_value_height, max_value_height);
	low_point_v = sf::Vector2i(max_value_height, max_value_height);
	found_raidus = false;
	radius_length = 0;

	int min_height = 10000;
	//if circles vector is empty then just find the max - meaning that no peaks exist currently so no  worries about checking for other peaks
	//if there are peaks, check if the current point is within a peak and if so dont find the max
	parallel_for(init.y_min, init.y_max, [&](int i) {
		for (int j = init.x_min; j < init.x_max; j++) {
			if (!circles_.empty()) {
				bool found_in_first_circle = false;
				bool not_found_in_circle = true;

				for (int c = 0; c < circles_.size(); c++) {
					int r = circles_[c].r_length * circles_[c].r_length;
					int a = pow((j - circles_[c].point.x), 2);				//x part squared
					int b = pow((i - circles_[c].point.y), 2);
					if (a + b > r)		//outside the circle, so find the max
					{
						not_found_in_circle = false;
					}
					else
					{
						found_in_first_circle = true;
					}
				}
				if (!not_found_in_circle && !found_in_first_circle) {				//so if not in ANY circle 
					if (!found_in_first_circle) {
						SetHighPoint(layers_, noise_grid, high_point_v, high_point, i, j, min_height, low_point_v);
					}
				}
			}
			else {
				SetHighPoint(layers_, noise_grid, high_point_v, high_point, i, j, min_height, low_point_v);
			}
		}
		});
	ArePointsFound(high_point, min_height);
}

void VoronoiDiagram::SetDirectionXY(int& signal, int& x, int& y, int a, int b, int c) {
	signal = a;
	x = b;
	y = c;
}

void VoronoiDiagram::DirectionDecider(const int& radius_cutoff_, const int& layers_, const int& index_v, int* const& noise_h_m, const sf::Vector2i& high_or_low, bool b_what_p, const ranges& init) {
	//identify where x and y are in relation to the complete image
	//so are they top left, top right, bottom left, bottom right
	//then mark that as the direction to go in 
	int signal, x_pos, y_pos;
	//	std::cout << "		HIGHEST point(peak): " << high_or_low.x << " " << high_or_low.y << "\n\n";
	if (high_or_low.x <= (grid_size_x / 2) && high_or_low.y <= (grid_size_x / 2)) {
		//square 1 in diagram(top left) - going south east
		SetDirectionXY(signal, x_pos, y_pos, 1, 1, 1);
		std::cout << "se\n";
	}
	else if (high_or_low.x >= (grid_size_x / 2) && high_or_low.x <= (grid_size_x) && high_or_low.y <= (grid_size_x / 2)) {
		//square 2 in diagram(top right) - going south west 
		SetDirectionXY(signal, x_pos, y_pos, 2, -1, 1);
		std::cout << "sw\n";
	}
	else if (high_or_low.x >= (grid_size_x / 2) && high_or_low.x <= (grid_size_x) && high_or_low.y >= (grid_size_x / 2) && high_or_low.y <= grid_size_x) {
		//square 4 in diagram(bottom right) - going north west
		SetDirectionXY(signal, x_pos, y_pos, 4, -1, -1);
		std::cout << "nw\n";
	}
	else if (high_or_low.x <= (grid_size_x / 2) && high_or_low.y >= (grid_size_x / 2) && high_or_low.y <= grid_size_x) {
		//square 3 in diagram(bottom left) - going north east
		SetDirectionXY(signal, x_pos, y_pos, 3, 1, -1);
		std::cout << "ne\n";
	}

	temp_rad.resize(2);
	FindCircumPoint(x_pos, y_pos, signal, radius_cutoff_, layers_, 1, 0, noise_h_m, circum_points[0], high_or_low, b_what_p);
	FindCircumPoint(-x_pos, -y_pos, signal, radius_cutoff_, layers_, -1, 1, noise_h_m, circum_points[1], high_or_low, b_what_p);
	radiiDecider(index_v, high_or_low);
}

void VoronoiDiagram::SetCircumPoint(sf::Vector2i& circum_point_, int x, int y, int iterator_, int place)
{
	circum_point_ = sf::Vector2i(x, y);
	/*std::cout << "		POINT on circumferenece: " << x << ", " << y << "\n";
	std::cout << "		RADIUS Length: " << iterator_ << "\n";*/
	found_raidus = true;
	temp_rad.at(place) = (iterator_);
}

void VoronoiDiagram::SwitchPoint(int& iterator, int& y_, int& x_, const int signal_, int modifier_)
{
	iterator++;
	switch (signal_)
	{
	case 1:
		y_ += (1 * modifier_), x_ += (1 * modifier_);		//south east
		break;
	case 2:
		y_ += (1 * modifier_), x_ -= (1 * modifier_);		//sw
		break;
	case 3:
		y_ -= (1 * modifier_), x_ += (1 * modifier_);		//ne
		break;
	case 4:
		y_ -= (1 * modifier_), x_ -= (1 * modifier_);		//nw
		break;
	}
}

void VoronoiDiagram::FindCircumPoint(int x_value_, int y_value_, int signal_, const int radius_cutoff_, const int layers_, int modifier_, int place, int* const& noise_h_m, sf::Vector2i& circum_point_, const sf::Vector2i& high_or_low, bool b_what_p)
{
	int y = high_or_low.y;
	int x = high_or_low.x;
	int iterator_ = 0;
	found_raidus = false;
	do {
		//travelled the length of the radius then set found etc
		if ((y + y_value_) >= 0 && (x + x_value_) >= 0 && (y + y_value_) <= grid_size_x && (x + x_value_) <= grid_size_x) {
			switch (b_what_p) {
			case true:
				if ((noise_h_m[((y + y_value_) * grid_size_x) + (x + x_value_)] / layers_) <= (radius_cutoff_)) {//set the circumferences because you have reached a point on the circumference
					if ((y + y_value_) >= 0 && (x + x_value_) >= 0 && (y + y_value_) <= grid_size_x && (x + x_value_) <= grid_size_x) {
						x += x_value_, y += y_value_;
						SetCircumPoint(circum_point_, x, y, iterator_, place);
					}
					else {
						SetCircumPoint(circum_point_, x, y, 50, place);
						std::cout << "		Else Statement.\n";
					}
				}
				else {
					SwitchPoint(iterator_, y, x, signal_, modifier_);
				}
				break;
			case false:
				if ((noise_h_m[((y + y_value_) * grid_size_x) + (x + x_value_)] / layers_) >= (radius_cutoff_)) {
					if ((y + y_value_) >= 0 && (x + x_value_) >= 0 && (y + y_value_) <= grid_size_x && (x + x_value_) <= grid_size_x) {
						x += x_value_, y += y_value_;
						SetCircumPoint(circum_point_, x, y, iterator_, place);
					}
					else {
						SetCircumPoint(circum_point_, x, y, 50, place);
						std::cout << "		Else Statement.\n";
					}
				}
				else {
					SwitchPoint(iterator_, y, x, signal_, modifier_);
				}
				break;
			}
		}
		else {
			SetCircumPoint(circum_point_, x, y, 70, place);						//if out of bounds (greater than resolution or less than resolution) then tell yourself that and set a fake radius.
			std::cout << "		Point too far.\n";
		}
	} while (found_raidus != true && !failed_);
}

void VoronoiDiagram::SelectRadii(const int& index_v, int a, int b, const sf::Vector2i& high_or_low)
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

	int r_ = (temp_rad[0] + temp_rad[1]) / 2;
	if (radius_length < r_)
	{
		radius_length = r_;
	}
	peak_.point = sf::Vector2i(high_or_low.x, high_or_low.y), peak_.r_length = radius_length;
	circles_.push_back(peak_);
}

void VoronoiDiagram::radiiDecider(const int& index_v, const sf::Vector2i& high_or_low)
{
	int p = 0;
	if (temp_rad.at(0) > temp_rad.at(1))					//chooses the bigger of the two but if its > 300 then chooses the smaller cos thats large
	{
		SelectRadii(index_v, 0, 1, high_or_low);
	}
	else
	{
		SelectRadii(index_v, 1, 0, high_or_low);
	}
}
void VoronoiDiagram::vector_all(int size) {
	circum_points.resize(2);
}

void VoronoiDiagram::InsertChunks() {
	distance_grid_vector.push_back(grid_distance);
	distance_grid_vector.push_back(grid_distance_c_1);
	distance_grid_vector.push_back(grid_distance_c_2);
	distance_grid_vector.push_back(grid_distance_c_3);
	grid_vector.push_back(grid_v_1);
	grid_vector.push_back(grid_chunk_1);
	grid_vector.push_back(grid_chunk_2);
	grid_vector.push_back(grid_chunk_3);
}

void VoronoiDiagram::TerrainSites() {
	//loop over number of sites for x and y 
	//generate random point for site and loop over this generation until it is not within the circle

	//for every site*2 so that gives x and y pos
	//create a random coordinate
	//check if that coordinate is within ANY circle or ON ANY circle
	//if true then discard point and create new point UNTIL point is NOT in circle
	int loc_num_sites = num_of_sites * 2;
	int loc_grid_size = grid_size_x;
	auto temp_circl = circles_;
	if (local_is_chunking) {
		delete[] sites_v_1;
		loc_num_sites = (num_of_sites * 2) * 4;//param the 25 
		sites_v_1 = new int[loc_num_sites];
		loc_grid_size *= 2;
		temp_circl = all_circles_vector;
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, loc_grid_size);
	for (int i = 0; i < (loc_num_sites); i++) {
		bool found = false;
		while (!found) {
			bool false_in_first_cirlce = false;
			sites_v_1[i] = distribution(generator);
			i++;
			sites_v_1[i] = distribution(generator);
			for (int c = 0; c < temp_circl.size(); c++) {
				int r = pow(temp_circl[c].r_length, 2);						//radius squared
				int a = pow((sites_v_1[i - 1] - temp_circl[c].point.x), 2);				//x part squared
				int b = pow((sites_v_1[i] - temp_circl[c].point.y), 2);					//y part squared
				if (a + b < r || a + b == r) {				//the circle formula - checking whether the point exist in the circle and if it does then set the iterator back to what it was and go again
					found = false;
					false_in_first_cirlce = true;			//so if this is true then when it comes to circle(n+1) the found wont trigger
				}
				else {
					if (!false_in_first_cirlce) {
						found = true;		//point is not in the circle so exit loop and create new site
					}
				}
			}
			if (false_in_first_cirlce) {
				i--;
			}
		}
	}

	for (int i = 0; i < loc_num_sites; i++) {
		std::cout << "Site x: " << sites_v_1[i] << " ";
		i++;
		std::cout << "Site y: " << sites_v_1[i] << "\n";
	}

	int iterator_ = 0;
	for (int i = 0; i < temp_circl.size(); i++) {
		sites_v_1[iterator_] = temp_circl[i].point.x;				//setting the first sites the the centre point of the circles
		iterator_++;
		sites_v_1[iterator_] = temp_circl[i].point.y;
		iterator_++;
		std::cout << "Centre " << i << " (" << temp_circl[i].point.x << ", " << temp_circl[i].point.y << ") Radius: " << temp_circl[i].r_length << "\n";
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
	all_circles_vector.clear();
}

void VoronoiDiagram::AddingCirclesToContainer(const ranges& init) {
	int x_addition = 0; int y_addition;
	x_addition = init.x_min;
	y_addition = init.y_min;
	for (int i = 0; i < circles_.size(); i++) {
		circles_[i].point.x += x_addition;
		circles_[i].point.y += y_addition;

		all_circles_vector.push_back(circles_[i]);
	}
}

void VoronoiDiagram::DivideChunks() {
	int local_grid_size = grid_size_x * 2;
	for (size_t j = 0; j < local_grid_size; j++){
		for (int i = 0; i < local_grid_size; i++){
			if (i < 400 && j < 400) {
				//in [0]
				grid_vector[0][(j * grid_size_x) + i] = full_grid_chunking[(j * local_grid_size) + i];
			}
			else if (i >= 400 && j < 400) {
				//in [1]
				grid_vector[1][(j * grid_size_x) + (i - 400)] = full_grid_chunking[(j * local_grid_size) + i];
			}
			else if (i < 400 && j >= 400) {
				//in [2]
				grid_vector[2][((j - 400) * grid_size_x) + i] = full_grid_chunking[(j * local_grid_size) + i];
			}
			else if (i >= 400 && j >= 400) {
				//in [3]
				grid_vector[3][((j - 400) * grid_size_x) + (i - 400)] = full_grid_chunking[(j * local_grid_size) + i];
			}
		}
	}
	delete[] full_grid_chunking;
}