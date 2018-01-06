#include "stdafx.h"
#include "CS452Assign2.h"
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
using namespace std;
// TODO: Declare global variables here
struct edge
{
	int ymin;
	int ymax;
	double x;
	double m_inv;
	
};
char* filename = "output.png"; //this is the file in which your result will be automatically saved
FIBITMAP* bitmap;
void getPixelColor(int x, int y, RGBQUAD & color, int val1, int val2, int val3);
void drawline(int x0, int y0, int x1, int y1, FIBITMAP* bitmap, RGBQUAD color);
void scanLine(Coord *vertices, int nVertices, RGBQUAD edgeColor, RGBQUAD fillColor, FIBITMAP* bitmap);
void drawCircle(int xc, int yc, int r, FIBITMAP* bitmap, RGBQUAD edgecolor);
void Initialize()
{
	/* TODO: Initialize your global variables here.
	You might also want to set the window bounds
	using the setWindowBounds function which
	has already been implemented.

	This function is called before the program
	enter the main event loop. */
	FreeImage_Initialise();

	bitmap = FreeImage_Allocate(500, 500, BPP); // create image of appropriate size according to your needs
	if (!bitmap) {
		cerr << "Could not allocate bitmap!" << endl;
	}



	int x0 = 250;
	int y0 = 250;
	int r = 200;
	RGBQUAD newcolor, edgecolor, oldcolor, otherline, background, tempcol;
	getPixelColor(10, 10, newcolor, 255, 255, 255);
	getPixelColor(10, 10, edgecolor, 255, 255, 255);
	getPixelColor(10, 10, oldcolor, 0, 0, 0);
	getPixelColor(10, 10, otherline, 200, 200, 200);
	getPixelColor(10, 10, background, 44, 15, 98);
	getPixelColor(10, 10, tempcol, 100, 100, 100);

	/*drawCircle(x0, y0, r, bitmap, otherline);
	drawCircle(x0, y0 + r / 2, r / 2, bitmap, edgecolor);
	drawCircle(x0, y0 - r / 2, r / 2, bitmap, edgecolor);
	FloodFill4(200, 250, newcolor, oldcolor, bitmap);
	FloodFill4(250, 299, newcolor, oldcolor, bitmap);
	FloodFill4(1, 1, background, oldcolor, bitmap);
	FloodFill4(250, 300, tempcol, newcolor, bitmap);
	FloodFill4(300, 250, newcolor, oldcolor, bitmap);
	FloodFill4(250, 200, newcolor, oldcolor, bitmap);
	FloodFill4(250, 200, oldcolor, newcolor, bitmap);
	FloodFill4(250, 300, newcolor, tempcol, bitmap);
	drawCircle(x0, y0 + (r / 2), r / 4, bitmap, oldcolor);
	drawCircle(x0, y0-(r/2), r/4, bitmap, edgecolor);
	FloodFill4(x0, y0 + (r/2), oldcolor, newcolor, bitmap);
	FloodFill4(x0, y0 - (r/2), newcolor, oldcolor, bitmap);*/

	//----------
	RGBQUAD sky, sea, clouds, boat, sun;
	getPixelColor(10, 10, sky, 108, 202, 225);
	getPixelColor(10, 10, sea, 63, 72, 204);
	getPixelColor(10, 10, clouds, 255, 255, 255);
	getPixelColor(10, 10, boat, 119, 73, 49);
	getPixelColor(10, 10, sun, 255, 242, 0);

	FloodFill4(10, 10, sky, oldcolor, bitmap);

	Coord waves[8];
	Coord point;
	point.x = 0; point.y = 0;
	waves[0] = point;
	point.x = 500; point.y = 0;
	waves[7] = point;
	waves[1].x = 0;
	waves[1].y = 80;
	for (int j = 2; j < 7; j++) {
		waves[j].x = j*(500 / 6);
		if (j % 2 != 0) {
			waves[j].y = 80;
		}
		else {
			waves[j].y = 70;
		}
	}
	scanLine(waves, 8, sea, sea, bitmap);

	Coord boat_vals[4];
	boat_vals[0].x = 100; boat_vals[0].y = 95;
	boat_vals[1].x = 250; boat_vals[1].y = 95;
	boat_vals[2].x = 210; boat_vals[2].y = 50;
	boat_vals[3].x = 160; boat_vals[3].y = 50;
	scanLine(boat_vals, 4, boat, boat, bitmap);
	drawCircle(420, 420, 50, bitmap, sun);
	FloodFill4(420, 420, sun, sky, bitmap);
	drawCircle(45, 420, 40, bitmap, clouds);
	FloodFill4(30, 420, clouds, sky, bitmap);
	drawCircle(84, 415, 41, bitmap, clouds);
	FloodFill4(90, 415, clouds, sky, bitmap);

	drawCircle(200, 430, 40, bitmap, clouds);
	FloodFill4(200, 420, clouds, sky, bitmap);
	drawCircle(248, 432, 41, bitmap, clouds);
	FloodFill4(250, 415, clouds, sky, bitmap);

	//------------------------------
	/*RGBQUAD beats;
	getPixelColor(10, 10, beats, 237, 28, 36);
	drawCircle(200, 200, 150, bitmap, beats);
	BoundaryFill4(200, 200, beats, beats, bitmap);
	drawCircle(200, 200, 75, bitmap, edgecolor);
	drawCircle(200, 200, 46, bitmap, edgecolor);
	BoundaryFill4(200, 250, edgecolor, edgecolor, bitmap);
	drawline(125, 200, 125, 350, bitmap, edgecolor);
	drawline(154, 200, 154, 350, bitmap, edgecolor);
	FloodFill4(145, 300, edgecolor, beats, bitmap);
	FloodFill4(10, 10, edgecolor, oldcolor, bitmap);
	renderImage(bitmap, 1.0);*/
}

