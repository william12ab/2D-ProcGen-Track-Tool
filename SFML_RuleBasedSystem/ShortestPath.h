#pragma once
class ShortestPath
{
public:
	ShortestPath();
	void Initgrid(int grid_size, int*grid, int num_points);
	void PhaseOne(int grid_size, int *grid, int count_holder, bool found_end,int it,bool end, int x_holder, int y_holder, int end_n);
	void PhaseTwo(int grid_size, int *grid, bool end, int x_holder, int y_holder, int count_holder, int end_n);

	void ChangePoint(int grid_size, int* grid, int point, int new_point);
	void PrintOutStartEnd(int grid_size, int* grid);

	bool bGetFoundEnd() { return found_end; }
	int GetCountHolder() { return count_holder_; }
	int GetIt() { return it; }
	bool bGetEnd() { return end_;}
	int GetXHolder() { return x_holder_; }
	int GetYHolder() { return y_holder_; }
private:


	
	bool found_end;				//start as false;
	int count_holder_;
	int it;
	bool end_;					//start as false;
	int x_holder_, y_holder_;
};

