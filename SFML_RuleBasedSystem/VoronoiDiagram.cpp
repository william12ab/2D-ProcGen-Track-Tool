#include "VoronoiDiagram.h"
#include <iostream>
#include <ppl.h>

using namespace concurrency;

#include <SFML/Graphics.hpp>
#include <random>


VoronoiDiagram::VoronoiDiagram()
{
	type = 1;
	grid_size_x = 0;
	num_of_points = 0;
	num_of_sites = 0;
	grid_v_1 = nullptr;
	sites_v_1 = nullptr;
	grid_distance = nullptr;
	heightmap_ = nullptr;
	heightmap_fbm_ = nullptr;
	noise_heightmap_ = nullptr;
	alpha_channel_ = nullptr;
	failed_ = false;
	srand(static_cast <unsigned> (time(0)));

	max_distance_ = 0;
	site_iterator = 0;

	high_point = 0;
	high_point_x = 0;
	high_point_y = 0;
	found_raidus = false;
	radius_length = 0;

	

	do_testing_ = false;
	
	track_max=0;
	track_min=0;


}

VoronoiDiagram::~VoronoiDiagram()
{
	delete[] grid_v_1;
	delete[] sites_v_1;				//crashes on this line 
	delete[] grid_distance;
	delete[] heightmap_;
	delete[] noise_heightmap_;
	delete[]alpha_channel_;
	delete[]heightmap_fbm_;
}

void VoronoiDiagram::InitVector(int grid_size, int num_points, int num_sites)
{
	grid_size_x = grid_size;
	grid_v_1 = new int[grid_size_x * grid_size_x];
	grid_distance = new int[ grid_size_x * grid_size_x ];
	heightmap_ = new float[grid_size_x * grid_size_x];
	heightmap_fbm_= new float[grid_size_x * grid_size_x];
	noise_heightmap_ = new int[grid_size_x * grid_size_x];
	alpha_channel_ = new int[grid_size_x * grid_size_x];
	sites_v_1 = new int[num_sites*2];

}


void VoronoiDiagram::RandomPlaceSites(int num_sites, int grid_size)
{
	
	//loop over the number of sites and push back sites
	for (int i = 0; i < (num_sites*2); i++)
	{
		sites_v_1[i] = rand() % grid_size;
	}
}



void VoronoiDiagram::DistributeSites(int num_sites, int grid_size)
{
	for (int i = 0; i < (num_sites * 2); i++)
	{
		
		sites_v_1[i] = rand() % (grid_size);
		i++;
		sites_v_1[i] = rand() % (grid_size );
	}
	sites_v_1[24] = grid_size/2;
	sites_v_1[25] = grid_size/2;
	for (int i = 0; i < (num_sites * 2); i++)
	{
		//if x is within the x of number 24 and if y is within y of 25
		if (i != 24)
		{
			if (sites_v_1[i] > (sites_v_1[24] - 50) && sites_v_1[i]< (sites_v_1[24] + 50) && sites_v_1[i + 1] > (sites_v_1[25] - 50) && sites_v_1[i+1] < (sites_v_1[25] + 50))
			{
				int s = sites_v_1[24]-sites_v_1[i];
				sites_v_1[i] -= rand() % 50+ 100;
				i++;
				int a = sites_v_1[25] - sites_v_1[i];
				sites_v_1[i] -= rand() % 50 + 100;
			}
			else
			{
				i++;
			}
		}
		else
		{
			i++;
		}
	}
}

