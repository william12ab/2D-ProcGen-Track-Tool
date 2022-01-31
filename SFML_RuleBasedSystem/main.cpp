// SFML includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <list>
#include "MainHeader.h"
#include <chrono>

#include <thread>

using std::thread;

using std::chrono::duration_cast;
using std::chrono::milliseconds; 
using the_clock = std::chrono::steady_clock;

void SettingText()
{
	title_name_.setFont(font);
	title_name_.setString("2D Track Generator\n Options");
	title_name_.setCharacterSize(18);
	title_name_.setPosition(sf::Vector2f(805, 10));


}


int main()
{
	sf::Clock clock;
	// Seed the random number generator
	srand((unsigned)std::time(NULL));

	font.loadFromFile("DefaultAriel.ttf");
	//text setting
	SettingText();


	// Create the window and UI bar on the right
	sf::RenderWindow window(sf::VideoMode(1000,800), "2D Track Generator", sf::Style::Close);

	ImGui::SFML::Init(window);

	sf::Clock deltaClock;
	float elapsed = 0.0f;

	//set the defaults for the application
	resolution_ = 800;
	sites_ = 500;
	points_ =2;
	regen_ = false;
	track_type_ = 1;  //1=p2p,0=loop

	VoronoiDiagram* v_d_p = new VoronoiDiagram();
	
	//set the size, number of sites and points. this will take input 
	v_d_p->SetGridSize(resolution_);
	v_d_p->SetNumberOfSites(sites_);
	v_d_p->SetNumberOfPoints(points_);
	v_d_p->~VoronoiDiagram();
	//resize the arrays
	v_d_p->InitVector(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), v_d_p->GetNumberOfSites());
	
	//places the sites
	v_d_p->DistributeSites(v_d_p->GetNumberOfSites(),v_d_p->GetGridSize());
	//v_d_p->EqualDSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
	//v_d_p->DisplacePoints(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());

	//sets up the vertex array
	sf::VertexArray voronoi_d(sf::Points, (v_d_p->GetGridSize()*v_d_p->GetGridSize()));

	//creates the vd in grid_v_1
	the_clock::time_point startTime = the_clock::now();
	

	thread th(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(),0, 200);
	thread th2(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 200, 400);
	thread th3(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 400, 600);
	thread th4(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 600, 800);
	th.join();
	th2.join();
	th3.join();
	th4.join();

	//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
	
	the_clock::time_point endTime = the_clock::now();
	v_d_p->SetEdges(v_d_p->GetGridSize());
	auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
	v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_);
	//sets the points to connect the distance



	//init grid should be fine, no need to change.
	shortest_path_.Initgrid(v_d_p->GetGridSize(),v_d_p->GetGrid(),v_d_p->GetNumberOfPoints());

	//pass in the start and end to both these functions
	int start = -4;
	shortest_path_.PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());
	for (int i = 0; i < (v_d_p->GetNumberOfPoints()-1); i++)
	{
		shortest_path_.PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), shortest_path_.GetCountHolder(), shortest_path_.bGetFoundEnd(), shortest_path_.GetIt(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), -3);
		shortest_path_.PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), shortest_path_.GetCountHolder(), 0);
		//changes start point first then the end point to start point, and second end point to 1st end point
		//so p0=p-1, p1=0,p2=1
		std::cout <<"time: "<< time_taken; std::cout << std::endl;
		shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
		shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
		shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);
	}
	v_d_p->DrawVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
	// While the window is open, update
	while (window.isOpen())
	{
		// Parse events
		sf::Event sf_event;
		while (window.pollEvent(sf_event)) {
			// Close the window when the close button is pressed
			ImGui::SFML::ProcessEvent(sf_event);
			if (sf_event.type == sf::Event::Closed) {
				window.close();
			}
			
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Options");
		ImGui::SliderInt("Resolution", &resolution_, 100, 800);
		ImGui::SliderInt("Sites", &sites_, 5, 100);
		ImGui::SliderInt("Points", &points_, 2, 5);
		ImGui::SliderInt("Track Type",&track_type_, 0, 2);
		ImGui::Text("0 = triangular\n1 = point to point\n2 = obtuse triangle");
		if (ImGui::Button("Regenerate"))
		{
			v_d_p->SetType(track_type_);
			voronoi_d.clear();
			v_d_p->~VoronoiDiagram();
			v_d_p->SetGridSize(resolution_);
			v_d_p->SetNumberOfSites(sites_);
			v_d_p->SetNumberOfPoints(points_);
			voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
			//resize the arrays
			v_d_p->InitVector(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), v_d_p->GetNumberOfSites());

			//places the sites
			v_d_p->RandomPlaceSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());


			//creates the vd in grid_v_1
		
			//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(),0,400);

			thread th(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 0, 200);
			thread th2(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 200, 400);
			thread th3(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 400, 600);
			thread th4(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 600, 800);
			th.join();
			th2.join();
			th3.join();
			th4.join();

			v_d_p->SetEdges(v_d_p->GetGridSize());
	
			v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_);




			//init grid should be fine, no need to change.
			shortest_path_.Initgrid(v_d_p->GetGridSize(), v_d_p->GetGrid(), v_d_p->GetNumberOfPoints());

			//pass in the start and end to both these functions
			int start = -4;
			shortest_path_.PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());
			
			//if type 2, then need to loop over number of points differently and check when the index is = 1 so that the starting point can be changed to the end
			if (v_d_p->GetType() == 2)
			{
				the_clock::time_point startTime = the_clock::now();
				for (int i = 0; i < (v_d_p->GetNumberOfPoints() ); i++)
				{
					shortest_path_.PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), shortest_path_.GetCountHolder(), shortest_path_.bGetFoundEnd(), shortest_path_.GetIt(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), -3);
					shortest_path_.PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), shortest_path_.GetCountHolder(), 0);
					//changes start point first then the end point to start point, and second end point to 1st end point
					//so p0=p-1, p1=0,p2=1
					if (i==1)
					{
						shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -1234, -5);
					}
					shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
					shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
					shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);

			
					shortest_path_.CleanGrid(v_d_p->GetGridSize(), v_d_p->GetGrid());

				
				}
				the_clock::time_point endTime = the_clock::now();

				auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();

				std::cout << "time taken: " << time_taken; std::cout << std::endl;
			}
			else
			{
				the_clock::time_point startTime = the_clock::now();
				for (int i = 0; i < (v_d_p->GetNumberOfPoints() - 1); i++)
				{
					shortest_path_.PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), shortest_path_.GetCountHolder(), shortest_path_.bGetFoundEnd(), shortest_path_.GetIt(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), -3);
					shortest_path_.PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), shortest_path_.bGetEnd(), shortest_path_.GetXHolder(), shortest_path_.GetYHolder(), shortest_path_.GetCountHolder(), 0);
					//changes start point first then the end point to start point, and second end point to 1st end point
					//so p0=p-1, p1=0,p2=1
					shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
					shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
					shortest_path_.ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);

	
					shortest_path_.CleanGrid(v_d_p->GetGridSize(), v_d_p->GetGrid());

		
				}
				the_clock::time_point endTime = the_clock::now();

				auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();

				std::cout << "time taken 1 or 0: " << time_taken; std::cout << std::endl;
			}
			
			v_d_p->DrawVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
		}

		ImGui::End();
		//used to display the whole voronoi diagram
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			v_d_p->DrawFullVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
			shortest_path_.PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());

		}
		window.clear();
		window.draw(voronoi_d);
		window.draw(title_name_);
		ImGui::SFML::Render(window);
		window.display();
	}

	delete v_d_p;
	ImGui::SFML::Shutdown();
	return 0;
}