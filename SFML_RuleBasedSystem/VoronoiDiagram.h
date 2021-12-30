#pragma once

#include <SFML/Graphics/VertexArray.hpp>
class VoronoiDiagram
{
public:
	VoronoiDiagram();
	~VoronoiDiagram();
	
	void RandomPlaceSites(int num_sites, int grid_size);				//distrubutes the sites
	void EqualDSites(int num_sites, int grid_size);
	void DisplacePoints(int num_sites, int grid_size);
	void DistributeSites(int num_sites, int grid_size);


	int DistanceSqrt(int x, int y, int x2, int y2);						//the distance formula
	void CreateDiagram(int num_sites,int grid_size);					//creates the voronoi diagram
	void SetEdges(int grid_size);										//sets the edges of voronoi diagram, so it is just the edges in the vector
	
	void SetPoint(int grid_size, int num_points, int type);				//Sets the point(s) for the distance.
	void InitVector(int grid_size, int num_points, int num_sites);		//inits the vector to correct size;


	//getters		


	int* GetGrid() { return grid_v_1; }
	int* GetSites() { return sites_v_1; }


	int GetNumberOfSites() { return num_of_sites; }						//returns the number of sites
	int GetGridSize() { return grid_size_x; };							//returns the size of the grid
	int GetType() { return type; };										//returns the type of track, closed or open
	int GetNumberOfPoints() { return num_of_points; }					//returns the number of points

	//setters
	void SetNumberOfSites(int p) { num_of_sites = p; }					//sets the number of sites
	void SetGridSize(int p) { grid_size_x=p; }							//sets the x and y of the grid
	void SetNumberOfPoints(int p) { num_of_points = p; }				//sets the number of points in the track
	void SetType(int p) { type = p; }									//sets the type of track


	void DrawVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size);
	void DrawFullVoronoiDiagram(sf::VertexArray& vertexarray, int grid_size);
private:
	int num_of_sites;							//number of sites that form the diagram
	int num_of_points;							//number of points that form the track.
	int type;

	int grid_size_x;							//size in the x axis of the diagram

	int* sites_v_1;								//stores the sites in dynamic array
	int* grid_v_1;								//stores the grid in dynamic array


	int site_iterator = 0;
};