void VoronoiDiagram::EqualDSites(int num_sites, int grid_size, int times_, int displacement)
{	
	srand(time(NULL));
	site_iterator = 0;
	sites_v_1[site_iterator] = 0;
	site_iterator++;
	sites_v_1[site_iterator] = 0;
	site_iterator++;

	sites_v_1[site_iterator] = 0;
	site_iterator++;
	sites_v_1[site_iterator] = (grid_size-1);
	site_iterator++;

	sites_v_1[site_iterator] = grid_size;
	site_iterator++;
	sites_v_1[site_iterator] = (grid_size)-1;
	site_iterator++;

	sites_v_1[site_iterator] = grid_size;
	site_iterator++;
	sites_v_1[site_iterator] = (grid_size - 1);
	site_iterator++;

	//only works for 25 sites for some reason
	//so 25 = 256, 81 = 128, 289=64
	int iter_max = 0;
	if (num_sites==25)
	{
		iter_max = 256;
	}
	if (num_sites==81)
	{
		iter_max = 128;
	}
	for (int sideLength = grid_size - 1; sideLength >= iter_max; sideLength /= 2)
	{
		int halfSide = sideLength / 2;

		//diamond
		for (int y = 0; y < (grid_size - 1); y += sideLength)
		{
			for (int x = 0; x < (grid_size - 1); x += sideLength)
			{
				sites_v_1[site_iterator] = (x + halfSide);
				site_iterator++;
				sites_v_1[site_iterator] = y + halfSide;
				site_iterator++;
			}
		}
			//square
		for (int j = 0; j < (grid_size ); j += halfSide)
		{
			for (int i = (j + halfSide) % sideLength; i < (grid_size ); i += sideLength)
			{
				sites_v_1[site_iterator] = i;
				site_iterator++;
				sites_v_1[site_iterator] = j;
				site_iterator++;
			}

		}

	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(-displacement, displacement);

	
	for (int t = 0; t < times_; t++)
	{
		
		for (int i = 0; i < (num_sites * 2); i++)
		{
			sites_v_1[i] += distribution(generator);
			//sites_v_1[i] += rand() % (displacement *2) + (-displacement);
			//so this is the amount added on
			//so between -80 and 80
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
			//sites_v_1[i] += rand() % (displacement * 2) + (-displacement);
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
					int w = grid_v_1[(j * grid_size) + i];
				}
			}
	});
	delete[] incr;

	//std::cout << "max distance :" << max_distance_<<"\n";
}


//the vector of ints "incr" is used in replace of the colors for the bruteforce method 
//the loop pushes back i into the vector at the amount of sites.
//each element is a sites "colour" 
//the distance is found at each site in comparison to the index of the loop in x and y direction.
//relative to the distance the cell is found of the diagram.
void VoronoiDiagram::CreateDiagram(int num_sites, int grid_size, int start, int end)
{
	int d = 0;											
	int* incr;
	incr = new int[num_sites];
	
	for (size_t i = 0; i < num_sites; i++)
	{
		incr[i] = i+1;
	}
	for (size_t j = start; j < end; j++)
	{
		for (size_t i=0;i< grid_size;i++)
		{
			int ind = -1, dist = INT_MAX;

			int s = 0;
			for (int p = 0; p < num_sites; p++)
			{
				d = DistanceSqrt(sites_v_1[s], sites_v_1[s+1], i, j);
				s += 2;
		
				if (d<dist)
				{
					dist = d;
					ind = p;
				}
			}
			//so if this point has a distance which all points do
			if (ind>-1)
			{
				int s = grid_v_1[(j * grid_size) + i];
				int p = incr[ind];
				grid_v_1[(j* grid_size)+i]=incr[ind];
				grid_distance[(j * grid_size) + i] = dist;
				int w = grid_v_1[(j * grid_size) + i];
			}
		}
	}
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
		for (int i=0; i< grid_size; i++)
		{
			if (i+1<grid_size&& j + 1 < grid_size)		//if in the bounds
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



void VoronoiDiagram::DrawVD(sf::VertexArray& vertextarray, int grid_size, int num_sites, int num_, float div_a)
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
						if (grid_distance[(i * grid_size) + j] >max_d_)
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
						c /= div_a;						//makes the colour smaller
						h_c /= div_a;
						heightmap_[i * grid_size + j] = h_c;

						vertextarray[i * grid_size + j].position = sf::Vector2f(j, i);
						vertextarray[i * grid_size + j].color = sf::Color{ c , c , c };
					}
		});


	//std::wcout <<"max distance: " <<max_d_<<"\n";
}

void VoronoiDiagram::CreateFinalHM(int grid_size, sf::VertexArray& vertexarray, int layers_)
{
	//y=i, x=j
	for (int i= 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			int i_alpha_two = alpha_channel_[i * grid_size + j];				//int version of alpha
			float i_alpha_percent = (float)i_alpha_two / 255.0f;				//alpha as value between 0.0 to 1.0

			int i_c_one = int(heightmap_[i * grid_size + j]);					//int value of c
			int i_c_two = (noise_heightmap_[i * grid_size + j] / layers_);					//int value of co

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

			vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
			vertexarray[i * grid_size + j].color = sf::Color{ final_c , final_c , final_c, final_a };
		}
	}
}

