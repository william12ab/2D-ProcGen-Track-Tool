// SFML includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <list>
#include "MainHeader.h"
#include <chrono>
#include <random>
#include "Input.h"
#include "RenderHandler.h"

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

void ClearConsoleWin() {
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

void ClearMeasurements() {
	measurements_.control_points_.clear();
	measurements_.vec_angles.clear();
	measurements_.vec_new_angles.clear();
	measurements_.vec_number_of_turns.clear();
	measurements_.vec_segment_lengths.clear();
	measurements_.vec_track_points.clear();
	measurements_.vec_new_track_points.clear();
	measurements_.vec_curve_points.clear();
}
void AddMeasurements(ShortestPath& s_p) {
	measurements_.control_points_.push_back(s_p.GetControlPoints());
	measurements_.vec_new_angles.push_back(s_p.GetAngles());
	measurements_.vec_number_of_turns.push_back(s_p.GetNumberOfTurns());
	measurements_.vec_segment_lengths.push_back(s_p.GetLengths());
	measurements_.vec_track_points.push_back(s_p.GetTrackPoints());
}
void WidthDecider(WidthCalculator& w_c, TrackTools& t_t, ShortestPath& s_p, VoronoiDiagram& v_d, ImageProcessing& i_p, CatmullRomSpline& c_r, 
	std::vector<sf::VertexArray*>voronoi_diagrams,std::vector<sf::Vector2i> track_points) {

	if (!is_chunking_) {
		t_t.WidthSettings(w_c, s_p, v_d, i_p, *voronoi_diagrams[0], layers_, track_points, s_p.GetControlPoints(), 0);
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (is_curved_){
				t_t.WidthSettings(w_c, s_p, v_d, i_p, *voronoi_diagrams[i], layers_, measurements_.vec_curve_points[i], measurements_.control_points_[i], i);
			}
			else {
				t_t.WidthSettings(w_c, s_p, v_d, i_p, *voronoi_diagrams[i], layers_, measurements_.vec_track_points[i], measurements_.control_points_[i], i);
			}
			measurements_.vec_new_track_points.push_back(w_c.GetNewTrack());
		}
	}
}
void Init(sf::RenderWindow& window) {
	font.loadFromFile("DefaultAriel.ttf");
	//text setting
	SettingText();
	ImGui::SFML::Init(window);

	//defaults: 
	resolution_ = 400;
	sites_ = 25;
	points_ = 2;
	regen_ = false;
	track_type_ = 1;  //1=p2p,0=loop
	render_height_map_ = false;
	n_render_height_map_ = false;
	is_render_track = false;
	number_ = 35;
	div_ = 2.0f;
	height_ = 1.0f;
	frequency_ = 0.010f;
	alpha_ = 255;
	layers_ = 1;
	octaves_ = 1;
	full_random_ = true;
	times_ = 1;
	displacement_ = 100;
	radius_cutoff = 115;
	peaks_to_count_ = 1;
	do_testing_ = true;
	step_curve = 0.01f;
	alpha_cm_ = 0.5f;
	reso_ = 1;
	is_curved_ = false;
	is_widthed_ = false;
	is_chunking_ = false;
	struct_obj_render.render_diagram = false;
	struct_obj_render.render_track = false;
	noise_seed = 0;
	points_min = 2;
	is_points_changed = false;
	is_written = false;
}

