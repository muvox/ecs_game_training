#include "Game.h"
#include "Platform/Platform.hpp"

Game::Game(const std::string & config)
{
	init(config);
}

void Game::init(const std::string & path)
{
	util::Platform platform;
	// TODO: 	read in config file here
	//			use the premade playerconfig, enemyconfig and bulletconfig variables

	std::ifstream inFile(path);
	std::string line;
	if(!inFile.is_open())
	{
		std::cerr << "Could not open file: " << path << std::endl;
	}
	else
	{
		// Read window line
		inFile >> line;
		if (line == "Window")
		{
			std::cout << line << " config" << std::endl;
			inFile >> m_wWidth >> m_wHeight >> m_frameLimit >> m_fullscreen;
			std::cout << "Config read with resolution of " << m_wWidth << "x"
				<< m_wHeight << ", framerate limit of: " << m_frameLimit
				<< ", and fullscreen set to: " << m_fullscreen << std::endl;
		}
		else
			std::cerr << "Could not read window config properly!" << std::endl;

		// Read player line
		inFile >> line;
		if (line == "Player")
		{
			std::cout << "Reading player config" << std::endl;
			inFile >> m_playerConfig.SR >> m_playerConfig.CR
				>> m_playerConfig.S >> m_playerConfig.FR
				>> m_playerConfig.FG >> m_playerConfig.FB
				>> m_playerConfig.OR >> m_playerConfig.OG
				>> m_playerConfig.OB >> m_playerConfig.OT
				>> m_playerConfig.V;
		}
		else
			std::cerr << "Could not read player config properly!" << std::endl;

		// Read enemy line
		inFile >> line;
		if (line == "Enemy")
		{
			std::cout << "Reading enemy config" << std::endl;
			inFile >> m_enemyConfig.SR >> m_enemyConfig.CR
				>> m_enemyConfig.SMIN >> m_enemyConfig.SMAX
				>> m_enemyConfig.OR >> m_enemyConfig.OG
				>> m_enemyConfig.OB >> m_enemyConfig.OT
				>> m_enemyConfig.VMIN >> m_enemyConfig.VMAX
				>> m_enemyConfig.L >> m_enemyConfig.SI;

		}
		else
			std::cerr << "Could not read enemy config properly!" << std::endl;

		// Read bullet line

		inFile >> line;
		if (line == "Bullet")
		{
			std::cout << "Reading bullet config" << std::endl;
			inFile >> m_bulletConfig.SR >> m_bulletConfig.CR
				>> m_bulletConfig.FR >> m_bulletConfig.FG
				>> m_bulletConfig.FB >> m_bulletConfig.OR
				>> m_bulletConfig.OG >> m_bulletConfig.OB
				>> m_bulletConfig.V;
		}
		else
			std::cerr << "Could not read bullet config properly!" << std::endl;
	}

	// SR, CR, S, FR, FG, FB, OR, OG, OB, OT, V
	const sf::Vector2u win(m_wWidth, m_wHeight);
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

		if (!m_player->isActive())
			spawnPlayer();

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
	// reading from playerconfig
	entity->cTransform = std::make_shared<CTransform>(
		sf::Vector2f((float)m_wWidth/2, (float)m_wHeight/2),
		sf::Vector2f(0.0f, 0.0f), 0.0f);

	// Shape component, radius 32, 8 sides, dark grey fill and red outline with thickness of 4
	entity->cShape = std::make_shared<CShape>(
		m_playerConfig.SR, m_playerConfig.V,
		sf::Color(m_playerConfig.FR,m_playerConfig.FG,m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
		m_playerConfig.OT);

	// Input component so we can move the player
	entity->cInput = std::make_shared<CInput>();

	// collision component
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.SR);

	// Set the Game objects player the entity.
	// NOTE: 	Goes slightly against the EntityManager paradigm, but the player is used so often
	//			that this makes it easier.
	m_player = entity;

}