void VoronoiDiagram::WriteToFile(int grid_size, sf::VertexArray& vertexarray, int layers_)
{
	const int dimensions_ = grid_size;

	sf::Image voronoi_output;
	sf::Image noise_output;
	sf::Image final_i;
	sf::Image track_output;

	voronoi_output.create(grid_size, grid_size);
	noise_output.create(grid_size, grid_size);
	final_i.create(grid_size, grid_size);
	track_output.create(grid_size, grid_size);

	
	parallel_for(0, dimensions_, [&](int i)
	
		{
			for (int j = 0; j < dimensions_; j++)
			{
				sf::Uint8 c = int(heightmap_[i * grid_size + j]);					//the voronoidiagram colour
				sf::Uint8 co = (noise_heightmap_[i * grid_size + j]/ layers_);					//noise colour
				sf::Uint8 a = alpha_channel_[i * grid_size + j];					//alpha colour value of noise


				int i_alpha_two = alpha_channel_[i * grid_size + j];				//int version of alpha
				float i_alpha_percent = (float)i_alpha_two / 255.0f;				//alpha as value between 0.0 to 1.0
				int i_c_one = int(heightmap_[i * grid_size + j]);					//int value of c
				int i_c_two = (noise_heightmap_[i * grid_size + j]/ layers_);					//int value of co


				float i_c_t_a = (float)i_c_two / 255.0f;							//decimal value of co
				float is = (float)i_c_one / 255.0f;									//decimal value of c

				float alpha_percent_ = i_alpha_percent + 1.0f * (1.0f - i_alpha_percent);							//alpha_f = alpha_a + alpha_b(1-alpha_a)	(as a decimal value)
				float final_color_p = (i_c_t_a * i_alpha_percent + is * 1.0f * (1.0f - i_alpha_percent)) / alpha_percent_;		//final_c = (colour_a*alpha_a + colour_b*alpha_b(1-alpha_a))/alpha_final		as a percent
				//

				if ( final_color_p < 0.0f)
				{
					final_color_p = 0.0;
					//just to check if its out of bounds
					//happens for some reason when j=0 to 512 and i = 512
					//becuase of error in voronoi
					//fix it
				}
				if (final_color_p>1.0f)
				{
					int a = 1;
					final_color_p = 1.0;
				}

				//this is the premultiplied
				float i_c_f_t = i_c_t_a + is * (1.0f - i_alpha_percent);
				if (i_c_f_t > 1.0f)
				{
					i_c_f_t = 1.0f;
				}

				int premultiplied_version_colour = i_c_f_t * 255;
				int premultiplied_version_alpha = 255 * alpha_percent_;

				int f_c = 255 * final_color_p;											//need to multiply it by 255 to get as rgb value out of 255 instead of decimal
				int f_a = 255 * alpha_percent_;

				sf::Uint8 final_c = f_c;
				sf::Uint8 final_a = f_a;


				//setting the pixels of the output images
				voronoi_output.setPixel(j, i, sf::Color{ c , c , c });
				noise_output.setPixel(j, i, sf::Color{ co , co , co,a });
				final_i.setPixel(j, i, sf::Color{ final_c,final_c,final_c ,final_a });
				track_output.setPixel(j, i, sf::Color{ vertexarray[i * grid_size + j].color.r,vertexarray[i * grid_size + j].color.g,vertexarray[i * grid_size + j].color.b });

			}


	});
	noise_output.saveToFile("noise_layer.png");
	voronoi_output.saveToFile("voronoi_layer.png");
	final_i.saveToFile("final.png");
	track_output.saveToFile("track_image.png");
}

void VoronoiDiagram::DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			if (grid_v_1[(i * grid_size) + j] == -1)							//diagram
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Green;
			}
			if (grid_v_1[(i * grid_size) + j] > 0)								//pathway
			{
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
			}

			
		}
	}


}

void VoronoiDiagram::DrawVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size, int num_sites)
{


	for (int i = 0; i < grid_size; i++)
	{
		for (int j=0;j<grid_size;j++)
		{
			switch (grid_v_1[(i * grid_size) + j])
			{
			case -12303:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::White;
				break;
			case -1234:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
				break;
			case 0:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Red;
				break;
			case -3:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color::Blue;
				break;
			default:
				vertexarray[i * grid_size + j].position = sf::Vector2f(j, i);
				vertexarray[i * grid_size + j].color = sf::Color{ 0,0,0,0 };
				break;
			}
		}
	}
}

