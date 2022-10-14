#pragma once
#include <vector>
class DeCastelJau
{
public:
	DeCastelJau();
	~DeCastelJau();

	void CreateCurve(std::vector<std::pair<int,int>> control_points, int grid_size, int* grid);
};

