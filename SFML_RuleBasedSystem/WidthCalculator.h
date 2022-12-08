#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class WidthCalculator
{
public:
	WidthCalculator();


	void FindMinMax(const int& layers_, int* const& noise_grid, const int&grid_size);
	void FindTrackMinMax(const std::vector<sf::Vector2i>& track_points, const int& grid_size, const int& layers_, int* const& noise_grid);

	sf::Vector2i Lerp(const sf::Vector2i& p1, const sf::Vector2i& p2, const float& t);
	float FindT(const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3);

	void TrackTValues(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points);
	void FindInclinePoints(const std::vector<sf::Vector2i>& vector_, const int& grid_size, const int& layers_, std::vector<int>& results_, int* const& noise_grid);

	void FindDirectionBetweenCP(const std::vector<sf::Vector2i>& control_points);


	std::vector<int>& GetCPIncline() { return cp_inc_; }
	std::vector<int>& GetPointIncline() { return point_inc_; }
	void Clear();
private:

	int track_max;
	int image_max;

	int image_min;
	int track_min;


	static std::vector<int> point_inc_;
	static std::vector<int> cp_inc_;
	
};

