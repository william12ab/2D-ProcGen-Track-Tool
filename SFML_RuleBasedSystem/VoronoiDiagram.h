#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include "SimplexNoise.h"
#include <iostream>
#include <vector>
#include "StructRanges.h"
#include <random>
class VoronoiDiagram
{
public:
	VoronoiDiagram();
	~VoronoiDiagram();
	
	void RandomPlaceSites();				//distrubutes the sites
	void EqaullyDispursSites(const int &times_, const int &displacement);				//equally places sites
	void TerrainSites();					//site placement for terrain 

	int DistanceSqrt(const int& x, const int& y, const int& x2, const int& y2);						//the distance formula
	void SetEdges(const int& chunk_index);										//sets the edges of voronoi diagram, so it is just the edges in the vector
	void InitVector(const int &grid_size, const int &num_points, const int &num_sites);		//inits the vector to correct size;
	void DiagramAMP(const int& chunk_index);						//creates the vd using parallel for

	//getters		
	int GetRadius() { return radius_length; }
	bool GetFailed() { return failed_; }						
	int* GetGrid(const int& index) { return grid_vector[index]; }
	int* GetSites() { return sites_v_1; }
	int* GetGridDistance(const int& index) { return distance_grid_vector[index]; }
	int GetNumberOfSites() { return num_of_sites; }						//returns the number of sites
	int GetGridSize() { return grid_size_x; };							//returns the size of the grid
	int GetType() { return type; };										//returns the type of track, closed or open
	int GetNumberOfPoints() { return num_of_points; }					//returns the number of points
	bool GetTesting() { return do_testing_; }
	sf::Vector2i &GetHighPoint() { return high_point_v; }
	sf::Vector2i &GetLowPoint() { return low_point_v; }
	bool GetStopH() const{ return stop_high_; }
	bool GetStopL() const { return stop_low_; }
	bool GetFailSP() const { return is_fail_sp; }
	

	std::vector<sf::Vector2i> GetPointPos() { return point_pos; }
	//
	void EmptyCircles();
	void EmptyAllCircleVec();
	//setters
	void SetNumberOfSites(int p) { num_of_sites = p; }					//sets the number of sites
	void SetGridSize(int p) { grid_size_x=p; }							//sets the x and y of the grid
	void SetNumberOfPoints(int p) { num_of_points = p; }				//sets the number of points in the track
	void SetType(int p) { type = p; }									//sets the type of track
	void SetFaile(bool f) { failed_ = f; }
	void SetTesting(bool f) { do_testing_ = f; }
	void SetStopH(bool f) { stop_high_ = f; }
	void SetStopL(bool f) { stop_low_ = f; }
	void SetFailSP(bool f) { is_fail_sp = f; }
	//
	
	//these functions should go in another class, is to do with heightmap information and setting of circles
	void DirectionDecider(const int &radius_cutoff_,const int &layers_,const int &index_v, int* const& noise_h_m, const sf::Vector2i& high_or_low, bool b_what_p, const ranges& init);
	void FindCircumPoint(int x_value_, int y_value_, int signal_, const int radius_cutoff_, const int layers_, int modifier_, int place, int* const& noise_h_m, sf::Vector2i&circum_point_, const sf::Vector2i& high_or_low, bool b_what_p);
	void radiiDecider(const int &index_v, const sf::Vector2i& high_or_low);
	void ResetVars();
	void FindMax(const int &layers_,int* const &noise_grid, const ranges&init);			//finds high point in terrain
	void SetHighPoint(const int &layers_, int* const &noise_grid, sf::Vector2i& high_point_v_, int& high_point_,const int &i,const int &j, int& min_height, sf::Vector2i &low_point_v_);
	void vector_all(int size); //resets vectors for terrain.
	void SetDirectionXY(int &signal, int &x, int &y, int a, int b, int c);
	void SetCircumPoint(sf::Vector2i& circum_point_, int x, int y, int iterator_, int place);
	void SelectRadii(const int &index_v,int a,int b, const sf::Vector2i& high_or_low);
	void SwitchPoint(int& iterator, int& y_, int& x_, const int signal_, int modifier_);
	void ArePointsFound(const int& high_point,const int& low_point);
	//

	//for chunking
	void AddingCirclesToContainer(const ranges&init);
	void ResetSitesForChunking(const int& num_of_sites_param);
	void InsertChunks();
	void SetForChunks();
	void SetIsChunking(bool f) { local_is_chunking = f; }
	bool GetIsChunking(){return local_is_chunking;}
	void DivideChunks();
	void XYPass(const int& chunk_index, int& x_, int& y_, const sf::Vector2i& p_last_point);
	void CaseFunction( const int& chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_, bool& found_, int& counter_, int& x, int& y);
	void SetFirstPoint();
	void PushFirstPoint(const int& chunk_index);
	//
	void SetSitesForLarge(std::default_random_engine gen_, std::uniform_int_distribution<int> dist_);
	//
	//this annoying is here
	void ResizeGrid(float scale);																//resizes the grid
	void UpScaleGrid(int grid_size, float scale);																//upscales the grid
	//

	//for setting points
	void SetPointModi(int& x, int& x_2, int& y, int& y_2, const float &x_v_1, const float &x_v_2, const float &y_v_1, const float &y_v_2);
	void PlacePoint(int x,int y, int i, bool&found_, const int & chunk_index);
	void ThreePoints(const float values_[12], const int& chunk_index);
	void SetPoint(int type, const int & chunk_index);				//Sets the point(s) for the distance.
	void SetPointDefault(const int& chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_, const int& used_if_chunked);
	void SetPointHeightExtented(const int& chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_);
	void SetPointOnEdgeHeight(bool& found_, int& counter_, const int& chunk_index, std::default_random_engine gen_, std::uniform_int_distribution<int> dist_, int& x_pos_changed, int& y_pos_changed);
	void SetPointInMiddle(bool& found_, int& counter_, int& x_pos_changed, int& y_pos_changed, const int& chunk_index, std::default_random_engine gen_);
private:
	SimplexNoise perlin_;

	int num_of_sites;							//number of sites that form the diagram
	int num_of_points;							//number of points that form the track.
	int type;
	int grid_size_x;							//size in the x axis of the diagram
	int* sites_v_1;								//stores the sites in dynamic array
	int* grid_v_1;								//stores the grid in dynamic array
	int* grid_chunk_1;
	int* grid_chunk_2;
	int* grid_chunk_3;
	int* grid_distance;							//stores the distance map of the voronoi diagram
	int* grid_distance_c_1;
	int* grid_distance_c_2;
	int* grid_distance_c_3;
	std::vector<int*> grid_vector;
	std::vector<int*> distance_grid_vector;

	int* full_grid_chunking;

	static std::vector<sf::Vector2i> point_pos;
	sf::Vector2i last_point_pos;

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
	
	static std::vector<peaks_>all_circles_vector;

	bool do_testing_;


	int max_value_height;			//for chekcing if all points of interest have been found.
	bool stop_high_;				//for returning if all points have been found
	bool stop_low_;

	bool local_is_chunking;

	sf::Vector2i temp_vec_last_point;
	bool is_fail_sp;
};

