#include "ImageProcessing.h"
#include <ppl.h>

using namespace concurrency;

ImageProcessing::ImageProcessing() {
	heightmap_ = nullptr;
	heightmap_1 = nullptr;
	heightmap_2 = nullptr;
	heightmap_3 = nullptr;
	heightmap_fbm_ = nullptr;
	noise_heightmap_ = nullptr;
	noise_heightmap_1 = nullptr;
	noise_heightmap_2 = nullptr;
	noise_heightmap_3 = nullptr;
	alpha_channel_ = nullptr;
	is_chunking_ = false;
}

ImageProcessing::~ImageProcessing(){
	delete[] heightmap_;
	delete[] heightmap_1;
	delete[] heightmap_2;
	delete[] heightmap_3;
	delete[] noise_heightmap_;
	delete[] noise_heightmap_1;
	delete[] noise_heightmap_2;
	delete[] noise_heightmap_3;
	delete[]alpha_channel_;
	delete[]heightmap_fbm_;
}

void ImageProcessing::InitStructures(int grid_size) {
	heightmap_ = new float[grid_size * grid_size];
	heightmap_1 = new float[grid_size * grid_size];
	heightmap_2 = new float[grid_size * grid_size];
	heightmap_3 = new float[grid_size * grid_size];
	heightmap_fbm_ = new float[grid_size * grid_size];
	noise_heightmap_ = new int[grid_size * grid_size];
	noise_heightmap_1 = new int[grid_size * grid_size];
	noise_heightmap_2 = new int[grid_size * grid_size];
	noise_heightmap_3 = new int[grid_size * grid_size];
	alpha_channel_ = new int[grid_size * grid_size];

	if (!noise_maps_vector.empty()){
		noise_maps_vector.clear();
		distance_heightmaps_vector.clear();
	}
	noise_maps_vector.push_back(noise_heightmap_);
	noise_maps_vector.push_back(noise_heightmap_1);
	noise_maps_vector.push_back(noise_heightmap_2);
	noise_maps_vector.push_back(noise_heightmap_3);
	distance_heightmaps_vector.push_back(heightmap_);
	distance_heightmaps_vector.push_back(heightmap_1);
	distance_heightmaps_vector.push_back(heightmap_2);
	distance_heightmaps_vector.push_back(heightmap_3);
}

void ImageProcessing::DrawCurve(sf::VertexArray& vertexarray, int grid_size, int num_sites, int* grid) {
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++) {
			switch (grid[(i * grid_size) + j]) {
			case -5:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::White;
				break;
			default:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color{ 0,0,0,0 };
				break;
			}
		}
	}
}

