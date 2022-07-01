#include "Platform/Platform.hpp"
#include <iostream>

int main()
{
	util::Platform platform;

	const unsigned int wWidth = 640;
	const unsigned int wHeight = 480;
	const sf::Vector2u win(wWidth, wHeight);

	sf::RenderWindow window(sf::VideoMode(win), "SFML works!");
	// in Windows at least, this must be called before creating the window
	// float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	// window.create(sf::VideoMode(200.0f * screenScalingFactor, 200.0f * screenScalingFactor), "SFML works!");
	platform.setIcon(window.getSystemHandle());

	sf::CircleShape circle(20);

	circle.setFillColor(sf::Color::White);

	sf::Color circleColor = sf::Color::Yellow;
	circle.setFillColor(circleColor);

	float circleMoveSpeedX = -0.90f;
	float circleMoveSpeedY = -0.90f;
	sf::Event event;

	// Main loop
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			std::cout << "Key pressed with code = " << event.key.code << "\n";
		}


		sf::Vector2f previousPosition = circle.getPosition();
		if (previousPosition.x < 0.0f)
		{
			circleMoveSpeedX *= -1.0f;
		} else if (previousPosition.x > wWidth - circle.getRadius()*2)
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

		circle.setPosition(newPosition);

		window.clear();
		window.draw(circle);
		window.display();
	}

	return 0;
}
