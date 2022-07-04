#include "Platform/Platform.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

class CShape
{
        float x, y;
        float sx, sy;
        int r, g, b;
        float sizeX, sizeY;
        float rad;
        string shape;
        string name;

    public:

        CShape(){};

        CShape(	float initX, float initY, float speedX, float speedY,
                int red, int green, int blue, float sizeX, float sizeY,
                string shape, string name)
            : 	x(initX), y(initY), sx(speedX), sy(speedY), r(red), g(green),
                b(blue),  sizeX(sizeX), sizeY(sizeY), shape(shape), name(name) {};

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
        os << "Size: " << c.sizeX << ", " << c.sizeY << endl;
    else
        os << "Radius: " << c.rad << endl;
    os << "############################" << endl;
    return os;
}

void constructShapes(string shape, istringstream * pConfigLine, vector<CShape> * pShapes);

int main()
{
    string filename("content/config");
    util::Platform platform;

    unsigned int wWidth = 640;
    unsigned int wHeight = 480;


    vector<string> lines;
    vector<CShape> shapes;
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
                lines.push_back(line);
        }
    }

    cout << "printingline to see what tht efuck" << endl;
    for (auto l = lines.begin(); l != lines.end(); ++l)
    {
        istringstream iss(*l);

        cout << "line: " << iss.str() << endl;
    }

    // set up config
    for (auto l = lines.begin(); l != lines.end(); ++l)
    {
        //cout << *l << endl;

        // int i = 0;

        istringstream iss(*l);

        do
        {
            string variable;

            iss >> variable;

            //cout << variable << endl;

            // If first string is Window, next two are width and height
            if(variable == "Window")
            {
                //cout << "Ooh, lala!" << endl;
                iss >> variable;
                wWidth = stoi(variable);
                iss >> variable;
                wHeight = stoi(variable);
            }
            else if(variable.length() != 0)
            {
                cout << "printing variable: " << variable << endl;
                constructShapes(variable ,&iss, &shapes);
            }

        } while (iss);

    }

    for (CShape &s: shapes)
        cout << s;



    // idea, initialize i, if first window, set window props, i+ and then loop through shapese


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

    float circleMoveSpeedX = -0.10f;
    float circleMoveSpeedY = -0.10f;
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

void constructShapes(string shape, istringstream * configLine, vector<CShape> * pShapes)
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
        cout << "Name: " << name << endl;
    *configLine >> initialX;
        cout << "x: " << initialX << endl;

    *configLine >> initialY;
        cout << "x: " <<initialY << endl;

    *configLine >> initialSpeedX;
        cout << "x: " <<initialSpeedX << endl;

    *configLine >> initialSpeedY;
        cout << "x: " <<initialSpeedY << endl;

    *configLine >> red;
        cout << "x: " <<red << endl;

    *configLine >> green;
        cout << "x: " <<green << endl;

    *configLine >> blue;
        cout << "x: " << blue << endl;

    if (shape == "Rectangle")
    {
        *configLine >> width;
        *configLine >> height;
        cout << "height: " << height << ", width: " << width << endl;

        CShape newShape(initialX, initialY, initialSpeedX,
                        initialSpeedY, red, green, blue,
                        width, height, shape, name);
        pShapes->push_back(newShape);
    }
    else
    {
        *configLine >> radius;
        cout << "Radius: " << radius << endl;
        CShape newShape(initialX, initialY, initialSpeedX,
                        initialSpeedY, red, green, blue,
                        radius, shape, name);
        pShapes->push_back(newShape);
    }

    // cout << variable << endl;
}