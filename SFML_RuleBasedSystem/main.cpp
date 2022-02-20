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


void SettingText()
{
	title_name_.setFont(font);
	title_name_.setString("2D Track Generator\n Options");
	title_name_.setCharacterSize(18);
	title_name_.setPosition(sf::Vector2f(805, 10));
}


void Init(sf::RenderWindow &window)
{
	font.loadFromFile("DefaultAriel.ttf");
	//text setting
	SettingText();
	ImGui::SFML::Init(window);


	resolution_ = 513;
	sites_ = 25;
	points_ = 2;
	regen_ = false;
	track_type_ = 1;  //1=p2p,0=loop
	render_height_map_ = false;
	number_ = 35;
	div_ = 2.0f;
	catch_ = 100;
	height_ = 1.0f;
	frequency_ = 1.0f;
	alpha_ = 255;
	layers_ = 1;
	octaves_ = 1;
	full_random_ = true;
	times_ = 1;

}

void ResetVars(VoronoiDiagram*v_d_p, ShortestPath*s_p_p, sf::VertexArray& voronoi_d, sf::VertexArray& height_map)
{
	voronoi_d.clear();
	height_map.clear();
	v_d_p->SetFaile(false);
	s_p_p->SetFailed(false);
	height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
}
void SetVars(VoronoiDiagram*v_d_p)
{
	v_d_p->SetType(track_type_);
	v_d_p->SetGridSize(resolution_);
	v_d_p->SetNumberOfSites(sites_);
	v_d_p->SetNumberOfPoints(points_);
	v_d_p->~VoronoiDiagram();
	v_d_p->InitVector(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), v_d_p->GetNumberOfSites());

}
void CreateVoronoi(VoronoiDiagram* v_d_p, sf::VertexArray &height_map)
{
	if (full_random_)
	{
		v_d_p->RandomPlaceSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
	}
	else
	{
		v_d_p->EqualDSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), times_);
	}
	
	v_d_p->DiagramAMP(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
	//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), 0, v_d_p->GetGridSize());
	v_d_p->DrawVD(height_map, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites(), number_, catch_, div_);
	v_d_p->SetEdges(v_d_p->GetGridSize());
	v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_, v_d_p->GetFailed());

}
void CreateTrack(VoronoiDiagram* v_d_p, ShortestPath* s_p_p)
{
	//init grid should be fine, no need to change.
	s_p_p->Initgrid(v_d_p->GetGridSize(), v_d_p->GetGrid(), v_d_p->GetNumberOfPoints());
	//pass in the start and end to both these functions
	int start = -4;
	s_p_p->PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());
	//if type 2, then need to loop over number of points differently and check when the index is = 1 so that the starting point can be changed to the end
	if (v_d_p->GetType() == 2)
	{

		for (int i = 0; i < (v_d_p->GetNumberOfPoints()); i++)
		{
			s_p_p->PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->GetCountHolder(), s_p_p->bGetFoundEnd(), s_p_p->GetIt(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), -3, s_p_p->GetFailed(), 0, v_d_p->GetGridSize());
			s_p_p->PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), s_p_p->GetCountHolder(), 0);
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			if (i == 1)
			{
				s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -1234, -5);
			}
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);
			s_p_p->CleanGrid(v_d_p->GetGridSize(), v_d_p->GetGrid());
			//std::cout << "successful path\n";
		}

	}
	else
	{
		//the_clock::time_point startTime = the_clock::now();
		for (int i = 0; i < (v_d_p->GetNumberOfPoints() - 1) && !s_p_p->GetFailed(); i++)
		{
			s_p_p->PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->GetCountHolder(), s_p_p->bGetFoundEnd(), s_p_p->GetIt(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), -3, s_p_p->GetFailed(), 0, v_d_p->GetGridSize());
			s_p_p->PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), s_p_p->GetCountHolder(), 0);
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);
			s_p_p->CleanGrid(v_d_p->GetGridSize(), v_d_p->GetGrid());
		}
	}
}



