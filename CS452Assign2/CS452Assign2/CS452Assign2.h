#pragma once

#include "resource.h"
#include <string>
#include <iostream>
#include <FreeImage.h>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

#define BPP 24

// The functions below are to be implemented in morphing.cpp
void Initialize();
void Render(int mouseX, int mouseY);
void Destroy();

// The functions below have been implemented.
void setWindowSize(int width, int height);
void renderImage(FIBITMAP *bitmap, float scale);

// Struct representing an integral coordinate
typedef struct
{
	int x;	// X coordinate
	int y;	// Y coordinate
} Coord;

void getPixelColor(int x, int y, RGBQUAD & color, int val);
void drawline(int x0, int y0, int x1, int y1, FIBITMAP* bitmap);
void drawCircle(int xc, int yc, int r, FIBITMAP* bitmap);
void BoundaryFill4(int x, int y, RGBQUAD newcolor, RGBQUAD edgecolor, FIBITMAP* bitmap);
void FloodFill4(int x, int y, RGBQUAD newcolor, RGBQUAD oldColor, FIBITMAP* bitmap);