void Render(int mouseX, int mouseY)
{
	/* TODO: Render images here. The coordinates of
	mouse pointer are passed in as parameters.
	Use the renderImage function which has
	already been implemented to display an
	image onto the screen.

	This function is called once for each
	iteration of the main event loop. */
	RGBQUAD color;
	getPixelColor(10, 10, color, 255, 255, 255);
	int x0,y0,x1,y1;
	//Uncomment FOR loop below to test your code
	//for (float theta = 0; theta <= 2 * M_PI; theta = theta + M_PI / 28)
	//{
	//	x0 = 150; y0 = 150;
	//	x1 = x0 + (int)(175 * cos(theta));
	//	y1 = y0 + (int)(175 * sin(theta));

	//
	//drawline(x0,y0,x1,y1,bitmap, color);  //In case your drawing code is very slow introduce a flag such that bitmap is only updated when there is a change otherwise it simply calls renderImage


	//renderImage(bitmap,1.0);
	//} //end of for theta

	/*drawline(200, 100, 200, 200, bitmap);
	drawline(100, 100, 200, 100, bitmap);
	drawline(200, 200, 100, 200, bitmap);
	drawline(100, 100, 100, 200, bitmap);*/
	/*x0 = 250;
	y0 = 250;*/
	int r = 200;
	RGBQUAD newcolor, edgecolor, oldcolor, otherline, background, tempcol;
	getPixelColor(10, 10, newcolor, 255, 255, 255);
	getPixelColor(10, 10, edgecolor, 255, 255, 255);
	getPixelColor(10, 10, oldcolor, 0, 0, 0);
	getPixelColor(10, 10, otherline, 200, 200, 200);
	getPixelColor(10, 10, background, 44, 15, 98);
	getPixelColor(10, 10, tempcol, 100, 100, 100);

	/*drawCircle(x0, y0, r, bitmap, otherline);
	drawCircle(x0, y0+r/2, r/2, bitmap, edgecolor);
	drawCircle(x0, y0-r/2, r/2, bitmap, edgecolor);
	FloodFill4(200, 250, newcolor, oldcolor, bitmap);
	FloodFill4(250, 299, newcolor, oldcolor, bitmap);
	FloodFill4(1, 1, background, oldcolor, bitmap);
	FloodFill4(250, 300, tempcol, newcolor, bitmap);	
	FloodFill4(300, 250, newcolor, oldcolor, bitmap);
	FloodFill4(250, 200, newcolor, oldcolor, bitmap);
	FloodFill4(250, 200, oldcolor, newcolor, bitmap);
	FloodFill4(250, 300, newcolor, tempcol, bitmap);
	drawCircle(x0, y0+(r/2), r/4, bitmap, oldcolor);*/
	//drawCircle(x0, y0-(r/2), r/4, bitmap, edgecolor);
	//FloodFill4(x0, y0 + (r/2), newcolor, oldcolor, bitmap);
	//FloodFill4(x0, y0 - (r/2), oldcolor, newcolor, bitmap);

	/*RGBQUAD sky, sea, clouds, boat;
	getPixelColor(10, 10, sky, 108, 202, 225);
	getPixelColor(10, 10, sea, 63, 72, 204);
	getPixelColor(10, 10, clouds, 255, 255, 255);
	getPixelColor(10, 10, boat, 119, 73, 49);
	FloodFill4(10, 10, sky, oldcolor, bitmap);

	Coord waves[8];
	Coord point;
	point.x = 0; point.y = 0;
	waves[0] = point;
	point.x = 500; point.y = 0;
	waves[7] = point;
	waves[1].x = 0;
	waves[1].y = 80;
	for (int j = 2; j < 7; j++) {
		waves[j].x = j*(500 / 6);
		if (j % 2 != 0) {
			waves[j].y = 80;
		}
		else {
			waves[j].y = 70;
		}
	}
	scanLine(waves, 8, sea, sea, bitmap);

	Coord boat_vals[4];
	boat_vals[0].x = 84; boat_vals[0].y = 85;
	boat_vals[1].x = 180; boat_vals[1].y = 85;
	boat_vals[2].x = 160; boat_vals[2].y = 50;
	boat_vals[3].x = 104; boat_vals[3].x = 50;
	scanLine(boat_vals, 4, boat, boat, bitmap);*/
	//drawCircle(x0, y0+r/4, r/4, bitmap);
	//drawCircle(x0, y0 - r/4, r/4, bitmap);
	renderImage(bitmap, 1.0);
	//BoundaryFill4(150, 150, newcolor, edgecolor, bitmap);
	//getPixelColor(10, 10, newcolor, 27, 158, 60);
	//FloodFill4(20, 20, newcolor, oldcolor, bitmap);
	Coord pentagon[6];
	Coord temp;
	/*temp.x = 56; temp.y = 115;
	pentagon[0] = temp;
	temp.x = 166; temp.y = 24;
	pentagon[1] = temp;
	temp.x = 300; temp.y = 128;
	pentagon[2] = temp;
	temp.x = 231; temp.y = 252;
	pentagon[3] = temp;
	temp.x = 171; temp.y = 200;
	pentagon[4] = temp;
	temp.x = 111; temp.y = 242;
	pentagon[5] = temp;
	scanLine(pentagon, 6, color, newcolor, bitmap);
	renderImage(bitmap, 1);*/
	//FloodFill4(160, 160, newcolor, oldcolor, bitmap);

	//Hints for Hall of Fame
	/* By calling drawline and draw circle repeatedly you can draw complex polygons and shapes.
	Then if you call floodfill or boundary fill with different initial position and color values you can draw a nice drawing

	You may wish to draw a scene in mspaint simply using lines and circles and then try to achieve the same using your own code
	*/
}

