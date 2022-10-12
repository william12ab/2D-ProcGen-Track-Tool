#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

class ShortestPath
{
public:
	ShortestPath();
	void Initgrid(int grid_size, int*grid, int num_points);																				//inits the grid to the correct numbers
	void PhaseOne(int grid_size, int *grid, int count_holder, bool found_end,int it,bool end, int x_holder, int y_holder, int end_n, bool b_failed_, int start_p, int end_p);	//finds the distances between start and end
	void PhaseTwo(int grid_size, int *grid, bool end, int x_holder, int y_holder, int count_holder, int end_n);							//finds a pathway between start and end

	void CleanGrid(int grid_size, int* grid);																							//removes numbers found by phase 1


	void ChangePoint(int grid_size, int* grid, int point, int new_point);																//changes the point passed in
	void PrintOutStartEnd(int grid_size, int* grid);																					//prints out the points in the track, useful for debugging

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

	//failled and restarting
	void SetFailed(bool f) { failed_ = f; }
	bool GetFailed() { return failed_; }
	int DistanceSqrt(int x, int y, int x2, int y2);

	void SegmentAngles();
	void WriteToFile(int track_max, int track_min);

	void SetTesting(bool f) { do_testing_ = f; }
	bool GetTesting() { return do_testing_; }
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
	std::pair<int, int> first_position;		//used
	std::vector<int> segment_lengths_;		//used
	std::vector<int> angles_;				//used

	std::vector<std::pair<int, int>> line_positions; //used
	std::vector<std::pair<int, int>> control_points;		//control points used

	int number = 1;
	std::pair<int, int> start_p;
	std::pair<int, int> middle_p;
	std::pair<int, int> end_p;
};

