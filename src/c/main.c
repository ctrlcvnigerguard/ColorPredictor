#include "nn.h"

enum { widthI = 600, heightI = 300 };

#include <stdlib.h>
#include <string.h>

#include <SFML/Graphics.h>

// Constants
static const float heightF = 600.F, widthF = 300.F;

sfRenderWindow *window;

static NeuralNetwork *ptr;

static int r, g, b, frameCount = 1;

static char* which = "black";

void pickColor() {
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
}

char* colorPredictor(int red, int green, int blue) {
    register float inputs[3];
    
    inputs[0] = red / 255;
    inputs[1] = green / 255;
    inputs[2] = blue / 255;
    
    const float* outputs = neural_network_predict(ptr, inputs);
    
    register char* result;
    if (outputs[0] > outputs[1])
        result = "black";
    else
        result = "white";
    
    return result;
}

float* trainColor(int red, int green, int blue) {
    register float* t = (float *)malloc(2);
    if (red + green + blue > (255 * 3) / 2) {
        t[0] = 1;
        t[1] = 0;
    } else {
        t[0] = 0;
        t[1] = 1;
    }
    
    return t;
}

void setup() {
    window = sfRenderWindow_create((sfVideoMode){ widthI, heightI }, "Neural Networks", sfDefaultStyle, NULL);
    
    pickColor();
}

void mousePressed() {
    pickColor();
    
    which = colorPredictor(r, g, b);
    frameCount = 0;
}

void draw() {
    sfRenderWindow_clear(window, (sfColor){ r, g, b });
    
    sfText *black = sfText_create();
    sfText *white = sfText_create();

    sfFont *f = sfFont_createFromFile("/System/Library/Fonts/Helvetica.ttc");

    sfText_setFont(black, f);
    sfText_setFont(white, f);

    /* Set text */
    sfText_setString(black, "Black");
    sfText_setStyle(black, sfTextBold);
    sfText_setString(white, "White");
    sfText_setStyle(white, sfTextBold);

    /* Set the character size */
    sfText_setCharacterSize(black, 64);
    sfText_setCharacterSize(white, 64);
    
    /* Set color */
    sfText_setFillColor(black, (sfColor){ 0, 0, 0 });
    sfText_setFillColor(white, (sfColor){ 255, 255, 255 });

    /* Set pos */
    sfText_setPosition(black, (sfVector2f){ 50, 50 });
    sfText_setPosition(white, (sfVector2f){ 380, 50 });

    /* Draw texts */
    sfText_setFillColor(black, (sfColor){ 0, 0, 0 });
    sfText_setFillColor(white, (sfColor){ 255, 255, 255 });
    
    sfRenderWindow_drawText(window, black, NULL);
    sfRenderWindow_drawText(window, white, NULL);

    /* Draw some Line */
    sfVertexArray *vertices = sfVertexArray_create();
    sfVertexArray_append(vertices, (sfVertex){ (sfVector2f){ widthF, 0 }, sfBlack });
    sfVertexArray_append(vertices, (sfVertex){ (sfVector2f){ widthF, heightF }, sfBlack });
    
    sfRenderWindow_drawVertexArray(window, vertices, NULL);

    if (strncmp(which, "black", 5) && (frameCount == 0)) {
        sfCircleShape *shape;
        sfCircleShape_setRadius(shape, 30.F);
        sfCircleShape_setFillColor(shape, (sfColor){ 0, 0, 0 });
        sfCircleShape_setPosition(shape, (sfVector2f){ 100, 175 });
        sfRenderTexture_drawCircleShape(window, shape, NULL);
    } else if (strncmp(which, "white", 5) && frameCount == 0) {
        sfCircleShape *shape;
        sfCircleShape_setRadius(shape, 30.F);
        sfCircleShape_setFillColor(shape, (sfColor){ 255, 255, 255 });
        sfCircleShape_setPosition(shape, (sfVector2f){ 435, 175 });
        sfRenderTexture_drawCircleShape(window, shape, NULL);
    }
    
    sfRenderWindow_display(window);
}

int main(void) {
    setup();
    
    register int i = 0;
    while (sfRenderWindow_isOpen(window)) {
        register NeuralNetwork *brain = neural_network_new_with_args(3, 3, 2);
        ptr = brain;
        while (i < 10000) {
            register int red = rand() % 255 + 0, green = rand() % 255 + 0, blue = rand() % 255 + 0;
            register float* targets = trainColor(red, green, blue);
            register float inputs[3];
            
            inputs[0] = red / 255;
            inputs[1] = green / 255;
            inputs[2] = blue / 255;
            
            neural_network_train(brain,inputs, targets);

            ++i;
        }
        
        sfEvent *event;
        while (sfRenderWindow_pollEvent(window, event)) {
            if (event->type == sfEvtClosed || event->key.code == sfKeyEscape)
                sfRenderWindow_close(window);
            else if (event->type == sfEvtMouseButtonPressed)
                mousePressed();
        }
        draw();
    }
    
    return 0;
}
