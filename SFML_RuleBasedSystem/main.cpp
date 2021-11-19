// SFML includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <list>
#include "MainHeader.h"
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds; 
using the_clock = std::chrono::steady_clock;


int main()
{
	// Seed the random number generator
	srand((unsigned)std::time(NULL));

	// Create the window and UI bar on the right
	sf::RenderWindow window(sf::VideoMode(1000,800), "2D Track Generator", sf::Style::Close);



	// Create the font
	sf::Font font;
	font.loadFromFile("DefaultAriel.ttf");
	// Clock for timing
	sf::Clock clock;
	float elapsed = 0.0f;


	int counter = 0;
	//set the size, number of sites and points. this will take input 
	Voronoi_Diagram.SetGridSize(400);
	Voronoi_Diagram.SetNumberOfSites(100);
	Voronoi_Diagram.SetNumberOfPoints(3);

	//resize the arrays
	Voronoi_Diagram.InitVector(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetNumberOfPoints(),Voronoi_Diagram.GetNumberOfSites());
	
	//places the sites
	Voronoi_Diagram.RandomPlaceSites(Voronoi_Diagram.GetNumberOfSites(),Voronoi_Diagram.GetGridSize());

	//sets up the vertex array
	sf::VertexArray voronoi_d(sf::Points, (Voronoi_Diagram.GetGridSize()*Voronoi_Diagram.GetGridSize()));


	//creates the vd in grid_v_1
	the_clock::time_point startTime = the_clock::now();
	Voronoi_Diagram.CreateDiagram(Voronoi_Diagram.GetNumberOfSites(), Voronoi_Diagram.GetGridSize());
	Voronoi_Diagram.SetEdges(Voronoi_Diagram.GetGridSize());
	the_clock::time_point endTime = the_clock::now();

	auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
	Voronoi_Diagram.SetPoint(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetNumberOfPoints(), 1);




	//init grid should be fine, no need to change.
	shortest_path_.Initgrid(Voronoi_Diagram.GetGridSize(),Voronoi_Diagram.GetGrid(),Voronoi_Diagram.GetNumberOfPoints());

	//pass in the start and end to both these functions
	int start = -4;
	shortest_path_.PrintOutStartEnd(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid());
	for (int i = 0; i < (Voronoi_Diagram.GetNumberOfPoints()-1); i++)
	{
		shortest_path_.PhaseOne(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), shortest_path_.GetCountHolder(), shortest_path_.bGetFoundEnd(), shortest_path_.GetIt(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), -3);
		shortest_path_.PhaseTwo(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), shortest_path_.GetCountHolder(), 0);
		//changes start point first then the end point to start point, and second end point to 1st end point
		//so p0=p-1, p1=0,p2=1
		std::cout << time_taken; std::cout << std::endl;
		shortest_path_.ChangePoint(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), 0, -1234);
		shortest_path_.ChangePoint(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), -3, 0);
		shortest_path_.ChangePoint(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), start - i, -3);
	}
	Voronoi_Diagram.DrawVoronoiDiagram(voronoi_d, Voronoi_Diagram.GetGridSize());
	// While the window is open, update
	while (window.isOpen())
	{
		// Parse events
		sf::Event sf_event;
		while (window.pollEvent(sf_event)) {
			// Close the window when the close button is pressed
			if (sf_event.type == sf::Event::Closed) {
				//Voronoi_Diagram.~VoronoiDiagram();
				window.close();
				
			}
			
		}
		//used to display the whole voronoi diagram
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			Voronoi_Diagram.DrawFullVoronoiDiagram(voronoi_d, Voronoi_Diagram.GetGridSize());
			shortest_path_.PrintOutStartEnd(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid());

		}
		window.draw(voronoi_d);
		window.display();
	}
	return 0;
}