void ImageProcessing::DrawTrack(sf::VertexArray& vertexarray, int grid_size, int num_sites, int* grid, const int&chunk_index, const ranges& limits_) {
	int x_index = 0;
	int y_index = 0;
	for(int i = limits_.y_min; i < limits_.y_max; i++) {
		for (int j = limits_.x_min; j < limits_.x_max; j++) {
			switch (grid[(y_index * grid_size) + x_index]) {
			case -12303:
				vertexarray[y_index * grid_size + x_index].position = sf::Vector2f(j, i);
				vertexarray[y_index * grid_size + x_index].color = sf::Color::White;
				break;
			case -1234:
				vertexarray[y_index * grid_size + x_index].position = sf::Vector2f(j, i);
				vertexarray[y_index * grid_size + x_index].color = sf::Color::White;
				break;
			case 0:
				vertexarray[y_index * grid_size + x_index].position = sf::Vector2f(j, i);
				vertexarray[y_index * grid_size + x_index].color = sf::Color::White;
				break;
			case -3:
				vertexarray[y_index * grid_size + x_index].position = sf::Vector2f(j, i);
				vertexarray[y_index * grid_size + x_index].color = sf::Color::White;
				break;
			default:
				vertexarray[y_index * grid_size + x_index].position = sf::Vector2f(j, i);
				vertexarray[y_index * grid_size + x_index].color = sf::Color{ 0,0,0,0 };
				break;
			}
			x_index += 1;
		}
		y_index += 1;
		x_index = 0;
		}
}
void ImageProcessing::DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size, int* grid, const int& chunk_index, const ranges& limits_) {
	int index_x = 0;
	int index_y = 0;
	for (int i = limits_.y_min; i < limits_.y_max; i++) {
		index_x = 0;
	
		for (int j = limits_.x_min; j < limits_.x_max; j++) {
			if (grid[(index_y * grid_size) + index_x] == -1) {							//diagram
				vertexarray[index_y * grid_size + index_x].position = sf::Vector2f(j, i);
				vertexarray[index_y * grid_size + index_x].color = sf::Color::Green;
			}
			index_x += 1;
		}
		index_y += 1;
	}
}
void ImageProcessing::DrawVoronoiNoise(sf::VertexArray& vertextarray, const int& grid_size, const int& num_sites, const int& num_, int* const& grid_distance, const int& chunk_index, const ranges& init) {
	int max_d_ = 0;
	int index_x = 0;
	int index_y = 0;
	for (int i = init.y_min; i < init.y_max; i++) {
		index_x = 0;
		for (int j = init.x_min; j < init.x_max; j++) {
			float s = float((float)1 / (float)num_sites);							//gets the thing as a percentage
					//for example 1/100 = 0.01
			int d = grid_distance[(index_y * grid_size) + index_x] / (float)num_;			//the distance divided by a number, so makes the distance smaller or bigger
					//for example could be 1000/25 = 40
					//so distance is higher with less sites, distance lower with more sites, so with higher sites you need lower num_

			int n = s * 255;										//percentage * 255
					//this is the percentage of the colour
			if (grid_distance[(index_y * grid_size) + index_x] > max_d_) {
				max_d_ = grid_distance[(index_y * grid_size) + index_x];
			}
			int r = d;
			//so the further away the distance is higher r
			if (r > 255) {
				r = 255;				//keeps the values within range
			}
			sf::Uint8 c = 255 - r;
			float h_c = int(255 - r);
			//so end color, closer to the site you are - brighter, further away - darker
			c /= 2;						//makes the colour smaller
			h_c /= 2;
			distance_heightmaps_vector[chunk_index][index_y * grid_size + index_x] = h_c;

			vertextarray[index_y * grid_size + index_x].position = sf::Vector2f(j, i);
			vertextarray[index_y * grid_size + index_x].color = sf::Color{ c , c , c };
			index_x += 1;
		}
		index_y += 1;
	}
}
void ImageProcessing::DrawNoise(sf::VertexArray& vertexarray, int grid_size, int layers_, const float& frequency, const int& chunk_, const int& seed_) {
	int start_x = 0;
	int end_x = grid_size;
	int start_y = 0;
	int end_y = grid_size;
	switch (chunk_) {
	case 0:
		break;
	case 1:
		start_x = grid_size;
		end_x = grid_size * 2;
		break;
	case 2:
		start_x = 0;
		end_x = grid_size;
		start_y = grid_size;
		end_y = grid_size * 2;
		break;
	case 3:
		start_x = grid_size;
		end_x = grid_size * 2;
		start_y = grid_size;
		end_y = grid_size * 2;
		break;
	}

	for (size_t i = 0; i < grid_size; i++) {
		for (size_t j = 0; j < grid_size; j++) {
			noise_maps_vector[chunk_][(i * grid_size) + j] = 0;
		}
	}
	const float scale = 100.0f / (float)grid_size;
	float low_ = 0.01f;
	float high_ = 0.020f;
	float r3 = low_ + static_cast <float>(0.1f);
	// (rand()) / (static_cast <float> (RAND_MAX / (high_ - low_)));				//the frequency of the noise between low and high
	float e2 = r3;

	float min_ = INT_MAX;
	float max_ = 0.0f;


	float old_range_ = (1 - (-1));
	float new_range_ = (1 - 0);

	int x_iter = 0, y_iter = 0;
	for (int a = 0; a < layers_; a++) {
		float r3 = frequency;
		for (int i = start_y; i < end_y; i++) {
			x_iter = 0;
			for (int j = start_x; j < end_x; j++) {
				float height = (float)perlin_.noise(j+ seed_, i+ seed_, (r3 * scale)) * pHeightRange;
				float new_value_ = (((height - (-1)) * new_range_) / old_range_) + 0;
				height = 0.5f * (height + 1.0f);				//keeps the value between 0-1 instead of the -1 to 1 range it is in initially
				int co = int(new_value_ * 255);						//geets as rgb value

				noise_maps_vector[chunk_][(y_iter * grid_size) + x_iter] += co;				//stores colour value for use in saving images
				alpha_channel_[y_iter * grid_size + x_iter] = 255;				//stores alpha value
				sf::Uint8 c = (noise_maps_vector[chunk_][(y_iter * grid_size) + x_iter] / layers_);														//this needs changed - might be right actually

				vertexarray[y_iter * grid_size + x_iter].position = sf::Vector2f(j, i);
				vertexarray[y_iter * grid_size + x_iter].color = sf::Color{ c , c , c };
				if (new_value_ < min_) {
					min_ = new_value_;
				}
				if (new_value_ > max_) {
					max_ = new_value_;
				}
				x_iter += 1;
			}
			y_iter += 1;
		}
	}
}
void ImageProcessing::DrawFBM(sf::VertexArray& vertexarray, int grid_size, int octaves_, const float& frequency) {
	for (size_t i = 0; i < grid_size; i++) {
		for (size_t j = 0; j < grid_size; j++) {
			heightmap_fbm_[(i * grid_size) + j] = 0;
		}
	}
	const float scale = 100.0f / (float)grid_size;
	float low_ = 0.01f;
	float high_ = 0.020f;
	float r3 = low_ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high_ - low_)));//the frequency of the noise between low and high
	float min_ = INT_MAX;
	float max_ = 0.0f;
	float new_range_ = (1 - 0);
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++) {
			float a = 3.0f;						//controls the height.
			float mul = scale * 1.0f;
			float height = 0.0f;
			for (int o = 0; o < octaves_; o++) {
				height += (perlin_.noise(i * mul, j * mul, frequency)) * a;
				a *= 0.5f;
				mul *= 2.0f;
			}
			heightmap_fbm_[(i * grid_size) + j] = height;
			if (height < min_) {					//gets the max and min values that the algo produces, so that they can then be changed to the range between 0 and 1 which can then be used to get a colour value
				min_ = height;
			}
			if (height > max_) {
				max_ = height;
			}
		}
	}
	//prints out those values 
	float new_min_ = INT_MAX;
	float new_max_ = 0;
	float old_range_ = (max_ - min_);										//gets the range of the old values
	for (size_t i = 0; i < grid_size; i++) {
		for (size_t j = 0; j < grid_size; j++) {
			float new_value_ = (((heightmap_fbm_[(i * grid_size) + j] - min_) * new_range_) / old_range_) + 0;	//gets the oldd value and turns it into a value between the new range
			int co = int(new_value_ * 255);						//geets as rgb value
			if (co > 255) {
				co = 255;
			}
			if (co < 0) {
				co = 0;									//a problem currently exists where there is a region escaping the limits iof the colour which tells me that the range is somehow wrong.
			}
			noise_heightmap_[(i * grid_size) + j] = co;				//sets the colour value to the heightmap
			alpha_channel_[i * grid_size + j] = 255;				//stores alpha value
			sf::Uint8 c = (noise_heightmap_[(i * grid_size) + j]);		//sets the colour value to an sfml colour value

			vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
			vertexarray[i * grid_size + j].color = sf::Color{ c , c , c };
			if (new_value_ < new_min_) {	//gets the new max and min for debugging purposes 
				new_min_ = new_value_;
			}
			if (new_value_ > new_max_) {
				new_max_ = new_value_;
			}
		}
	}
}
//

