#pragma once
#include "Input.h"
#include "VoronoiDiagram.h"
#include "ImageProcessing.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics.hpp>

class InputManager
{
public:
	InputManager(Input*in, sf::View* view, sf::RenderWindow* hwnd );
	~InputManager();

	void HandleInput(VoronoiDiagram &v_d_p, sf::VertexArray& vertexarray,bool &r_h_m,bool &r_n_h_m,bool &r_f_h_m, ImageProcessing &i_p_p);

	void Zoom();
private:
	Input* input;
	sf::View* view_;
	sf::RenderWindow* window;

};

