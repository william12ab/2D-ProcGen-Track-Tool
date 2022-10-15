#pragma once
#include <vector>
class DeCastelJau
{
public:
	DeCastelJau();
	~DeCastelJau();

	void CreateCurve(std::vector<std::pair<int,int>> control_points, int grid_size, int* grid);

	//setters
	void SetStepSize(float a_) {step_size = a_;}

	//getters
	float GetStepSize() { return step_size; }

private:
	float step_size;
};

