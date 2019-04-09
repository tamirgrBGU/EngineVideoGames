#pragma once


#ifndef BEIZER2D_H
#define BEIZER2D_H
#include "bezier1D.h"
#include "stdio.h"
#include <iostream>//toremove
#define M_double_PI 2*M_PI
class Bezier2D
{
	int circularSubdivision; //(usualy 4) how many subdivision in circular direction
	Bezier1D b;
	vec3 axis;
	mat4 *radiusRotator;

public:
	Bezier2D(void);
	Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision);

	IndexedModel GetSurface(int resT, int resS);						//generates model for rendering using MeshConstructor::initMeshs
	Vertex GetVertex(int segmentT, int segmentS, float t, float s);		//returns point on surface in the requested segments for value of t and s
	glm::vec3 GetNormal(int segmentT, int segmentS, float t, float s);		//returns point on surface in the requested segments for value of t and s
	
	~Bezier2D(void);

private:

	void dumpMat4(mat4 &a, int name) {
		printf("%d\n", name);
		for (int i = 0; i < 4; i++) {
			printf("\t<%f %f %f %f>\n", a[i].x, a[i].y, a[i].z, a[i].w);
		}
	}

	void dumpVec3(vec3 &a) {
		printf("<%f %f %f>\n", a.x, a.y, a.z);
	}

	void dumpVec4(vec4 &a) {
		printf("<%f %f %f %f>\n", a.x, a.y, a.z, a.w);
	}

	void moveByVector(mat4 *seg, float x);

	glm::vec4 calc_bezier_point2D(glm::mat4* surfaceSeg, float u, float v) {
		vec4 location(0);
		for (int i = 0; i < SEG_CON_PTS; i++)
		{
			float temp_factor = b.calc_bezier_factor(i, u);
			for (int j = 0; j < SEG_CON_PTS; j++)
			{
				location = location +
				   (
					temp_factor*
					b.calc_bezier_factor(j, v)
					)*surfaceSeg[j][i];
			}
		}
		location.w = 1;
		return location;
	}

	//glm::vec3 calc_bezier_point2D_get_normal(glm::mat4* surfaceSeg, float u, float v) {
	//	/*printf("%f %f\n", u, v);
	//	dumpMat4(surfaceSeg[0], 1);
	//	dumpMat4(surfaceSeg[1], 2);
	//	dumpMat4(surfaceSeg[2], 3);
	//	dumpMat4(surfaceSeg[3], 4);*/
	//	vec4 dt = calc_bezier_point2D_velosity(surfaceSeg, v, u);
	//	vec3 dt3(dt.x, dt.y, dt.z);
	//	//dumpVec4(dt);
	//	vec4 ds = calc_bezier_point2D_velosity(surfaceSeg, u, v);
	//	vec3 ds3(ds.x, ds.y, ds.z);

	//	/*dumpVec4(ds);
	//	vec3 out = glm::cross(dt3, ds3);
	//	dumpVec3(out);
	//	int age;
	//	std::cin >> age;
	//	return out;*/
	//	return glm::cross(dt3, ds3);
	//}

	glm::vec3 normaliz(vec3 &pos) {
		float len = glm::length(pos);
		if (len > 0)
			return glm::normalize(pos);
		return pos;
	}

	glm::vec3 calc_bezier_point2D_get_normal(int segmentT, vec3 &pos, float u) {
		vec3 rad(0, pos.y, pos.z);
		vec3 sN = normaliz(glm::cross(rad, axis));
		vec3 tN = glm::normalize(b.GetVelosity(segmentT, u));
		vec3 out = glm::cross(tN, sN);
		return out;
	}
	

	//will derivate v
	glm::vec4 calc_bezier_point2D_velosity(glm::mat4* surfaceSeg, float u, float v) {
		vec4 velosity(0);
		for (int i = 0; i < SEG_CON_PTS; i++)
		{
			float temp_factor = b.calc_bezier_factor(i, u);
			for (int j = 0; j < SEG_CON_PTS; j++)
			{
				/*printf("%d %d %f %f\n", i, j, temp_factor, b.calc_bezier_factor_derivate(j, v));
				dumpVec4(surfaceSeg[j][i]);*/
				velosity = velosity +
				   (
					temp_factor *
					b.calc_bezier_factor_derivate(j, v)
					)*surfaceSeg[j][i];
				//dumpVec4(velosity);
			}
		}
		velosity.w = 0;
		return velosity;
	}

	static glm::mat4 initSegmentPartOfCycle(int circularSubdivision) {
		double half_angle = M_PI / circularSubdivision;
		double angle = 2.0*half_angle;
		double cosA = glm::cos(angle), sinA = glm::sin(angle);
		double c = 4 * (1 - glm::cos(half_angle)) / 3 / glm::sin(half_angle);
		return mat4x4(vec4(1.0, 0.0, 0.0, 1.0),
			vec4(1.0, c, 0.0, 1.0),
			vec4(cosA + c*sinA, sinA - c*cosA, 0.0, 1.0),
			vec4(cosA, sinA, 0.0, 1.0)
		);
	}

	static void rotateMat(mat4 *part, mat4 *rotator) {
		for (int i = 0; i < 4; i++) {
			(*part)[i] = (*part)[i] * (*rotator);
		}
	}

	static void scaleMat(mat4 *part, float scalar) {
		for (int i = 0; i < 4; i++) {
			(*part)[i] *= scalar;
			(*part)[i].w = 1;
		}
	}

	void gen_surface(mat4 *gen_surface, mat4 segmentT, int segmentS);

	mat4 angleRotator;
	void initParts(mat4 *parts, glm::vec3 axis) {
		mat4 rotatorToX = glm::rotate((float) 90.0, glm::cross(vec3(0, 0, 1), axis));
		parts[0] = initSegmentPartOfCycle(circularSubdivision);
		rotateMat(&parts[0], &rotatorToX);

		for (int i = 0; i < 4; i++)
			parts[0][i].x = 0;

		float angle = float(360.0 / circularSubdivision);
		angleRotator = glm::rotate(angle, axis);

		for (int i = 1; i < circularSubdivision; i++) {
			parts[i] = parts[i - 1];
			rotateMat(&parts[i], &angleRotator);
		}
	}

	void scaleCircle(mat4 *parts, float newRad) {
		for (int i = 1; i < circularSubdivision; i++) {
			scaleMat(&parts[i], newRad);
		}
	}
};

#endif