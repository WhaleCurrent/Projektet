#include "Game.h"

Game::Game()
{
	m_dt = 0.0f;
}

Game::~Game()
{


}

void Game::newDT(float dt)
{
	m_dt = dt;
}