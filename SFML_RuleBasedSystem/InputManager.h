#pragma once
#include "Input.h"
#include "VoronoiDiagram.h"
#include "ImageProcessing.h"
#include <SFML/Graphics/VertexArray.hpp>

class InputManager
{
public:
	InputManager(Input*in);
	~InputManager();

	void HandleInput(VoronoiDiagram* v_d_p, sf::VertexArray& vertexarray,bool &r_h_m,bool &r_n_h_m,bool &r_f_h_m, ImageProcessing* i_p_p);
private:
	Input* input;
};

