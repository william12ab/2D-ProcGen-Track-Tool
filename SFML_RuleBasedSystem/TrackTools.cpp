#include "TrackTools.h"
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using the_clock = std::chrono::steady_clock;
TrackTools::TrackTools(){
	is_done_setup = false;
}

void TrackTools::ResetVars(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map){
	is_done_setup = false;
	voronoi_d.clear();
	v_d_p.SetFaile(false);
	s_p_p.SetFailed(false);
	voronoi_d.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
}
void TrackTools::SetVars(VoronoiDiagram &v_d_p, ImageProcessing &i_p_p, int track_type_, int resolution_, int sites_, int points_){
	v_d_p.~VoronoiDiagram();
	v_d_p.SetType(track_type_);
	v_d_p.SetGridSize(resolution_);
	v_d_p.SetNumberOfSites(sites_);
	v_d_p.SetNumberOfPoints(points_);
	v_d_p.InitVector(v_d_p.GetGridSize(), v_d_p.GetNumberOfPoints(), v_d_p.GetNumberOfSites());
	i_p_p.InitStructures(v_d_p.GetGridSize());
}
void TrackTools::ClearStructs(VoronoiDiagram &v_d_p, sf::VertexArray& voronoi_d, sf::VertexArray& n_height_map, sf::VertexArray& distance_map, ImageProcessing &i_p_p, int track_type_, int resolution_, int sites_, int points_)
{
	voronoi_d.clear();
	n_height_map.clear();
	distance_map.clear();
	v_d_p.ResetVars();
	SetVars(v_d_p, i_p_p,track_type_,resolution_,sites_,points_);
	v_d_p.SetGridSize(resolution_);
	voronoi_d.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
	n_height_map.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
	distance_map.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
}

void TrackTools::CreateVoronoi(VoronoiDiagram &v_d_p, sf::VertexArray& height_map,ImageProcessing&i_p_p, int times_, int displacement_, int number_, bool full_random_, int track_type_){
	//bool choosing what distribution to use
	if (full_random_){
		v_d_p.RandomPlaceSites();
	}
	else{
		v_d_p.EqaullyDispursSites(times_, displacement_);
	}

	//creates vd diagram, creates distance map, sets only edges in vertexarray, sets points of track. in that order.
	v_d_p.DiagramAMP(0);
	ranges default_r;
	RangesDecider(0, default_r.x_min, default_r.x_max, default_r.y_min, default_r.y_max,v_d_p.GetGridSize());
	i_p_p.DrawVoronoiNoise(height_map, v_d_p.GetGridSize(), v_d_p.GetNumberOfSites(), number_, v_d_p.GetGridDistance(0),0,default_r);
	v_d_p.SetEdges(0);
	v_d_p.SetPoint(track_type_,0);
}

void TrackTools::CreateTrack(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, const int& chunk_index){
	//init grid should be fine, no need to change.
	s_p_p.Initgrid(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), v_d_p.GetNumberOfPoints());
	//pass in the start and end to both these functions
	int start = -4;
	s_p_p.PrintOutStartEnd(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index));
	//if type 2, then need to loop over number of points differently and check when the index is = 1 so that the starting point can be changed to the end
	if (v_d_p.GetType() == 2){
		for (int i = 0; i < (v_d_p.GetNumberOfPoints()); i++){
			s_p_p.PhaseOne(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), -3,0, v_d_p.GetGridSize());
			s_p_p.PhaseTwo(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), 0,v_d_p.GetNumberOfPoints(),i,v_d_p.GetPointPos()[i],sf::Vector2i(0,0));
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			if (i == 1){
				s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), -1234, -5);
			}
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), 0, -1234);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), -3, 0);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), start - i, -3);
			s_p_p.CleanGrid(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index));
		}
	}
	else{
		for (int i = 0; i < (v_d_p.GetNumberOfPoints() - 1) && !s_p_p.GetFailed(); i++){
			s_p_p.PhaseOne(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), -3, 0, v_d_p.GetGridSize());
			s_p_p.PhaseTwo(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), 0, v_d_p.GetNumberOfPoints(), i, v_d_p.GetPointPos()[i], v_d_p.GetPointPos()[i+1]);
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), 0, -1234);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), -3, 0);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index), start - i, -3);
			s_p_p.CleanGrid(v_d_p.GetGridSize(), v_d_p.GetGrid(chunk_index));
		}
	}
}
void TrackTools::Generate(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map, sf::VertexArray& n_height_map, ImageProcessing &i_p_p, int times_, int displacement_, int number_, bool full_random_, int track_type_){
	do{
		if (v_d_p.GetFailed() || s_p_p.GetFailed()){//clears the diagram and resets the fail condition
			ResetVars(v_d_p, s_p_p, voronoi_d, height_map);
		}
		CreateVoronoi(v_d_p, height_map,i_p_p,times_,displacement_,number_,full_random_,track_type_);
		CreateTrack(v_d_p, s_p_p,0);
	} while (v_d_p.GetFailed() || s_p_p.GetFailed());
	ranges init;
	RangesDecider(0, init.x_min, init.x_max, init.y_min, init.y_max, v_d_p.GetGridSize());

	i_p_p.DrawTrack(voronoi_d, v_d_p.GetGridSize(), v_d_p.GetNumberOfSites(), v_d_p.GetGrid(0),0, init);
	s_p_p.SortControlPoints();
}

