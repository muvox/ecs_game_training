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

		// if paused, dont do Certain stuff

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
	// TODO, set the game to pause
	m_paused = paused;
}

void Game::spawnPlayer()
{
	// TODO: Add all the properties from playerconfig to the player entity

	// Use the entitymanager to add entities
	// as this is a shared_pointer (pointer) the entities vector reflects the changes made
	auto entity = m_entities.addEntity("player");

	// Give the entity a transform component and spawn it to 200, 200 with velocity 1 and angle 0
	// use m_playerConfig.BLAH
	entity->cTransform = std::make_shared<CTransform>(sf::Vector2f(200.0f, 200.0f), sf::Vector2f(1.0f, 1.0f), 0.0f);

	// Shape component, radius 32, 8 sides, dark grey fill and red outline with thickness of 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10,10,10), sf::Color(255, 0, 0), 4.0f);

	// Input component so we can move the player
	entity->cInput = std::make_shared<CInput>();

	// Set the Game objects player the entity.
	// NOTE: 	Goes slightly against the EntityManager paradigm, but the player is used so often
	//			that this makes it easier.
	m_player = entity;

}

// spawn enemy to random location (within bounds of the game window)
void Game::spawnEnemy()
{

	// TODO:	make sure the enemy is spawned properly according to enemyConfig variables

	m_lastEnemySpawnTime = m_currentFrame;
}

// Spawn tiny enemies when big one dies
void Game::spawnSmallEnemies(ptr<Entity> e)
{
	// Read props from the original
	int points = e->cShape->circle.getPointCount();

	// When creating smaller enemies, read the parameters of the original
	// spawn as many small ones as the original had points
	// small ones the same color as "host", half size
	// reward double points than the big one
	for (int i = 0; i < points; i++)
	{
		auto smallEnemy = m_entities.addEntity("small");
		smallEnemy->cScore = std::make_shared<CScore>(e->cScore->score * 2);
	}

}

void Game::spawnBullet(ptr<Entity> entity, const sf::Vector2f & target)
{

	// TODO:	Implement the spawning of bullet, traveling towards target
	//			- scalar speed, formula in notes. Speed is the number of frames
	//				to move when fired
	//			s = hypotenuse, points from player to mousePosition


}

void Game::spawnSpecialWeapon(ptr<Entity> entity)
{

}

void Game::sMovement()
{
	// TODO:	Implement all the entity movement
	//			Read player->cInput for player movement

	sf::Vector2f playerVelocity;
	// Example of updating
	// m_player->cTransform->pos.x += m_player->cTransform->velocity.x,
	if(m_player->cInput->left)
		playerVelocity.x -= m_playerConfig.S;

	if(m_player->cInput->right)
		playerVelocity.x += m_playerConfig.S;

	if(m_player->cInput->up)
		playerVelocity.y += m_playerConfig.S;

	if(m_player->cInput->down)
		playerVelocity.y -= m_playerConfig.S;

	m_player->cTransform->velocity += playerVelocity;

	for (auto e: m_entities.getEntities())
	{
		e->cTransform->pos += e->cTransform->velocity;
	}

	//m_player->cTransform->pos -= m_player->cTransform->velocity;
}

void Game::sLifespan()
{
	for (auto e : m_entities.getEntities())
	{
		if (!e->cLifespan)
			continue;

	// auto color = e->cShape->circle.getFillColor();
	// int newAlpha = 100; // calculate new alpha based on lifespan
	// sf::Color newColor(color.r, color.g, color.b, newAlpha);
	// e->cShape->circle.setFillColor(newColor)M

	}
	// TODO: implement lifespan functionality (for small enemies)

	// for all entities
	// if lifespan remaining is over 0, reduce it by one (frame)
	// if lifespan and alive, scale its alpha channel

	// if lifespan and its 0, destroy (e->destroy();)
}

void Game::sCollision()
{
	// Enemy/Player collision
	for(auto p : m_entities.getEntities("player"))
		for(auto e : m_entities.getEntities("enemy"))
		{
			sf::Vector2f pPos = p->cTransform->pos;
			sf::Vector2f ePos = e->cTransform->pos;

			sf::Vector2f diffVec = pPos - ePos;

			float distSq = diffVec.x*diffVec.x + diffVec.y*diffVec.y;
			float pRad = p->cCollision->radius;
			float eRad = e->cCollision->radius;

			if ( distSq < (pRad + eRad)*(pRad + eRad))
			{
				e->destroy();
				p->destroy();
				//This means collision
			}

		}

	// Enemy/Bullet collision
	for(auto b : m_entities.getEntities("bullet"))
		for(auto e : m_entities.getEntities("enemy"))
		{
			sf::Vector2f pPos = b->cTransform->pos;
			sf::Vector2f ePos = e->cTransform->pos;

			sf::Vector2f diffVec = pPos - ePos;

			float distSq = diffVec.x*diffVec.x + diffVec.y*diffVec.y;
			float bRad = b->cCollision->radius;
			float eRad = e->cCollision->radius;

			if ( distSq < (bRad + eRad)*(bRad + eRad))
			{
				e->destroy();
				b->destroy();
				//This means collision
			}


			// If collision between e and p
			// p->cCollision->radius
			// p->cTransform->position
		}


	// TODO: Implement all proper collision between entities
	//			use collision radius, not shape radius.

}

void Game::sEnemySpawner()
{
	// TODO: enemy spawning

	// m_currentFrame - m_lastEnemySpawnTime since last enemy spawned
	// read from the config
}

void Game::sRender()
{
	// TODO: chanmge the code below to draw all of the enemies (for ents in getEnts)
	m_window.clear();

	//SFML 3.0 doesn't allow 2 floats to create a vector upon setPosition
	sf::Vector2f pPosVec(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	m_player->cShape->circle.setPosition(pPosVec);

	m_player->cTransform->angle += 1.0f;

	// player rotation
	if (!m_paused)
	{
		sf::Angle newAngle(m_player->cShape->circle.getRotation());
		m_player->cShape->circle.setRotation(newAngle);
	}

	m_window.draw(m_player->cShape->circle);

}

void Game::sUserInput()
{
	// TODO:	handle user input here
	//			only player, logic in movement system

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// Window closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// KeyPressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// W key
				m_player->cInput->up = true;
				break;

			case sf::Keyboard::A:
				// A key
				m_player->cInput->left = true;
				break;

			case sf::Keyboard::S:
				// S key
				m_player->cInput->down = true;
				break;

			case sf::Keyboard::D:
				// D key
				m_player->cInput->right = true;
				break;

			}
		}

		// KeyReleased
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// W key
				m_player->cInput->up = false;
				break;

			case sf::Keyboard::A:
				// A key
				m_player->cInput->left = false;
				break;

			case sf::Keyboard::S:
				// S key
				m_player->cInput->down = false;
				break;

			case sf::Keyboard::D:
				// D key
				m_player->cInput->right = false;
				break;

			}
		}

		if (event.mouseButton.button == sf::Mouse::Left)
		{
			std::cout << "MouseLeft pressed at: " << event.mouseButton.x  << ", " << event.mouseButton.y << std::endl;
			// spawnbullet! player to mouse
		}

		if (event.mouseButton.button == sf::Mouse::Right)
		{
			std::cout << "MouseRight pressed at: " << event.mouseButton.x  << ", " << event.mouseButton.y << std::endl;
			// spawnSpecialWeapon
		}

	}
}