#pragma once
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

	//failled and restarting
	void SetFailed(bool f) { failed_ = f; }
	bool GetFailed() { return failed_; }
private:


	
	bool found_end;				//start as false;
	int count_holder_;
	int it;
	bool end_;					//start as false;
	int x_holder_, y_holder_;
	bool failed_;
};