void Destroy()
{
	// TODO: Free all allocated memory here.
	// For example, all FreeImage_Unload calls go here.
	if (FreeImage_Save(FIF_PNG, bitmap, filename, 0)) {
		cout << "Image successfully saved!" << endl;
	}

	FreeImage_DeInitialise();
}

//////////////////////////////////////////////////////////
void getPixelColor(int x, int y, RGBQUAD & color, int val1, int val2, int val3) {
	// feel free to change this function
	color.rgbRed = val1;
	color.rgbGreen = val2;
	color.rgbBlue = val3;
}

// Implement this function to draw a line using Midpoint Line Algorithm
void drawline(int x0, int y0, int x1, int y1, FIBITMAP* bitmap, RGBQUAD color) {
	//RGBQUAD color;

	////// Sample code to set pixel color in an image
	int x = 10, y = 20;
	//getPixelColor(x, y, color, 255, 255, 255);
	//FreeImage_SetPixelColor(bitmap, x, y, &color);
	//////

	// TYPE YOUR CODE HERE
	//======================================
	int d_x = x1 - x0;
	int d_y = y1 - y0;
	if (d_x == 0) { //In the case of vertical lines
		if (d_y > 0) {
			while (y0 <= y1) {
				FreeImage_SetPixelColor(bitmap, x0, y0, &color);
				y0++;
			}
		}
		else {
			while (y1 <= y0) {
				FreeImage_SetPixelColor(bitmap, x0, y1, &color);
				y1++;
			}
		}
	}
	else {
		float m = float(d_y) / float(d_x);
		if (m >= 0 && m <= 1) {
			if (d_x < 0) {
				d_x = -d_x;
			}
			if (d_y < 0) {
				d_y = -d_y;
			}
			if (x1 < x0) {
				int tempx = x0;
				int tempy = y0;
				x0 = x1;
				y0 = y1;
				x1 = tempx;
				y1 = tempy;
			}
			int d = 2 * d_y - d_x;
			x = x0;
			y = y0;
			d_x = 2 * d_x;
			d_y = 2 * d_y;
			FreeImage_SetPixelColor(bitmap, x, y, &color);
			while (x < x1) {
				if (d <= 0) {
					d = d + d_y;
					x = x + 1;
				}
				else {
					d = d + d_y - d_x;
					x = x + 1;
					y = y + 1;
				}
				FreeImage_SetPixelColor(bitmap, x, y, &color);
			}
		}
		else if (m > 1) {
			{
				if (d_x < 0) {
					d_x = -d_x;
				}
				if (d_y < 0) {
					d_y = -d_y;
				}
				if (y1 < y0) {
					int tempx = x0;
					int tempy = y0;
					x0 = x1;
					y0 = y1;
					x1 = tempx;
					y1 = tempy;
				}
				int d = d_y - 2 * d_x;
				x = x0;
				y = y0;
				d_x = 2 * d_x;
				d_y = 2 * d_y;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				while (y < y1) {
					if (d <= 0) {
						d = d + d_y - d_x;
						x = x + 1;
						y = y + 1;
					}
					else {
						d = d - d_x;
						y = y + 1;
					}
					FreeImage_SetPixelColor(bitmap, x, y, &color);
				}
			}
		}
		else if (m >= -1 && m < 0) {
			{
				if (d_x < 0) {
					d_x = -d_x;
				}
				if (d_y < 0) {
					d_y = -d_y;
				}
				if (x1 > x0) {
					int tempx = x0;
					int tempy = y0;
					x0 = x1;
					y0 = y1;
					x1 = tempx;
					y1 = tempy;
				}
				int d = 2 * d_y - d_x;
				x = x0;
				y = y0;
				d_x = 2 * d_x;
				d_y = 2 * d_y;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				while (x > x1) {
					if (d > 0) {
						d = d - d_y;
						x = x - 1;
					}
					else {
						d = d - d_y + d_x;
						x = x - 1;
						y = y + 1;
					}
					FreeImage_SetPixelColor(bitmap, x, y, &color);
				}
			}
		}
		else if (m < -1) {
			if (d_x < 0) {
				d_x = -d_x;
			}
			if (d_y < 0) {
				d_y = -d_y;
			}
			if (y1 < y0) {
				int tempx = x0;
				int tempy = y0;
				x0 = x1;
				y0 = y1;
				x1 = tempx;
				y1 = tempy;
			}
			int d = d_y - 2 * d_x;
			x = x0;
			y = y0;
			d_x = 2 * d_x;
			d_y = 2 * d_y;
			FreeImage_SetPixelColor(bitmap, x, y, &color);
			while (y < y1) {
				if (d > 0) {
					d = d - d_y + d_x;
					x = x - 1;
					y = y + 1;
				}
				else {
					d = d + d_x;
					y = y + 1;
				}
				FreeImage_SetPixelColor(bitmap, x, y, &color);
			}
		}
	}
	//======================================
}

