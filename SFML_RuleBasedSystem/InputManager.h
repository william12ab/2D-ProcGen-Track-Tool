#pragma once
#include "Input.h"
#include "VoronoiDiagram.h"
#include "ImageProcessing.h"
#include "ShortestPath.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics.hpp>

class InputManager
{
public:
	InputManager(Input*in, sf::View* view, sf::RenderWindow* hwnd );
	~InputManager();

	void HandleInput(bool &r_h_m,bool &r_n_h_m,bool &r_f_h_m, ImageProcessing &i_p_p, ShortestPath& s_p, bool &r_t, bool &r_d);

	void Zoom();
private:
	Input* input;
	sf::View* view_;
	sf::RenderWindow* window;

};

