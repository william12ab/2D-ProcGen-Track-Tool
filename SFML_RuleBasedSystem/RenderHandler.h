#pragma once
#include "StructRanges.h"
#include "TrackTools.h"
class RenderHandler
{
public:
	RenderHandler();
	void RenderLoop(const bool& is_render_diagram, TrackTools& t_t, VoronoiDiagram& v_d, ImageProcessing& i_p,
		bool& render_diagram, bool& render_track, const bool& is_render_track, std::vector<sf::VertexArray*> voronoi_diagrams,
		const bool& render_height_map_, const bool& n_render_height_map_, const bool& f_render_height_map_,
		std::vector<sf::VertexArray*> distance_maps, std::vector<sf::VertexArray*> noise_maps, sf::VertexArray final_map, sf::RenderWindow &window);
};

