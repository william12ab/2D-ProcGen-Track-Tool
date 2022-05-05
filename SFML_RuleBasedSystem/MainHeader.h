#pragma once
#include "VoronoiDiagram.h"
#include "ShortestPath.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"


VoronoiDiagram Voronoi_Diagram;
ShortestPath shortest_path_;

sf::Font font;

sf::Text title_name_;


//the defaults for the application.
int resolution_;
int sites_;
int points_;
bool regen_;		//bool that tells the program whether it need to be regenerated or not
int track_type_;	//track type
bool render_height_map_;


int number_;		//make this high

float div_;			//make this high too

float frequency_;			//frequency of noise
float height_;				//heighht of noise

int alpha_;					//the alpha value for the slider that is passed into the function that changes the alpha
int layers_;				//how many layers of perlin(simplex) noise are you doing
int octaves_;				//how many octaves(basically how many times you doing the FBM) 

bool full_random_;			//deciding what metyhjdo to use
int times_;					//how many times to repeat the dispacement
int displacement_;


int radius_cutoff;
int peaks_to_count_;
bool do_testing_;

//time code

//the_clock::time_point startTime = the_clock::now();
	//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(),0, v_d_p->GetGridSize());
	//the_clock::time_point endTime = the_clock::now();
	//auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
	//std::cout << "time(v d): " << time_taken; std::cout << std::endl;
	//