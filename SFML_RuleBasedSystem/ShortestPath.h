#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <utility>

class ShortestPath
{
public:
	ShortestPath();
	void Initgrid(const int &grid_size, int*grid, const int &num_points);																				//inits the grid to the correct numbers
	void PhaseOne(const int &grid_size, int *grid, int end_n, int start_p, int end_p);	//finds the distances between start and end
	void PhaseTwo(const int &grid_size, int *grid, int end_n,const int&num_points, const int&currernt_point, const sf::Vector2i& pointpos, const sf::Vector2i& prevpointpos);							//finds a pathway between start and end

	void CleanGrid(const int &grid_size, int* grid);																							//removes numbers found by phase 1
	void ChangePoint(const int &grid_size, int* grid, int point, int new_point);																//changes the point passed in
	void PrintOutStartEnd(const int &grid_size,int* const& grid);																					//prints out the points in the track, useful for debugging

	void ReOrderArrays();
	void SetForChunk();
	//getters
	bool bGetFoundEnd() { return found_end; }								
	int GetCountHolder() { return count_holder_; }
	int GetIt() { return it; }
	bool bGetEnd() { return end_;}
	int GetXHolder() { return x_holder_; }
	int GetYHolder() { return y_holder_; }
	int GetTotalDistance() { return total_track_distance; }
	int GetNumberOfTurns() { return number_of_turns; }
	int GetNumberOfSegments() { return number_of_segments; }
	bool GetFailed() { return failed_; }
	bool GetTesting() { return do_testing_; }
	std::vector<sf::Vector2i> GetControlPoints() { return control_points; }
	std::vector<sf::Vector2i> &GetTrackPoints() { return track_points; }
	std::vector<int> GetAngles() { return new_angles_;}
	std::vector<int> GetLengths() { return segment_lengths_; }
	std::vector<int> GetDirections() { return directions_; }

	void SetControlPoints(std::vector<sf::Vector2i> temp_) { control_points = temp_; }
	//Setters
	void SetFailed(bool f) { failed_ = f; }
	void SetTesting(bool f) { do_testing_ = f; }

	void SortControlPoints();
	void FixLengthsAndLinePos();

	int DistanceSqrt(int x, int y, int x2, int y2);
	void SegmentAngles();
	void LeftOrRight();
	void PerpendicularLeftOrRight();
	void WriteToFile();
	void WriteTrackPoints(std::vector<sf::Vector2i>& track_, const bool& is_curved_, const bool& is_width_, const int& index_, const std::vector<sf::Vector2i>& control_points_p, const std::vector<sf::Vector2i>track_points_p);
	void FindTriangleAngles();

	void OrderControlPoints();
	void ScaleControlPoints(float scale);

	void SetNESW(const int &grid_size, std::vector<int> &occ);
	void EraseVector(std::vector<int> &occ, int &unique);

	void FindCompassPoss(const int& compass, int* grid, const int& grid_size, int& how_many, int& track_d, const int& x, const int& y, std::vector<sf::Vector2i>& vec_temp);
	

	void SetOldToNew(int* grid, const int& grid_size);
private:
	bool found_end;				//start as false;
	int count_holder_;
	int it;
	bool end_;					//start as false;
	int x_holder_, y_holder_;
	bool failed_;				//if the generation fails

	bool do_testing_;

	int total_track_distance;		//the total distance of the track
	int number_of_turns;			//the total number of turns
	int number_of_segments;

	int north_site;
	int north_e_site;
	int north_w_site;
	int west_site;
	int east_site;
	int south_site;
	int south_e_site;
	int south_w_site;

	int unique_count_occuarances;			//used
	int unique_count_old_occuarances;		//used
	std::vector<int> old_num;				//used
	std::vector<int> occurances;			//used
	std::vector<int> old_occurances;		//used
	sf::Vector2i first_position;			//used		takes first position in track
	std::vector<int> segment_lengths_;		//used
	std::vector<int> angles_;				//used
	std::vector<int> new_angles_;
	std::vector<int> directions_;		//used, -1 for right 1, left

	std::vector<sf::Vector2i> line_positions;		//used for angles - each control point but doubled
	std::vector<sf::Vector2i> control_points;		//control points used
	static std::vector<sf::Vector2i> track_points;

	int number = 1;
	sf::Vector2i start_p;
	sf::Vector2i middle_p;
	sf::Vector2i end_p;
};

