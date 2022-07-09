#include "Game.h"
#include "Platform/Platform.hpp"

Game::Game(const std::string & config)
{
	init(config);
}

void Game::init(const std::string & path)
{
	util::Platform platform;
	unsigned int wWidth = 640;
    unsigned int wHeight = 480;
	// TODO: 	read in config file here
	//			use the premade playerconfig, enemyconfig and bulletconfig variables

	std::ifstream fin(path);

	// TODO: iterate through the config

	// TODO: window config, w, h, fullscreen?
	// TODO: enemy config

	// TODO: bulletconfig

	// PlayerConfig
	fin >> m_playerConfig.SR >> m_playerConfig.CR
		>> m_playerConfig.S >> m_playerConfig.FR
		>> m_playerConfig.FG >> m_playerConfig.FB
		>> m_playerConfig.OR >> m_playerConfig.OG
		>> m_playerConfig.OB >> m_playerConfig.OT
		>> m_playerConfig.V;

	// SR, CR, S, FR, FG, FB, OR, OG, OB, OT, V
	const sf::Vector2u win(wWidth, wHeight);
	m_window.create(sf::VideoMode(win), "Game 1");
	m_window.setFramerateLimit(m_frameLimit);

	// SFML Boilerplate stuff
	platform.setIcon(m_window.getSystemHandle());

	spawnPlayer();

}

void Game::run()
{
	// TODO: 	add pause functionality
	//			some systems should function while paused (rendering)
	//			some systems shouldn't (move, input)

	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		m_currentFrame++;

	}

}

void Game::setPaused(bool paused)
{

}