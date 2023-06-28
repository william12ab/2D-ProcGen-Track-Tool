#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include "SimplexNoise.h"
#include "VoronoiDiagram.h"
#include "StructRanges.h"

class ImageProcessing
{
public:
	ImageProcessing();
	~ImageProcessing();

	//init
	void InitStructures(int grid_size);

	//setters
	void SetF(float f) { pFrequency = f; }
	void SetH(float h) { pHeightRange = h; }
	void SetIsChunking(bool f) { is_chunking_ = f; }

	//Getters
	float* GetHeightVal(const int& index_) { return distance_heightmaps_vector[index_]; }
	int* GetNoiseMap(const int&index_) const { return noise_maps_vector[index_]; }
	bool GetIsChunking() { return is_chunking_; }

	//Drawing functions
	void DrawCurve(sf::VertexArray& vertexarray, int grid_size, int num_sites,int *grid);						//draws the curve to the vertex array		NOT USED
	void DrawTrack(sf::VertexArray& vertexarray, int grid_size, int num_sites, int* grid, const int& chunk_index, const ranges& limits_);									//draws the track
	void DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size, int* grid, const int& chunk_index, const ranges&limits_);									//draws the voronoi diagram
	void DrawVoronoiNoise(sf::VertexArray& vertextarray, const int& grid_size, const int& num_sites, const int& num_,int * const &grid_distance,const int& chunk_index, const ranges& init);				//draws the voronoi noise "worely noise" etc
	void DrawNoise(sf::VertexArray& vertexarray, int grid_size, int layers_, const float &frequency, const int& chunk_);									//draws the perlin noise
	void DrawFBM(sf::VertexArray& vertexarray, int grid_size, int octaves_, const float& frequency);									//draws the fbm
	void DrawWidthTrack(sf::VertexArray& vertexarray, int grid_size, const std::vector<sf::Vector2i>& track_);
	void CreateImage(sf::VertexArray& vertexarray, int grid_size);
	//

	//image modifiers
	void ChangeAlpha(sf::VertexArray& vertexarray, int grid_size, int alpha_);									//changes alpha
	sf::Color AverageColour(sf::Color a, sf::Color b);															//gets average colour
	//

	//sampling functions
	void ResizeImage(int grid_size, float scale);																//downscales image
	void UpScaleGrid(int grid_size, float scale, int* grid, VoronoiDiagram& v_d_p);																//upscales the grid
	void UpScaleVertexArray(int grid_size, float scale, sf::VertexArray& vertextarray);							//upscales a vertexarray
	void ResizeGrid(int grid_size, float scale, int* grid);																//resizes the grid
	//

	//saving functions
	void CreateFinalHM(int grid_size, sf::VertexArray& vertexarray, sf::VertexArray& vertexarray1, sf::VertexArray& vertexarray2, sf::VertexArray& vertexarray3, int layers_);								//creates final heightmap
	void WriteToFile(int grid_size, sf::VertexArray&track_vertex_arr0, sf::VertexArray& track_vertex_arr1, sf::VertexArray& track_vertex_arr2, sf::VertexArray& track_vertex_arr3, int layers_);									//saves to file
	void SaveUpScale(int grid_sizez, float scale);																//saves a upscaled image- first version
	void SaveUpScaledImage(int grid_sizez, sf::VertexArray& vertexarray, float scale);							//saves an upscaled image

private:
	SimplexNoise perlin_;

	float* heightmap_;//contains the pixels from the distance map- for outputting to file
	float* heightmap_1;
	float* heightmap_2;
	float* heightmap_3;
	int* noise_heightmap_;//noise pixels for outputting to file
	int* noise_heightmap_1;
	int* noise_heightmap_2;
	int* noise_heightmap_3;
	std::vector<int*> noise_maps_vector;
	std::vector<float*> distance_heightmaps_vector;
	int* alpha_channel_;
	float* heightmap_fbm_;

	float pFrequency = 1.0f;	//simplex noise frequency
	float pHeightRange = 1.0f;

	bool is_chunking_;
};

