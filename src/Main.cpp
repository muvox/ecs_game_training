#include "Platform/Platform.hpp"
using std::cerr;

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	const unsigned int wWidth = 1280;
	const unsigned int wHeight = 720;
	float circleMoveSpeedX = -0.9f;
	float circleMoveSpeedY = -0.5f;
	bool xPressedAlready = false;
	const sf::Vector2u win(wWidth, wHeight);

	sf::RenderWindow window(sf::VideoMode(win), "SFML Might work!!??...");
	// // in Windows at least, this must be called before creating the window
	// float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// // Use the screenScalingFactor
	// window.create(sf::VideoMode(200.0f * screenScalingFactor, 200.0f * screenScalingFactor), "SFML works!");
	platform.setIcon(window.getSystemHandle());

	sf::CircleShape circle(50);
	circle.setFillColor(sf::Color::White);

	sf::Texture shapeTexture;
	if (!shapeTexture.loadFromFile("content/sfml.png"))
	{
		std::cerr << "file loading failed!\n";
		exit(1);
	}
	circle.setTexture(&shapeTexture);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
			{
				if (!xPressedAlready)
				{
					circleMoveSpeedX *= 0.1f;
					circleMoveSpeedY *= 0.1f;
					xPressedAlready = true;
				}
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::X)
			{
				circleMoveSpeedX *= 10.0f;
				circleMoveSpeedY *= 10.0f;
				xPressedAlready = false;
			}
		}
		sf::Vector2f previousPosition = circle.getPosition();
		if (previousPosition.x < 0)
		{
			circleMoveSpeedX *= -1.0f;
		}
		else if (previousPosition.x > wWidth - (circle.getRadius() * 2))
		{
			circleMoveSpeedX *= -1.0f;
		}

		if (previousPosition.y < 0)
		{
			circleMoveSpeedY *= -1.0f;
		}
		else if (previousPosition.y > wHeight - (circle.getRadius() * 2))
		{
			circleMoveSpeedY *= -1.0f;
		}

		sf::Vector2f moveVector = sf::Vector2f(circleMoveSpeedX, circleMoveSpeedY * 1.1f);
		sf::Vector2f newPosition = previousPosition + moveVector;
		circle.setPosition(newPosition);

		window.clear();
		window.draw(circle);
		window.display();
	}

	return 0;
}