int main()
{
	sf::Clock clock;
	sf::Clock deltaClock;
	// Seed the random number generator
	srand(static_cast <unsigned> (time(0)));


	// Create the window and UI bar on the right
	sf::RenderWindow window(sf::VideoMode(1000,800), "2D Track Generator", sf::Style::Close);

	

	Init(window);

	VoronoiDiagram* v_d_p = new VoronoiDiagram();
	ShortestPath* s_p_p = new ShortestPath();

	//track initialisation
	SetVars(v_d_p);
	//sets up the vertex array and the data structures for voronoi diagram
	sf::VertexArray voronoi_d(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	sf::VertexArray height_map(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));

		
	do 
	{
		if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
		{
			ResetVars(v_d_p,s_p_p, voronoi_d, height_map);
		}
		CreateVoronoi(v_d_p, height_map);

		CreateTrack(v_d_p, s_p_p);

	} while (v_d_p->GetFailed() || s_p_p->GetFailed());
	
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
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Track Variables"))
		{
			ImGui::SliderInt("Resolution", &resolution_, 100, 800);
			ImGui::SliderInt("Sites", &sites_, 5, 100);
			ImGui::SliderInt("Iterations of Displacement", &times_, 1, 32);
			ImGui::Checkbox("Random or Equal+Random?", &full_random_);
			ImGui::SliderInt("Points", &points_, 2, 5);
			ImGui::SliderInt("Track Type", &track_type_, 0, 2);
			ImGui::Text("0 = triangular\n1 = point to point\n2 = obtuse triangle");
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Heightmap Variables"))
		{
			ImGui::SliderInt("Size of cell outlines", &number_, 0, 100);
			ImGui::TextWrapped("Controls the outline of voronoi cells");
			ImGui::SliderFloat("Brightness", &div_, 0.0f, 2.0f);
			ImGui::TextWrapped("Controls controls the brightness of image(higher darker)");
			ImGui::SliderFloat("Perlin Height", &height_, 0.0f, 1.0f);
			ImGui::SliderInt("Number of Layers of Noise", &layers_,0, 10);
			v_d_p->SetH(height_);
			ImGui::SliderInt("Alpha", &alpha_, 0, 255);
			if (ImGui::Button("Change alpha"))
			{
				v_d_p->ChangeAlpha(height_map, v_d_p->GetGridSize(), alpha_);
			}
			if (ImGui::Button("Create Noise Image"))
			{
				height_map.clear();
				v_d_p->SetGridSize(resolution_);

				height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
				v_d_p->DrawNoise(height_map, v_d_p->GetGridSize(), layers_);
			}
			ImGui::SliderInt("Octaves: ", &octaves_, 1, 8);
			if (ImGui::Button("Create FBM Image"))
			{
				height_map.clear();
				v_d_p->SetGridSize(resolution_);

				height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
				v_d_p->DrawFBM(height_map, v_d_p->GetGridSize(), octaves_);
			}

		}
		ImGui::Text("\n");
		if (ImGui::Button("Regenerate"))
		{
			SetVars(v_d_p);
			voronoi_d.clear();
			height_map.clear();
			voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
			height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
			

			//places the sites
			do
			{
				if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
				{
					ResetVars(v_d_p, s_p_p, voronoi_d, height_map);
				}
				the_clock::time_point startTime = the_clock::now();
				CreateVoronoi(v_d_p, height_map);
				the_clock::time_point endTime = the_clock::now();
				auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
				std::cout << "time(v d): " << time_taken; std::cout << std::endl;
				CreateTrack(v_d_p, s_p_p);
				
			} while (v_d_p->GetFailed() || s_p_p->GetFailed());

			//v_d_p->DrawVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
		}
	

	
		if (ImGui::Button("Write to file"))
		{
	
			v_d_p->WriteToFile(v_d_p->GetGridSize(), voronoi_d, layers_);

		}
		ImGui::Text("\n");
		ImGui::End();
		//used to display the whole voronoi diagram
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			v_d_p->DrawFullVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
			//s_p_p->PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			render_height_map_ = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			render_height_map_ = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			v_d_p->DrawVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
		}
		window.clear();
		if (render_height_map_)
		{
			window.draw(height_map);
		}
		window.draw(voronoi_d);
		window.draw(title_name_);
		ImGui::SFML::Render(window);
		window.display();
	}

	delete v_d_p;
	delete s_p_p;
	ImGui::SFML::Shutdown();
	return 0;
}