// Implement this function to draw a Circle using Midpoint Line Algorithm
void drawCircle(int xc, int yc, int r, FIBITMAP* bitmap, RGBQUAD edgecolor) {
	//RGBQUAD color;

	////// Sample code to set pixel color in an image
	int x = 10, y = 20;
	//getPixelColor(x, y, color, 255, 255, 255);
	// FreeImage_SetPixelColor(bitmap, x, y, &color);
	//
	// Also check how to use FreeImage_GetPixelColor in FreeImage.h file
	//////	

	// TYPE YOUR CODE HERE
	//======================================
	x = 0;
	y = r;
	int d = 5 / 4 - r;
	FreeImage_SetPixelColor(bitmap, xc + x, yc + y, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc + y, yc + x, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc + y, yc - x, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc + x, yc - y, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc - x, yc - y, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc - y, yc - x, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc - y, yc + x, &edgecolor);
	FreeImage_SetPixelColor(bitmap, xc - x, yc + y, &edgecolor);
	while (x < y) {
		if (d > 0) {
			d = d + 2 * x - 2 * y + 2;
			x = x + 1;
			y = y - 1;
		}
		else {
			d = d + 2 * x + 1;
			x = x + 1;
		}
		FreeImage_SetPixelColor(bitmap, xc + x, yc + y, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc + y, yc + x, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc + y, yc - x, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc + x, yc - y, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc - x, yc - y, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc - y, yc - x, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc - y, yc + x, &edgecolor);
		FreeImage_SetPixelColor(bitmap, xc - x, yc + y, &edgecolor);
	}
	//======================================
}