void TrackTools::GenerateTerrainMethod(VoronoiDiagram &v_d_p, sf::VertexArray& vertex_array, ImageProcessing &i_p_p, int number_, int track_track_, const int& chunk_index){
	ranges init;
	if (!is_done_setup){
		v_d_p.TerrainSites();							//this takes no time
		v_d_p.DiagramAMP(chunk_index);
		v_d_p.SetEdges(chunk_index);
		if (is_chunk){
			v_d_p.DivideChunks();
		}
	}
	is_done_setup = true;
	RangesDecider(chunk_index, init.x_min, init.x_max, init.y_min, init.y_max, v_d_p.GetGridSize());
	i_p_p.DrawVoronoiNoise(vertex_array, v_d_p.GetGridSize(), v_d_p.GetNumberOfSites(), number_, v_d_p.GetGridDistance(chunk_index), chunk_index, init);
	if (is_chunk){
		v_d_p.SetPoint(3, chunk_index);
	}
	else {
		v_d_p.SetPoint(track_track_, chunk_index);
	}
	
}

void TrackTools::TerrainLoop(VoronoiDiagram &v_d_p,ShortestPath &s_p_p, sf::VertexArray&height_map,ImageProcessing&i_p_p, int number_, int track_type_, const int& index_){
	bool failed_sp = false;
	do{
		if (v_d_p.GetFailed() || s_p_p.GetFailed()){//clears the diagram and resets the fail condition
			break;
		}
		if (!failed_sp){
			GenerateTerrainMethod(v_d_p, height_map, i_p_p, number_, track_type_, index_);
		}
		CreateTrack(v_d_p, s_p_p, index_);
	} while (v_d_p.GetFailed() || s_p_p.GetFailed());
}

void TrackTools::WidthSettings(WidthCalculator& w_c, ShortestPath& s_p, VoronoiDiagram& v_d, ImageProcessing& i_p, sf::VertexArray& voronoi_d, const int &layers_, std::vector<sf::Vector2i> &track_, std::vector<sf::Vector2i> control_points_p, const int& chunk_index){
	w_c.Clear();
	w_c.FindDirectionBetweenCP(control_points_p);

	if (w_c.GetBoolAngles()){
		s_p.SegmentAngles();
	}
	w_c.FindMinMax(layers_, i_p.GetNoiseMap(chunk_index), v_d.GetGridSize());															//min max of image
	w_c.FindTrackMinMax(track_, v_d.GetGridSize(), layers_, i_p.GetNoiseMap(chunk_index));								//min max of track
	if (w_c.GetBoolTValues()){
		w_c.TrackTValues(track_, control_points_p);																					//give t value of lerp
	}
	if (w_c.GetBoolIncline()){
		w_c.FindInclinePoints(control_points_p, v_d.GetGridSize(), layers_, w_c.GetCPIncline(), i_p.GetNoiseMap(chunk_index));		//for the control points
		w_c.FindInclinePoints(v_d.GetPointPos(), v_d.GetGridSize(), layers_, w_c.GetPointIncline(), i_p.GetNoiseMap(chunk_index));		//for the points
	}
	if (w_c.GetBoolFlat()){
		//do nothing
	}
	else{
		w_c.FindRelatedHeight(i_p.GetNoiseMap(chunk_index), v_d.GetGridSize(), layers_, track_, control_points_p);
	}
	w_c.SetModi();
	w_c.FindWidth(track_, control_points_p, v_d.GetPointPos(), s_p.GetLengths(), s_p.GetAngles(),i_p.GetNoiseMap(chunk_index), v_d.GetGridSize());
	i_p.CreateImage(voronoi_d, v_d.GetGridSize());
	i_p.DrawWidthTrack(voronoi_d, v_d.GetGridSize(), w_c.GetNewTrack(), chunk_index);
}
void TrackTools::RangesDecider(const int& chunk_iter, int& x_min, int& x_max, int& y_min, int& y_max,const int& grid_size) {
	switch (chunk_iter){
	case 0:
		x_min = 0;
		x_max = grid_size;
		y_min = 0;
		y_max = grid_size;
		break;
	case 1:
		x_min = grid_size;
		x_max = grid_size*2;
		y_min = 0;
		y_max = grid_size;
		break;
	case 2:
		x_min = 0;
		x_max = grid_size;
		y_min = grid_size;
		y_max = grid_size*2;
		break;
	case 3:
		x_min = grid_size;
		x_max = grid_size*2;
		y_min = grid_size;
		y_max = grid_size*2;
		break;
	}
}

