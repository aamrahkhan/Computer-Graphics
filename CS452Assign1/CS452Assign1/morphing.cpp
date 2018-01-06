#include "stdafx.h"
#include "CS452Assign1.h"

// TODO: Declare global variables here
FIBITMAP *img_amb;
FIBITMAP *img_both;
FIBITMAP *img_left;
FIBITMAP *img_right;

void Initialize()
{
	/* TODO: Initialize your global variables here.
	   You might also want to set the window bounds
	   using the setWindowBounds function which
	   has already been implemented.

	   This function is called before the program
	   enter the main event loop. */
	img_amb = FreeImage_Load(FIF_BMP, "amb.BMP", 0);
	img_both = FreeImage_Load(FIF_BMP, "both.BMP", 0);
	img_left = FreeImage_Load(FIF_BMP, "left.BMP", 0);
	img_right = FreeImage_Load(FIF_BMP, "right.BMP", 0);

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
	RGBQUAD *random = new RGBQUAD;
	RGBQUAD *random1 = new RGBQUAD;
	RGBQUAD *random2 = new RGBQUAD;
	RGBQUAD *random3 = new RGBQUAD;
	RGBQUAD *color = new RGBQUAD;
	setWindowSize(640, 480);
	
	float weight = mouseX/(float) FreeImage_GetWidth(img_amb);
	float v_weight = mouseY/(float) FreeImage_GetHeight(img_amb);

	weight = weight < 0 ? 0 : weight;
	weight = weight > 1 ? 1 : weight;

	v_weight = v_weight < 0 ? 0 : v_weight;
	v_weight = v_weight > 1 ? 1 : v_weight;

	//out << weight << "\n";

	for (unsigned int i = 1; i < FreeImage_GetWidth(img_amb); i++) {
		for (unsigned int j = 1; j < FreeImage_GetHeight(img_amb); j++) {
			FreeImage_GetPixelColor(img_amb, i, j, random);
			FreeImage_GetPixelColor(img_left, i, j, random1);
			FreeImage_GetPixelColor(img_right, i, j, random2);
			FreeImage_GetPixelColor(img_both, i, j, random3);

			if(v_weight<0.3){
				color->rgbRed = weight*(random1->rgbRed) + (1 - weight)*(random2->rgbRed);
				color->rgbGreen = weight*(random1->rgbGreen) + (1 - weight)*(random2->rgbGreen);
				color->rgbBlue = weight*(random1->rgbBlue) + (1 - weight)*(random2->rgbBlue);

				FreeImage_SetPixelColor(img_amb, i, j, color);
			}
			if(v_weight>0.3 && v_weight<0.6){

				color->rgbRed = weight*(random2->rgbRed) + (1-weight)*(random1->rgbRed);
				color->rgbGreen = 0;
				color->rgbBlue = weight*(random1->rgbBlue) + (1-weight)*(random2->rgbBlue);

				FreeImage_SetPixelColor(img_amb, i, j, color);
			}
			if(v_weight>0.6){
				
				color->rgbRed = random3->rgbRed - (weight*random1->rgbRed + (1 - weight)*random2->rgbRed);
				color->rgbBlue = random3->rgbBlue - (weight*random1->rgbBlue + (1 - weight)*random2->rgbBlue);
				color->rgbGreen = random3->rgbGreen - (weight*random1->rgbGreen + (1 - weight)*random2->rgbGreen);
				FreeImage_SetPixelColor(img_amb, i, j, color);
			}

		}
	}

	renderImage(img_amb, 1);
}

void Destroy()
{
	// TODO: Free all allocated memory here.
	// For example, all FreeImage_Unload calls go here.
	FreeImage_Unload(img_amb);
	FreeImage_Unload(img_right);
	FreeImage_Unload(img_left);
	FreeImage_Unload(img_both);
}