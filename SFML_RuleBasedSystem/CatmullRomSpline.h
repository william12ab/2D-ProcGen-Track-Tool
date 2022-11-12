#pragma once
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
class CatmullRomSpline
{
public:
	CatmullRomSpline();
	~CatmullRomSpline();

	std::pair<int, int> CreatePoint(std::vector<std::pair<int, int>> control_points, bool is_looped,float t);

	void CreateCurve(std::vector<std::pair<int, int>> control_points, int grid_size, sf::VertexArray& vertexarray, bool is_looped);
	//setters
	void SetStepSize(float a_) { alpha_ = a_; }

	//getters
	float GetStepSize() { return alpha_; }

	void DrawControlPoints(std::vector<std::pair<int, int>> control_points, int grid_size, sf::VertexArray& vertexarray);
	void FixControlPoints(std::vector<std::pair<int, int>> & const control_points, int it_, std::pair<int, int> co);

private:

	float step_size;
	float alpha_;
};