/*4-connected Boundary Fill Algo*/
void BoundaryFill4(int x, int y, RGBQUAD newcolor, RGBQUAD edgecolor, FIBITMAP* bitmap)
{
	RGBQUAD color;
	FreeImage_GetPixelColor(bitmap, x, y, &color);
	// Above line will return the RGB value of color at pixel position x,y in the variable "color"

	// TYPE YOUR CODE HERE
	//======================================
	stack <Coord> pixels;
	Coord pos;
	pos.x = x;
	pos.y = y;
	pixels.push(pos);
	Coord i;
	while (pixels.size() > 0) {
		i = pixels.top();
		pixels.pop();
		FreeImage_GetPixelColor(bitmap, i.x, i.y, &color);
		if (color.rgbBlue != edgecolor.rgbBlue || color.rgbGreen != edgecolor.rgbGreen || color.rgbRed != edgecolor.rgbRed) {
			FreeImage_SetPixelColor(bitmap, i.x, i.y, &newcolor);
			i.x++;
			FreeImage_GetPixelColor(bitmap, i.x, i.y, &color);
			if (color.rgbBlue != newcolor.rgbBlue || color.rgbGreen != newcolor.rgbGreen || color.rgbRed != newcolor.rgbRed) {
				pixels.push(i); //east
			}
			i.x = i.x - 2;
			FreeImage_GetPixelColor(bitmap, i.x, i.y, &color);
			if (color.rgbBlue != newcolor.rgbBlue || color.rgbGreen != newcolor.rgbGreen || color.rgbRed != newcolor.rgbRed) {
				pixels.push(i); //west
			}
			i.x++;
			i.y++;
			FreeImage_GetPixelColor(bitmap, i.x, i.y, &color);
			if (color.rgbBlue != newcolor.rgbBlue || color.rgbGreen != newcolor.rgbGreen || color.rgbRed != newcolor.rgbRed) {
				pixels.push(i); //north
			}
			i.y = i.y - 2;
			FreeImage_GetPixelColor(bitmap, i.x, i.y, &color);
			if (color.rgbBlue != newcolor.rgbBlue || color.rgbGreen != newcolor.rgbGreen || color.rgbRed != newcolor.rgbRed) {
				pixels.push(i); //south
			}
		}
	}
	//======================================
}

