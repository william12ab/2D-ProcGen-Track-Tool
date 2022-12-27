#pragma once
#include "VoronoiDiagram.h"
#include "ShortestPath.h"
#include "ImageProcessing.h"
#include "WidthCalculator.h"
#include "CatmullRomSpline.h"

class TrackTools
{
public:
	TrackTools();

	void ResetVars(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map, sf::VertexArray& n_height_map);				//resets the vars
	void SetVars(VoronoiDiagram &v_d_p, ImageProcessing &i_p_p, int track_type_, int resolution_, int sites_, int points_);											//sets the vars used
	void ClearStructs(VoronoiDiagram &v_d_p, sf::VertexArray& voronoi_d, sf::VertexArray& n_height_map, sf::VertexArray& distance_map, ImageProcessing &i_p_p, int track_type_, int resolution_, int sites_, int points_);	//clears the structs

	void CreateVoronoi(VoronoiDiagram &v_d_p, sf::VertexArray& height_map, ImageProcessing& i_p_p,int times_, int displacement_, int number_,bool full_random_, int track_type_);																							//creates voronoi diagram and voronoi noise, sets points and edges
	void CreateTrack(VoronoiDiagram &v_d_p, ShortestPath &s_p_p);																									//creates the track, 
	void Generate(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map, sf::VertexArray& n_height_map, ImageProcessing &i_p_p, int times_, int displacement_, int number_, bool full_random_, int track_type_);				//pulls above together and outputs.
	void GenerateTerrainMethod(VoronoiDiagram &v_d_p, sf::VertexArray& vertex_array, ImageProcessing &i_p_p, int number_, int track_track_);

	void TerrainLoop(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map, sf::VertexArray& n_height_map, ImageProcessing &i_p_p, int number_, int track_type_);

	void WidthSettings(WidthCalculator& w_c, ShortestPath & s_p, VoronoiDiagram&v_d, ImageProcessing&i_p, sf::VertexArray&voronoi_d,const int& layers_, std::vector<sf::Vector2i>& track_);
	void WidthOnlyTValue(WidthCalculator& w_c, ShortestPath& s_p, VoronoiDiagram& v_d, ImageProcessing& i_p, sf::VertexArray& voronoi_d, std::vector<sf::Vector2i>& track_);
private:
};

