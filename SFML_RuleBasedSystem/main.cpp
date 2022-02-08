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
void threadfunc(std::vector<thread*> thread_vector, VoronoiDiagram* v_d_p)
{
	int start_ = 0;
	for (int i = 0; i < num_threads_; i++)
	{
		thread_vector.push_back(new thread(&VoronoiDiagram::CreateDiagram, v_d_p, v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(), start_, start_ + (resolution_ / num_threads_)));
		start_ += resolution_ / num_threads_;
	}
	int a = 0;
	for (thread* th : thread_vector)
	{
		// If thread Object is Joinable then Join that thread.
		if (th->joinable())
			th->join();
			delete th;
	}
}

//pass in the thing and v_d_p
void testing(sf::VertexArray& vertexarray, VoronoiDiagram* v_)
{
	for (int i = 0; i < v_->GetGridSize(); i++)
	{
		for (int j = 0; j < v_->GetGridSize(); j++)
		{

		}
	}
}




int main()
{
	sf::Clock clock;
	// Seed the random number generator
	srand((unsigned)std::time(NULL));

	font.loadFromFile("DefaultAriel.ttf");
	//text setting
	SettingText();


	std::vector<thread*> thread_vector;
	std::vector<thread*> thread_vector_path;

	// Create the window and UI bar on the right
	sf::RenderWindow window(sf::VideoMode(1000,800), "2D Track Generator", sf::Style::Close);

	ImGui::SFML::Init(window);

	sf::Clock deltaClock;
	float elapsed = 0.0f;

	//set the defaults for the application
	resolution_ = 400;
	sites_ = 25;
	points_ =2;
	regen_ = false;
	track_type_ = 1;  //1=p2p,0=loop
	num_threads_ = 8;
	render_height_map_ = false;

	number_ = 75;
	div_ = 1.0f;
	catch_ = 0.6f;
	mult_ = 1;

	height_ = 1.0f;
	frequency_ = 1.0f;

	VoronoiDiagram* v_d_p = new VoronoiDiagram();
	ShortestPath* s_p_p = new ShortestPath();

	//set the size, number of sites and points. this will take input 
	v_d_p->SetGridSize(resolution_);
	v_d_p->SetNumberOfSites(sites_);
	v_d_p->SetNumberOfPoints(points_);
	v_d_p->~VoronoiDiagram();
	//resize the arrays
	
	//places the sites
	//sets up the vertex array
	sf::VertexArray voronoi_d(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	
	sf::VertexArray height_map(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));

	//creates the vd in grid_v_1

	v_d_p->InitVector(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), v_d_p->GetNumberOfSites());

