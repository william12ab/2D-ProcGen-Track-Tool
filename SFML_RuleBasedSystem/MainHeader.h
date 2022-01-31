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
bool regen_;
int track_type_;
int num_threads_;