//image modifiers
void ImageProcessing::ChangeAlpha(sf::VertexArray& vertexarray, int grid_size, int alpha_) {
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++) {
			//stores alpha if changed and applies to exisiting image
			alpha_channel_[(i * grid_size) + j] = alpha_;
			sf::Uint8 c = alpha_;
			vertexarray[i * grid_size + j].color = sf::Color{ vertexarray[i * grid_size + j].color.r , vertexarray[i * grid_size + j].color.g ,vertexarray[i * grid_size + j].color.b, c };
		}
	}
}
sf::Color ImageProcessing::AverageColour(sf::Color a, sf::Color b) {
	sf::Color avr;
	avr.r = ((a.r + b.r) / (sf::Uint8)2);
	avr.g = avr.r;
	avr.b = avr.r;
	return avr;
}
//

//sampling functions
void ImageProcessing::ResizeImage(int grid_size, float scale)
{
	int new_size = grid_size * scale;

	sf::Image image;
	image.loadFromFile("track_image.png");
	sf::Image scaled_image;
	scaled_image.create(new_size, new_size);

	sf::Color p, q;
	sf::Color new_color;
	int x, y, x2, y2;
	int TgtWidth = new_size;
	int TgtHeight = new_size;
	for (y = 0; y < TgtHeight; y++) {
		y2 = 2 * y;
		for (x = 0; x < TgtWidth; x++) {
			x2 = 2 * x;
			p = AverageColour(image.getPixel(x2, y2), image.getPixel(x2 + 1, y2));
			q = AverageColour(image.getPixel(x2, y2 + 1), image.getPixel(x2 + 1, y2 + 1));
			new_color = AverageColour(p, q);
			scaled_image.setPixel(x, y, new_color);
		} /* for */
	}
	scaled_image.saveToFile("test.png");
}
void ImageProcessing::UpScaleGrid(int grid_size, float scale, int* grid, VoronoiDiagram& v_d_p)
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
				new_arr[x_dash * new_size + y_dash] = grid[i * grid_size + j];

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
				int c = grid[i / (int)scale * grid_size + j / (int)scale];
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
				int c = grid[i / (int)scale * grid_size + j / (int)scale];
				for (int g = 0; g < scale; g++)
				{

					new_arr[(i + g) * new_size + j] = c;
				}
			});
	}

	//re sizing the grid
	v_d_p.ResizeGrid(scale);

	parallel_for(0, new_size, [&](int i)
		{
			for (int j = 0; j < (new_size); j++)										//x
			{
				grid[i * new_size + j] = new_arr[i * new_size + j];

			}
		});
}
void ImageProcessing::UpScaleVertexArray(int grid_size, float scale, sf::VertexArray& vertexarray)
{
	int new_size = grid_size * scale;
	sf::VertexArray scale_array;
	scale_array.resize(new_size * new_size);

	//initial positions
	parallel_for(0, grid_size, [&](int i)
		{
			for (int j = 0; j < (grid_size); j++)										//x
			{

				int x_dash = j * new_size / grid_size;
				int y_dash = i * new_size / grid_size;
				scale_array[i * new_size + j].position = sf::Vector2f(x_dash, y_dash);
				scale_array[i * new_size + j].color = vertexarray[i * grid_size + j].color;
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
				sf::Color c = vertexarray[i / (int)scale * grid_size + j / (int)scale].color;
				for (int g = 0; g < scale; g++)
				{
					scale_array[(i * new_size) + (j + g)].position = sf::Vector2f(j + g, i);
					scale_array[(i * new_size) + (j + g)].color = c;
				}
			}
		});

	//////rows - same for rows

	for (int i = 0; i < (new_size - 1); i += scale)											//y
	{
		parallel_for(0, new_size, [&](int j)
			{
				sf::Color c = vertexarray[i / scale * grid_size + j / scale].color;
				for (int g = 0; g < scale; g++)
				{
					scale_array[(i + g) * new_size + j].position = sf::Vector2f(j, i + g);
					scale_array[(i + g) * new_size + j].color = c;
				}
			});
	}
	vertexarray.clear();
	vertexarray.resize(new_size * new_size);


	parallel_for(0, new_size, [&](int i)
		{
			for (int j = 0; j < (new_size); j++)										//x
			{
				vertexarray[i * new_size + j].position = scale_array[i * new_size + j].position;
				vertexarray[i * new_size + j].color = scale_array[i * new_size + j].color;
			}
		});
}
void ImageProcessing::ResizeGrid(int grid_size, float scale, int* grid)
{
	size_t new_size = grid_size * scale;
	int* new_arr = new int[new_size * new_size];

	//resizing grid
	std::copy(grid, grid + (grid_size * grid_size), new_arr + 0);
	grid_size = new_size;
	delete[] grid;
	grid = new_arr;
	delete[] new_arr;
}
//

