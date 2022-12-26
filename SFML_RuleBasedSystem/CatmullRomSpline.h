#pragma once
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>
class CatmullRomSpline
{
public:
	CatmullRomSpline();
	sf::Vector2i CreatePoint(std::vector<sf::Vector2i> control_points, bool is_looped,float t);
	sf::Vector2f CentripetalCurve(float t, std::vector<sf::Vector2i> control_points,int j, bool is_looped);

	void CreateCurve(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray& vertexarray, bool is_looped);
	void CreateCurve(int grid_size, sf::VertexArray& vertexarray, std::vector<sf::Vector2i> control_points, bool is_looped);

	//setters
	void SetStepSize(float a_) { alpha_ = a_; }

	//getters
	float GetStepSize() { return alpha_; }
	std::vector<sf::Vector2i> &GetCurve() { return new_track; }
	void DrawControlPoints(std::vector<sf::Vector2i> control_points, int grid_size, sf::VertexArray& vertexarray);
	void FixControlPoints(std::vector<sf::Vector2i> & const control_points, int it_, sf::Vector2i co);
	void ResetArray(sf::VertexArray& vertexarray, int grid_size);
	int DistanceSqrt(int x, int y, int x2, int y2);
	int* GetIndices(bool is_looped,float t, int size_);
	void RemoveDuplicates();

	
private:

	float step_size;
	float alpha_;

	static std::vector<sf::Vector2i> new_track;

	struct Segment
	{
		sf::Vector2f a;
		sf::Vector2f b;
		sf::Vector2f c;
		sf::Vector2f d;
	};
};

