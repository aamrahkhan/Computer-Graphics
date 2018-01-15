/*
 * Primitive.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Primitives.h"

Primitive::Primitive(RGB & c, Material & m, mat4 modelToWorld) {
    _c = c;
    _m = m;
    _modelToWorld = modelToWorld;
    _worldToModel = modelToWorld.inverse();
}

Primitive::~Primitive() {
}

void Primitive::setColor(RGB & c) {
    _c = c;
}

void Primitive::setMaterial(Material & m) {
    _m = m;
}

const RGB& Primitive::getColor() {
    return _c;
}
const Material& Primitive::getMaterial() {
    return _m;
}


Sphere::Sphere(double radius, RGB & c, Material & m, mat4 m2w): Primitive(c,m,m2w) {
    _r = radius;
}


//Checks for intersection with the given ray
double Sphere::intersect(Ray & ray) {

	//http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
	double t = numeric_limits<float>::infinity();
	mat4 m_temp = _modelToWorld.transpose();
	vec4 sph_c = m_temp[3]; //sphere center	

							// find discriminant B^2 -4AC
	double A = ray.direction()*ray.direction();							//d.d
	double B = ray.direction()*(ray.start() - sph_c);					//2d.(e-c)
	double C = (ray.start() - sph_c)*(ray.start() - sph_c) - _r*_r;		//(e-c)(e-c) - r*r

																		// if discriminant > 0 then compute the value of t0 and t1
	double det = B*B - A*C;
	if (det > 0) {
		double t1 = (-B - sqrt(det)) / (A);
		double t2 = (-B + sqrt(det)) / (A);

		if ((t1 >= 0) & (t2 > 0))
		{
			t = t1;
		}
	}

	return t;
}

//Calculates the normal for the given position on this sphere.
inline vec4 Sphere::calculateNormal(vec4 & position) {
	//http://www.ccs.neu.edu/home/fell/CSU540/programs/RayTracingFormulas.htm
	//Find the unit normal vector to the sphere at (x, y, z)
	// N = ((x - cx)/R, (y - cy)/R, (z - cz)/R)
	mat4 m_temp = _modelToWorld.transpose();
	vec4 sph_c = m_temp[3]; //sphere center

	vec4 unit_normal = (position - sph_c) / _r;
	//unit_normal = unit_normal* 
	return unit_normal;

}


Triangle::Triangle(vec3 a, vec3 b, vec3 c, RGB & col, Material & m, mat4 m2w) : Primitive(col,m,m2w) {
    verts[0] = a; verts[1] = b; verts[2] = c;
}


//*
double Triangle::intersect(Ray & ray) {
	//OPTIONAL EXTRA CREDIT YOUR CODE HERE
    IMPLEMENT_ME(__FILE__,__LINE__);
}
// */

vec4 Triangle::calculateNormal(vec4 & position) {
	//OPTIONAL EXTRA CREDIT YOUR CODE HERE
    IMPLEMENT_ME(__FILE__,__LINE__);
}