/*
* Viewport.cpp
*
*  Created on: Feb 19, 2009
*      Author: njoubert
*/

#include "Viewport.h"

Viewport::Viewport(vec4 eye, vec4 LL, vec4 UL, vec4 LR, vec4 UR, int width, int height) {
	_eye = eye;	//You should use this for the viewing ray
	_LL = LL;
	_UL = UL;
	_LR = LR;
	_UR = UR;
	_pixelsWide = width;
	_pixelsHigh = height;
	_raysPerPixel = RAYS_PER_PIXEL;
	_incPP = sqrt((float)_raysPerPixel);
	_incPR = 1.0 / _incPP;
}

Viewport::~Viewport() {

}

void Viewport::resetSampler() {
	_x = _y = 1.0 / (_incPP * 2); //Starts off at the center of a pixel.
}


/**
* This ONLY fills in the (i, j) coordinates of the point according
* to the pixels_wide and pixels_high settings
*/
bool Viewport::getSample(Sample & s) {
	//YOUR CODE HERE : 
	//_x and _y of viewport class with have value between (0,0) to (Width, Height)
	// u and v will have value between (0,0) to (1,1)
	//Sample.(_x,_y) will have values between (_LL,_UR)

	// given _x and _y compute u and v	
	double u = _x / _pixelsWide;
	double v = _y / _pixelsHigh;

	//Bilinear Interpolation
	//P(u, v) = (1 - u)[(1 - v)_LL + (v)_UL]+ (u)[(1 - v)_LR + (v)_UR]
	double temp = (1 - u)*((1 - v)*_LL[0] + (v)*_UL[0]) + (u)*((1 - v)*_LR[0] + (v)*_UR[0]);
	s.setX(temp);
	temp = (1 - u)*((1 - v)*_LL[1] + (v)*_UL[1]) + (u)*((1 - v)*_LR[1] + (v)*_UR[1]);
	s.setY(temp);
	s.setImgX(_x);
	s.setImgY(_y);

	// increment x and y	
	_x += _incPR;
	if (_x > _pixelsWide)
	{
		_x = _incPR;
		_y += _incPR;
	}

	if (_y > _pixelsHigh)
		return false;
	else
		return true;

	//IMPLEMENT_ME(__FILE__,__LINE__);
}

Ray Viewport::createViewingRay(Sample & s) {
	//YOUR CODE HERE
	//r(t) = e+t(p-e)
	vec3 e(_eye);  //start point
	vec3 p;		    //end point
	double t = 1; //time	

	p[0] = s.x();
	p[1] = s.y();
	p[2] = _LL[2];

	// calculate t
	return Ray(e, p, t);

	//IMPLEMENT_ME(__FILE__,__LINE__);
}

vec4 Viewport::getViewVector(vec4 & pos) {
	vec4 viewv = pos - this->_eye;
	return viewv.normalize();
}

int Viewport::getW() { return _pixelsWide; }

int Viewport::getH() { return _pixelsHigh; }