// spawn enemy to random location (within bounds of the game window)
void Game::spawnEnemy()
{
	std::cout << "Creating enemy! " << std::endl;
	// Add entity via entitymanager
	auto entity = m_entities.addEntity("enemy");

	// We have to randomize enemy spawning
	// Define max and min range within window, considering collision radius
	int spawnXMax = m_wWidth - m_enemyConfig.CR;
	int spawnXMin = m_enemyConfig.CR * 2;
	int spawnYMax = m_wHeight - m_enemyConfig.CR;
	int spawnYMin = m_enemyConfig.CR * 2;

	// set the rande for the spawn zone
	int rangeX = spawnXMax - spawnXMin -1;
	int rangeY = spawnYMax - spawnYMin -1;

	// randomize within range
	int spawnX = rand() % rangeX + spawnXMin;
	int spawnY = rand() % rangeY + spawnYMin;

	// randomize enemy speed
	int xSpeed = rand() % ((int)m_enemyConfig.SMAX*2 + 1) - ((int)m_enemyConfig.SMIN);
	int ySpeed = rand() % ((int)m_enemyConfig.SMAX*2 + 1) - ((int)m_enemyConfig.SMIN);

	// Give entity a transform so it has a position, velocity and angle
	entity->cTransform = std::make_shared<CTransform>(
		sf::Vector2f(spawnX, spawnY),
		sf::Vector2f((float)xSpeed, (float)ySpeed), 0.0f);

	// randomize the number of points the enemy has
	int pointRange = m_enemyConfig.VMAX - m_enemyConfig.VMIN -1;
	int numberOfPoints = rand() % pointRange + m_enemyConfig.VMIN;

	// Give entity a shape
	// randomize fill colors
	m_enemyConfig.FR = rand() % 255 + 1;
	m_enemyConfig.FG = rand() % 255 + 1;
	m_enemyConfig.FB = rand() % 255 + 1;

	entity->cShape = std::make_shared<CShape>(
		m_enemyConfig.SR, numberOfPoints,
		sf::Color(m_enemyConfig.FR, m_enemyConfig.FG, m_enemyConfig.FB),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
		m_enemyConfig.OT
	);

	// collision component
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.SR);

	m_lastEnemySpawnTime = m_currentFrame;

}

// Spawn tiny enemies when big one dies
void Game::spawnSmallEnemies(ptr<Entity> e)
{
	// Read props from the original
	int points = e->cShape->circle.getPointCount();

	// When creating smaller enemies, read the parameters of the original
	// spawn as many small ones as the original had points
	// small ones the me color as "host", half size
	// reward double points than the big one
	for (int i = 0; i < points; i++)
	{
		auto smallEnemy = m_entities.addEntity("small");
		smallEnemy->cScore = std::make_shared<CScore>(e->cScore->score * 2);
	}

}

void Game::spawnBullet(ptr<Entity> entity, const sf::Vector2f & target)
{
	std::cout << target.x << ", " << entity->isActive() << std::endl;

	auto bullet = m_entities.addEntity("bullet");


	// We have to randomize enemy spawning
	// Define max and min range within window, considering collision radius
	// int spawnXMax = m_wWidth - m_enemyConfig.CR;
	// int spawnXMin = m_enemyConfig.CR * 2;
	// int spawnYMax = m_wHeight - m_enemyConfig.CR;
	// int spawnYMin = m_enemyConfig.CR * 2;

	// randomize enemy speed
	int speed = m_bulletConfig.S;

	// Give entity a transform so it has a position, velocity and angle
	bullet->cTransform = std::make_shared<CTransform>(
		sf::Vector2f(m_player->cTransform->pos),
		sf::Vector2f((float)speed, (float)speed), 0.0f);

	// randomize the number of points the enemy has
	int pointRange = m_enemyConfig.VMAX - m_enemyConfig.VMIN -1;
	int numberOfPoints = rand() % pointRange + m_enemyConfig.VMIN;

	// Give entity a shape
	// randomize fill colors
	m_enemyConfig.FR = rand() % 255 + 1;
	m_enemyConfig.FG = rand() % 255 + 1;
	m_enemyConfig.FB = rand() % 255 + 1;

	bullet->cShape = std::make_shared<CShape>(
		m_enemyConfig.SR, numberOfPoints,
		sf::Color(m_enemyConfig.FR, m_enemyConfig.FG, m_enemyConfig.FB),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
		m_enemyConfig.OT
	);

	// collision component
	bullet->cCollision = std::make_shared<CCollision>(m_enemyConfig.SR);


	// TODO:	Implement the spawning of bullet, traveling towards target
	//			- scalar speed, formula in notes. Speed is the number of frames
	//				to move when fired
	//			s = hypotenuse, points from player to mousePosition


}

void Game::spawnSpecialWeapon(ptr<Entity> entity)
{
	std::cout << entity->cTransform->pos.x << std::endl;

}

