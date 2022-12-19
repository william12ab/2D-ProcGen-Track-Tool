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
	std::vector<sf::Vector2i>& GetNewTrack() { return new_track; }


	void Modi(const int& sign);

	void SetTrackSurface(int t) { track_surface = t; }

	void CompareHeights(const int&max_,const int&min_);


	void FindWidth(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points, const std::vector<sf::Vector2i>& points_pos, const std::vector<int>& lengths_, const std::vector<int> angles_);
	void Clear();
	int DistanceSqrt(int x, int y, int x2, int y2);
	void TrackLoop(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points, const std::vector<sf::Vector2i>& points_pos,const std::vector<int>& lengths_, const std::vector<int> angles_);

	void FindMaxWidth(int& max_width_d, int& x, int& y, const int& iter, int* const& noise_grid, const int& grid_size, const int& layers_, const sf::Vector2i& i,const int& mody);

	void CheckPoints(const std::vector<int>& inc_, const int& iter, const int& height_diff);
	void CheckLength(const std::vector<int>& lengths_, const int& it);
	void CheckTValues(const int& i);
	void CheckAngle(const int &angle_);

	void DefaultWidth(const sf::Vector2i& track_point, const int& size_, const int& count_, const int&count_c_p);		//used right now
	void DefaultPlus(const sf::Vector2i& track_point, const int& size_, const int& count_);			//un-used
	
	void WidthDirectionDecider(int count, const sf::Vector2i& track_point, std::vector<sf::Vector2i>& temp_vec);
	void WidthLoop(const sf::Vector2i& track_point, std::vector<sf::Vector2i>& temp_vec, const int& x_, const int& y_, const int& x_r, const int& y_r, const int& left_iter, const int& right_iter);

	void CalculateWidth(const sf::Vector2i& track_point, const int& size_, const int& count_);
	//used
	
private:
	int track_max;
	int image_max;
	int image_min;
	int track_min;

	static std::vector<int> point_inc_;
	static std::vector<int> cp_inc_;
	static std::vector<sf::Vector2f> normalised_opposite_direction;
	static std::vector<sf::Vector2i> max_width_directions;
	static std::vector<sf::Vector2f> normailised_direction_;


	static std::vector<float> t_values;
	static std::vector<sf::Vector2i> new_track;

	int max_width_left;
	int max_width_right;
	float min_width;
	int default_width;
	int track_surface;

	float average_length;

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

