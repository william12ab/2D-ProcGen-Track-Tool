#include "ImageProcessing.h"
#include <ppl.h>
#include <SFML/Graphics/Image.hpp>
using namespace concurrency;
ImageProcessing::ImageProcessing()
{
	heightmap_ = nullptr;
	heightmap_fbm_ = nullptr;
	noise_heightmap_ = nullptr;
	alpha_channel_ = nullptr;
}

ImageProcessing::~ImageProcessing()
{
	delete[] heightmap_;
	delete[] noise_heightmap_;
	delete[]alpha_channel_;
	delete[]heightmap_fbm_;
}

void ImageProcessing::InitStructures(int grid_size)
{
	heightmap_ = new float[grid_size * grid_size];
	heightmap_fbm_ = new float[grid_size * grid_size];
	noise_heightmap_ = new int[grid_size * grid_size];
	alpha_channel_ = new int[grid_size * grid_size];
}

void ImageProcessing::DrawCurve(sf::VertexArray& vertexarray, int grid_size, int num_sites,int *grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			switch (grid[(i * grid_size) + j])
			{
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

void ImageProcessing::DrawTrack(sf::VertexArray& vertexarray, int grid_size, int num_sites,int *grid)
{
	parallel_for(0, grid_size, [&](int i)
		{
			for (int j = 0; j < grid_size; j++)
			{
				switch (grid[(i * grid_size) + j])
				{
				case -12303:
					vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
					vertexarray[i * grid_size + j].color = sf::Color::White;
					break;
				case -1234:
					vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
					vertexarray[i * grid_size + j].color = sf::Color::White;
					break;
				case 0:
					vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
					vertexarray[i * grid_size + j].color = sf::Color::White;
					break;
				case -3:
					vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
					vertexarray[i * grid_size + j].color = sf::Color::White;
					break;
				default:
					vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
					vertexarray[i * grid_size + j].color = sf::Color{ 0,0,0,0 };
					break;
				}
			}
		});
}
void ImageProcessing::DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size,int*grid)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid[(i * grid_size) + j] == -1)							//diagram
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Green;
			}
			if (grid[(i * grid_size) + j] > 0)								//pathway
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
			}


		}
	}
}
void ImageProcessing::DrawVoronoiNoise(sf::VertexArray& vertextarray, int grid_size, int num_sites, int num_, int* grid_distance)
{
	int max_d_ = 0;
	parallel_for(0, grid_size, [&](int i)
		{
			for (int j = 0; j < grid_size; j++)
			{
				float s = float((float)1 / (float)num_sites);							//gets the thing as a percentage
						//for example 1/100 = 0.01

				int d = grid_distance[(i * grid_size) + j] / (float)num_;			//the distance divided by a number, so makes the distance smaller or bigger
						//for example could be 1000/25 = 40
						//so distance is higher with less sites, distance lower with more sites, so with higher sites you need lower num_

				int n = s * 255;										//percentage * 255
						//this is the percentage of the colour
				if (grid_distance[(i * grid_size) + j] > max_d_)
				{
					max_d_ = grid_distance[(i * grid_size) + j];
				}
				int r = d;
				//so the further away the distance is higher r
				if (r > 255)
				{
					r = 255;				//keeps the values within range
				}
				sf::Uint8 c = 255 - r;
				float h_c = int(255 - r);
				//so end color, closer to the site you are - brighter, further away - darker
				c /= 2;						//makes the colour smaller
				h_c /= 2;
				heightmap_[i * grid_size + j] = h_c;

				vertextarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertextarray[i * grid_size + j].color = sf::Color{ c , c , c };
			}
		});
}
void ImageProcessing::DrawNoise(sf::VertexArray& vertexarray, int grid_size, int layers_)
{
	for (size_t i = 0; i < grid_size; i++)
	{
		for (size_t j = 0; j < grid_size; j++)
		{
			noise_heightmap_[(i * grid_size) + j] = 0;
		}
	}
	const float scale = 100.0f / (float)grid_size;
	float low_ = 0.01f;
	float high_ = 0.020f;
	float r3 = low_ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high_ - low_)));				//the frequency of the noise between low and high
	float e2 = r3;

	float min_ = INT_MAX;
	float max_ = 0.0f;


	float old_range_ = (1 - (-1));
	float new_range_ = (1 - 0);

	for (int a = 0; a < layers_; a++)
	{
		float r3 = low_ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high_ - low_)));

		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{

				float height = (float)perlin_.noise(j, i, (r3 * scale)) * pHeightRange;
				float new_value_ = (((height - (-1)) * new_range_) / old_range_) + 0;
				height = 0.5f * (height + 1.0f);				//keeps the value between 0-1 instead of the -1 to 1 range it is in initially


				int co = int(new_value_ * 255);						//geets as rgb value


				noise_heightmap_[(i * grid_size) + j] += co;				//stores colour value for use in saving images
				alpha_channel_[i * grid_size + j] = 255;				//stores alpha value
				sf::Uint8 c = (noise_heightmap_[(i * grid_size) + j] / layers_);														//this needs changed - might be right actually


				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color{ c , c , c };
				if (new_value_ < min_)
				{
					min_ = new_value_;
				}
				if (new_value_ > max_)
				{
					max_ = new_value_;
				}
			}
		}
	}
}
void ImageProcessing::DrawFBM(sf::VertexArray& vertexarray, int grid_size, int octaves_)
{
	for (size_t i = 0; i < grid_size; i++)
	{
		for (size_t j = 0; j < grid_size; j++)
		{
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


	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			float a = 3.0f;						//controls the height.
			float mul = scale * 1.0f;
			float height = 0.0f;
			for (int o = 0; o < octaves_; o++)
			{
				height += (perlin_.noise(i * mul, j * mul, r3)) * a;
				a *= 0.5f;
				mul *= 2.0f;
			}
			heightmap_fbm_[(i * grid_size) + j] = height;


			if (height < min_)					//gets the max and min values that the algo produces, so that they can then be changed to the range between 0 and 1 which can then be used to get a colour value
			{
				min_ = height;
			}
			if (height > max_)
			{
				max_ = height;
			}
		}
	}

	//prints out those values 

	float new_min_ = INT_MAX;
	float new_max_ = 0;
	float old_range_ = (max_ - min_);										//gets the range of the old values

	for (size_t i = 0; i < grid_size; i++)
	{
		for (size_t j = 0; j < grid_size; j++)
		{
			float new_value_ = (((heightmap_fbm_[(i * grid_size) + j] - min_) * new_range_) / old_range_) + 0;	//gets the oldd value and turns it into a value between the new range




			int co = int(new_value_ * 255);						//geets as rgb value

			if (co > 255)
			{
				co = 255;
			}
			if (co < 0)
			{
				co = 0;									//a problem currently exists where there is a region escaping the limits iof the colour which tells me that the range is somehow wrong.
			}
			noise_heightmap_[(i * grid_size) + j] = co;				//sets the colour value to the heightmap
			alpha_channel_[i * grid_size + j] = 255;				//stores alpha value
			sf::Uint8 c = (noise_heightmap_[(i * grid_size) + j]);		//sets the colour value to an sfml colour value

			vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
			vertexarray[i * grid_size + j].color = sf::Color{ c , c , c };

			if (new_value_ < new_min_)			//gets the new max and min for debugging purposes 
			{
				new_min_ = new_value_;
			}
			if (new_value_ > new_max_)
			{
				new_max_ = new_value_;
			}

		}
	}
}
//