void VoronoiDiagram::FindMax(int grid_size, int layers_)
{
	high_point = 0;
	high_point_x = 0;
	high_point_y = 0;
	found_raidus = false;
	radius_length = 0;

	//if circles vector is empty then just find the max - meaning that no peaks exist currently so no  worries about checking for other peaks
	//if there are peaks, check if the current point is within a peak and if so dont find the max
	for (int i = 0; i < grid_size; i++)
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

					if (a+b>r)		//outside the circle, so find the max
					{
						not_found_in_circle = false;
						
					}
					else
					{
						found_in_first_circle = true;
					}
				}
				if (!not_found_in_circle&&!found_in_first_circle)				//so if not in ANY circle 
				{
					if (!found_in_first_circle)
					{
						int temp_height = noise_heightmap_[(i * grid_size) + j] / layers_;
						if (temp_height > high_point)
						{
							high_point = temp_height;
							high_point_x = j;
							high_point_y = i;
						}
					}
				}

			}
			else
			{
				int temp_height = noise_heightmap_[(i * grid_size) + j] / layers_;
				if (temp_height > high_point)
				{
					high_point = temp_height;
					high_point_x = j;
					high_point_y = i;
				}
			}
		
		}
	}

}
void VoronoiDiagram::DrawNoise(sf::VertexArray& vertexarray, int grid_size, int layers_)
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
				sf::Uint8 c = (noise_heightmap_[(i * grid_size) + j]/ layers_);														//this needs changed - might be right actually

			
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

void VoronoiDiagram::ChangeAlpha(sf::VertexArray& vertexarray, int grid_size, int alpha_)
{
	for (int i = 0; i < grid_size; i++)
	{
		for (int j = 0; j < grid_size; j++)
		{
			//stores alpha if changed and applies to exisiting image
			alpha_channel_[(i * grid_size) + j] = alpha_;
			sf::Uint8 c = alpha_;
			vertexarray[i * grid_size + j].color = sf::Color{ vertexarray[i * grid_size + j].color.r , vertexarray[i * grid_size + j].color.g ,vertexarray[i * grid_size + j].color.b, c};
		}
	}

}