//	v_d_p->DiagramAMP(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
		
	do 
	{
		if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
		{
			voronoi_d.clear();
			height_map.clear();
			v_d_p->SetFaile(false);
			s_p_p->SetFailed(false);
			thread_vector.clear();
			height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
			voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
		}
		v_d_p->DistributeSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
		//the_clock::time_point startTime = the_clock::now();
		threadfunc(thread_vector, v_d_p);
		//v_d_p->DrawVD(height_map, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites(), number_, catch_, div_);

		v_d_p->DrawWave(height_map, v_d_p->GetGridSize(), mult_);

		//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
		//the_clock::time_point endTime = the_clock::now();
		//auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
		//std::cout << "time(v d): " << time_taken; std::cout << std::endl;
		v_d_p->SetEdges(v_d_p->GetGridSize());
		v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_, v_d_p->GetFailed());
		//sets the points to connect the distance



	//init grid should be fine, no need to change.
		s_p_p->Initgrid(v_d_p->GetGridSize(), v_d_p->GetGrid(), v_d_p->GetNumberOfPoints());
		//pass in the start and end to both these functions
		int start = -4;
		s_p_p->PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());
		the_clock::time_point startTime_path = the_clock::now();

		for (int i = 0; i < (v_d_p->GetNumberOfPoints() - 1); i++)
		{
			s_p_p->PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->GetCountHolder(), s_p_p->bGetFoundEnd(), s_p_p->GetIt(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), -3, s_p_p->GetFailed(), 0, v_d_p->GetGridSize());
			s_p_p->PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), s_p_p->GetCountHolder(), 0);
			//changes start point first then the end point to start point, and second end point to 1st end point
			//so p0=p-1, p1=0,p2=1
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
			s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);
		}
		//the_clock::time_point endTime_path = the_clock::now();
		//auto time_taken_path = duration_cast<milliseconds>(endTime_path - startTime_path).count();
		//std::cout << "time(path): " << time_taken_path; std::cout << std::endl;
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
		ImGui::SliderInt("Num Threads", &num_threads_, 1, 16);
		ImGui::SliderInt("Resolution", &resolution_, 100, 800);
		ImGui::SliderInt("Sites", &sites_, 5, 100);
		ImGui::SliderInt("Points", &points_, 2, 5);
		ImGui::SliderInt("Track Type",&track_type_, 0, 2);
		ImGui::SliderInt("number", &number_, 0, 100);
		ImGui::SliderFloat("catch", &catch_, 0.01f, 1.0f);
		ImGui::SliderFloat("div", &div_, 0.0f, 2.0f);
		ImGui::SliderInt("multiplyer", &mult_, 1, 100);
		ImGui::SliderFloat("Perlin Frequency", &frequency_, 0.0010, 0.035);
		ImGui::SliderFloat("Perlin Height", &height_, 1.0f, 15.0f);
		v_d_p->SetF(frequency_);
		v_d_p->SetH(height_);

		ImGui::Text("0 = triangular\n1 = point to point\n2 = obtuse triangle");
		if (ImGui::Button("Regenerate"))
		{
			v_d_p->SetType(track_type_);
			voronoi_d.clear();
			height_map.clear();
			v_d_p->~VoronoiDiagram();
			v_d_p->SetGridSize(resolution_);
			v_d_p->SetNumberOfSites(sites_);
			v_d_p->SetNumberOfPoints(points_);
			height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
			voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
			//resize the arrays
			v_d_p->InitVector(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), v_d_p->GetNumberOfSites());

			//places the sites
			do
			{
				if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
				{
					voronoi_d.clear();
					height_map.clear();
					height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
					voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
					v_d_p->SetFaile(false);
					s_p_p->SetFailed(false);
					thread_vector.clear();
				}
				v_d_p->RandomPlaceSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());

				//v_d_p->CreateDiagram(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize(),0,400);
				//the_clock::time_point startTime = the_clock::now();
				thread_vector.clear();
				threadfunc(thread_vector, v_d_p);
				//v_d_p->DrawVD(height_map, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites(), number_, catch_, div_);
				v_d_p->DrawNoise(height_map, v_d_p->GetGridSize());

				//the_clock::time_point endTime = the_clock::now();

				//auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();

				//std::cout << "time taken (v_d ui): " << time_taken; std::cout << std::endl;
				v_d_p->SetEdges(v_d_p->GetGridSize());

				//the_clock::time_point startTimea = the_clock::now();
				v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_, v_d_p->GetFailed());
				//the_clock::time_point endTimea = the_clock::now();

				//auto time_takena = duration_cast<milliseconds>(endTimea - startTimea).count();

				//std::cout << "time taken (set point ui): " << time_takena; std::cout << std::endl;

			//init grid should be fine, no need to change.
				s_p_p->Initgrid(v_d_p->GetGridSize(), v_d_p->GetGrid(), v_d_p->GetNumberOfPoints());

			//pass in the start and end to both these functions
				int start = -4;
				s_p_p->PrintOutStartEnd(v_d_p->GetGridSize(), v_d_p->GetGrid());
			
			//if type 2, then need to loop over number of points differently and check when the index is = 1 so that the starting point can be changed to the end
				if (v_d_p->GetType() == 2)
				{
				
					for (int i = 0; i < (v_d_p->GetNumberOfPoints() ); i++)
					{
					
						//ThreadFuncPath(thread_vector, v_d_p, s_p_p);
						s_p_p->PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->GetCountHolder(), s_p_p->bGetFoundEnd(), s_p_p->GetIt(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), -3, s_p_p->GetFailed(), 0, v_d_p->GetGridSize());
						s_p_p->PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), s_p_p->GetCountHolder(), 0);
					//changes start point first then the end point to start point, and second end point to 1st end point
					//so p0=p-1, p1=0,p2=1
						if (i==1)
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
					
					//ThreadFuncPath(thread_vector, v_d_p, s_p_p);
						s_p_p->PhaseOne(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->GetCountHolder(), s_p_p->bGetFoundEnd(), s_p_p->GetIt(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), -3, s_p_p->GetFailed(), 0, v_d_p->GetGridSize());
						s_p_p->PhaseTwo(v_d_p->GetGridSize(), v_d_p->GetGrid(), s_p_p->bGetEnd(), s_p_p->GetXHolder(), s_p_p->GetYHolder(), s_p_p->GetCountHolder(), 0);
					//changes start point first then the end point to start point, and second end point to 1st end point
					//so p0=p-1, p1=0,p2=1
						s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), 0, -1234);
						s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), -3, 0);
						s_p_p->ChangePoint(v_d_p->GetGridSize(), v_d_p->GetGrid(), start - i, -3);
						s_p_p->CleanGrid(v_d_p->GetGridSize(), v_d_p->GetGrid());	
					}
				//the_clock::time_point endTime = the_clock::now();

				//auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();

				//std::cout << "time taken 1 or 0(path): " << time_taken; std::cout << std::endl;
			}
			} while (v_d_p->GetFailed() || s_p_p->GetFailed());

			v_d_p->DrawVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
		}

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