#pragma once
#include "VoronoiDiagram.h"
#include "ShortestPath.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "DeCastelJau.h"
#include "ImageProcessing.h"
#include "TrackTools.h"
#include "InputManager.h"
#include "CatmullRomSpline.h"
#include "WidthCalculator.h"

sf::Font font;

sf::Text title_name_;


//the defaults for the application.
int resolution_;
int sites_;
int points_;
bool regen_;		//bool that tells the program whether it need to be regenerated or not
int track_type_;	//track type
bool render_height_map_;
bool n_render_height_map_;
bool f_render_height_map_;
bool is_render_track;
bool is_render_diagram;
bool is_chunking_;

void SetRenderHeightMap(bool f) { render_height_map_ = f; }
void SetRenderNoiseMap(bool f) { n_render_height_map_ = f; }
void SetRenderFullMap(bool f) { f_render_height_map_ = f; }

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


bool is_curved_;
bool is_widthed_;


int radius_cutoff;
int peaks_to_count_;
bool do_testing_;

float image_scale;
float step_curve;
float alpha_cm_;

struct measurements_chunked {
	std::vector<std::vector<sf::Vector2i>>control_points_;//vector of a vector of vector2is, so... control_points_[i][j][xory]
	std::vector<int> vec_number_of_turns;
	std::vector<std::vector<int>> vec_angles;
	std::vector<std::vector<int>> vec_new_angles;
	std::vector<std::vector<int>> vec_segment_lengths;
	std::vector<std::vector<sf::Vector2i>> vec_track_points;
	
}measurements_;

struct render_condition {
	bool render_diagram;
	bool render_track;
}struct_obj_render;


