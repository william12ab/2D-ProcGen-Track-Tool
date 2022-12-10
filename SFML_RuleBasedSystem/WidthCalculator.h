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
	void FindRelatedHeight(int* const& noise_grid, const int&grid_size, const int&layers_, const std::vector<sf::Vector2i> &track_points, const std::vector<sf::Vector2i>& control_points);

	std::vector<int>& GetCPIncline() { return cp_inc_; }
	std::vector<int>& GetPointIncline() { return point_inc_; }

	void CompareHeights(const int&max_,const int&min_);


	void FindWidth();
	void Clear();
	int DistanceSqrt(int x, int y, int x2, int y2);

	void FindMaxWidth(int& max_width_d, int& x, int& y, const int& iter, int* const& noise_grid, const int& grid_size, const int& layers_, const sf::Vector2i& i,const int& mody);

private:
	int track_max;
	int image_max;
	int image_min;
	int track_min;

	static std::vector<int> point_inc_;
	static std::vector<int> cp_inc_;
	static std::vector<sf::Vector2f> normalised_opposite_direction;
	static std::vector<sf::Vector2i> max_width_directions;

	int max_width_left;
	int max_width_right;
	float min_width;
	int default_width;
	int track_surface;

	struct width_modi
	{
		float modi_left;
		float max_m_left;
		float min_m_left;
		float modi_right;
		float min_m_right;
		float max_m_right;
	}width_m;
};

