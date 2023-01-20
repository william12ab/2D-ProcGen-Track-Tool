#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class WidthCalculator
{
public:
	WidthCalculator();
	void FindMinMax(const int& layers_, int* const& noise_grid, const int&grid_size);			//min max of image
	void FindTrackMinMax(const std::vector<sf::Vector2i>& track_points, const int& grid_size, const int& layers_, int* const& noise_grid);		//min max of track

	//lerp and lerp written to find T
	sf::Vector2i Lerp(const sf::Vector2i& p1, const sf::Vector2i& p2, const float& t);
	float FindT(const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3);
	void TrackTValues(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points);

	//finds incline of points/control_points
	void FindInclinePoints(const std::vector<sf::Vector2i>& vector_, const int& grid_size, const int& layers_, std::vector<int>& results_, int* const& noise_grid);

	//finds direction of the current segment created by the two current control points
	void FindDirectionBetweenCP(const std::vector<sf::Vector2i>& control_points);
	//finds the height to the side of the current point on the track
	void FindRelatedHeight(int* const& noise_grid, const int&grid_size, const int&layers_, const std::vector<sf::Vector2i> &track_points, const std::vector<sf::Vector2i>& control_points);
	void FindMaxWidth(int& max_width_d, int& x, int& y, const int& iter, int* const& noise_grid, const int& grid_size, const int& layers_, const sf::Vector2i& i, const int& mody);
	void SetModi();

	std::vector<int>& GetCPIncline() { return cp_inc_; }
	std::vector<int>& GetPointIncline() { return point_inc_; }
	std::vector<sf::Vector2i>& GetNewTrack() { return new_track; }

	bool& GetBoolAngles() { return bool_obj.is_angles_; }
	bool& GetBoolCurved() { return bool_obj.is_curved_; }
	bool& GetBoolGblobal() { return bool_obj.is_global_; }
	bool& GetBoolIncline() { return bool_obj.is_incline_; }
	bool& GetBoolLength() { return bool_obj.is_length_; }
	bool& GetBoolRelatedWidth() { return bool_obj.is_related_width; }
	bool& GetBoolTValues() { return bool_obj.is_t_values_; }
	bool& GetBoolRand() { return bool_obj.is_rand_; }
	bool& GetBoolInfluenceT() { return bool_obj.is_influenced_t; }

	//modifier for left and right - adds up to total 1(or-1) which is used in positive/negative check func
	void Modi(const int& sign);

	//sets the track surface
	void SetTrackSurface(int t) { width_m.track_surface = t; }

	//compares the min and max of the track/terrain
	void CompareHeights(const int&max_,const int&min_);

	//switches over the track_surface to determine the width
	void FindWidth(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points, const std::vector<sf::Vector2i>& points_pos, const std::vector<int>& lengths_, const std::vector<int> angles_);
	//resets functions
	void Clear();
	int DistanceSqrt(int x, int y, int x2, int y2);

	//performs all the chances to add up the modi and then calcs the width
	void TrackLoop(const std::vector<sf::Vector2i>& track_points, const std::vector<sf::Vector2i>& control_points, const std::vector<sf::Vector2i>& points_pos,const std::vector<int>& lengths_, const std::vector<int> angles_);

	//checks the incline of the points/control points
	void CheckPoints(const std::vector<int>& inc_, const int& iter, const int& height_diff);
	//checks length of current segment
	void CheckLength(const std::vector<int>& lengths_, const int& it);
	//checks the T value found in the func FINDT
	void CheckTValues(const int& i);
	//checks angle
	void CheckAngle(const int &angle_);

	//function for calculating width
	void DefaultWidth(const sf::Vector2i& track_point, const int& size_, const int& count_, const int&count_c_p);		//used right now
	//unused
	void DefaultPlus(const sf::Vector2i& track_point, const int& size_, const int& count_);			//un-used
	
	//finds the correct placement of new track
	void WidthDirectionDecider(int count, const sf::Vector2i& track_point, std::vector<sf::Vector2i>& temp_vec);
	//actually gives new width
	void WidthLoop(const sf::Vector2i& track_point, std::vector<sf::Vector2i>& temp_vec, const int& x_, const int& y_, const int& x_r, const int& y_r);

	//calculates width and has checkers for bounds
	void CalculateWidth(const sf::Vector2i& track_point, const int& size_, const int& count_);

	//loop for if decreasing or increasing
	void PositiveCheck(const float& dir_, const float&p_, int &width_);
	void NegativeCheck(const float& dir_, const float&p_, int& width_);

	//checks if below min width and stuff like that
	void BoundsCheck();
	
	
private:
	int track_max;
	int image_max;
	int image_min;
	int track_min;

	static std::vector<int> point_inc_;					//stores incline between points
	static std::vector<int> cp_inc_;					//stores incline between control - points
	static std::vector<sf::Vector2f> normalised_opposite_direction;			//normalised direction (-1 to 1) of the opposite of the curent segment. so perpendicular direction,
	static std::vector<sf::Vector2i> max_width_directions;					//the max direction for left and right. .X is LEFT. .Y is RIGHT.
	static std::vector<sf::Vector2f> normailised_direction_;				//the normalised direction for the current segment


	static std::vector<float> t_values;				//stores t values
	static std::vector<sf::Vector2i> new_track;		//stores the track]

	int max_width_left;			//current max width at given track point
	int max_width_right;		//same above

	float average_length;		//average length of the track segments

	struct width_modi
	{
		float modi_left;			//modi for left and right used in func modi and calculate width and check
		float modi_right;		//same
		int w_left;				//width value for left
		int w_right;			//for right
		float min_width;		//min width
		int default_width;		//default width
		int track_surface;		//surface 
	}width_m;

	struct bool_options
	{
		bool is_curved_;
		bool is_length_;
		bool is_t_values_;
		bool is_angles_;
		bool is_incline_;
		bool is_related_width;
		bool is_global_;
		bool is_rand_;
		bool is_influenced_t;
	}bool_obj;
	
	float modi_value;
};

