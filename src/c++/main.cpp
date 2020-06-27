#include "nn.hpp"

NeuralNetwork *ptr;

int r, g, b, frameCount = 1;

string which = "black";

void pickColor() {
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
}

string colorPredictor(int red, int green, int blue) {
    float *inputs = new float[3];
    const float *outputs;

    inputs[0] = red / 255;
    inputs[1] = green / 255;
    inputs[2] = blue / 255;

    outputs = ptr->predict(inputs);
    if (outputs[0] > outputs[1])
        return "black";
    else
        return "white";
}

float* trainColor(int red, int green, int blue) {
    float *t = new float[2];
    if (red + green + blue > (255 * 3) / 2) {
        t[0] = 1;
        t[1] = 0;
        return t;
    } else {
        t[0] = 0;
        t[1] = 1;
        return t;
    }
}

void setup() {
    pickColor();
}

void mousePressed() {
    pickColor();

    which = colorPredictor(r, g, b);
    frameCount = 0;
}

void draw() {
    window.clear(Color(r, g, b));

    Font f;
    Text black, white;

    f.loadFromFile("/System/Library/Fonts/Helvetica.ttc");

    black.setFont(f);
    white.setFont(f);

    // Set text
    black.setString("Black");
    black.setStyle(Text::Bold);
    white.setString("White");
    white.setStyle(Text::Bold);

    // set the character size
    black.setCharacterSize(64);
    white.setCharacterSize(64);

    // Set color
    black.setFillColor(Color(0, 0, 0));
    white.setFillColor(Color(255, 255, 255));

    // Set pos
    black.setPosition(50, 50);
    white.setPosition(380, 50);

    // Draw texts
    window.draw(black);
    window.draw(white);

    // Draw some Line
    Vertex vertices[2] = {
            Vertex (Vector2f(widthF, 0), Color::Black),
            Vertex (Vector2f(widthF, heightF), Color::Black),
    };

    window.draw(vertices, 2, Lines);

    if (which == "black" && frameCount == 0) {
        CircleShape shape;
        shape.setRadius(30);
        shape.setFillColor(Color(0, 0, 0));
        shape.setPosition(100 ,175);
        window.draw(shape);
    } else if (which == "white" && frameCount == 0) {
        CircleShape shape;
        shape.setRadius(30);
        shape.setFillColor(Color(255, 255, 255));
        shape.setPosition(435, 175);
        window.draw(shape);
    }

    window.display();
}

int main() {
    setup();
    int i = 0;
    while (window.isOpen()) {
        NeuralNetwork brain = *new NeuralNetwork(3, 3, 2);
        ptr = &brain;
        while (i < 10000) {
            int red = rand() % 255 + 0, green = rand() % 255 + 0, blue = rand() % 255 + 0;
            float *targets = trainColor(red, green, blue), *inputs = new float[3];

            inputs[0] = red / 255;
            inputs[1] = green / 255;
            inputs[2] = blue / 255;

            brain.train(inputs, targets);
            i++;
        }
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
                mousePressed();
        }
        draw();
    }
	return 0;
}
