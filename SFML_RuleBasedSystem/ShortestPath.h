#pragma once
class ShortestPath
{
public:
	ShortestPath();
	void Initgrid(int grid_size, int*grid);
	void PhaseOne(int grid_size, int *grid, int count_holder, bool found_end,int it,bool end);
	void PhaseTwo(int grid_size, int *grid, bool end);
private:


	bool found_empty;			//make this local to phase1
	bool found_end;				//start as false;
	int count_holder;
	int it;
	bool end;					//start as false;
};

