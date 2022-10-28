#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include "SimplexNoise.h"
#include <iostream>
#include <vector>
class VoronoiDiagram
{
public:
	VoronoiDiagram();
	~VoronoiDiagram();
	
	void RandomPlaceSites(int num_sites, int grid_size);				//distrubutes the sites
	void EqualDSites(int num_sites, int grid_size, int times_, int displacement);						//distributes the sites in equal distance first then displaces them 
	void DistributeSites(int num_sites, int grid_size);					//random but one in the centre
	void TerrainSites(int num_sites, int grid_size);

	int DistanceSqrt(int x, int y, int x2, int y2);						//the distance formula
	void CreateDiagram(int num_sites,int grid_size, int start, int end);					//creates the voronoi diagram
	void SetEdges(int grid_size);										//sets the edges of voronoi diagram, so it is just the edges in the vector
	
	void SetPoint(int grid_size, int num_points, int type, bool b_failed);				//Sets the point(s) for the distance.
	void InitVector(int grid_size, int num_points, int num_sites);		//inits the vector to correct size;


	void DiagramAMP(int num_sites, int grid_size);						//creates the vd using parallel for


	//getters		
	int GetRadius() { return radius_length; }
	int GetCentreX() { return high_point_x; }
	int GetCentreY() { return high_point_y; }


	bool GetFailed() { return failed_; }						

	int* GetGrid() { return grid_v_1; }
	int* GetSites() { return sites_v_1; }
	float* GetHeightVal() { return heightmap_; }

	int GetNumberOfSites() { return num_of_sites; }						//returns the number of sites
	int GetGridSize() { return grid_size_x; };							//returns the size of the grid
	int GetType() { return type; };										//returns the type of track, closed or open
	int GetNumberOfPoints() { return num_of_points; }					//returns the number of points

	//setters
	void SetNumberOfSites(int p) { num_of_sites = p; }					//sets the number of sites
	void SetGridSize(int p) { grid_size_x=p; }							//sets the x and y of the grid
	void SetNumberOfPoints(int p) { num_of_points = p; }				//sets the number of points in the track
	void SetType(int p) { type = p; }									//sets the type of track
	void SetFaile(bool f) { failed_ = f; }
	void SetF(float f) { pFrequency = f; }
	void SetH(float h) { pHeightRange = h; }

	void DrawCurve(sf::VertexArray& vertexarray, int grid_size, int num_sites);
	void DrawVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size, int num_sites);
	void DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size);
	
	//sampling functions
	void ResizeImage(int grid_size, float scale);
	void UpScaleImage(int grid_sizez, float scale);
	sf::Color AverageColour(sf::Color a,sf::Color b);
	//


	void DrawVD(sf::VertexArray& vertextarray, int grid_size, int num_sites, int num_, float div_a);
	void DrawNoise(sf::VertexArray& vertexarray, int grid_size, int layers_);
	void DrawFBM(sf::VertexArray& vertexarray, int grid_size, int octaves_);

	void ChangeAlpha(sf::VertexArray& vertexarray, int grid_size, int alpha_);

	void CreateFinalHM(int grid_size, sf::VertexArray& vertexarray, int layers_);
	void WriteToFile(int grid_size, sf::VertexArray& vertexarray, int layers_);

	

	void HighPointFunc(int grid_size, int radius_cutoff_,int layers_, int index_v);
	void LoopPart(int grid_size, int x_value_, int y_value_, int signal_, int radius_cutoff_, int layers_, int modifier_, int place);
	void radiiDecider(int index_v);

	void ResetVars();

	void FindMax(int grid_size, int layers_);

	void FindMinMax(int grid_size, int layers_);

	int GetTrackMax() { return track_max; }
	int GetTrackMin() { return track_min; }


	void vector_all(int size);

	void SetTesting(bool f) { do_testing_ = f; }
	bool GetTesting() { return do_testing_; }
private:
	SimplexNoise perlin_;


	int num_of_sites;							//number of sites that form the diagram
	int num_of_points;							//number of points that form the track.
	int type;

	int grid_size_x;							//size in the x axis of the diagram

	int* sites_v_1;								//stores the sites in dynamic array
	int* grid_v_1;								//stores the grid in dynamic array

	int* grid_distance;
	float* heightmap_;
	float* heightmap_fbm_;
	int* noise_heightmap_;
	int* alpha_channel_;

	
	std::vector<int> temp_rad;

	int max_distance_;

	int site_iterator;

	bool failed_;

	float pFrequency = 1.0f;	//simplex noise frequency
	float pHeightRange = 1.0f;


	struct peaks_
	{
		int centre_x;
		int centre_y;
		int r_length;
	};

	int high_point;
	int high_point_x;
	int high_point_y;
	bool found_raidus;
	int radius_length;

	std::vector<peaks_>circles_;

	int track_max;
	int track_min;
	

	bool do_testing_;
};