/*4-connected Flood Fill Algo*/
void FloodFill4(int x, int y, RGBQUAD newcolor, RGBQUAD oldColor, FIBITMAP* bitmap)
{
	RGBQUAD color;
	FreeImage_GetPixelColor(bitmap, x, y, &color);
	// Above line will return the RGB value of color at pixel position x,y in the variable "color"

	// TYPE YOUR CODE HERE
	//======================================
	int height = FreeImage_GetHeight(bitmap);
	int width = FreeImage_GetWidth(bitmap);
	stack <Coord> pixels;
	Coord pos;
	pos.x = x;
	pos.y = y;
	pixels.push(pos);
	Coord i;
	while (pixels.size() > 0) {
		i = pixels.top();
		pixels.pop();
		FreeImage_GetPixelColor(bitmap, i.x, i.y, &color);
		if (color.rgbBlue == oldColor.rgbBlue && color.rgbGreen == oldColor.rgbGreen && color.rgbRed == oldColor.rgbRed) {
			FreeImage_SetPixelColor(bitmap, i.x, i.y, &newcolor);
			i.x++;
			if (i.x <= width) {
				pixels.push(i); //east
			}
			i.x = i.x - 2;
			if (i.x >= 0) {
				pixels.push(i); //west
			}
			i.x++;
			i.y++;
			if (i.y <= height) {
				pixels.push(i); //north
			}
			i.y = i.y - 2;
			if (i.y >= 0) {
				pixels.push(i); //south
			}
		}
	}
	//======================================
}

bool compare(edge e1, edge e2)
{
	return e1.x < e2.x;
}
void scanLine(Coord *vertices, int nVertices, RGBQUAD edgeColor, RGBQUAD fillColor, FIBITMAP* bitmap)
{
	// TYPE YOUR CODE HERE
	//======================================
	vector <edge> ET;
	for (int i = 0; i < nVertices; i++) {
		drawline(vertices[i].x, vertices[i].y, vertices[(i + 1) % nVertices].x, vertices[(i + 1) % nVertices].y, bitmap, edgeColor);
		edge temp;
		int ymin, ymax;
		double xmin, m_inv;
		if (vertices[i].y < vertices[(i + 1) % nVertices].y) {
			ymin = vertices[i].y;
			ymax = vertices[(i + 1) % nVertices].y;
			xmin = vertices[i].x;
		}
		else {
			ymin = vertices[(i + 1) % nVertices].y;
			ymax = vertices[i].y;
			xmin = vertices[(i + 1) % nVertices].x;
		}
		if (vertices[i].y != vertices[(i + 1) % nVertices].y) {
			m_inv = (vertices[(i + 1) % nVertices].x - vertices[i].x) / double(vertices[(i + 1) % nVertices].y - vertices[i].y);
		}
		else {
			m_inv = 0;
		}
		temp.m_inv = m_inv;
		temp.x = xmin;
		temp.ymax = ymax;
		temp.ymin = ymin;
		ET.push_back(temp);
	}
	vector <edge> AET;
	int min = ET[0].ymin;
	for (int i = 0; i < ET.size(); i++) {
		if (ET[i].ymin < min) {
			min = ET[i].ymin;
		}
	
	}
	for (int j = 0; j < ET.size(); j++) {
		if (ET[j].ymin == min) {
			AET.push_back(ET[j]);
			ET.erase(ET.begin() + j);
			j--;
		}
	}
	//min++;
	while (ET.size() > 0 || AET.size() > 0) {
		for (int k = 0; k < AET.size(); k++) {
			if ((min == AET[k].ymax) || (AET[k].ymin == AET[k].ymax)) {
				AET.erase(AET.begin() + k);
				k--;
			}
		}
		sort(AET.begin(), AET.end(), compare);
		int n = 0;
		if (AET.size() >= 2) {
			int from = ceil(AET[n].x);
			int to = floor(AET[n+1].x);
			for (int i = 0; i<floor(AET.size() / 2); i++) {
				for (int m = from; m <= to; m++) {
					FreeImage_SetPixelColor(bitmap, m, min, &fillColor);
				}
				n++;
				if (to == (floor(AET.back().x))) {
					break;
				}
				from = ceil(AET[n + 1].x);
				to = floor(AET[n + 2].x);
			}
		}
		min++;
		for (int k = 0; k < AET.size(); k++) {
			AET[k].x = AET[k].x + AET[k].m_inv;
			}
		for (int j = 0; j < ET.size(); j++) {
			if (ET[j].ymin == min) {
				AET.push_back(ET[j]);
				ET.erase(ET.begin() + j);
				j--;
			}
		}
	}
	//======================================
}