#include "nn.h"

enum { widthI = 600, heightI = 300 };

#include <iostream>

#include <SFML/Graphics.hpp>

int r = 0;
int g = 0;
int b = 0;

int frameCount = 1;

std::string which = "black";

// Constants
const float heightF = 600.F;
const float widthF = 300.F;

sf::ContextSettings settings(24, 0, 8);
sf::RenderWindow window(sf::VideoMode(widthI, heightI), "Neural Networks", sf::Style::Default, settings);

void pickColor() {
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
}

void colorPredictor(const NeuralNetwork *__nn_param) {
    const float inputs[3] = {
        (float)r / 255.F,
        (float)g / 255.F,
        (float)b / 255.F
    };
    
    const float* outputs = neural_network_predict(__nn_param, inputs);

    if (outputs[0] > outputs[1])
        which = "black";
    else
        which = "white";
}

void draw() {
    window.clear(sf::Color(r, g, b));

    sf::Font f;
    sf::Text black, white;

    f.loadFromFile("/System/Library/Fonts/Helvetica.ttc");

    black.setFont(f);
    white.setFont(f);

    // Set text
    black.setString("Black");
    black.setStyle(sf::Text::Bold);
    white.setString("White");
    white.setStyle(sf::Text::Bold);

    // set the character size
    black.setCharacterSize(64);
    white.setCharacterSize(64);

    // Set color
    black.setFillColor(sf::Color(0, 0, 0));
    white.setFillColor(sf::Color(255, 255, 255));

    // Set pos
    black.setPosition(50, 50);
    white.setPosition(380, 50);

    // Draw texts
    window.draw(black);
    window.draw(white);

    // Draw some Line
    sf::Vertex vertices[2] = {
	sf::Vertex (sf::Vector2f(widthF, 0), sf::Color::Black),
        sf::Vertex (sf::Vector2f(widthF, heightF), sf::Color::Black),
    };

    window.draw(vertices, 2, sf::Lines);

    if (frameCount == 0) {
        if (which == "black") {
            sf::CircleShape shape(30.F);
            shape.setFillColor(sf::Color(0, 0, 0));
            shape.setPosition(100 ,175);
            
            window.draw(shape);
        } else if (which == "white") {
            sf::CircleShape shape(30.F);
            shape.setFillColor(sf::Color(255, 255, 255));
            shape.setPosition(435, 175);

            window.draw(shape);
        }
    }

    window.display();
}

int main() {
    pickColor();

    NeuralNetwork *brain;

    sf::Event event;
    int i = 0;

    /* Start the game loop */
    while (window.isOpen()) {
        brain = neural_network_new_with_args(3, 3, 2);
        while (i < 10000) {
            int red = rand() % 256;
            int green = rand() % 256;
            int blue = rand() % 256;
            
            const float inputs[3] = {
                (float)red / 255.F,
                (float)green / 255.F,
                (float)blue / 255.F
            };
            
            float targets[2] = { 0.F };
            
            int first = red + green + blue;
            int second = (255 * 3) / 2;
            
            if (first > second) {
                targets[0] = 1.F;
                targets[1] = 0.F;
            } else {
                targets[0] = 0.F;
                targets[1] = 1.F;
            }
            neural_network_train(brain, inputs, targets);

            ++i;
        }
        while (window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Escape))
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                pickColor();
                
                colorPredictor(brain);
                frameCount = 0;
            }
        }
        
        /* Cleanup resources */
        neural_network_free(brain);
        
        draw();
    }
    
    return 0;
}
