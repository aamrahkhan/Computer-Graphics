/*
* Film.cpp
*
*  Created on: Feb 19, 2009
*      Author: njoubert
*/

#include "Film.h"

Film::Film(int w, int h, string filename) {
	_w = w;
	_h = h;
	_filename = filename;
	dat = new double[_h*RAYS_PER_PIXEL*_w*RAYS_PER_PIXEL*3/4];
}

Film::~Film() {
	delete[]dat;
}

/**
* Here we stick the "saving" of the image.
* In our first case, this involves using OpenGL to display the pixels.
*/
void Film::expose(RGB c, Sample & s) {

	if (c[0] > 0)
		c[0] = c[0];
	//YOUR CODE HERE
	int row = 0, col = 0;

	// I should have a martix of (WIDTHxRAYS_PER_PIXEL X HEIGHTxRAYS_PER_PIXEL X 3) to store RBG
	col = s.img_x()*RAYS_PER_PIXEL;
	row = s.img_y()*RAYS_PER_PIXEL;
	col = col - col / 2 - 1;
	row = row - row / 2 - 1;
	// I should convert s into a index in the above matrix
	dat[row*_w * 6 + 3 * col + RED] = c[RED];
	dat[row*_w * 6 + 3 * col + GREEN] = c[GREEN];
	dat[row*_w * 6 + 3 * col + BLUE] = c[BLUE];

	//IMPLEMENT_ME(__FILE__,__LINE__);
}

void Film::bakeAndSave() {
	FreeImage_Initialise();
	FIBITMAP *output = FreeImage_Allocate(_w, _h, 24);
	RGBQUAD color;
	double r = 0, g = 0, b = 0;

	// For each pixel (i,j) on the image
	for (int x = 0; x < _w; x++) {
		for (int y = 0; y < _h; y++) {
			r = 0, g = 0, b = 0;

			// mean of color by all the rays for this pixel
			for (int rpp_row = 0; rpp_row < RAYS_PER_PIXEL / 2; rpp_row++)
			{
				for (int rpp_col = 0; rpp_col < RAYS_PER_PIXEL / 2; rpp_col++)
				{
					r += dat[(y * 2 + rpp_row)*_w * 6 + x * 6 + rpp_col * 3 + RED];
					g += dat[(y * 2 + rpp_row)*_w * 6 + x * 6 + rpp_col * 3 + GREEN];
					b += dat[(y * 2 + rpp_row)*_w * 6 + x * 6 + rpp_col * 3 + BLUE];
				}
			}

			r = (255 * r / RAYS_PER_PIXEL);
			g = (255 * g / RAYS_PER_PIXEL);
			b = (255 * b / RAYS_PER_PIXEL);

			color.rgbRed = MIN(r, 255.0);
			color.rgbGreen = MIN(g, 255.0);
			color.rgbBlue = MIN(b, 255.0);

			FreeImage_SetPixelColor(output, x, y, &color);
		}
	}

	FreeImage_Save(FIF_PNG, output, _filename.c_str());
	FreeImage_DeInitialise();
}
