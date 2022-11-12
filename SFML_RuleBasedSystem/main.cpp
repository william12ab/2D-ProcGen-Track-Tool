// SFML includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <list>
#include "MainHeader.h"
#include <chrono>
#include <random>
#include "Input.h"

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

	//defaults: 
	resolution_ = 513;
	sites_ = 25;
	points_ = 2;
	regen_ = false;
	track_type_ = 1;  //1=p2p,0=loop
	render_height_map_ = false;
	n_render_height_map_ = false;
	number_ = 35;
	div_ = 2.0f;
	height_ = 1.0f;
	frequency_ = 1.0f;
	alpha_ = 255;
	layers_ = 1;
	octaves_ = 1;
	full_random_ = true;
	times_ = 1;
	displacement_ = 100;
	radius_cutoff = 120;
	peaks_to_count_ = 1;
	do_testing_ = false;
	step_curve=0.01;

}

int main()
{
	Input input;
	sf::Clock clock;
	sf::Clock deltaClock;
	// Seed the random number generator
	srand(static_cast <unsigned> (time(0)));


	// Create the window and UI bar on the right
	sf::RenderWindow window(sf::VideoMode(1000,800), "2D Track Generator", sf::Style::Close);
	Init(window);
	sf::View view_;
	view_.reset(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
	//objs for main
	VoronoiDiagram* v_d_p = new VoronoiDiagram();
	ShortestPath* s_p_p = new ShortestPath();
	DeCastelJau* d_c_j = new DeCastelJau();
	ImageProcessing* i_p_p = new ImageProcessing();
	TrackTools* t_t_p = new TrackTools();
	CatmullRomSpline* c_r_s = new CatmullRomSpline();
	InputManager input_manager(&input,&view_);
	//

	//
	t_t_p->SetVars(v_d_p, i_p_p, track_type_, resolution_, sites_, points_);
	//
	//sets up the vertex array and the data structures for voronoi diagram
	sf::VertexArray voronoi_d(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	sf::VertexArray height_map(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	sf::VertexArray n_height_map(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	sf::VertexArray final_map(sf::Points, (v_d_p->GetGridSize() * v_d_p->GetGridSize()));
	//

	//creates a track initially 
	t_t_p->Generate(v_d_p, s_p_p, voronoi_d, height_map, n_height_map, i_p_p, times_, displacement_, number_, full_random_, track_type_);
	//
	



	// While the window is open, update
	while (window.isOpen())
	{
		// Parse events
		sf::Event sf_event;
		while (window.pollEvent(sf_event)) {
			// Close the window when the close button is pressed
			ImGui::SFML::ProcessEvent(sf_event);
			switch (sf_event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				// update input class
				input.setKeyDown(sf_event.key.code);
				break;
			case sf::Event::KeyReleased:
				//update input class
				input.setKeyUp(sf_event.key.code);
				break;
			case sf::Event::MouseMoved:
				//update input class
				input.setMousePosition(sf_event.mouseMove.x, sf_event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (sf_event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setMouseLDown(true);
				}
				else if (sf_event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRDown(true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (sf_event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setMouseLDown(false);
				}
				else if (sf_event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRDown(false);
				}
				break;
			}
		}
		window.setView(view_);
		ImGui::SFML::Update(window, deltaClock.restart());
		
		ImGui::Begin("Options");
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Track Variables"))
		{
			ImGui::SliderInt("Resolution", &resolution_, 100, 800);
			ImGui::SliderInt("Sites", &sites_, 5, 100);
			ImGui::SliderInt("Iterations of Displacement", &times_, 1, 32);
			ImGui::SliderInt("Displacement at Each Displacement", &displacement_, 1, 200);
			ImGui::Checkbox("Random or Equal+Random?", &full_random_);
			ImGui::SliderInt("Points", &points_, 2, 5);
			ImGui::SliderInt("Track Type", &track_type_, 0, 2);
			ImGui::Text("0 = triangular\n1 = point to point\n2 = loop");
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Heightmap Variables"))
		{
			ImGui::SliderInt("Size of cell outlines", &number_, 0, 100);
			ImGui::Text("lower = large outlines");
			ImGui::Text("higher = less effect");
			ImGui::SliderInt("Number of Layers of Noise", &layers_,0, 10);
			ImGui::SliderInt("Alpha", &alpha_, 0, 255);
			ImGui::SliderInt("Radius Cut-off:", &radius_cutoff, 50, 255);
			ImGui::Text("lower = larger radius");
			ImGui::Text("higher = smaller radius");
			ImGui::SliderInt("Number of Peaks:", &peaks_to_count_, 1, 9);
			ImGui::SliderInt("Octaves: ", &octaves_, 1, 8);
			if (ImGui::Button("Change alpha"))
			{
				i_p_p->ChangeAlpha(n_height_map, v_d_p->GetGridSize(), alpha_);
			}
			if (ImGui::Button("Create Noise Image"))
			{
				t_t_p->ClearStructs(v_d_p, voronoi_d, n_height_map, height_map, i_p_p,track_type_,resolution_,sites_,points_);
				i_p_p->DrawNoise(n_height_map, v_d_p->GetGridSize(), layers_);
			}

			if (ImGui::Button("Create FBM Image"))
			{
				t_t_p->ClearStructs(v_d_p, voronoi_d, n_height_map, height_map, i_p_p, track_type_, resolution_, sites_, points_);
				i_p_p->DrawFBM(n_height_map, v_d_p->GetGridSize(), octaves_);
			}

		}
		ImGui::Text("\n");
		if (ImGui::Button("Renerate (Noise Method)"))
		{
			v_d_p->vector_all(peaks_to_count_);
			for (int i = 0; i < peaks_to_count_; i++)
			{
				v_d_p->FindMax(v_d_p->GetGridSize(), layers_,i_p_p->GetNoiseMap());
				v_d_p->HighPointFunc(v_d_p->GetGridSize(), radius_cutoff, layers_,i, i_p_p->GetNoiseMap());
			}
			do
			{
				if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
				{
					t_t_p->ResetVars(v_d_p, s_p_p, voronoi_d, height_map, n_height_map);
				}
				t_t_p->GenerateTerrainMethod(v_d_p, height_map, i_p_p, number_, track_type_);
				t_t_p->CreateTrack(v_d_p, s_p_p);
			} while (v_d_p->GetFailed() || s_p_p->GetFailed());
			
		}
		if (ImGui::Button("Regenerate"))
		{
			t_t_p->ClearStructs(v_d_p, voronoi_d, n_height_map, height_map, i_p_p, track_type_, resolution_, sites_, points_);
			t_t_p->Generate(v_d_p, s_p_p, voronoi_d, height_map, n_height_map,i_p_p,times_,displacement_,number_,full_random_,track_type_);
		}
		if (ImGui::Button("Create Final Heightmap"))
		{
			i_p_p->CreateFinalHM(v_d_p->GetGridSize(), final_map, layers_);
		}
	
		if (ImGui::Button("Write to file"))
		{
			final_map.resize(v_d_p->GetGridSize() * v_d_p->GetGridSize());
			i_p_p->CreateFinalHM(v_d_p->GetGridSize(), final_map, layers_);
			i_p_p->WriteToFile(v_d_p->GetGridSize(), voronoi_d, layers_);
			s_p_p->WriteToFile(v_d_p->GetTrackMax(),v_d_p->GetTrackMin());
		}
		if (ImGui::Button("Clear Console"))																	//https://stackoverflow.com/questions/5866529/how-do-we-clear-the-console-in-assembly/5866648#5866648
		{
			char fill = ' ';
			COORD tl = { 0,0 };
			CONSOLE_SCREEN_BUFFER_INFO s;
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(console, &s);
			DWORD written, cells = s.dwSize.X * s.dwSize.Y;
			FillConsoleOutputCharacter(console, fill, cells, tl, &written);
			FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
			SetConsoleCursorPosition(console, tl);
		}
		ImGui::Text("\n");
		ImGui::Text("\n");

		if (ImGui::CollapsingHeader("Keyboard Controls"))
		{
			ImGui::TextWrapped("Press A to display diagram");
			ImGui::TextWrapped("Press S to display distance map");
			ImGui::TextWrapped("Press D to hide distance map");
			ImGui::TextWrapped("Press E to display hieghtmap");
			ImGui::TextWrapped("Press R to hide heightmap");
			ImGui::TextWrapped("Press Z to final hieghtmap");
			ImGui::TextWrapped("Press X to final heightmap");
			ImGui::TextWrapped("Press F to display track");
		}
		if (ImGui::CollapsingHeader("How-to/Guide"))
		{
			ImGui::TextWrapped("Set the factors before generation and then select 'regenerate'");
			ImGui::TextWrapped("If want to use displacement, select resolution as 513 and sites as either 25 or 81 otherwise will crash.");
			ImGui::TextWrapped("To use noise method select factors like before and generate noise image first, selecting and changing alpha too. Then select 'generate (noise method)'");
			ImGui::TextWrapped("Please Ignore 'Testing Options'");
		}
		if (ImGui::CollapsingHeader("Curves"))
		{
			ImGui::SliderFloat("Scale", &image_scale, 0, 1);
			if (ImGui::Button("Downscale"))
			{
				i_p_p->ResizeImage(v_d_p->GetGridSize(),image_scale);								//need to first regenerate image, save image, then this button and it saves to output. "test.png"
			}
			if (ImGui::Button("Upscale Image and Save"))			//saves image
			{
				i_p_p->SaveUpScaledImage(v_d_p->GetGridSize(), voronoi_d,image_scale);				//need to regenerate image first, then this button and it saves to output. "test.jpg"
			}
			if (ImGui::Button("Upscale Grid"))			//displays image
			{
				v_d_p->UpScaleGrid(v_d_p->GetGridSize(), image_scale);								//scales the "grid"/2darray structure - doing this means that the rest of the functions can be used. 
				i_p_p->UpScaleVertexArray(v_d_p->GetGridSize(), image_scale, height_map);			//can scale a vertex array
				resolution_ = resolution_ * image_scale;
				v_d_p->SetGridSize(resolution_);
				voronoi_d.resize(resolution_* resolution_);
			}
			if (ImGui::Button("Test (scales final (if made))"))																//scales the "grid"/2darray structure - doing this means that the rest of the functions can be used. 
			{
				i_p_p->UpScaleVertexArray(v_d_p->GetGridSize(), image_scale, final_map);			//can scale a vertex array
				resolution_ = resolution_ * image_scale;
				v_d_p->SetGridSize(resolution_);
				voronoi_d.resize(resolution_* resolution_);
			}		
			if (ImGui::Button("Scale Control-Points"))											//need to regenerate first, then upscale array, then this button and will display curve, need to select scale factor, and preform testing
			{
				s_p_p->OrderControlPoints();
				s_p_p->ScaleControlPoints(image_scale);
			}
			if (ImGui::Button("DeCastelJau"))
			{
				s_p_p->OrderControlPoints();
				d_c_j->CreateCurve(s_p_p->GetControlPoints(), v_d_p->GetGridSize(), voronoi_d);				//draws curve
			}
			if (ImGui::Button("CatmullRom"))
			{
				s_p_p->OrderControlPoints();
				bool looped = false;
				if (track_type_==2)
				{
					looped = true;
				}
				c_r_s->CreateCurve(s_p_p->GetControlPoints(),v_d_p->GetGridSize(), voronoi_d, looped);
			}
			if (ImGui::Button("Draw Control Points"))
			{
				s_p_p->OrderControlPoints();
				c_r_s->DrawControlPoints(s_p_p->GetControlPoints(), v_d_p->GetGridSize(), voronoi_d);
			}
			if (ImGui::CollapsingHeader("ControlPoints"))
			{
				auto temp_ = s_p_p->GetControlPoints();
				int x = temp_[0].first, y= temp_[0].second;
				//ImGui::InputInt("x: ",  &x, 1, 2,0);
				ImGui::SliderInt("x: ", &x, 0, 1);
				ImGui::SliderInt("y: ", &y, 0, 1);
				c_r_s->FixControlPoints(temp_, 0, std::pair<int, int>(x, y));
				s_p_p->SetControlPoints(temp_);
			}
			ImGui::SliderFloat("Definition of Curve:", &step_curve, 0, 1);
			d_c_j->SetStepSize(step_curve);
			c_r_s->SetStepSize(step_curve);
		}
		
		if (ImGui::CollapsingHeader("Measurements"))
		{
			ImGui::Text("Total Length= %d", s_p_p->GetTotalDistance());
			ImGui::Text("Number of Turns = %d", s_p_p->GetNumberOfTurns());
			ImGui::Text("Number of Segments = %d", s_p_p->GetNumberOfSegments());
		}

		ImGui::Checkbox("Perform Testing?", &do_testing_);
		s_p_p->SetTesting(do_testing_);
		v_d_p->SetTesting(do_testing_);

		ImGui::End();
		//used to display the whole voronoi diagram
		input_manager.HandleInput(v_d_p, voronoi_d, render_height_map_, n_render_height_map_, f_render_height_map_,i_p_p);
		input_manager.Zoom();
		//render
		window.clear();
		if (render_height_map_)
		{
			window.draw(height_map);
		}
		if (n_render_height_map_)
		{
			window.draw(n_height_map);
		}
		if (f_render_height_map_)
		{
			window.draw(final_map);
		}
		window.draw(voronoi_d);
		
		window.draw(title_name_);
		ImGui::SFML::Render(window);

		window.setView(window.getDefaultView());
		window.display();
		//
	}
	
	delete v_d_p;
	delete s_p_p;
	delete d_c_j;
	delete i_p_p;
	delete t_t_p;
	delete c_r_s;
	ImGui::SFML::Shutdown();
	return 0;
}