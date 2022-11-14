#pragma once
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
class CatmullRomSpline
{
public:
	CatmullRomSpline();
	~CatmullRomSpline();

	sf::Vector2i CreatePoint(std::vector<sf::Vector2i> control_points, bool is_looped,float t);

	void CreateCurve(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray& vertexarray, bool is_looped);
	
	sf::Vector2f CentripetalCurve(float t);

	void CreateCurve(int grid_size, sf::VertexArray& vertexarray);

	//setters
	void SetStepSize(float a_) { alpha_ = a_; }

	//getters
	float GetStepSize() { return alpha_; }

	void DrawControlPoints(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray& vertexarray);
	void FixControlPoints(std::vector<sf::Vector2i> & const control_points, int it_, sf::Vector2i co);


	int DistanceSqrt(int x, int y, int x2, int y2);
private:

	float step_size;
	float alpha_;

	struct Segment
	{
		sf::Vector2f a;
		sf::Vector2f b;
		sf::Vector2f c;
		sf::Vector2f d;
	};
};

