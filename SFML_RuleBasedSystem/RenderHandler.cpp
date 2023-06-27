#include "RenderHandler.h"
RenderHandler::RenderHandler() {

}
void RenderHandler::RenderLoop(const bool& is_render_diagram, TrackTools& t_t, VoronoiDiagram& v_d, ImageProcessing& i_p,
	bool&render_diagram,bool&render_track, const bool &is_render_track, std::vector<sf::VertexArray*> voronoi_diagrams, 
	const bool& render_height_map_,const bool & n_render_height_map_,const bool& f_render_height_map_, 
	std::vector<sf::VertexArray*> distance_maps, std::vector<sf::VertexArray*> noise_maps,sf::VertexArray final_map,sf::RenderWindow &window) {
	if (is_render_diagram) {
		if (!render_diagram) {
			if (i_p.GetIsChunking()) {
				for (int i = 0; i < 4; i++) {
					ranges limits_;
					t_t.RangesDecider(i, limits_.x_min, limits_.x_max, limits_.y_min, limits_.y_max, v_d.GetGridSize());
					i_p.DrawFullVoronoiDiagram(*voronoi_diagrams[i], v_d.GetGridSize(), v_d.GetGrid(i), i, limits_);
				}
				render_diagram = true;
				render_track = false;
				false;
			}
			else {
				render_diagram = true;
				render_track = false;
				ranges limits_;
				t_t.RangesDecider(0, limits_.x_min, limits_.x_max, limits_.y_min, limits_.y_max, v_d.GetGridSize());
				i_p.DrawFullVoronoiDiagram(*voronoi_diagrams[0], v_d.GetGridSize(), v_d.GetGrid(0), 0, limits_);
			}
		}
	}
	if (is_render_track) {
		if (!render_track) {
			if (i_p.GetIsChunking()) {
				for (int i = 0; i < 4; i++) {
					ranges limits_;
					t_t.RangesDecider(i, limits_.x_min, limits_.x_max, limits_.y_min, limits_.y_max, v_d.GetGridSize());
					i_p.DrawTrack(*voronoi_diagrams[i], v_d.GetGridSize(), v_d.GetNumberOfSites(), v_d.GetGrid(i), i, limits_);
				}
				render_track = true;
				render_diagram = false;
			}
			else {
				ranges limits_;
				t_t.RangesDecider(0, limits_.x_min, limits_.x_max, limits_.y_min, limits_.y_max, v_d.GetGridSize());
				i_p.DrawTrack(*voronoi_diagrams[0], v_d.GetGridSize(), v_d.GetNumberOfSites(), v_d.GetGrid(0), 0, limits_);
				render_track = true;
				render_diagram = false;
			}
		}
	}
	if (render_height_map_) {
		if (i_p.GetIsChunking()) {
			for (int i = 0; i < 4; i++) {
				window.draw(*distance_maps[i]);
			}
		}
		else {
			window.draw(*distance_maps[0]);
			std::cout << "ds\n";
		}
	}
	if (n_render_height_map_) {
		if (!i_p.GetIsChunking()) {
			window.draw(*noise_maps[0]);
		}
		else {
			for (int i = 0; i < 4; i++) {
				window.draw(*noise_maps[i]);
			}
		}
	}
	if (f_render_height_map_) {//full
		window.draw(final_map);
	}
	if (i_p.GetIsChunking()) {
		for (int i = 0; i < 4; i++) {
			window.draw(*voronoi_diagrams[i]);
		}
	}
	else {
		window.draw(*voronoi_diagrams[0]);
	}
}