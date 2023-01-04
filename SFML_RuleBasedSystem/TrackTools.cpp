#include "TrackTools.h"
TrackTools::TrackTools()
{
}
void TrackTools::ResetVars(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map, sf::VertexArray& n_height_map)
{
	n_height_map.clear();
	voronoi_d.clear();
	//height_map.clear();
	v_d_p.SetFaile(false);
	s_p_p.SetFailed(false);
	//height_map.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
	n_height_map.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
	voronoi_d.resize((v_d_p.GetGridSize() * v_d_p.GetGridSize()));
}
void TrackTools::SetVars(VoronoiDiagram &v_d_p, ImageProcessing &i_p_p, int track_type_, int resolution_, int sites_, int points_)
{
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

void TrackTools::CreateVoronoi(VoronoiDiagram &v_d_p, sf::VertexArray& height_map,ImageProcessing&i_p_p, int times_, int displacement_, int number_, bool full_random_, int track_type_)
{
	//bool choosing what distribution to use
	if (full_random_)
	{
		v_d_p.RandomPlaceSites();
	}
	else
	{
		v_d_p.EqaullyDispursSites(times_, displacement_);
	}

	//creates vd diagram, creates distance map, sets only edges in vertexarray, sets points of track. in that order.
	v_d_p.DiagramAMP();
	i_p_p.DrawVoronoiNoise(height_map, v_d_p.GetGridSize(), v_d_p.GetNumberOfSites(), number_, v_d_p.GetGridDistance());
	v_d_p.SetEdges();
	v_d_p.SetPoint(track_type_);
}

void TrackTools::CreateTrack(VoronoiDiagram &v_d_p, ShortestPath &s_p_p)
{
	//init grid should be fine, no need to change.
	s_p_p.Initgrid(v_d_p.GetGridSize(), v_d_p.GetGrid(), v_d_p.GetNumberOfPoints());
	//pass in the start and end to both these functions
	int start = -4;
	s_p_p.PrintOutStartEnd(v_d_p.GetGridSize(), v_d_p.GetGrid());
	//if type 2, then need to loop over number of points differently and check when the index is = 1 so that the starting point can be changed to the end
	if (v_d_p.GetType() == 2)
	{
		for (int i = 0; i < (v_d_p.GetNumberOfPoints()); i++)
		{
			s_p_p.PhaseOne(v_d_p.GetGridSize(), v_d_p.GetGrid(), -3,0, v_d_p.GetGridSize());
			s_p_p.PhaseTwo(v_d_p.GetGridSize(), v_d_p.GetGrid(), 0);
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			if (i == 1)
			{
				s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), -1234, -5);
			}
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), 0, -1234);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), -3, 0);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), start - i, -3);
			s_p_p.CleanGrid(v_d_p.GetGridSize(), v_d_p.GetGrid());
		}
	}
	else
	{
		for (int i = 0; i < (v_d_p.GetNumberOfPoints() - 1) && !s_p_p.GetFailed(); i++)
		{
			s_p_p.PhaseOne(v_d_p.GetGridSize(), v_d_p.GetGrid(), -3, 0, v_d_p.GetGridSize());
			s_p_p.PhaseTwo(v_d_p.GetGridSize(), v_d_p.GetGrid(), 0);
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), 0, -1234);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), -3, 0);
			s_p_p.ChangePoint(v_d_p.GetGridSize(), v_d_p.GetGrid(), start - i, -3);
			s_p_p.CleanGrid(v_d_p.GetGridSize(), v_d_p.GetGrid());
		}
	}
	s_p_p.ReOrderArrays();
}
void TrackTools::Generate(VoronoiDiagram &v_d_p, ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map, sf::VertexArray& n_height_map, ImageProcessing &i_p_p, int times_, int displacement_, int number_, bool full_random_, int track_type_)
{
	do
	{
		if (v_d_p.GetFailed() || s_p_p.GetFailed())		//clears the diagram and resets the fail condition
		{
			ResetVars(v_d_p, s_p_p, voronoi_d, height_map, n_height_map);
		}
		CreateVoronoi(v_d_p, height_map,i_p_p,times_,displacement_,number_,full_random_,track_type_);

		CreateTrack(v_d_p, s_p_p);

	} while (v_d_p.GetFailed() || s_p_p.GetFailed());
	i_p_p.DrawTrack(voronoi_d, v_d_p.GetGridSize(), v_d_p.GetNumberOfSites(), v_d_p.GetGrid());
	s_p_p.SortControlPoints();
}

void TrackTools::GenerateTerrainMethod(VoronoiDiagram &v_d_p, sf::VertexArray& vertex_array, ImageProcessing &i_p_p, int number_, int track_track_)
{
	v_d_p.TerrainSites();							//this takes no time
	v_d_p.DiagramAMP();
	i_p_p.DrawVoronoiNoise(vertex_array, v_d_p.GetGridSize(), v_d_p.GetNumberOfSites(), number_, v_d_p.GetGridDistance());
	v_d_p.SetEdges();
	v_d_p.SetPoint(track_track_);
}

void TrackTools::TerrainLoop(VoronoiDiagram &v_d_p,ShortestPath &s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray&height_map, sf::VertexArray&n_height_map,ImageProcessing&i_p_p, int number_, int track_type_)
{
	do
	{
		if (v_d_p.GetFailed() || s_p_p.GetFailed())		//clears the diagram and resets the fail condition
		{
			ResetVars(v_d_p, s_p_p, voronoi_d, height_map, n_height_map);
		}
		GenerateTerrainMethod(v_d_p, height_map, i_p_p, number_, track_type_);
		CreateTrack(v_d_p, s_p_p);
	} while (v_d_p.GetFailed() || s_p_p.GetFailed());
}

void TrackTools::WidthSettings(WidthCalculator& w_c, ShortestPath& s_p, VoronoiDiagram& v_d, ImageProcessing& i_p, sf::VertexArray& voronoi_d, const int &layers_, std::vector<sf::Vector2i> &track_)
{
	w_c.Clear();
	w_c.FindDirectionBetweenCP(s_p.GetControlPoints());

	if (w_c.GetBoolAngles())
	{
		s_p.SegmentAngles();
	}
	if (w_c.GetBoolGblobal())
	{
		w_c.FindMinMax(layers_, i_p.GetNoiseMap(), v_d.GetGridSize());															//min max of image
		w_c.FindTrackMinMax(track_, v_d.GetGridSize(), layers_, i_p.GetNoiseMap());								//min max of track
	}
	if (w_c.GetBoolTValues())
	{
		w_c.TrackTValues(track_, s_p.GetControlPoints());																					//give t value of lerp
	}
	if (w_c.GetBoolIncline())
	{
		w_c.FindInclinePoints(s_p.GetControlPoints(), v_d.GetGridSize(), layers_, w_c.GetCPIncline(), i_p.GetNoiseMap());		//for the control points
		w_c.FindInclinePoints(v_d.GetPointPos(), v_d.GetGridSize(), layers_, w_c.GetPointIncline(), i_p.GetNoiseMap());		//for the points
	}
	//if (w_c.GetBoolRelatedWidth())
	//{
		w_c.FindRelatedHeight(i_p.GetNoiseMap(), v_d.GetGridSize(), layers_, track_, s_p.GetControlPoints());
	//}
	w_c.SetModi();
	w_c.FindWidth(track_, s_p.GetControlPoints(), v_d.GetPointPos(), s_p.GetLengths(), s_p.GetAngles());
	i_p.CreateImage(voronoi_d, v_d.GetGridSize());
	i_p.DrawWidthTrack(voronoi_d, v_d.GetGridSize(), w_c.GetNewTrack());
}