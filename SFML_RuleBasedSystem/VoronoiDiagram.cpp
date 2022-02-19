#include "VoronoiDiagram.h"
#include <iostream>
//#include <amp.h>
#include <ppl.h>
using namespace concurrency;
using std::mutex;
using std::unique_lock;

#include <SFML/Graphics.hpp>



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

}

VoronoiDiagram::~VoronoiDiagram()
{
	delete[] grid_v_1;
	delete[] sites_v_1;
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
	//create temp vector, loop over twice as this is a 2D space, x and y position.
	for (int i = 0; i < (num_sites*2); i++)
	{
		sites_v_1[i] = rand() % grid_size;
		std::cout << i << " site x: " << sites_v_1[i] << "\n";
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

void VoronoiDiagram::EqualDSites(int num_sites, int grid_size)
{
	for (int i = 0; i < num_sites * 2; i++)
	{
		sites_v_1[i] = 0;
	}
	bool loop_cond = false;

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

	for (int sideLength = grid_size - 1; sideLength >= 256; sideLength /= 2)
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


	for (int i = 0; i < (num_sites * 2); i++)
	{
		sites_v_1[i] += rand()%200+(-100);
		std::cout << "rand num " << rand() % 200 + (-100) <<"\n";
		std::cout << i << " site x before change: " << sites_v_1[i] << "  ";
		if (sites_v_1[i]>=grid_size)
		{
			int difference = sites_v_1[i] - grid_size;
			difference+= rand() % 100 + 1;
			sites_v_1[i] -= difference;
		}
		if (sites_v_1[i] <= 0)
		{
			sites_v_1[i] += (-sites_v_1[i] - sites_v_1[i]);
		}
		std::cout <<i<< " site x after change: " << sites_v_1[i] << "\n";
		i++;
		sites_v_1[i] += rand() % 200 + (-100);
		std::cout << "site y before change: " << sites_v_1[i] << "  ";

		if (sites_v_1[i] >= grid_size)
		{
			int difference = sites_v_1[i] - grid_size;
			difference += rand() % 100 + 1;
			sites_v_1[i] -= difference;
		}
		if (sites_v_1[i] <= 0)
		{
			sites_v_1[i] += (-sites_v_1[i] - sites_v_1[i]);
		}
		std::cout << "site y after change: " << sites_v_1[i] << "\n";
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
	//mutex distance_mutex;
	

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
					//unique_lock<mutex> lock(distance_mutex);
					d = DistanceSqrt(sites_v_1[s], sites_v_1[s + 1], j, i);
					
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

	std::cout << "max distance :" << max_distance_<<"\n";
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
				d = DistanceSqrt(sites_v_1[s], sites_v_1[s+1], j, i);
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
void VoronoiDiagram::SetEdges(int grid_size)
{
	for (int j = 0; j < grid_size; j++)
	{
		for (int i=0; i< grid_size; i++)
		{
			if (i+1<grid_size)		//if in the bounds
			{
				if (grid_v_1[(j * grid_size) + i] != 0 && grid_v_1[(j * grid_size) + (i + 1)] != 0)		//if current position isnt 0 and position one to left is not 0 
				{
					if (grid_v_1[(j * grid_size) + i] != grid_v_1[(j * grid_size) + (i + 1)])		//if the current pos and pos 1 to the left are not the same
					{
						grid_v_1[(j * grid_size) + i] = 0;			//set to path way
					}
				}
			}
			if (j + 1 < grid_size)
			{
				if (grid_v_1[(j * grid_size) + i] != 0 && grid_v_1[((j + 1) * grid_size) + i] != 0)
				{
					if (grid_v_1[(j * grid_size) + i] != grid_v_1[((j + 1) * grid_size) + i])
					{
						grid_v_1[(j * grid_size) + i] = 0;
					}
				}
			}

		}
	}
}



void VoronoiDiagram::DrawVD(sf::VertexArray& vertextarray, int grid_size, int num_sites, int num_, float c_, float div_a)
{
	int max_d_ = 0;
	parallel_for(0, grid_size, [&](int i)
		{
			for (int j = 0; j < grid_size; j++)
			{
				for (int a = 1; a <= num_sites; a++)
				{
					
					if (grid_v_1[(i * grid_size) + j] == a)
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
				}
			}
		});


	std::wcout <<"max distance: " <<max_d_<<"\n";
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


				if (final_color_p > 1.0f || final_color_p < 0.0f)
				{
					int a = 1;
					//just to check if its out of bounds
					//happens for some reason when j=0 to 512 and i = 512
					//becuase of error in voronoi
					//fix it
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

void VoronoiDiagram::DrawVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size)
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

void VoronoiDiagram::DrawNoise(sf::VertexArray& vertexarray, int grid_size, int layers_)
{
	for (size_t i = 0; i < grid_size_x; i++)
	{
		for (size_t j = 0; j < grid_size_x; j++)
		{
			noise_heightmap_[(i * grid_size) + j] = 0;
		}
	}
	const float scale = 100.0f / (float)grid_size;
	float low_ = 0.01f;
	float high_ = 0.010f;
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
		std::wcout << "min : " << min_ << "\n";
		std::wcout << "max : " << max_ << "\n";
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


	for (size_t i = 0; i < grid_size; i++)
	{
		for (size_t j = 0; j< grid_size; j++)
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
			heightmap_fbm_[(i * grid_size) + j] = height - 1;


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
	std::wcout << "min : " << min_ << "\n";
	std::wcout << "max : " << max_ << "\n";
	//prints out those values 

	float new_min_ = INT_MAX;
	float new_max_ = 0;
	float old_range_ = (max_ - min_);										//gets the range of the old values
	std::wcout << "old range: " << old_range_ << "\n";						//prints this out for debugging
	for (size_t i = 0; i < grid_size; i++)
	{
		for (size_t j = 0; j < grid_size; j++)
		{
			float new_value_ = (((heightmap_fbm_[(i * grid_size) + j] - min_) * new_range_) / old_range_) + 0;	//gets the oldd value and turns it into a value between the new range
				

		
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
	std::wcout << "new min : " << min_ << "\n";
	std::wcout << "new max : " << max_ << "\n";

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
	//zero is loop, 1 is p2p

	switch (type)
	{
	case 0:
	{
		int x_pos_one = (grid_size * 0.13);
		int x_pos_two = (grid_size * 0.02);
		int y_pos_one = (grid_size * 0.15);
		int y_pos_two = (grid_size * 0.80);
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
				y_pos_one = (grid_size * 0.13);
				y_pos_two = (grid_size * 0.02);
			}
			if (i == 1)
			{
				x_pos_one = (grid_size * 0.15);
				x_pos_two = (grid_size * 0.80);
				y_pos_one = (grid_size * 0.15);
				y_pos_two = (grid_size * 0.80);
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
		int y_pos_one = (grid_size * 0.10);					//45% and 55% of y
		int y_pos_two = (grid_size * 0.45);
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
				x_pos_one = (grid_size * 0.10);
				x_pos_two = (grid_size * 0.45);			//x is between 45% and 55%
				y_pos_one = (grid_size * 0.10);			//y is between 20% and 30%
				y_pos_two = (grid_size * 0.20);
			}
			if (i == 1)
			{
				x_pos_one = (grid_size * 0.05);
				x_pos_two = (grid_size * 0.80);			//x is between 80% and 85%
				y_pos_one = (grid_size * 0.10);
				y_pos_two = (grid_size * 0.45);			//y is between 45% and 55%
			}
		}
	}
		break;
	}

}

