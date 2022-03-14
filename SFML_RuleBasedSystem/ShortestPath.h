#pragma once
#include <string>
#include <vector>

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
	int GetNumberOfTurns2() { return number_of_turns2; }

	//failled and restarting
	void SetFailed(bool f) { failed_ = f; }
	bool GetFailed() { return failed_; }
private:


	
	bool found_end;				//start as false;
	int count_holder_;
	int it;
	bool end_;					//start as false;
	int x_holder_, y_holder_;
	bool failed_;				//if the generation fails



	int total_track_distance;		//the total distance of the track
	int number_of_turns;			//the total number of turns
	int number_of_turns2;
	
	std::vector<std::string> direction_;

	std::string new_direction;
	std::string old_direction;
	int counter_;
	int counter_g;
	float gradient_;
	float angle_;
	int temp_x,temp_y;

	std::vector<float> angles_;
	std::vector<float> gradients_;



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
	std::pair<int, int> previous_;
	int line_length;						//not currently used
};

