#pragma once
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>

class DeCastelJau
{
public:
	DeCastelJau();
	~DeCastelJau();

	void CreateCurve(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray &vertexarray);

	//setters
	void SetStepSize(float a_) {step_size = a_;}

	//getters
	float GetStepSize() { return step_size; }

private:
	float step_size;
};