//saving functions
void ImageProcessing::CreateFinalHM(int grid_size, sf::VertexArray& vertexarray, sf::VertexArray& vertexarray1, sf::VertexArray& vertexarray2, sf::VertexArray& vertexarray3, int layers_) {
	//y=i, x=j
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++) {
			int i_alpha_two = alpha_channel_[i * grid_size + j];				//int version of alpha
			float i_alpha_percent = (float)i_alpha_two / 255.0f;				//alpha as value between 0.0 to 1.0
			for (int index_ = 0; index_ < 4; index_++){
				int i_c_one = int(distance_heightmaps_vector[index_][i * grid_size + j]);					//int value of c
				int i_c_two = (noise_maps_vector[index_][i * grid_size + j] / layers_);					//int value of co

				float i_c_t_a = (float)i_c_two / 255.0f;							//decimal value of co
				float is = (float)i_c_one / 255.0f;									//decimal value of c

				float alpha_percent_ = i_alpha_percent + 1.0f * (1.0f - i_alpha_percent);							//alpha_f = alpha_a + alpha_b(1-alpha_a)	(as a decimal value)
				float final_color_p = (i_c_t_a * i_alpha_percent + is * 1.0f * (1.0f - i_alpha_percent)) / alpha_percent_;		//final_c = (colour_a*alpha_a + colour_b*alpha_b(1-alpha_a))/alpha_final		as a percent

				if (final_color_p < 0.0f)
				{
					final_color_p = 0.0;
					//just to check if its out of bounds
					//happens for some reason when j=0 to 512 and i = 512
					//becuase of error in voronoi
					//fix it
				}
				if (final_color_p > 1.0f)
				{
					int a = 1;
					final_color_p = 1.0;
				}

				int f_c = 255 * final_color_p;											//need to multiply it by 255 to get as rgb value out of 255 instead of decimal
				int f_a = 255 * alpha_percent_;

				sf::Uint8 final_c = f_c;
				sf::Uint8 final_a = f_a;
				switch (index_){
				case 0:
					vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
					vertexarray[i * grid_size + j].color = sf::Color{ final_c , final_c , final_c, final_a };
					break;
				case 1:
					vertexarray1[i * grid_size + j].position = sf::Vector2f(j+grid_size, i);
					vertexarray1[i * grid_size + j].color = sf::Color{ final_c , final_c , final_c, final_a };
					break;
				case 2:
					vertexarray2[i * grid_size + j].position = sf::Vector2f(j, i + grid_size);
					vertexarray2[i * grid_size + j].color = sf::Color{ final_c , final_c , final_c, final_a };
					break;
				case 3:
					vertexarray3[i * grid_size + j].position = sf::Vector2f(j + grid_size, i + grid_size);
					vertexarray3[i * grid_size + j].color = sf::Color{ final_c , final_c , final_c, final_a };
					break;
				}
			}
		}
	}
}
void ImageProcessing::FinalImageLoop(const int& i, const int&j, const int&grid_size, const int&layers_, const int&index_,sf::Image &return_val) {
	int i_alpha_two = alpha_channel_[i * grid_size + j];				//int version of alpha
	float i_alpha_percent = (float)i_alpha_two / 255.0f;				//alpha as value between 0.0 to 1.0
	int i_c_one = int(distance_heightmaps_vector[index_][i * grid_size + j]);					//int value of c
	int i_c_two = (noise_maps_vector[index_][i * grid_size + j] / layers_);					//int value of co
	float i_c_t_a = (float)i_c_two / 255.0f;							//decimal value of co
	float is = (float)i_c_one / 255.0f;									//decimal value of c
	float alpha_percent_ = i_alpha_percent + 1.0f * (1.0f - i_alpha_percent);							//alpha_f = alpha_a + alpha_b(1-alpha_a)	(as a decimal value)
	float final_color_p = (i_c_t_a * i_alpha_percent + is * 1.0f * (1.0f - i_alpha_percent)) / alpha_percent_;		//final_c = (colour_a*alpha_a + colour_b*alpha_b(1-alpha_a))/alpha_final		as a percent
	//
	if (final_color_p < 0.0f) {
		final_color_p = 0.0;
	}
	if (final_color_p > 1.0f) {
		final_color_p = 1.0;
	}
	//this is the premultiplied
	float i_c_f_t = i_c_t_a + is * (1.0f - i_alpha_percent);
	if (i_c_f_t > 1.0f) {
		i_c_f_t = 1.0f;
	}
	int premultiplied_version_colour = i_c_f_t * 255;
	int premultiplied_version_alpha = 255 * alpha_percent_;
	int f_c = 255 * final_color_p;											//need to multiply it by 255 to get as rgb value out of 255 instead of decimal
	int f_a = 255 * alpha_percent_;
	sf::Uint8 final_c = f_c;
	sf::Uint8 final_a = f_a;

	return_val.setPixel(j, i, sf::Color{ final_c,final_c,final_c ,final_a });
}