//image modifiers
void ImageProcessing::ChangeAlpha(sf::VertexArray& vertexarray, int grid_size, int alpha_)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			//stores alpha if changed and applies to exisiting image
			alpha_channel_[(i * grid_size) + j] = alpha_;
			sf::Uint8 c = alpha_;
			vertexarray[i * grid_size + j].color = sf::Color{ vertexarray[i * grid_size + j].color.r , vertexarray[i * grid_size + j].color.g ,vertexarray[i * grid_size + j].color.b, c };
		}
	}
}
sf::Color ImageProcessing::AverageColour(sf::Color a, sf::Color b)
{
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
void ImageProcessing::UpScaleGrid(int grid_size, float scale,int *grid)
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
	ResizeGrid(grid_size, scale);

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
void ImageProcessing::ResizeGrid(int grid_size, float scale,int *grid)
{
	size_t new_size = grid_size * scale;
	int* new_arr = new int[new_size * new_size];

	//resizing grid
	std::copy(grid, grid + (grid_size* grid_size), new_arr + 0);
	grid_size = new_size;
	delete[] grid;
	grid = new_arr;
}
//

//saving functions
void ImageProcessing::CreateFinalHM(int grid_size, sf::VertexArray& vertexarray, int layers_)
{

}
void ImageProcessing::WriteToFile(int grid_size, sf::VertexArray& vertexarray, int layers_)
{}
void ImageProcessing::SaveUpScale(int grid_sizez, float scale)
{

}
void ImageProcessing::SaveUpScaledImage(int grid_sizez, sf::VertexArray& vertexarray, float scale)
{

}