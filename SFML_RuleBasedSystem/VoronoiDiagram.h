#pragma once
#include <vector>
class VoronoiDiagram
{
public:
	VoronoiDiagram();
	
	void RandomPlaceSites();					//distrubutes the sites
	int DistanceSqrt(int x, int y, int x2, int y2);		//the distance formula
	void CreateDiagram();						//creates the voronoi diagram
	void SetEdges();							//sets the edges of voronoi diagram, so it is just the edges in the vector
	void SetBorders();							//removes the borders from the structure as we dont want them
	void SetPoint();							//Sets the point(s) for the distance.

	//getters
	std::vector<std::vector<int>>GetGridVector() { return grid_v; }		//returns the grid vector
	std::vector<std::vector<int>>GetSitesVector() { return sites_v; }	//returns the size vector
	std::vector<std::vector<int>>GetSitesVector() { return points_v; }	//returns the points vector		
	int GetNumberOfSites() { return num_of_sites; }						//returns the number of sites
	int GetGridSize() { return grid_size_x; };							//returns the size of the grid
	
	//setters
	int SetNumberOfsites(int p) { num_of_points = p; }					//sets the number of sites
	int SetGridSize(int p) { grid_size_x=p; }							//sets the x and y of the grid

	

private:
	std::vector<std::vector<int>> grid_v;		//contains the positions of the voronoi diagram,
	std::vector<std::vector<int>> sites_v;		//contains the positions of the sites
	std::vector<std::vector<int>> points_v;		//contains the positions of the points that are connected to form the track.

	int num_of_sites;							//number of sites that form the diagram
	int num_of_points;							//number of points that form the track.

	int grid_size_x;							//size in the x axis of the diagram
	int grid_size_y;							//size in the y axis of the diagram
};