void VoronoiDiagram::DrawFBM(sf::VertexArray& vertexarray, int grid_size, int octaves_)
{
	for (size_t i = 0; i < grid_size_x; i++)
	{
		for (size_t j = 0; j < grid_size_x; j++)
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
		for (int j = 0; j< grid_size; j++)
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
			heightmap_fbm_[(i * grid_size) + j] = height ;


			if (height<min_)					//gets the max and min values that the algo produces, so that they can then be changed to the range between 0 and 1 which can then be used to get a colour value
			{
				min_ = height;
			}
			if (height>max_)
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
			float new_value_ = ( ( (heightmap_fbm_[(i * grid_size) + j] - min_) * new_range_) / old_range_) + 0;	//gets the oldd value and turns it into a value between the new range
				
		

		
			int co = int(new_value_ * 255);						//geets as rgb value
			
			if (co>255)
			{
				co = 255;							
			}
			if (co<0)
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

	switch (type)
	{
	case 0:
	{
		int x_pos_one = (grid_size * 0.19);
		int x_pos_two = (grid_size * 0.01);
		int y_pos_one = (grid_size * 0.20);
		int y_pos_two = (grid_size * 0.75);
		for (int i = 0; i < num_points; i++)
		{
			bool found = false;
			int counter = 0;
			while (!found)
			{
				int x = rand() % x_pos_one + x_pos_two;
				int y = rand() % y_pos_one + y_pos_two;
				if (grid_v_1[(y * grid_size) + x] == 0)
				{
					found = true;
					grid_v_1[(y * grid_size) + x] = 2000 + i;
				}
				counter++;
				if (counter>200)
				{
					b_failed = true;
					break;
					std::cout << "didnt set a point\n";
				}
			}
			std::cout <<"counter(how many iters to find point): "<< counter<<std::endl;
			if (i==0)
			{
				x_pos_one = (grid_size * 0.20);
				x_pos_two = (grid_size * 0.45);
				y_pos_one = (grid_size * 0.19);
				y_pos_two = (grid_size * 0.01);
			}
			if (i == 1)
			{
				x_pos_one = (grid_size * 0.20);
				x_pos_two = (grid_size * 0.75);
				y_pos_one = (grid_size * 0.20);
				y_pos_two = (grid_size * 0.75);
			}
		}
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
			if (start+iter>grid_size)
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
				int y = rand() % (int)(grid_size / 1.1) + (grid_size / 16);
				if (grid_v_1[(y * grid_size) + x] == 0)
				{
					found = true;
					grid_v_1[(y * grid_size) + x] = 2000 + i;
				}
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
		int x_pos_one = (grid_size * 0.05);					//setting initial points so start point ranges
		int x_pos_two = (grid_size * 0.15);					//so its between 15% and 20% of x 
		int y_pos_one = (grid_size * 0.20);					//45% and 55% of y
		int y_pos_two = (grid_size * 0.40);
		for (int i = 0; i < num_points; i++)				//run for number of points needed(3)
		{
			bool found = false;
			int counter = 0;
			while (!found)									//create a point in the ranges until that point exists on the v_d
			{
				int x = rand() % x_pos_one + x_pos_two;
				int y = rand() % y_pos_one + y_pos_two;
				if (grid_v_1[(y * grid_size) + x] == 0)			//if found then set the grid structure to be the starting/middle/end point
				{
					found = true;
					grid_v_1[(y * grid_size) + x] = 2000 + i;
				}
				counter++;
				if (counter > 200)								//this is used incase there is no point that could be this - so if its taken too long to search for a point, give up and let the main program know that youve given up so it can make a decision from there
				{
					failed_ = true;
					break;
					std::cout << "didnt set a point\n";
				}
			}
			std::cout << "counter(how many iters to find point): " << counter << std::endl;

			if (i == 0)					//now change the point selection based on the iterator, so change to the middle and then the end for the selection
			{
				x_pos_one = (grid_size * 0.20);
				x_pos_two = (grid_size * 0.40);			//x is between 45% and 55%
				y_pos_one = (grid_size * 0.15);			//y is between 20% and 30%
				y_pos_two = (grid_size * 0.15);
			}
			if (i == 1)
			{
				x_pos_one = (grid_size * 0.05);
				x_pos_two = (grid_size * 0.80);			//x is between 80% and 85%
				y_pos_one = (grid_size * 0.20);
				y_pos_two = (grid_size * 0.40);			//y is between 45% and 55%
			}
		}
	}
		break;
	}

}


void VoronoiDiagram::HighPointFunc(int grid_size, int radius_cutoff_, int layers_,int index_v)
{
	//identify where x and y are in relation to the complete image
	//so are they top left, top right, bottom left, bottom right
	//then mark that as the direction to go in 
	int signal, x_pos,y_pos;
	std::cout << "HIGHEST point(peak): " << high_point_x << " " << high_point_y << "\n";
	if (high_point_x<=(grid_size/2) && high_point_y<=(grid_size/2))
	{ 
		//square 1 in diagram(top left) - going south east
		//[((y-1)*grid_size) + (x+1)]
		//y=-1, x=+1;
		x_pos =1;
		y_pos =1;
		signal = 1;
		
	}
	else if (high_point_x>= (grid_size / 2) && high_point_x <= (grid_size) && high_point_y<=(grid_size/2))
	{
		//square 2 in diagram(top right) - going south west 
		//[((y-1)*grid_size) + (x-1)]
		//y=-1, x=-1 for each iteration
		x_pos = -1;
		y_pos = 1;
		signal = 2;
		
	}
	else if (high_point_x<=(grid_size/2) && high_point_y>=(grid_size/2) && high_point_y<=grid_size)
	{
		//square 3 in diagram(bottom left) - going north east
		//[((y+1)*grid_size) + (x+1)]
		//y=+1,x=+1
		signal = 3;
		x_pos = 1;
		y_pos =- 1;
	}
	else if (high_point_x >= (grid_size / 2) && high_point_x <= (grid_size) && high_point_y>=(grid_size / 2) && high_point_y <= grid_size)
	{
		//square 4 in diagram(bottom right) - going north west
		//[((y+1)*grid_size) + (x-1)]
		//y=+1, x=-1
		signal = 4;
		x_pos =- 1;
		y_pos =- 1;	
	}
	temp_rad.resize(2);
	
	LoopPart(grid_size, x_pos, y_pos, signal, radius_cutoff_, layers_,1,0);
	LoopPart(grid_size, -x_pos, -y_pos, signal, radius_cutoff_, layers_,-1,1);
	radiiDecider(index_v);
}

void VoronoiDiagram::LoopPart(int grid_size, int x_value_, int y_value_, int signal_, int radius_cutoff_, int layers_, int modifier_, int place)
{
	int y =high_point_y;
	int x=high_point_x;
	int iterator_ = 0;
	found_raidus = false;
	do
	{
		//travelled the length of the radius then set found etc
		if (y > 0 &&  x>0 &&y<400 &&x<400)
		{
			if ((noise_heightmap_[((y + y_value_) * grid_size) + (x + x_value_)] / layers_) <= (radius_cutoff_))
			{

				std::cout << "POINT on radius: " << x + x_value_ << " " << y + y_value_ << "\n";

				std::cout << "RADIUS L: " << iterator_ << "\n";
				found_raidus = true;
				temp_rad.at(place) = (iterator_);

			}
			else
			{
				//other wise move one place in the correct direction 
				iterator_++;
				switch (signal_)
				{
				case 1:
					y += (1 * modifier_), x += (1 * modifier_);
					break;
				case 2:
					y += (1 * modifier_), x -= (1 * modifier_);
					break;
				case 3:
					y -= 1, x += 1;
					break;
				case 4:
					y -= (1 * modifier_), x -= (1 * modifier_);
					break;
				}
			}
		}
		else
		{
			failed_ = true;
			std::cout << "POINT on radius (at edge): " << x + x_value_ << " " << y + y_value_ << "\n";
			std::cout << "RADIUS L: " << iterator_ << "\n";
			found_raidus = true;
			temp_rad.at(place) = (iterator_);
		}
	} while (found_raidus!=true&& !failed_);

	
}

void VoronoiDiagram::radiiDecider(int index_v)
{
	if (temp_rad.at(0)>temp_rad.at(1) )					//chooses the bigger of the two but if its > 300 then chooses the smaller cos thats large
	{
		int p = 0;
		if (temp_rad.at(0) > 200 )
		{
			p = 1;
		}
		std::cout << "Selected radius "<< p <<"\n\n\n";
		
		radius_length = temp_rad.at(p);
		if (radius_length>200)
		{
			radius_length = 200 - (radius_length - 200);
		}

		circles_[index_v].centre_x = high_point_x;
		circles_[index_v].centre_y = high_point_y;
		circles_[index_v].r_length= radius_length;
	}
	else
	{
		int p = 1;
		if (temp_rad.at(1)>200)
		{
			p = 0;
		}

		std::cout << "Selected radius " << p << "\n\n\n";
		radius_length = temp_rad.at(p);
		std::cout << "RADIUS L: " << radius_length << "\n";
		if (radius_length > 200)
		{
			radius_length = 200 - (radius_length - 200);
		}

		circles_[index_v].centre_x = high_point_x;
		circles_[index_v].centre_y = high_point_y;
		circles_[index_v].r_length = radius_length;


	}
	//temp_rad.clear();
	
	
}
void VoronoiDiagram::vector_all(int size)
{
	//circles_.reserve(size);

	
	circles_.resize(size);
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
		sites_v_1[iterator_ ] = circles_[i].centre_y;
		iterator_++;
	}
	


}

void VoronoiDiagram::ResetVars()
{
	high_point=0;
	high_point_x=0;
	high_point_y=0;
	found_raidus=false;
	radius_length=0;
	circles_.clear();
	temp_rad.clear();
}


void VoronoiDiagram::FindMinMax(int grid_size, int layers_)
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
				if ((noise_heightmap_[i * grid_size + j] / layers_)>max_)
				{
					max_ = noise_heightmap_[i * grid_size + j] / layers_;
				}

				if ((noise_heightmap_[i * grid_size + j] / layers_) > min_)
				{
					min_ = noise_heightmap_[i * grid_size + j] / layers_;
				}
				//find max
				//find min of noise heighmap
				//noise_heightmap_[i * grid_size + j] / layers_

			}
		}
	}

	track_max=max_;
	track_min=min_;
}