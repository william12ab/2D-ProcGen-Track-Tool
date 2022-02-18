#pragma once
#include "VoronoiDiagram.h"
#include "ShortestPath.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include <thread>
using std::thread;
VoronoiDiagram Voronoi_Diagram;
ShortestPath shortest_path_;

sf::Font font;

sf::Text title_name_;

std::vector<thread*> thread_vector;
//the defaults for the application.
int resolution_;
int sites_;
int points_;
bool regen_;
int track_type_;
int num_threads_;
bool render_height_map_;


int number_;		//make this high
float catch_;		//unused
float div_;			//make this high too

float frequency_;
float height_;

int alpha_;
int layers_;
int octaves_;

//time code

//the_clock::time_point startTime = the_clock::now();
	//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(),0, v_d_p->GetGridSize());
	//the_clock::time_point endTime = the_clock::now();
	//auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
	//std::cout << "time(v d): " << time_taken; std::cout << std::endl;
	//