void ImageProcessing::WriteToFile(int grid_size, sf::VertexArray& track_vertex_arr0, sf::VertexArray& track_vertex_arr1, sf::VertexArray& track_vertex_arr2, sf::VertexArray& track_vertex_arr3, int layers_) {
	const int dimensions_ = grid_size;
	sf::Image voronoi_output;
	sf::Image voronoi_output1;
	sf::Image voronoi_output2;
	sf::Image voronoi_output3;
	sf::Image noise_output;
	sf::Image noise_output_1;
	sf::Image noise_output_2;
	sf::Image noise_output_3;
	sf::Image track_output;
	sf::Image track_output1;
	sf::Image track_output2;
	sf::Image track_output3;
	sf::Image final_i;
	sf::Image final_i1;
	sf::Image final_i2;
	sf::Image final_i3;
	std::vector<sf::Image> noise_output_vector;
	std::vector<sf::Image> voronoi_output_vector;
	std::vector<sf::Image> track_output_vector;
	std::vector<sf::Image> final_image_vector;
	voronoi_output_vector.push_back(voronoi_output3);
	voronoi_output_vector.push_back(voronoi_output1);
	voronoi_output_vector.push_back(voronoi_output2);
	voronoi_output_vector.push_back(voronoi_output3);
	final_image_vector.push_back(final_i);
	final_image_vector.push_back(final_i2);
	final_image_vector.push_back(final_i2);
	final_image_vector.push_back(final_i3);
	noise_output_vector.push_back(noise_output);
	noise_output_vector.push_back(noise_output_1);
	noise_output_vector.push_back(noise_output_2);
	noise_output_vector.push_back(noise_output_3);
	track_output_vector.push_back(track_output);
	track_output_vector.push_back(track_output1);
	track_output_vector.push_back(track_output2);
	track_output_vector.push_back(track_output3);
	
	if (!is_chunking_) {
		noise_output_vector[0].create(grid_size, grid_size);
		voronoi_output_vector[0].create(grid_size, grid_size);
		track_output_vector[0].create(grid_size, grid_size);
		final_image_vector[0].create(grid_size, grid_size);
	}
	else {
		for (int i = 0; i < 4; i++) {
			voronoi_output_vector[i].create(grid_size, grid_size);
			noise_output_vector[i].create(grid_size, grid_size);
			track_output_vector[i].create(grid_size, grid_size);
			final_image_vector[i].create(grid_size, grid_size);
		}
	}

	for (int i = 0; i < dimensions_; i++) {
		for (int j = 0; j < dimensions_; j++) {
			sf::Uint8 co, co_1, co_2, co_3;
			sf::Uint8 c, c1, c2, c3;
			std::vector<sf::Uint8> noise_color;
			std::vector<sf::Uint8> distance_heightmap_color;
			distance_heightmap_color.push_back(c);
			distance_heightmap_color.push_back(c1);
			distance_heightmap_color.push_back(c2);
			distance_heightmap_color.push_back(c3);
			noise_color.push_back(co);
			noise_color.push_back(co_1);
			noise_color.push_back(co_2);
			noise_color.push_back(co_3);
			sf::Uint8 a = alpha_channel_[i * grid_size + j];					//alpha colour value of noise
			if (!is_chunking_) {
				noise_color[0] = (noise_maps_vector[0][i * grid_size + j] / layers_);					//noise colour
				distance_heightmap_color[0] = int(distance_heightmaps_vector[0][i * grid_size + j]);					//the voronoidiagram colour

				noise_output_vector[0].setPixel(j, i, sf::Color{ noise_color[0] , noise_color[0] , noise_color[0],a });
				voronoi_output_vector[0].setPixel(j, i, sf::Color{ distance_heightmap_color[0] , distance_heightmap_color[0] , distance_heightmap_color[0] });
				track_output_vector[0].setPixel(j, i, sf::Color{ track_vertex_arr0[i * grid_size + j].color.r,track_vertex_arr0[i * grid_size + j].color.g,track_vertex_arr0[i * grid_size + j].color.b });
				FinalImageLoop(i, j, grid_size, layers_, 0, final_image_vector[0]);
			}
			else {
				for (int iterator_chunk = 0; iterator_chunk < 4; iterator_chunk++) {
					noise_color[iterator_chunk] = (noise_maps_vector[iterator_chunk][i * grid_size + j] / layers_);					//noise colour
					distance_heightmap_color[iterator_chunk] = int(distance_heightmaps_vector[iterator_chunk][i * grid_size + j]);					//the voronoidiagram colour

					noise_output_vector[iterator_chunk].setPixel(j, i, sf::Color{ noise_color[iterator_chunk] , noise_color[iterator_chunk] , noise_color[iterator_chunk],a });
					voronoi_output_vector[iterator_chunk].setPixel(j, i, sf::Color{ distance_heightmap_color[iterator_chunk] , distance_heightmap_color[iterator_chunk] , distance_heightmap_color[iterator_chunk] });
					FinalImageLoop(i, j, grid_size, layers_, iterator_chunk, final_image_vector[iterator_chunk]);
				}
				track_output_vector[0].setPixel(j, i, sf::Color{ track_vertex_arr0[i * grid_size + j].color.r,track_vertex_arr0[i * grid_size + j].color.g,track_vertex_arr0[i * grid_size + j].color.b });
				track_output_vector[1].setPixel(j, i, sf::Color{ track_vertex_arr1[i * grid_size + j].color.r,track_vertex_arr1[i * grid_size + j].color.g,track_vertex_arr1[i * grid_size + j].color.b });
				track_output_vector[2].setPixel(j, i, sf::Color{ track_vertex_arr2[i * grid_size + j].color.r,track_vertex_arr2[i * grid_size + j].color.g,track_vertex_arr2[i * grid_size + j].color.b });
				track_output_vector[3].setPixel(j, i, sf::Color{ track_vertex_arr3[i * grid_size + j].color.r,track_vertex_arr3[i * grid_size + j].color.g,track_vertex_arr3[i * grid_size + j].color.b });
			}
		}
		//});
	}
	if(!is_chunking_) {
		noise_output_vector[0].saveToFile("0noise_layer.png");
		voronoi_output_vector[0].saveToFile("0voronoi_layer.png");
		track_output_vector[0].saveToFile("0track_image.png");
		final_image_vector[0].saveToFile("0final.png");
	}
	else {
		for (int c_i = 0; c_i < 4; c_i++) {
			std::string s = std::to_string(c_i);
			char const* c = "noise_layer.png";
			s += c;
			noise_output_vector[c_i].saveToFile(s);
			s = std::to_string(c_i);
			c = "voronoi_layer.png";
			s += c;
			voronoi_output_vector[c_i].saveToFile(s);
			s = std::to_string(c_i);
			c = "track_image.png";
			s += c;
			track_output_vector[c_i].saveToFile(s);
			s = std::to_string(c_i);
			c = "final.png";
			s += c;
			final_image_vector[c_i].saveToFile(s);
		}
	}
}
void ImageProcessing::SaveUpScale(int grid_size, float scale)
{
	int new_size = grid_size * scale;
	sf::Image image;
	image.loadFromFile("track_image.png");
	sf::Image scaled_image;
	scaled_image.create(new_size, new_size);

	for (int i = 0; i < (grid_size); i++)											//y
	{
		for (int j = 0; j < (grid_size); j++)										//x
		{
			sf::Uint8 c = image.getPixel(j, i).r;		//x,y
			int x_dash = j * new_size / grid_size;
			int y_dash = i * new_size / grid_size;
			scaled_image.setPixel(x_dash, y_dash, sf::Color{ c,c,c,255 });
		}
	}


	image.~Image();

	//coluns
	for (int i = 0; i < (new_size); i++)											//y
	{
		for (int j = 0; j < (new_size - 1); j += scale)
		{
			sf::Uint8 c = scaled_image.getPixel(j, i).r;		//x,y
			for (int g = 0; g < scale; g++)
			{
				scaled_image.setPixel(j + g, i, sf::Color{ c,c,c,255 });
			}
		}
	}
	//rows
	int j = 0;
	for (int i = 0; i < (new_size - 1); i += scale)											//y
	{
		for (j = 0; j < (new_size); j++)
		{
			sf::Uint8 c = scaled_image.getPixel(j, i).r;		//x,y
			for (int g = 0; g < scale; g++)
			{
				scaled_image.setPixel(j, i + g, sf::Color{ c,c,c,255 });
			}
		}
	}
	scaled_image.saveToFile("test.png");
}
void ImageProcessing::SaveUpScaledImage(int grid_sizez, sf::VertexArray& vertexarray, float scale)
{
	int new_size = grid_sizez * scale;
	//Creates the new image
	sf::Image scaled_image;
	scaled_image.create(new_size, new_size);
	parallel_for(0, grid_sizez, [&](int i)
		{
			for (int j = 0; j < (grid_sizez); j++)										//x
			{
				int x_dash = j * new_size / grid_sizez;
				int y_dash = i * new_size / grid_sizez;
				scaled_image.setPixel(x_dash, y_dash, vertexarray[i * grid_sizez + j].color);
			}
		});
	//coloms - fill in the coloms with the existing coloms
	parallel_for(0, new_size, [&](int i)
		{
			for (int j = 0; j < (new_size - 1); j += scale)
			{
				sf::Uint8 c = scaled_image.getPixel(j, i).r;		//x,y
				for (int g = 0; g < scale; g++)
				{
					scaled_image.setPixel(j + g, i, { c,c,c,255 });
				}
			}
		});

	//rows - same for rows
	int j = 0;
	for (int i = 0; i < (new_size - 1); i += scale)											//y
	{
		parallel_for(0, new_size, [&](int j)
			{
				sf::Uint8 c = scaled_image.getPixel(j, i).r;		//x,y
				for (int g = 0; g < scale; g++)
				{
					scaled_image.setPixel(j, i + g, { c,c,c,255 });
				}
			});
	}
	scaled_image.saveToFile("test.jpg");																//this takes the longest time complexituy wise.
}


