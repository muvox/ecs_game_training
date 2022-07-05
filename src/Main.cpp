#include "Platform/Platform.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

class CShape
{
    public:
        float x, y;
        float sx, sy;
        int r, g, b;
        float width, height;
        float rad;
        string shape;
        string name;
        sf::RectangleShape square;
        sf::CircleShape circle;

        CShape(){};

        CShape(	float initX, float initY, float speedX, float speedY,
                int red, int green, int blue, float width, float height,
                string shape, string name)
            : 	x(initX), y(initY), sx(speedX), sy(speedY), r(red), g(green),
                b(blue),  width(width), height(height), shape(shape), name(name) {};

        CShape(	float initX, float initY, float speedX, float speedY,
                int red, int green, int blue, float radius,
                string shape, string name)
            : 	x(initX), y(initY), sx(speedX), sy(speedY), r(red), g(green),
                b(blue),  rad(radius), shape(shape), name(name) {};

        friend ostream& operator<<(ostream& os, const CShape& c);

};

ostream& operator<<(ostream& os, const CShape& c)
{
    os << "############################" << endl;
    os << "Name: " << c.name << endl;
    os << "Shape: " << c.shape << endl;
    os << "Position: " << c.x << ", " << c.y << endl;
    os << "Speed: " << c.sx << ", " << c.sy << endl;
    os << "RGB: " << c.r << ", " << c.g << ", " << c.b << endl;
    if(c.shape == "Rectangle")
        os << "Size: " << c.width << ", " << c.height << endl;
    else
        os << "Radius: " << c.rad << endl;
    os << "############################" << endl;
    return os;
}

void generateShapes(string shape, istringstream * pConfigLine, vector<CShape> * pShapes);
bool configure(unsigned int * width, unsigned int * height, vector<string> * lines, vector<CShape> * shapes);

int main()
{

    util::Platform platform;

    unsigned int wWidth = 640;
    unsigned int wHeight = 480;

    vector<string> lines;
    vector<CShape> shapes;

    configure(&wWidth, &wHeight, &lines, &shapes);

    const sf::Vector2u win(wWidth, wHeight);

    sf::RenderWindow window(sf::VideoMode(win), "Muvoksis manic makings!");
    // in Windows at least, this must be called before creating the window
    // float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
    // Use the screenScalingFactor
    platform.setIcon(window.getSystemHandle());

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


        window.clear();

        for(CShape &s: shapes)
        {
            sf::Vector2f previousPosition;

            if(s.shape == "Rectangle")
            {
                previousPosition = s.square.getPosition();
                if (previousPosition.x < 0.0f)
                    s.sx *= -1.0f;
                else if (previousPosition.x > wWidth - s.width)
                    s.sx *= -1.0f;

                if (previousPosition.y < 0.0f)
                    s.sy *= -1.0f;
                else if (previousPosition.y > wHeight - s.height)
                    s.sy *= -1.0f;
            }

            else
            {
                previousPosition = s.circle.getPosition();
                if (previousPosition.x < 0.0f)
                    s.sx *= -1.0f;
                else if (previousPosition.x > wWidth - (s.rad * 2))
                    s.sx *= -1.0f;

                if (previousPosition.y < 0.0f)
                    s.sy *= -1.0f;
                else if (previousPosition.y > wHeight - (s.rad * 2))
                    s.sy *= -1.0f;

            }

            sf::Vector2f moveVector = sf::Vector2f(s.sx, s.sy);
            sf::Vector2f newPosition = previousPosition + moveVector;

            if(s.shape == "Rectangle")
            {
                s.square.setPosition(newPosition);
                s.x = newPosition.x;
                s.y = newPosition.y;
                window.draw(s.square);

            }
            else
            {
                s.circle.setPosition(newPosition);
                s.x = newPosition.x;
                s.y = newPosition.y;
                window.draw(s.circle);
            }

        }

        window.display();
    }

    return 0;
}

void generateShapes(string shape, istringstream * configLine, vector<CShape> * pShapes)
{

    string name;
    float initialX;
    float initialY;
    float initialSpeedX;
    float initialSpeedY;
    int red;
    int green;
    int blue;
    float width;
    float height;
    float radius;

    *configLine >> name;
    *configLine >> initialX;
    *configLine >> initialY;
    *configLine >> initialSpeedX;
    *configLine >> initialSpeedY;
    *configLine >> red;
    *configLine >> green;
    *configLine >> blue;
    if (shape == "Rectangle")
    {
        *configLine >> width;
        *configLine >> height;
        CShape newShape(initialX, initialY, initialSpeedX,
                        initialSpeedY, red, green, blue,
                        width, height, shape, name);
        pShapes->push_back(newShape);
    }
    else
    {
        *configLine >> radius;
        CShape newShape(initialX, initialY, initialSpeedX,
                        initialSpeedY, red, green, blue,
                        radius, shape, name);
        pShapes->push_back(newShape);
    }
}

bool configure(unsigned int * width, unsigned int * height, vector<string> * lines, vector<CShape> * shapes)
{
    string filename("content/config");
    string line;

    // open the file
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cerr << "Could not open the file - " << filename << endl;
        return EXIT_FAILURE;
    }
    else
    {
        // slap all the lines to a nice vector
        while (getline(inFile, line))
        {
            if(line.size() > 0)
                lines->push_back(line);
        }
    }

    for (auto l = lines->begin(); l != lines->end(); ++l)
    {
        istringstream iss(*l);

        cout << "line: " << iss.str() << endl;
    }

    // go through the config lines
    for (auto l = lines->begin(); l != lines->end(); ++l)
    {
        istringstream iss(*l);

        do
        {
            string variable;
            iss >> variable;

            // If first string is Window, next two are width and height
            if(variable == "Window")
            {
                iss >> variable;
                *width = stoi(variable);
                iss >> variable;
                *height = stoi(variable);
            }
            else if(variable.length() != 0)
            {
                generateShapes(variable ,&iss, shapes);
            }

        } while (iss);

    }

    for (CShape &s: *shapes)
    {
        // Here it gets a little funky. As per design the config file may contain either rectangles or circles
        // so they have different amount of parameters to play with.
        if (s.shape == "Circle")
        {
            sf::CircleShape c(s.rad);
            c.setFillColor(sf::Color(s.r, s.g, s.b));
            sf::Vector2f posVector(s.x, s.y);
            c.setPosition(posVector);
            s.circle = c;
        }
        else
        {
            sf::Vector2f sizeVector(s.width, s.height);
            sf::RectangleShape r(sizeVector);
            r.setFillColor(sf::Color(s.r, s.g, s.b));
            sf::Vector2f posVector(s.x, s.y);
            r.setPosition(posVector);
            s.square = r;
        }

    }

    return true;
}