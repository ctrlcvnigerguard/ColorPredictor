#pragma once
enum { widthI = 600, heightI = 300 };

// Includes
#include <deque>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <json.hpp>

// SFML
#include <SFML/Graphics.hpp>

// Namespaces
using namespace std;
using namespace sf;

// Constants
static const float heightF = 600.f, widthF = 300.f;

// Variables
ContextSettings settings(32, 0, 8);
RenderWindow window(VideoMode(widthI, heightI), "Neural Networks", Style::Default, settings);