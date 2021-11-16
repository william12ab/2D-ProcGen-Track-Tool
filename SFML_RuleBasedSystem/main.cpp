// SFML includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <list>
#include "MainHeader.h"
// The defines that state the size of the grid and the speed of the game.
#define GRID_SIZE_X 800
#define GRID_SIZE_Y 800
#define SQUARE_SIZE 10
#define FRAMES_PER_SECOND 5



int main()
{
	// Seed the random number generator
	srand((unsigned)std::time(NULL));

	// Create the window and UI bar on the right
	const sf::Vector2f gridSize(GRID_SIZE_X*SQUARE_SIZE, GRID_SIZE_Y*SQUARE_SIZE);
	sf::RenderWindow window(sf::VideoMode(800,800), "SFML_RuleBasedSystem", sf::Style::Close);



	// Create the font
	sf::Font font;
	font.loadFromFile("DefaultAriel.ttf");

	// Create the start/stop text and position+rotate it

	// Flag for when the game plays
	bool event_playing = false;

	// Clock for timing
	sf::Clock clock;
	float elapsed = 0.0f;


	int counter = 0;




	
	//set the size, number of sites and points. this will take input 
	Voronoi_Diagram.SetGridSize(800);
	Voronoi_Diagram.SetNumberOfSites(50);
	Voronoi_Diagram.SetNumberOfPoints(3);

	//resize the arrays
	Voronoi_Diagram.InitVector(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetNumberOfPoints(),Voronoi_Diagram.GetNumberOfSites());
	
	//places the sites
	Voronoi_Diagram.RandomPlaceSites(Voronoi_Diagram.GetNumberOfSites(),Voronoi_Diagram.GetGridSize());

	//sets up the vertex array
	sf::VertexArray voronoi_d(sf::Points, (Voronoi_Diagram.GetGridSize()*Voronoi_Diagram.GetGridSize()));


	//creates the vd in grid_v_1

	Voronoi_Diagram.CreateDiagram(Voronoi_Diagram.GetNumberOfSites(), Voronoi_Diagram.GetGridSize());
	Voronoi_Diagram.SetEdges(Voronoi_Diagram.GetGridSize());

	Voronoi_Diagram.SetPoint(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetNumberOfPoints(), 1);




	//init grid should be fine, no need to change.
	shortest_path_.Initgrid(Voronoi_Diagram.GetGridSize(),Voronoi_Diagram.GetGrid(),Voronoi_Diagram.GetNumberOfPoints());

	//pass in the start and end to both these functions
	int start = -4;
	
	for (int i = 0; i < (Voronoi_Diagram.GetNumberOfPoints()-1); i++)
	{
		shortest_path_.PhaseOne(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), shortest_path_.GetCountHolder(), shortest_path_.bGetFoundEnd(), shortest_path_.GetIt(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), -3);
		shortest_path_.PhaseTwo(Voronoi_Diagram.GetGridSize(), Voronoi_Diagram.GetGrid(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), shortest_path_.GetCountHolder(), 0);
		//changes start point first then the end point to start point, and second end point to 1st end point
		//so p0=p-1, p1=0,p2=1

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
				window.close();
			}
			
		}

		

		//used to regen the vd
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