void ImageProcessing::CreateImage(sf::VertexArray& vertexarray, int grid_size){
	parallel_for(0, grid_size, [&](int i){
			for (int j = 0; j < grid_size; j++){
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Transparent;
			}
		});
}

void ImageProcessing::DrawWidthTrack(sf::VertexArray& vertexarray, int grid_size, const std::vector<sf::Vector2i>& track_, const int& chunk_index){
	for (int i = 0; i < track_.size(); i++){
		auto x = track_[i].x;
		auto y = track_[i].y;
		if (x < grid_size &&y<grid_size){
			if (x < 0) {
				x = track_[i + 1].x;
			}
			if (y < 0) {
				y = track_[i + 1].y;
			}
			auto y_it = y;
			auto x_it = x;
			switch (chunk_index){
			case 0:
				break;
			case 1:
				x += grid_size;
				break;
			case 2:
				x += grid_size;
				y += grid_size;
				break;
			case 3:
				y += grid_size;
				break;
			}
			
			vertexarray[y_it * grid_size + x_it].position = sf::Vector2f(x, y);
			vertexarray[y_it * grid_size + x_it].color = sf::Color::White;
		}
	}
}

void ImageProcessing::WriteMetaFile() {
	std::ofstream meta_;
	char const* c = "meta.txt";
	meta_.open(c);
	if (is_chunking_){
		meta_ << "true";
	}
	else {
		meta_ << "false";
	}
	meta_.close();
}