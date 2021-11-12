#pragma once
class ShortestPath
{
public:
	ShortestPath();
	void Initgrid(int grid_size, int*grid);
	void PhaseOne(int grid_size, int *grid, int count_holder, bool found_end,int it,bool end, int x_holder, int y_holder);
	void PhaseTwo(int grid_size, int *grid, bool end, int x_holder, int y_holder, int count_holder);


	bool bGetFoundEnd() { return found_end; }
	int GetCountHolder() { return count_holder_; }
	int GetIt() { return it; }
	bool bGetEnd() { return end;}
	int GetXHolder() { return x_holder_; }
	int GetYHolder() { return y_holder_; }
private:


	
	bool found_end;				//start as false;
	int count_holder_;
	int it;
	bool end;					//start as false;
	int x_holder_, y_holder_;
};

