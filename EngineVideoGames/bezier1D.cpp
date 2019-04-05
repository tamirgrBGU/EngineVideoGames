#include "bezier1D.h"

Bezier1D::Bezier1D(void)
{
	mat4 seg0 = mat4(0);
	seg0[0] = vec4(0, 0, -3, 1);
	seg0[1] = vec4(1, 1, -2, 1);
	seg0[2] = vec4(2, 1, -1, 1);
	seg0[3] = vec4(3, 0, 0, 1);
	/*seg0[0] = vec4(0, 1, 0, 1);
	seg0[1] = vec4(1, 1, 0, 1);
	seg0[2] = vec4(2, 1, 0, 1);
	seg0[3] = vec4(3, 1, 0, 1);*/
	mat4 seg1 = mat4(0);
	seg1[0] = vec4(3, 0, 0, 1);
	seg1[1] = vec4(4, -1, 1, 1);
	seg1[2] = vec4(5, -1, 2, 1);
	seg1[3] = vec4(6, 0, 5, 1);
	/*seg1[0] = vec4(3, 1, 0, 1);
	seg1[1] = vec4(4, 1, 0, 1);
	seg1[2] = vec4(5, 1, 0, 1);
	seg1[3] = vec4(6, 1, 0, 1);*/
	segments.push_back(seg0);
	segments.push_back(seg1);
}

float randomTillOne() {
	return ((float)rand()) / ((float)RAND_MAX);
}

vec3 randomVec() {
	return vec3(rand() % 20, rand() % 20, rand() % 20);
}

IndexedModel Bezier1D::GetLine(int resT) {
	IndexedModel model;

	for (unsigned int j = 0; j<segments.size(); j++) {
		for (int i = 0; i<resT + 1; i++) {
			vec4 v4 = calc_bezier_point(i / ((float)resT), segments[j]);
			vec3 beizeLine(v4.x, v4.y, v4.z);
			model.positions.push_back(beizeLine);
			model.colors.push_back(randomVec());
		}
	}

	model.indices.push_back(0);

	for (unsigned int i = 1; i<model.positions.size()-1; i++) {
		model.indices.push_back(i);
		model.indices.push_back(i);
	}

	if (resT>1) {
		model.indices.push_back(model.positions.size()-1);
	}

	return model;
}

LineVertex Bezier1D::GetVertex(int segment, float t) {
	vec4 v4 = calc_bezier_point(t, segments[segment]);
	vec3 beizeLine(v4.x, v4.y, v4.z);
	return LineVertex(beizeLine, vec3(0));
}

LineVertex Bezier1D::GetControlPoint(int segment, int indx) {
	vec4 v4 = segments[segment][indx];
	vec3 beizeLine(v4.x, v4.y, v4.z);
	return LineVertex(beizeLine, vec3(0));
}

glm::mat4 Bezier1D::GetSegment(int segment) {
	return segments[segment];
}

glm::vec3 Bezier1D::GetVelosity(int segment, float t) {
	vec4 v4 = calc_bezier_point_derivate(t, segments[segment]);
	return vec3(v4.x, v4.y, v4.z);
}

void Bezier1D::moveFixedPoint(int segmentA, int segmentB, glm::vec4 newPosition) {
	if (!checkSegment(segmentA, (unsigned)segmentB)) return;
	vec4 delta = newPosition - segments[segmentA][3];
	segments[segmentA][2].operator+=(delta);
	segments[segmentB][1].operator+=(delta);
	segments[segmentA][3] = newPosition;
	segments[segmentB][0] = newPosition;
}

void Bezier1D::moveNonFixedPoint(int segmentA, int segmentB, int changedIndx, glm::vec4 newPosition) {
	if (!checkSegment(segmentA, (unsigned)segmentB)) return;
	vec4 A, B;
	if (changedIndx == 1) {
		A = segments[segmentA][2];
		B = newPosition;
	}
	else {//changedIndx == 3
		A = newPosition;
		B = segments[segmentB][1];
	}

	vec4 newMiddle = (A += B) *= 0.5;
	segments[segmentA][3] = (newMiddle);
	segments[segmentB][0] = (newMiddle);
}

bool Bezier1D::checkSegment(int segmentA, unsigned int segmentB) {
	if (segmentA != -2) {
		if (segmentA < 0)
			return false;

		if (segmentB >= segments.size())
			return false;
	}
	return true;
}
void Bezier1D::updateSegment(int segment, int indx, glm::vec4 newPosition) {
	int segmentA = -2;
	unsigned int segmentB;
	if (indx == 0) {
		segmentA = segment - 1; segmentB = segment;
	}
	else if (indx == 1) {
		segments[segment][indx] = newPosition;
	}
	else if (indx == 2) {
		segments[segment][indx] = newPosition;
	}
	else if (indx == 3) {
		segmentA = segment; segmentB = segment + 1;
	}
	if (segmentA != -2) {
		if (segmentA < 0) {
			segments[segmentB][0] = newPosition;
			return;
		}
		if (segmentB >= segments.size()) {
			segments[segmentA][3] = newPosition;
			return;
		}

		segments[segmentA][3] = newPosition;
		segments[segmentB][0] = newPosition;
	}
}

void Bezier1D::MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition) {
	if (preserveC1) {
		if (indx == 0) {
			moveFixedPoint(segment - 1, segment, newPosition);
		}
		else if (indx == 1) {
			moveNonFixedPoint(segment - 1, segment, indx, newPosition);
		}
		else if (indx == 2) {
			moveNonFixedPoint(segment, segment + 1, indx, newPosition);
		}
		else if (indx == 3) {
			moveFixedPoint(segment, segment + 1, newPosition);
		}
	}
	updateSegment(segment, indx, newPosition);
}

Bezier1D::~Bezier1D(void)
{
	segments.clear();
	segments.~vector();
}