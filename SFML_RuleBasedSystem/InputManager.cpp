#include "InputManager.h"
#include <SFML/Window/Keyboard.hpp>

InputManager::InputManager(Input*in, sf::View* view, sf::RenderWindow* hwnd)
{
	input = in;
	view_ = view;
	window = hwnd;
}

InputManager::~InputManager() 
{

}

void InputManager::Zoom()
{
	
	if (input->isKeyDown(sf::Keyboard::Dash))
	{
		int m_x= input->getMouseX();
		int m_y = input->getMouseY();
		view_->setCenter(sf::Vector2f(m_x, m_y));
		sf::Vector2f a= view_->getCenter();
		std::cout << a.x <<" "<< a.y; 
		view_->zoom(1.5f);
		input->setKeyUp(sf::Keyboard::Dash);
	}
	if (input->isKeyDown(sf::Keyboard::Equal))
	{
		view_->zoom(0.5f);
		int m_x = input->getMouseX();
		int m_y = input->getMouseY();
		view_->setCenter(sf::Vector2f(m_x, m_y));
		input->setKeyUp(sf::Keyboard::Equal);
	}
	if (input->isKeyDown(sf::Keyboard::Num0))
	{
		int m_x = input->getMouseX();
		int m_y = input->getMouseY();
		view_->setCenter(sf::Vector2f(m_x, m_y));
	}
	if (input->isKeyDown(sf::Keyboard::Num9))
	{
		view_->reset(sf::FloatRect(0.f, 0.f, (float)1000, (float)800));

	}
}

void InputManager::HandleInput(VoronoiDiagram &v_d_p,sf::VertexArray& vertexarray, bool &r_h_m, bool &r_n_h_m, bool &r_f_h_m, ImageProcessing& i_p_p, ShortestPath&s_p)
{
	if (input->isKeyDown(sf::Keyboard::Keyboard::A))
	{
		i_p_p.DrawFullVoronoiDiagram(vertexarray, v_d_p.GetGridSize(),v_d_p.GetGrid());
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
		i_p_p.DrawTrack(vertexarray, v_d_p.GetGridSize(),v_d_p.GetNumberOfSites(),v_d_p.GetGrid());
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

	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		window->close();
		input->setKeyUp(sf::Keyboard::Escape);
	}
	if (input->isKeyDown(sf::Keyboard::F1))
	{
		window->setSize(sf::Vector2u(1080, 800));
		view_->reset(sf::FloatRect(0.f, 0.f, (float)window->getSize().x, (float)window->getSize().y));
		input->setKeyUp(sf::Keyboard::F1);
	}
	if (input->isKeyDown(sf::Keyboard::F2))
	{
		window->create(sf::VideoMode::VideoMode(), "2D Track Generator,", sf::Style::Fullscreen);
		view_->reset(sf::FloatRect(0.f, 0.f, (float)window->getSize().x, (float)window->getSize().y));
		input->setKeyUp(sf::Keyboard::F2);
	}
	if (input->isKeyDown(sf::Keyboard::F3))
	{
		window->create(sf::VideoMode::getDesktopMode(), "2D Track Generator,", sf::Style::Default);
		view_->reset(sf::FloatRect(0.f, 0.f, (float)window->getSize().x, (float)window->getSize().y));
		input->setKeyUp(sf::Keyboard::F3);
	}
}
