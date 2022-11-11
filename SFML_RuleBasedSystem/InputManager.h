#pragma once
#include "Input.h"
#include "VoronoiDiagram.h"
#include "ImageProcessing.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>

class InputManager
{
public:
	InputManager(Input*in, sf::View* view);
	~InputManager();

	void HandleInput(VoronoiDiagram* v_d_p, sf::VertexArray& vertexarray,bool &r_h_m,bool &r_n_h_m,bool &r_f_h_m, ImageProcessing* i_p_p);

	void Zoom(sf::View &view__);
private:
	Input* input;
	sf::View* view_;
};

