#pragma once


#ifndef BEIZER2D_H
#define BEIZER2D_H
#include "bezier1D.h"
#include "stdio.h"
#define M_double_PI 2*M_PI
class Bezier2D
{
	int circularSubdivision; //(usualy 4) how many subdivision in circular direction
	Bezier1D b;
	vec3 axis;
	vec3 first;
	vec3 last;

public:
	Bezier2D(void);
	Bezier2D(Bezier1D &b, int circularSubdivision);

	IndexedModel GetSurface(int resT, int resS);						//generates model for rendering using MeshConstructor::initMeshs
	Vertex GetVertex(int segmentT, int segmentS, float t, float s);		//returns point on surface in the requested segments for value of t and s
	glm::vec3 GetNormal(int segmentT, int segmentS, float t, float s);		//returns point on surface in the requested segments for value of t and s
	
	void updateAxis() {
		this->first = Bezier1D::v4to3(b.GetSegment(0)[0]);
		this->last  = Bezier1D::v4to3(b.GetSegment(b.segNo() - 1)[3]);
		this->axis  = glm::normalize(this->last - this->first);
	}

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

	void moveByVector(mat4 *seg, vec3 x);

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
					)*surfaceSeg[i][j];
			}
		}
		location.w = 1;
		return location;
	}

	glm::vec3 normaliz(vec3 &normal) {
		float len = glm::length(normal);
		if (len > 0)
			return glm::normalize(normal);
		return normal;
	}

	glm::vec3 calc_bezier_point2D_get_normal(int segmentT, vec3 &pos, float u) {
		vec3 radius = pos - this->first;
		//if (std::isnan(pos.x)) printf("yo");//bad thing
		float angleToRotate = angle_mine_rad(radius, axis);
		if (!std::isnan(angleToRotate)) {
			radius = pos - axis*(glm::length(radius)*glm::cos(angleToRotate));
			vec3 sN = normaliz(glm::cross(radius, axis));
			vec3 tN = normaliz(b.GetVelosity(segmentT, u));
			return glm::cross(tN, sN);
		}
		return vec3(0);
	}
	

	//will derivate v
	glm::vec4 calc_bezier_point2D_velosity(glm::mat4* surfaceSeg, float u, float v) {
		vec4 velosity(0);
		for (int i = 0; i < SEG_CON_PTS; i++)
		{
			float temp_factor = b.calc_bezier_factor(i, u);
			for (int j = 0; j < SEG_CON_PTS; j++)
			{
				velosity = velosity +
				   (
					temp_factor *
					b.calc_bezier_factor_derivate(j, v)
					)*surfaceSeg[i][j];
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
	
	float multParams(vec3 Axis, vec3 rotateBy) {
		return Axis.x * rotateBy.x + Axis.y * rotateBy.y + Axis.z * rotateBy.z;
	}

	float nan = std::sqrt(-1);
	float angle_mine_rad(vec3 v1, vec3 v2) {
		if (std::isnan(v1.x) || std::isnan(v2.x))
			return nan;
		v1 = normaliz(v1);
		v2 = normaliz(v2);
		return glm::acos(multParams(v1, v2));
	}

	float angle_mine_deg(vec3 v1, vec3 v2) {
		float lens = glm::length(v1) * glm::length(v2);
		if (lens == 0)
			return 0;
		float angleToRotate = glm::acos(multParams(v1, v2) / lens);
		return glm::degrees(angleToRotate);
	}

	void rotataByAxis(vec3 Axis, vec3 rotateBy, mat4 *toRotate) {
		float angleToRotate = angle_mine_deg(Axis,rotateBy);

		if (angleToRotate > 0) {
			mat4 rotator = glm::rotate(angleToRotate, glm::cross(Axis, rotateBy));
			rotateMat(toRotate, &rotator);
		}
	}

	mat4 angleRotator;
	vec3 xAxis = vec3(1, 0, 0);
	vec3 yAxis = vec3(0, 1, 0);
	vec3 zAxis = vec3(0, 0, 1);
	void initParts(mat4 *parts, glm::vec3 axis) {
		parts[0] = initSegmentPartOfCycle(circularSubdivision);
		//rotataByAxis(xAxis, axis, &parts[0]);
		//rotataByAxis(yAxis, axis, &parts[0]);
		rotataByAxis(zAxis, axis, &parts[0]);
		
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