void Game::sMovement()
{
	// TODO:	Implement all the entity movement
	//			Read player->cInput for player movement

	sf::Vector2f playerVelocity;
	// Example of updating
	// m_player->cTransform->pos.x += m_player->cTransform->velocity.x
	// player speed from config: m_playerConfig.S;
	if(m_player->cInput->left)
		playerVelocity.x -= m_playerConfig.S;

	if(m_player->cInput->right)
		playerVelocity.x += m_playerConfig.S;

	if(m_player->cInput->up)
		playerVelocity.y -= m_playerConfig.S;

	if(m_player->cInput->down)
		playerVelocity.y += m_playerConfig.S;


	m_player->cTransform->velocity = playerVelocity;

	for (auto e: m_entities.getEntities())
	{
		if(e->tag() == "enemy")
		{
			sf::Vector2f previousPosition = e->cTransform->pos;
			if (previousPosition.x <= 0.0+e->cShape->circle.getRadius())
				e->cTransform->velocity.x *= -1.0f;
			else if (previousPosition.x >= m_wWidth - e->cShape->circle.getRadius())
				e->cTransform->velocity.x *= -1.0f;

			if (previousPosition.y <= 0.0 + e->cShape->circle.getRadius())
				e->cTransform->velocity.y *= -1.0f;
			else if (previousPosition.y >= m_wHeight - e->cShape->circle.getRadius())
				e->cTransform->velocity.y *= -1.0f;
		}
		// std::cout << "Old pos: " << e->cTransform->pos.x << ", " << e->cTransform->pos.y << std::endl;
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
		// std::cout << "New pos: " << e->cTransform->pos.x << ", " << e->cTransform->pos.y << std::endl;
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
	{
		sf::Vector2f pPos = p->cTransform->pos;

		for(auto e : m_entities.getEntities("enemy"))
		{
			sf::Vector2f ePos = e->cTransform->pos;
			sf::Vector2f diffVec = pPos - ePos;

			float distSq = diffVec.x*diffVec.x + diffVec.y*diffVec.y;
			float pRad = p->cCollision->radius;
			float eRad = e->cCollision->radius;

			if ( distSq < ((pRad + eRad) * (pRad + eRad)))
			{
				std::cout << "collision if" << std::endl;

				e->destroy();
				p->destroy();
				//This means collision
			}

			std::cout << "Checked for collision" << std::endl;

		}
	}
	// Enemy/Bullet collision
	for(auto b : m_entities.getEntities("bullet"))
	{
		// for(auto e : m_entities.getEntities("enemy"))
		// {
		// 	sf::Vector2f pPos = b->cTransform->pos;
		// 	sf::Vector2f ePos = e->cTransform->pos;

		// 	sf::Vector2f diffVec = pPos - ePos;

		// 	float distSq = diffVec.x*diffVec.x + diffVec.y*diffVec.y;
		// 	float bRad = b->cCollision->radius;
		// 	float eRad = e->cCollision->radius;

		// 	if ( distSq < (bRad + eRad)*(bRad + eRad))
		// 	{
		// 		e->destroy();
		// 		b->destroy();
		// 		//This means collision
		// 	}


		// 	// If collision between e and p
		// 	// p->cCollision->radius
		// 	// p->cTransform->position
		// }
	}
	// TODO: Implement all proper collision between entities
	//			use collision radius, not shape radius.

}

void Game::sEnemySpawner()
{
	// TODO: enemy spawning

	if ((m_currentFrame - m_lastEnemySpawnTime) > m_enemyConfig.SI )
	{
		spawnEnemy();
		std::cout << "Enemy spawned" <<  std::endl;

	}


	// m_currentFrame - m_lastEnemySpawnTime since last enemy spawned
	// read from the config
}

void Game::sRender()
{
	// TODO: chanmge the code below to draw all of the enemies (for ents in getEnts)
	m_window.clear();

	for(auto ent: m_entities.getEntities()){
		// Basic movement
		//SFML 3.0 doesn't allow 2 floats to create a vector upon setPosition
		sf::Vector2f pPosVec(ent->cTransform->pos.x, ent->cTransform->pos.y);
		ent->cShape->circle.setPosition(pPosVec);

		// rotation
		// NOTE: 	rotation works with wonky angles now. Create a new rotation
		// 			give it sf::degrees or sf::radian as a value and use the rotate function
		sf::Angle newRot(sf::degrees(1.0f));
		if (!m_paused)
		{
			ent->cShape->circle.rotate(newRot);
		}
		m_window.draw(ent->cShape->circle);

	}

	m_window.display();
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
				std::cout << "W pressed" << std::endl;
				break;

			case sf::Keyboard::A:
				// A key
				m_player->cInput->left = true;
				std::cout << "A pressed" << std::endl;
				break;

			case sf::Keyboard::S:
				// S key
				m_player->cInput->down = true;
				std::cout << "S pressed" << std::endl;
				break;

			case sf::Keyboard::D:
				// D key
				m_player->cInput->right = true;
				std::cout << "D pressed" << std::endl;
				break;
			default:
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
				std::cout << "W released" << std::endl;
				break;

			case sf::Keyboard::A:
				// A key
				m_player->cInput->left = false;
				std::cout << "A released" << std::endl;

				break;

			case sf::Keyboard::S:
				// S key
				m_player->cInput->down = false;
				std::cout << "S released" << std::endl;
				break;

			case sf::Keyboard::D:
				// D key
				m_player->cInput->right = false;
				std::cout << "D released" << std::endl;
				break;
			default:
				break;

			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
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
}