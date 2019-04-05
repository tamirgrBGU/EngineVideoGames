#pragma once


#ifndef BEIZER2D_H
#define BEIZER2D_H
#include "bezier1D.h"
#define M_double_PI 2*M_PI
class Bezier2D
{
	int circularSubdivision; //(usualy 4) how many subdivision in circular direction
	Bezier1D b;
	vec3 axis;

public:
	Bezier2D(void);
	Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision);

	IndexedModel GetSurface(int resT, int resS);						//generates model for rendering using MeshConstructor::initMeshs
	Vertex GetVertex(int segmentT, int segmentS, float t, float s);		//returns point on surface in the requested segments for value of t and s
	glm::vec3 GetNormal(int segmentT, int segmentS, float t, float s);		//returns point on surface in the requested segments for value of t and s

	glm::mat4 *TestHelper();

	~Bezier2D(void);

private:
	void moveByVector(mat4 *seg, float x);

	static glm::vec4 calc_bezier_point_derivateS(glm::mat4 segmentT, glm::mat4 segmentS, float t, float s) {
		return Bezier1D::calc_bezier_point(t, segmentT)*Bezier1D::calc_bezier_point_derivate(s, segmentS);
	}

	static glm::vec4 calc_bezier_point_derivateT(glm::mat4 segmentT, glm::mat4 segmentS, float t, float s) {
		return Bezier1D::calc_bezier_point_derivate(t, segmentT)*Bezier1D::calc_bezier_point(s, segmentS);
	}

	static glm::vec4 calc_bezier_point2D(glm::mat4 segmentT, glm::mat4 segmentS, float t, float s) {
		return Bezier1D::calc_bezier_point(s, segmentS);
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


	glm::mat4 genSeg(vec4 radios, int segmentS);

	void dumpMat4(mat4 &a, int name) {
		printf("%d\n", name);
		for (int i = 0; i < 4; i++) {
			printf("<%f %f %f %f>\n", a[i].x, a[i].y, a[i].z, a[i].w);
		}
	}

	void initParts(mat4 *parts, glm::vec3 axis) {
		mat4 rotatorToX = glm::rotate((float) 90.0, glm::cross(vec3(0, 0, 1), axis));
		parts[0] = initSegmentPartOfCycle(circularSubdivision);
		rotateMat(&parts[0], &rotatorToX);

		for(int i = 0; i < 4; i++)
			parts[0][i].x = 0;

		float angle = (float) 360.0 / circularSubdivision;
		mat4 rotator = glm::rotate(angle, axis);

		for (int i = 1; i < circularSubdivision; i++) {
			parts[i] = parts[i - 1];
			rotateMat(&parts[i], &rotator);
		}
	}

	void scaleCircle(mat4 *parts, float newRad) {
		for (int i = 1; i < circularSubdivision; i++) {
			scaleMat(&parts[i], newRad);
		}
	}
};

#endif