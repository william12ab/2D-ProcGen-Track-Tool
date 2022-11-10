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

	//objs for main
	VoronoiDiagram* v_d_p = new VoronoiDiagram();
	ShortestPath* s_p_p = new ShortestPath();
	DeCastelJau* d_c_j = new DeCastelJau();
	ImageProcessing* i_p_p = new ImageProcessing();
	TrackTools* t_t_p = new TrackTools();
	InputManager input_manager(&input);
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
			ImGui::SliderFloat("Perlin Height", &height_, 0.0f, 1.0f);
			ImGui::SliderInt("Number of Layers of Noise", &layers_,0, 10);
			v_d_p->SetH(height_);
			ImGui::SliderInt("Alpha", &alpha_, 0, 255);
			ImGui::SliderInt("Radius Cut-off:", &radius_cutoff, 50, 255);
			ImGui::Text("lower = larger radius");
			ImGui::Text("higher = smaller radius");
			ImGui::SliderInt("Number of Peaks:", &peaks_to_count_, 1, 9);
			ImGui::SliderInt("Octaves: ", &octaves_, 1, 8);
			if (ImGui::Button("Change alpha"))
			{
				v_d_p->ChangeAlpha(n_height_map, v_d_p->GetGridSize(), alpha_);
			}
			if (ImGui::Button("Create Noise Image"))
			{
				t_t_p->ClearStructs(v_d_p, voronoi_d, n_height_map, height_map, i_p_p,track_type_,resolution_,sites_,points_);
				v_d_p->DrawNoise(n_height_map, v_d_p->GetGridSize(), layers_);
			}

			if (ImGui::Button("Create FBM Image"))
			{
				t_t_p->ClearStructs(v_d_p, voronoi_d, n_height_map, height_map, i_p_p, track_type_, resolution_, sites_, points_);
				v_d_p->DrawFBM(n_height_map, v_d_p->GetGridSize(), octaves_);
			}

		}
		ImGui::Text("\n");
		if (ImGui::Button("Renerate (Noise Method)"))
		{
			v_d_p->vector_all(peaks_to_count_);
			the_clock::time_point startTimea = the_clock::now();
			for (int i = 0; i < peaks_to_count_; i++)
			{
				v_d_p->FindMax(v_d_p->GetGridSize(), layers_);
				v_d_p->HighPointFunc(v_d_p->GetGridSize(), radius_cutoff, layers_,i);
			}
			the_clock::time_point endTimea = the_clock::now();

			auto time_takena = duration_cast<milliseconds>(endTimea - startTimea).count();
			std::cout << "time(HIGH_POINTS): " << time_takena; std::cout << std::endl;
			do
			{
				if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
				{
					t_t_p->ResetVars(v_d_p, s_p_p, voronoi_d, height_map, n_height_map);
				}
				

				v_d_p->TerrainSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());							//this takes no time
	
				v_d_p->DiagramAMP(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
				v_d_p->DrawVD(height_map, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites(), number_, div_);
				v_d_p->SetEdges(v_d_p->GetGridSize());
				v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_, v_d_p->GetFailed());
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
			v_d_p->CreateFinalHM(v_d_p->GetGridSize(), final_map, layers_);
		}
	
		if (ImGui::Button("Write to file"))
		{
			final_map.resize(v_d_p->GetGridSize() * v_d_p->GetGridSize());
			v_d_p->CreateFinalHM(v_d_p->GetGridSize(), final_map, layers_);
			v_d_p->WriteToFile(v_d_p->GetGridSize(), voronoi_d, layers_);
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
				v_d_p->ResizeImage(v_d_p->GetGridSize(),image_scale);								//need to first regenerate image, save image, then this button and it saves to output. "test.png"
			}
			if (ImGui::Button("Upscale Image"))			//saves image
			{
				v_d_p->UpScaleImagetwo(v_d_p->GetGridSize(), voronoi_d,image_scale);				//need to regenerate image first, then this button and it saves to output. "test.jpg"
			}
			if (ImGui::Button("Upscale Grid"))			//displays image
			{
				v_d_p->UpScaleGrid(v_d_p->GetGridSize(), image_scale);								//scales the "grid"/2darray structure - doing this means that the rest of the functions can be used. 
				v_d_p->UpScaleVertexArray(v_d_p->GetGridSize(), image_scale, height_map);			//can scale a vertex array
				resolution_ = resolution_ * image_scale;
				v_d_p->SetGridSize(resolution_);
				voronoi_d.resize(resolution_* resolution_);
			}
			if (ImGui::Button("Test"))																//scales the "grid"/2darray structure - doing this means that the rest of the functions can be used. 
			{
				v_d_p->UpScaleVertexArray(v_d_p->GetGridSize(), image_scale, final_map);			//can scale a vertex array
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
				d_c_j->CreateCurve(s_p_p->GetControlPoints(), v_d_p->GetGridSize(), v_d_p->GetGrid(), voronoi_d);				//draws curve
			}
			ImGui::SliderFloat("Definition of Curve:", &step_curve, 0, 1);
			d_c_j->SetStepSize(step_curve);
		}
		if (ImGui::CollapsingHeader("Testing Options (runs multi times)"))
		{
			if (ImGui::Button("25"))
			{
				points_ = 3;
				sites_ = 81;
				times_ = 1;
				full_random_ = false;
				displacement_ = 1;
				track_type_ = 2;
				for (int a = 0; a < 25; a++)
				{
					t_t_p->SetVars(v_d_p, i_p_p, track_type_, resolution_, sites_, points_);
					voronoi_d.clear();
					height_map.clear();
					voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
					height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));


					//places the sites
					do
					{
						if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
						{
							t_t_p->ResetVars(v_d_p, s_p_p, voronoi_d, height_map, n_height_map);
						}

						t_t_p->CreateVoronoi(v_d_p, height_map,i_p_p,times_,displacement_,number_,full_random_,track_type_);
						the_clock::time_point startTime = the_clock::now();
						t_t_p->CreateTrack(v_d_p, s_p_p);
						the_clock::time_point endTime = the_clock::now();
						auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
						std::cout << "		time(v d): " << time_taken; std::cout << std::endl;

					} while (v_d_p->GetFailed() || s_p_p->GetFailed());

					v_d_p->WriteToFile(v_d_p->GetGridSize(), voronoi_d, layers_);
					s_p_p->WriteToFile(v_d_p->GetTrackMax(), v_d_p->GetTrackMin());

				}
			}

			if (ImGui::Button("25 times * 4"))
			{
				resolution_ = 400;
				octaves_ = 1;
				points_ = 2;
				sites_ = 25;
				track_type_ = 1;
				radius_cutoff = 125;
				peaks_to_count_ = 3;
				full_random_ = true;
				for (int w = 0; w < 5; w++)
				{
					sites_ = 25;
					for (int i = 0; i < 5; i++)
					{
						for (int a = 0; a < 25; a++)
						{
							//1. reset vars
							voronoi_d.clear();
							t_t_p->SetVars(v_d_p, i_p_p, track_type_, resolution_, sites_, points_);
							voronoi_d.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
							v_d_p->SetGridSize(resolution_);
							v_d_p->ResetVars();

							//2. create noise
							height_map.clear();
							v_d_p->SetGridSize(resolution_);

							height_map.resize((v_d_p->GetGridSize() * v_d_p->GetGridSize()));
							v_d_p->DrawFBM(height_map, v_d_p->GetGridSize(), octaves_);

							//3. generate 
							the_clock::time_point startTimea = the_clock::now();
							for (int i = 0; i < peaks_to_count_; i++)
							{
								v_d_p->FindMax(v_d_p->GetGridSize(), layers_);
								if (i == 5)
								{
									int s = 2;

								}
								v_d_p->HighPointFunc(v_d_p->GetGridSize(), radius_cutoff, layers_, i);

							}
							the_clock::time_point endTimea = the_clock::now();

							auto time_takena = duration_cast<milliseconds>(endTimea - startTimea).count();
							std::cout << "time(HIGH_POINTS): " << time_takena; std::cout << std::endl;



							do
							{
								if (v_d_p->GetFailed() || s_p_p->GetFailed())		//clears the diagram and resets the fail condition
								{
									t_t_p->ResetVars(v_d_p, s_p_p, voronoi_d, height_map, n_height_map);
								}


								v_d_p->TerrainSites(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());							//this takes no time

								v_d_p->DiagramAMP(v_d_p->GetNumberOfSites(), v_d_p->GetGridSize());
								v_d_p->DrawVD(height_map, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites(), number_, div_);
								v_d_p->SetEdges(v_d_p->GetGridSize());
								v_d_p->SetPoint(v_d_p->GetGridSize(), v_d_p->GetNumberOfPoints(), track_type_, v_d_p->GetFailed());
								the_clock::time_point startTime = the_clock::now();
								t_t_p->CreateTrack(v_d_p, s_p_p);
								the_clock::time_point endTime = the_clock::now();
								auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
								std::cout << "		time(v d): " << time_taken; std::cout << std::endl;
								std::cout << std::endl;
								std::cout << std::endl;

							} while (v_d_p->GetFailed() || s_p_p->GetFailed());

							v_d_p->WriteToFile(v_d_p->GetGridSize(), voronoi_d, layers_);
							s_p_p->WriteToFile(v_d_p->GetTrackMax(), v_d_p->GetTrackMin());

						}
						sites_ += 25;
					}
					if (w == 0)
					{
						points_ = 3;
						//track_type_=0;
					}
					if (w == 1)
					{
						points_ = 5;
					}
					if (w == 2)
					{
						points_ = 3;
						track_type_ = 2;
					}
					if (w == 3)
					{
						points_ = 3;
						track_type_ = 0;
					}
				}


			}
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
		window.display();
		//
	}

	delete v_d_p;
	delete s_p_p;
	delete d_c_j;
	delete i_p_p;
	delete t_t_p;
	ImGui::SFML::Shutdown();
	return 0;
}