#include "InputManager.h"
#include <SFML/Window/Keyboard.hpp>

InputManager::InputManager(Input*in) 
{
	input = in;
}

InputManager::~InputManager() 
{

}

void InputManager::HandleInput(VoronoiDiagram* v_d_p,sf::VertexArray& vertexarray, bool &r_h_m, bool &r_n_h_m, bool &r_f_h_m, ImageProcessing* i_p_p)
{
	if (input->isKeyDown(sf::Keyboard::Keyboard::A))
	{
		i_p_p->DrawFullVoronoiDiagram(vertexarray, v_d_p->GetGridSize(),v_d_p->GetGrid());
		input->setKeyUp(sf::Keyboard::A);
	}
	if (input->isKeyDown(sf::Keyboard::S))
	{
		r_h_m = true;
	}
	if (input->isKeyDown(sf::Keyboard::D))
	{
		r_h_m = false;
	}
	if (input->isKeyDown(sf::Keyboard::E))
	{
		r_n_h_m = true;
	}
	if (input->isKeyDown(sf::Keyboard::R))
	{
		r_n_h_m = false;
	}
	if (input->isKeyDown(sf::Keyboard::F))
	{
		i_p_p->DrawTrack(vertexarray, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites(),v_d_p->GetGrid());
		input->setKeyUp(sf::Keyboard::F);

	}
	if (input->isKeyDown(sf::Keyboard::Z))
	{
		r_f_h_m = true;
	}
	if (input->isKeyDown(sf::Keyboard::X))
	{
		r_f_h_m = false;
	}
}


//
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//{
//	v_d_p->DrawFullVoronoiDiagram(voronoi_d, v_d_p->GetGridSize());
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//{
//	render_height_map_ = true;
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//{
//	render_height_map_ = false;
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
//{
//	n_render_height_map_ = true;
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
//{
//	n_render_height_map_ = false;
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
//{
//	v_d_p->DrawVoronoiDiagram(voronoi_d, v_d_p->GetGridSize(), v_d_p->GetNumberOfSites());
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
//{
//	f_render_height_map_ = true;
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
//{
//	f_render_height_map_ = false;
//}