int main() {
	Input input;
	sf::Clock clock;
	sf::Clock deltaClock;
	// Seed the random number generator
	srand(static_cast <unsigned> (time(0)));
	// Create the window and UI bar on the right
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "2D Track Generator", sf::Style::Default);
	Init(window);
	sf::View view_;
	view_.reset(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
	//objs for main

	VoronoiDiagram v_d;
	ShortestPath s_p;
	DeCastelJau d_c;
	ImageProcessing i_p;
	TrackTools t_t;
	CatmullRomSpline c_r;
	WidthCalculator w_c;
	RenderHandler r_w;
	i_p.SetIsChunking(false);
	InputManager input_manager(&input, &view_, &window);
	//
	//
	t_t.SetVars(v_d, i_p, track_type_, resolution_, sites_, points_);
	//
	//sets up the vertex array and the data structures for voronoi diagram
	sf::VertexArray voronoi_d(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray voronoi_d_1(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray voronoi_d_2(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray voronoi_d_3(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray height_map(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray height_map_1(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray height_map_2(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray height_map_3(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray n_height_map_chunk_0(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray n_height_map_chunk_1(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray n_height_map_chunk_2(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray n_height_map_chunk_3(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray final_map(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray final_map1(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray final_map2(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	sf::VertexArray final_map3(sf::Points, (v_d.GetGridSize() * v_d.GetGridSize()));
	std::vector<sf::VertexArray*>final_maps;
	std::vector<sf::VertexArray*>noise_maps;
	std::vector<sf::VertexArray*>voronoi_diagrams;
	std::vector<sf::VertexArray*>distance_maps;
	noise_maps.push_back(&n_height_map_chunk_0);
	noise_maps.push_back(&n_height_map_chunk_1);
	noise_maps.push_back(&n_height_map_chunk_2);
	noise_maps.push_back(&n_height_map_chunk_3);
	//
	voronoi_diagrams.push_back(&voronoi_d);
	voronoi_diagrams.push_back(&voronoi_d_1);
	voronoi_diagrams.push_back(&voronoi_d_2);
	voronoi_diagrams.push_back(&voronoi_d_3);
	//
	distance_maps.push_back(&height_map);
	distance_maps.push_back(&height_map_1);
	distance_maps.push_back(&height_map_2);
	distance_maps.push_back(&height_map_3);
	//
	final_maps.push_back(&final_map);
	final_maps.push_back(&final_map1);
	final_maps.push_back(&final_map2);
	final_maps.push_back(&final_map3);
	//creates a track initially 

	t_t.Generate(v_d, s_p, *voronoi_diagrams[0], *distance_maps[0], *noise_maps[0], i_p, times_, displacement_, number_, full_random_, track_type_);

	//
	// While the window is open, update	

	while (window.isOpen()) {
		// Parse events
		sf::Event sf_event;
		while (window.pollEvent(sf_event)) {
			// Close the window when the close button is pressed
			ImGui::SFML::ProcessEvent(sf_event);
			switch (sf_event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(view_);
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
				if (sf_event.mouseButton.button == sf::Mouse::Left) {
					//update input class
					input.setMouseLDown(true);
				}
				else if (sf_event.mouseButton.button == sf::Mouse::Right) {
					input.setMouseRDown(true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (sf_event.mouseButton.button == sf::Mouse::Left) {
					//update input class
					input.setMouseLDown(false);
				}
				else if (sf_event.mouseButton.button == sf::Mouse::Right) {
					input.setMouseRDown(false);
				}
				break;
			}
		}
		window.setView(view_);
		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Options");
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Track Variables")) {

			ImGui::SliderInt("Resolution", &reso_, 1, 2);
			if (reso_==1){
				resolution_ = 400;
				points_min = 2;
				is_points_changed = false;
			}
			else {
				resolution_ = 800;
				points_min = 4;
				if (!is_points_changed){
					points_= 4;
					is_points_changed = true;
				}
			}
			ImGui::SliderInt("Sites", &sites_, 5, 100);
			ImGui::SliderInt("Iterations of Displacement", &times_, 1, 32);
			ImGui::SliderInt("Displacement at Each Displacement", &displacement_, 1, 200);
			ImGui::Checkbox("Random or Equal+Random?", &full_random_);
			ImGui::SliderInt("Points", &points_, points_min, 5);
			ImGui::SliderInt("Track Type", &track_type_, 0, 2);
			ImGui::Text("0 = triangular\n1 = point to point\n2 = loop");
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Heightmap Variables")) {
			ImGui::Checkbox("chunking?", &is_chunking_);
			i_p.SetIsChunking(is_chunking_);
			ImGui::SliderInt("Size of cell outlines", &number_, 0, 100);
			ImGui::Text("lower = large outlines");
			ImGui::Text("higher = less effect");
			ImGui::SliderInt("Number of Layers", &layers_, 1, 10);
			ImGui::SliderInt("Alpha", &alpha_, 0, 255);
			ImGui::SliderInt("Radius Cut-off:", &radius_cutoff, 50, 255);
			ImGui::Text("lower = larger radius");
			ImGui::Text("higher = smaller radius");
			ImGui::SliderInt("Number of Peaks:", &peaks_to_count_, 1, 9);
			ImGui::SliderInt("Octaves: ", &octaves_, 1, 8);
			ImGui::SliderFloat("Frequency:,", &frequency_, 0.0, 1.0f);
			if (ImGui::Button("Generate Seed")){
				SimplexNoise* temp = new SimplexNoise;
				noise_seed = temp->ReturnSeed();
				delete temp;
			}
			ImGui::Text("Seed: %i",noise_seed);
			if (ImGui::Button("Change alpha")) {
				if (!i_p.GetIsChunking()) {
					i_p.ChangeAlpha(*noise_maps[0], v_d.GetGridSize(), alpha_);
				}
				else {
					for (int i = 0; i < 4; i++) {
						i_p.ChangeAlpha(*noise_maps[i], v_d.GetGridSize(), alpha_);
					}
				}
			}
			if (ImGui::Button("Create Noise Image")) {
				is_written = false;
				if (!i_p.GetIsChunking()) {
					t_t.ClearStructs(v_d, *voronoi_diagrams[0], *noise_maps[0], *distance_maps[0], i_p, track_type_, resolution_, sites_, points_);
					i_p.DrawNoise(*noise_maps[0], v_d.GetGridSize(), layers_, frequency_, 0, noise_seed);
				}
				else {
					t_t.ClearStructs(v_d, *voronoi_diagrams[0], *noise_maps[0], *distance_maps[0], i_p, track_type_, resolution_, sites_, points_);
					for (int i = 1; i < 4; i++) {
						noise_maps[i]->clear();
						noise_maps[i]->resize(resolution_ * resolution_);
					}
					for (int i = 0; i < 4; i++) {
						i_p.DrawNoise(*noise_maps[i], v_d.GetGridSize(), layers_, frequency_, i, noise_seed);
					}
				}
			}
			if (ImGui::Button("Create FBM Image")) {
				t_t.ClearStructs(v_d, *voronoi_diagrams[0], *noise_maps[0], *distance_maps[0], i_p, track_type_, resolution_, sites_, points_);
				i_p.DrawFBM(*noise_maps[0], v_d.GetGridSize(), octaves_, frequency_);
			}
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Curve Variables")) {
			ImGui::SliderFloat("Definition of Curve:", &step_curve, 0, 1);
			d_c.SetStepSize(step_curve);
			ImGui::Text("Keep alpha at 0 or 0.5");
			ImGui::SliderFloat("Aplha for CatmullRom:", &alpha_cm_, 0, 1);
			c_r.SetStepSize(alpha_cm_);
			ImGui::Text("\n");
			if (ImGui::CollapsingHeader("Change ControlPoints")) {
				auto temp_ = s_p.GetControlPoints();
				auto size_ = s_p.GetControlPoints().size();
				std::vector<int> x_, y_;
				for (int i = 0; i < size_; i++) {
					x_.push_back(temp_[i].x);
					y_.push_back(temp_[i].y);
				}
				for (int i = 0; i < size_; i++) {
					int x_y[2] = { x_[i],y_[i] };
					std::string s_x = "x: " + std::to_string(i) + " y: " + std::to_string(i);
					const char* l_x = s_x.c_str();
					ImGui::SliderInt2(l_x, x_y, 0, int(resolution_));
					c_r.FixControlPoints(temp_, i, sf::Vector2i(x_y[0], x_y[1]));
					s_p.SetControlPoints(temp_);
				}
			}
			ImGui::Text("\n");
			if (ImGui::Button("DeCastelJau")) {
				s_p.OrderControlPoints();
				d_c.CreateCurve(s_p.GetControlPoints(), v_d.GetGridSize(), *voronoi_diagrams[0]);				//draws curve
			}
			if (ImGui::Button("CatmullRom")) {
				s_p.OrderControlPoints();
				bool looped = false;
				if (track_type_ == 2) {
					looped = true;
				}
				c_r.CreateCurve(s_p.GetControlPoints(), v_d.GetGridSize(), *voronoi_diagrams[0], looped);
			}
			if (ImGui::Button("Centripetal CatmullRom")) {
				if (!i_p.GetIsChunking()) {
					i_p.CreateImage(*voronoi_diagrams[0], v_d.GetGridSize());
					bool looped = false;
					if (track_type_ == 2) {
						looped = true;
					}
					auto temp_cp = s_p.GetControlPoints();
					c_r.CreateCurve(v_d.GetGridSize(), *voronoi_diagrams[0], s_p.GetControlPoints(), looped);
					c_r.RemoveDuplicates();
					i_p.DrawWidthTrack(*voronoi_diagrams[0], v_d.GetGridSize(), c_r.GetCurve(), 0);
					s_p.SetControlPoints(temp_cp);
					is_curved_ = true;
				}
				else {
					for (int i = 0; i < 4; i++) {
						i_p.CreateImage(*voronoi_diagrams[i], v_d.GetGridSize());
						bool looped = false;
						if (track_type_ == 2) {
							looped = true;
						}
						c_r.CreateCurve(v_d.GetGridSize(), *voronoi_diagrams[i], measurements_.control_points_[i], looped);
						c_r.RemoveDuplicates();
						i_p.DrawWidthTrack(*voronoi_diagrams[i], v_d.GetGridSize(), c_r.GetCurve(), i);
						is_curved_ = true;
						measurements_.vec_curve_points.push_back(c_r.GetCurve());
					}
				}
			}
			if (ImGui::Button("Draw Control Points")) {
				s_p.OrderControlPoints();
				c_r.DrawControlPoints(s_p.GetControlPoints(), v_d.GetGridSize(), *voronoi_diagrams[0]);
			}
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Scaling Variables"))
		{
			ImGui::SliderFloat("Scale", &image_scale, 1, 2);
			if (ImGui::Button("Downscale image"))
			{
				i_p.ResizeImage(v_d.GetGridSize(), image_scale);								//need to first regenerate image, save image, then this button and it saves to output. "test.png"
			}
			if (ImGui::Button("Upscale Image and Save"))			//saves image
			{
				i_p.SaveUpScaledImage(v_d.GetGridSize(), *voronoi_diagrams[0], image_scale);				//need to regenerate image first, then this button and it saves to output. "test.jpg"
			}
			if (ImGui::Button("Upscale Grid"))			//displays image
			{
				v_d.UpScaleGrid(v_d.GetGridSize(), image_scale);								//scales the "grid"/2darray structure - doing this means that the rest of the functions can be used. 
				i_p.UpScaleVertexArray(v_d.GetGridSize(), image_scale, *distance_maps[0]);			//can scale a vertex array
				resolution_ = resolution_ * image_scale;
				v_d.SetGridSize(resolution_);
				voronoi_diagrams[0]->resize(resolution_ * resolution_);//might be broken now
			}
			if (ImGui::Button("Test (scales final (if made))"))																//scales the "grid"/2darray structure - doing this means that the rest of the functions can be used. 
			{
				i_p.UpScaleVertexArray(v_d.GetGridSize(), image_scale, final_map);			//can scale a vertex array
				resolution_ = resolution_ * image_scale;
				v_d.SetGridSize(resolution_);
				voronoi_diagrams[0]->resize(resolution_ * resolution_);//might be broken now
			}
			if (ImGui::Button("Scale Control-Points"))											//need to regenerate first, then upscale array, then this button and will display curve, need to select scale factor, and preform testing
			{
				s_p.OrderControlPoints();
				s_p.ScaleControlPoints(image_scale);
			}
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Generate Options")) {
			if (ImGui::Button("Renerate (Noise Method)")) {
				ClearConsoleWin();
				is_written = false;
				t_t.SetChunk(i_p.GetIsChunking());
				if (resolution_==400) {
					is_curved_ = false; is_widthed_ = false;
					t_t.HeightLoop(0, v_d, peaks_to_count_, layers_, i_p, radius_cutoff, number_, track_type_, s_p, *distance_maps[0]);
				}
				else {
					
					t_t.HeightLoop(0, v_d, peaks_to_count_, layers_, i_p, radius_cutoff, number_, track_type_, s_p, *distance_maps[0]);
				}
			}
			if (ImGui::Button("Regenerate")) {
				the_clock::time_point startTime = the_clock::now();
				is_written = false;
				is_curved_ = false;
				is_widthed_ = false;
				is_chunking_ = false;
				i_p.SetIsChunking(false);
				t_t.SetChunk(i_p.GetIsChunking());
				v_d.SetIsChunking(i_p.GetIsChunking());
				ClearConsoleWin();
				t_t.ClearStructs(v_d, *voronoi_diagrams[0], *noise_maps[0], *distance_maps[0], i_p, track_type_, resolution_, sites_, points_);
				t_t.Generate(v_d, s_p, *voronoi_diagrams[0], *distance_maps[0], *noise_maps[0], i_p, times_, displacement_, number_, full_random_, track_type_);
				the_clock::time_point endTime = the_clock::now();
				auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
				std::cout << "time(phase 1): " << time_taken; std::cout << std::endl;
			}
			if (ImGui::Button("Create Final Heightmap")) {
				i_p.CreateFinalHM(v_d.GetGridSize(), *final_maps[0], *final_maps[1], *final_maps[2], *final_maps[3], layers_);
			}
			if (ImGui::Button("Write to file")) {
				final_maps[0]->resize(v_d.GetGridSize() * v_d.GetGridSize());
				i_p.CreateFinalHM(v_d.GetGridSize(), *final_maps[0], *final_maps[1], *final_maps[2], *final_maps[3], layers_);
				i_p.WriteToFile(v_d.GetGridSize(), *voronoi_diagrams[0], *voronoi_diagrams[1], *voronoi_diagrams[2], *voronoi_diagrams[3], layers_);
				auto temp_controlpoints = s_p.GetControlPoints();
				auto pointpos = v_d.GetPointPos();
				if (v_d.GetNumberOfPoints()>=3){
					int index_point = 1;
					for (size_t i = 0; i < temp_controlpoints.size(); i++){
						if (index_point<(v_d.GetNumberOfPoints()-1)){
							if (temp_controlpoints[i] == v_d.GetPointPos()[index_point]) {
								auto it = temp_controlpoints.begin() + i;
								temp_controlpoints.erase(it);
								it = temp_controlpoints.begin() + i;
								temp_controlpoints.erase(it);
								index_point++;
							}
						}
					}
				}
				temp_controlpoints.emplace(temp_controlpoints.end(), pointpos[v_d.GetNumberOfPoints() - 1]);//above removes bad controol points
				s_p.SetControlPoints(temp_controlpoints);
				s_p.FixLengthsAndLinePos();
				s_p.WriteToFile();
				i_p.WriteMetaFile(resolution_,v_d.GetPointPos(),s_p.GetTotalDistance(),s_p.GetNumberOfTurns());
				t_t.WritePacenoteInfo(s_p, w_c, is_widthed_);
			}
			if (ImGui::Button("Write Track Points")) {
				if (resolution_==400) {
					s_p.WriteTrackPoints(w_c.GetNewTrack(), is_curved_, is_widthed_, 0, s_p.GetControlPoints(), s_p.GetTrackPoints());
				}
				else {
					measurements_.vec_track_points.resize(4);
					measurements_.control_points_.resize(4);
					measurements_.vec_new_track_points.resize(4);
					auto temp_points = s_p.GetTrackPoints();
					if (is_widthed_){
						temp_points = w_c.GetNewTrack();
					}
					for (size_t i = 0; i < temp_points.size(); i++){
						auto x = temp_points[i].x;
						auto y = temp_points[i].y;
						if (x < 400 && y < 400) {
							measurements_.vec_track_points[0].push_back(temp_points[i]);
						}
						else if (x >= 400 && y < 400) {
							temp_points[i].x -= 400;
							measurements_.vec_track_points[1].push_back(temp_points[i]);
						}
						else if (x < 400 && y >= 400) {
							temp_points[i].y -= 400;
							measurements_.vec_track_points[2].push_back(temp_points[i]);
						}
						else if (x >= 400 && y >= 400) {
							temp_points[i].x -= 400;
							temp_points[i].y -= 400;
							measurements_.vec_track_points[3].push_back(temp_points[i]);
						}
					}
					
					if (is_widthed_) {
						for (size_t i = 0; i < 4; i++) {
							measurements_.vec_new_track_points[i] = measurements_.vec_track_points[i];
						}
					}
					auto c = s_p.GetControlPoints();
					for (size_t i = 0; i < s_p.GetControlPoints().size(); i++) {
						auto x = s_p.GetControlPoints()[i].x;
						auto y = s_p.GetControlPoints()[i].y;
						if (x < 400 && y < 400) {
							measurements_.control_points_[0].push_back(s_p.GetControlPoints()[i]);
						}
						else if (x >= 400 && y < 400) {
							auto point = s_p.GetControlPoints()[i];
							point.x -= 400;
							measurements_.control_points_[1].push_back(point);
						}
						else if (x < 400 && y >= 400) {
							auto point = s_p.GetControlPoints()[i];
							point.y -= 400;
							measurements_.control_points_[2].push_back(point);
						}
						else if (x >= 400 && y >= 400) {
							auto point = s_p.GetControlPoints()[i];
							point.x -= 400;
							point.y -= 400;
							measurements_.control_points_[3].push_back(point);
						}
					}
					for (int i = 0; i < 4; i++) {
						s_p.WriteTrackPoints(measurements_.vec_new_track_points[i], is_curved_, is_widthed_, i, measurements_.control_points_[i], measurements_.vec_track_points[i]);
					}
				}
			}
			if (ImGui::Button("Write Curve Points")) {
				s_p.WriteTrackPoints(c_r.GetCurve(), is_curved_, is_widthed_, 0, s_p.GetControlPoints(), s_p.GetTrackPoints());
			}
		}
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Width Options")) {
			if (ImGui::CollapsingHeader("Options")) {
				ImGui::Checkbox("Flat?", &w_c.GetBoolFlat());
				ImGui::Checkbox("Curved?", &w_c.GetBoolCurved());
				ImGui::Checkbox("Length?", &w_c.GetBoolLength());
				ImGui::Checkbox("T-Values?", &w_c.GetBoolTValues());
				ImGui::Checkbox("Angles?", &w_c.GetBoolAngles());
				ImGui::Checkbox("Inclination Between C.P(s)?", &w_c.GetBoolIncline());
				ImGui::Checkbox("Global?", &w_c.GetGlobal());
				ImGui::Checkbox("Random Modi?", &w_c.GetBoolRand());
				ImGui::Checkbox("Influenced T?", &w_c.GetBoolInfluenceT());
				int w = w_c.GetMaxWidth();
				ImGui::SliderInt("Max Width Value:", &w, 2, 10);
				w_c.SetMaxWidth(w);
			}
			if (ImGui::Button("Create width")) {
				is_widthed_ = true;
				//curved code
				if (w_c.GetBoolCurved()) {
					WidthDecider(w_c, t_t, s_p, v_d, i_p, c_r, voronoi_diagrams, c_r.GetCurve());
				}
				else {
					WidthDecider(w_c, t_t, s_p, v_d, i_p, c_r, voronoi_diagrams, s_p.GetTrackPoints());
				}
			}
		}
		ImGui::Text("\n");
		ImGui::Text("\n");
		ImGui::Text("\n");
		if (ImGui::CollapsingHeader("Keyboard Controls")){
			ImGui::TextWrapped("Press A to display diagram");
			ImGui::TextWrapped("Press S to display distance map");
			ImGui::TextWrapped("Press D to hide distance map");
			ImGui::TextWrapped("Press E to display hieghtmap");
			ImGui::TextWrapped("Press R to hide heightmap");
			ImGui::TextWrapped("Press Z to final hieghtmap");
			ImGui::TextWrapped("Press X to final heightmap");
			ImGui::TextWrapped("Press F to display track");
		}
		if (ImGui::CollapsingHeader("How-to/Guide")){
			ImGui::TextWrapped("Set the factors before generation and then select 'regenerate'");
			ImGui::TextWrapped("If want to use displacement, select a square number of sites, eg: 25, 49, 81 etc...");
			ImGui::TextWrapped("To use noise method select factors like before and generate noise image first, selecting and changing alpha too. Then select 'generate (noise method)'");
			ImGui::TextWrapped("Please Ignore 'Testing Options'");
		}
		if (ImGui::CollapsingHeader("Measurements")){
			ImGui::Text("Total Length= %d", s_p.GetTotalDistance());
			ImGui::Text("Number of Turns = %d", s_p.GetNumberOfTurns());
			ImGui::Text("Number of Segments = %d", s_p.GetNumberOfSegments());
		}
		if (ImGui::Button("Clear Console"))		//https://stackoverflow.com/questions/5866529/how-do-we-clear-the-console-in-assembly/5866648#5866648
		{
			ClearConsoleWin();
		}
		ImGui::End();

		//used to display the whole voronoi diagram
		input_manager.HandleInput(render_height_map_, n_render_height_map_, f_render_height_map_, i_p, s_p, is_render_track, is_render_diagram);
		input_manager.Zoom();

		//render

		if (!is_written){
			window.clear();
			r_w.RenderLoop(is_render_diagram, t_t, v_d, i_p, struct_obj_render.render_diagram, struct_obj_render.render_track, is_render_track, voronoi_diagrams,
				render_height_map_, n_render_height_map_, f_render_height_map_, distance_maps, noise_maps, final_maps, window);
		}
		window.draw(title_name_);
		ImGui::SFML::Render(window);
		window.setView(window.getDefaultView());
		window.display();
	}
	ImGui::SFML::Shutdown();
	return 0;
}