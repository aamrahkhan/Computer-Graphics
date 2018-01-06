#pragma once

#include "resource.h"

// The functions below are to be implemented in morphing.cpp
void Initialize();
void Render(int mouseX, int mouseY);
void Destroy();

// The functions below have been implemented.
void setWindowSize(int width, int height);
void renderImage(FIBITMAP *bitmap, float scale);