void TrackTools::HeightLoop(const int& chunk_iter,VoronoiDiagram& v_d, const int& peaks_to_count_,const int& layers_, ImageProcessing& i_p,const int&radius_cutoff,const int& number_, const int& track_type_, ShortestPath& s_p, sf::VertexArray& height_map) {
	v_d.SetIsChunking(is_chunk);//sets boool
	int index_=1;
	if (is_chunk){
		index_ = 4;
	}
	if (!is_done_setup) {
		for (int chunk_index_loc = 0; chunk_index_loc < index_; chunk_index_loc++) {
			ranges init;
			RangesDecider(0, init.x_min, init.x_max, init.y_min, init.y_max, v_d.GetGridSize());
			if (is_chunk) {
				v_d.ResetSitesForChunking(v_d.GetNumberOfSites());//resets sites array
				v_d.SetForChunks();//sets vars to default
				s_p.SetForChunk();
			}
			v_d.EmptyCircles();
			v_d.vector_all(peaks_to_count_ * 2);
			int i = 0;
			while (!v_d.GetStopH() || !v_d.GetStopL()) {//these are for stopping if track is below or above point
				v_d.FindMax(layers_, i_p.GetNoiseMap(chunk_index_loc), init);
				if (!v_d.GetStopH()) {//finds the highest point in the terrain
					v_d.DirectionDecider(radius_cutoff, layers_, i, i_p.GetNoiseMap(chunk_index_loc), v_d.GetHighPoint(), true, init);		//finds point on circumference 
					i++;
				}
				if (!v_d.GetStopL()) {
					v_d.DirectionDecider(80, layers_, i + 1, i_p.GetNoiseMap(chunk_index_loc), v_d.GetLowPoint(), false, init);		//finds point on circumference 
					i++;
				}
			}
			RangesDecider(chunk_index_loc, init.x_min, init.x_max, init.y_min, init.y_max, v_d.GetGridSize());//used for adding on 
			v_d.AddingCirclesToContainer(init);//adds circles to overall array
		}
	}
	TerrainLoop(v_d, s_p, height_map, i_p, number_, track_type_, chunk_iter);//passes in the vector of vertex array for diagram, distancemap,heightmap, and index
	v_d.SetStopL(false);
	v_d.SetStopH(false);
}

void TrackTools::WritePacenoteInfo(ShortestPath&s_p, WidthCalculator& w_c, const bool& is_withd){
	std::ofstream results_;
	char const* c = "pacenote_info.txt";
	results_.open(c);

	results_ << "a\n";
	for (int i = 0; i < s_p.GetAngles().size(); i++){
		results_ << s_p.GetAngles()[i] << "\n";
	}
	results_ << "d\n";
	for (size_t i = 0; i < s_p.GetDirections().size(); i++){
		results_ << s_p.GetDirections()[i] << "\n";
	}
	results_ << "l\n";
	for (int i = 0; i < s_p.GetLengths().size(); i++) {
		results_ << s_p.GetLengths()[i] << "\n";
	}
	if (is_withd){
		results_ << "inc\n";
		for (size_t i = 0; i < w_c.GetCPIncline().size(); i++){
			results_ << w_c.GetCPIncline()[i] << "\n";
		}
		results_ << "w\n";
		for (size_t i = 0; i < w_c.GetWidthAcrossTrack().size(); i++){
			results_ << w_c.GetWidthAcrossTrack()[i] << "\n";
		}
	}
	else {
		results_ << "non\n";
	}
}