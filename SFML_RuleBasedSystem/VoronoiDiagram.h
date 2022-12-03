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
	void EqaullyDispursSites(int num_sites, int grid_size, int times_, int displacement);
	void TerrainSites(int num_sites, int grid_size);

	int DistanceSqrt(int x, int y, int x2, int y2);						//the distance formula
	void SetEdges(int grid_size);										//sets the edges of voronoi diagram, so it is just the edges in the vector
	
	void InitVector(int grid_size, int num_points, int num_sites);		//inits the vector to correct size;


	void DiagramAMP(int num_sites, int grid_size);						//creates the vd using parallel for


	//getters		
	int GetRadius() { return radius_length; }
	int GetCentreX() { return high_point_v.x; }
	int GetCentreY() { return high_point_v.y; }
	bool GetFailed() { return failed_; }						

	int* GetGrid() { return grid_v_1; }
	int* GetSites() { return sites_v_1; }
	int* GetGridDistance() { return grid_distance; }

	int GetNumberOfSites() { return num_of_sites; }						//returns the number of sites
	int GetGridSize() { return grid_size_x; };							//returns the size of the grid
	int GetType() { return type; };										//returns the type of track, closed or open
	int GetNumberOfPoints() { return num_of_points; }					//returns the number of points
	bool GetTesting() { return do_testing_; }
	int GetTrackMax() { return track_max; }
	int GetTrackMin() { return track_min; }
	sf::Vector2i &GetHighPoint() { return high_point_v; }
	sf::Vector2i &GetLowPoint() { return low_point_v; }
	bool GetStop() const{ return stop_; }
	//

	//setters
	void SetNumberOfSites(int p) { num_of_sites = p; }					//sets the number of sites
	void SetGridSize(int p) { grid_size_x=p; }							//sets the x and y of the grid
	void SetNumberOfPoints(int p) { num_of_points = p; }				//sets the number of points in the track
	void SetType(int p) { type = p; }									//sets the type of track
	void SetFaile(bool f) { failed_ = f; }
	void SetTesting(bool f) { do_testing_ = f; }
	void SetStop(bool f) { stop_ = f; }
	//
	
	//these functions should go in another class
	void DirectionDecider(const int &radius_cutoff_,const int &layers_,const int &index_v, int* const& noise_h_m, const sf::Vector2i& high_or_low, bool b_what_p);
	void FindCircumPoint(int x_value_, int y_value_, int signal_, const int radius_cutoff_, const int layers_, int modifier_, int place, int* const& noise_h_m, sf::Vector2i&circum_point_, const sf::Vector2i& high_or_low, bool b_what_p);
	void radiiDecider(const int &index_v, const sf::Vector2i& high_or_low);
	void ResetVars();
	void FindMax(const int &layers_,int* const &noise_grid);			//finds high point in terrain
	void FindMinMax(int layers_, int* const& noise_grid);			//finds low and high
	void SetHighPoint(const int &layers_, int* const &noise_grid, sf::Vector2i& high_point_v_, int& high_point_,const int &i,const int &j, int& min_height, sf::Vector2i &low_point_v_);
	void vector_all(int size); //resets vectors for terrain.
	void SetDirectionXY(int &signal, int &x, int &y, int a, int b, int c);
	void SetCircumPoint(sf::Vector2i& circum_point_, int x, int y, int iterator_, int place);
	void SelectRadii(const int &index_v,int a,int b, const sf::Vector2i& high_or_low);
	void SwitchPoint(int& iterator, int& y_, int& x_, const int signal_, int modifier_);
	void ArePointsFound(const int& high_point,const int& low_point);
	//

	//this annoying is here
	void ResizeGrid(int grid_size, float scale);																//resizes the grid
	void UpScaleGrid(int grid_size, float scale);																//upscales the grid
	

	//for setting points
	void SetPointModi(int& x, int& x_2, int& y, int& y_2, int grid_size, float x_v_1, float x_v_2, float y_v_1, float y_v_2);
	void PlacePoint(int x,int y,int grid_size, int i, bool&found_);
	void ThreePoints(int grid_size, int num_points, bool &b_failed, float values_[12]);
	void SetPoint(int grid_size, int num_points, int type, bool b_failed);				//Sets the point(s) for the distance.
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
		sf::Vector2i point=sf::Vector2i(0,0);
		int r_length=0;
	}peak_;

	int high_point;			//used			stores value of highest point. eg: say it was 200m in real values
	sf::Vector2i high_point_v;		//used		coords of highpoint
	sf::Vector2i low_point_v;
	bool found_raidus;
	int radius_length;

	std::vector<sf::Vector2i> circum_points;		//keeps track of the poiints on the circumference

	static std::vector<peaks_>circles_;
	

	int track_max;
	int track_min;
	bool do_testing_;


	int max_value_height;			//for chekcing if all points of interest have been found.
	bool stop_;						//for returning if all points have been found
};

