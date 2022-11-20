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
	void EqaullyDispursSites(int num_sites, int grid_size, int times_, int displacement);
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
	int* GetGridDistance() { return grid_distance; }

	int GetNumberOfSites() { return num_of_sites; }						//returns the number of sites
	int GetGridSize() { return grid_size_x; };							//returns the size of the grid
	int GetType() { return type; };										//returns the type of track, closed or open
	int GetNumberOfPoints() { return num_of_points; }					//returns the number of points
	//

	//setters
	void SetNumberOfSites(int p) { num_of_sites = p; }					//sets the number of sites
	void SetGridSize(int p) { grid_size_x=p; }							//sets the x and y of the grid
	void SetNumberOfPoints(int p) { num_of_points = p; }				//sets the number of points in the track
	void SetType(int p) { type = p; }									//sets the type of track
	void SetFaile(bool f) { failed_ = f; }
	//
	
	//these functions should go in another class
	void HighPointFunc(int grid_size, int radius_cutoff_,int layers_, int index_v,int*noise_h_m);
	void LoopPart(int grid_size, int x_value_, int y_value_, int signal_, int radius_cutoff_, int layers_, int modifier_, int place, int* noise_h_m);
	void radiiDecider(int index_v);
	void ResetVars();
	void FindMax(int grid_size, int layers_,int* noise_grid);
	void FindMinMax(int grid_size, int layers_,int*noise_grid);
	//

	void ResizeGrid(int grid_size, float scale);																//resizes the grid
	void UpScaleGrid(int grid_size, float scale);																//upscales the grid


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

