#include "bezier2D.h"


Bezier2D::Bezier2D(void){}

Bezier2D::Bezier2D(Bezier1D &b, int circularSubdivision, vec3 shapeaxis, vec3 startPoint) {
	this->b = b;
	this->color = glm::vec3(1.f, 1.f, 1.f);
	this->first = startPoint;	this->axis = glm::normalize(shapeaxis);
	axismode = true;
	this->circularSubdivision = circularSubdivision;
	segmentCircleParts = new mat4[circularSubdivision];
	initParts(segmentCircleParts);
}

Bezier2D::Bezier2D(Bezier1D &b, int circularSubdivision) {
	this->b = b;
	this->color = glm::vec3(1.f, 1.f, 1.f);
	updateAxis();
	this->circularSubdivision = circularSubdivision;
	segmentCircleParts = new mat4[circularSubdivision];
	initParts(segmentCircleParts);
}

Bezier2D::~Bezier2D(void)
{
	if (segmentCircleParts != nullptr) {
		b.~Bezier1D();
		delete segmentCircleParts;
		segmentCircleParts = nullptr;
	}
}

inline void Bezier2D::prepSegSandPushToModel(IndexedModel& model, mat4**& surfaces, int segmentTindx, float tPart, float SEGtPart, int resS) {
	for (int segmentSindx = 0; segmentSindx < circularSubdivision; segmentSindx++) {
		for (int s = 0; s < resS; s++) {
			float sPart = (s + resS*segmentSindx) / float(resS*circularSubdivision);
			float SEGsPart = s / float(resS);
			vec4 pos = calc_bezier_point2D(surfaces[segmentSindx], SEGtPart, SEGsPart);
			vec3 pos3(pos.x, pos.y, pos.z);

			model.positions.push_back(pos3);
			model.normals.push_back(calc_bezier_point2D_get_normal(segmentTindx, pos3, tPart));
			model.colors.push_back(color);
			model.weights.push_back(calcWeight(segmentTindx, segmentSindx, tPart, sPart));
			model.texCoords.push_back(vec2(tPart, sPart));
		}
	}//we recycled points we need to have the closing points of the cycle again to prevent big diffrent value on texCoords
	vec4 pos = calc_bezier_point2D(surfaces[circularSubdivision - 1], SEGtPart, 1);
	vec3 pos3(pos.x, pos.y, pos.z);

	model.positions.push_back(pos3);
	model.normals.push_back(calc_bezier_point2D_get_normal(segmentTindx, pos3, tPart));
	model.colors.push_back(color);
	model.weights.push_back(calcWeight(segmentTindx, circularSubdivision - 1, tPart, 1));
	model.texCoords.push_back(vec2(tPart, 1));
}

void addIndicies(IndexedModel& model, int fullCycle) {
	int pointIndex = 0;
	int end = model.positions.size() - 1 - fullCycle;
	while (pointIndex < end) {
		int temp = 0;
		while (++temp < fullCycle) {
			int a = pointIndex, b = pointIndex++ + fullCycle, c = pointIndex, d = b + 1;
			model.indices.push_back(a);
			model.indices.push_back(b);
			model.indices.push_back(c);
			model.indices.push_back(c);
			model.indices.push_back(d);
			model.indices.push_back(b);
		}
		int a = pointIndex, b = pointIndex++ + fullCycle, d = pointIndex, c = d - fullCycle;
		model.indices.push_back(a);
		model.indices.push_back(b);
		model.indices.push_back(c);
		model.indices.push_back(c);
		model.indices.push_back(d);
		model.indices.push_back(b);
	}
}

IndexedModel Bezier2D::GetSurface(int resT, int resS) {
	IndexedModel model;
	updateAxis();
	mat4** surfaces = new mat4*[circularSubdivision];
	for (int i = 0; i < circularSubdivision; ++i)
		surfaces[i] = new mat4[SEG_CON_PTS];

	for (int segmentTindx = 0; segmentTindx < b.segNo(); segmentTindx++) {
		mat4 segmentT = b.GetSegment(segmentTindx);
		for (int segmentSindx = 0; segmentSindx < circularSubdivision; segmentSindx++) {
			gen_surface(surfaces[segmentSindx], segmentT, segmentSindx);
		}
		for (int t = 0; t < resT; t++)
			prepSegSandPushToModel(model, surfaces, segmentTindx, 
				(t + resT*segmentTindx) / float(resT*b.segNo() - 1), t / float(resT), resS);
	}

	prepSegSandPushToModel(model, surfaces, b.segNo()-1, 1, 1, resS);

	for (int i = 0; i < circularSubdivision; ++i)
		delete(surfaces[i]);
	delete(surfaces);

	addIndicies(model, resS*circularSubdivision + 1);

	return model;
}

const float YAxIntersection = 1.0f;
const float minPointY = 0.15f;
const float minPointX = 0.5f;
const float myBaseEdge = 0.2f;
const float cP = (YAxIntersection - myBaseEdge - minPointY)*0.5f;
const float aP = (cP - minPointY) / (minPointX*minPointX);
const float bP = -2 * minPointX * aP;
//we gonna divide the rest between next and current
/*glm::vec3 Bezier2D::calcWeight(int segmentT, int segmentS, float t, float s)
{
	if (segmentT == 0) {
		//float x = aP*t*t + bP*t + cP;
		return glm::vec3(0, 1 - 0.1f, 0.1f);
	}
	if (segmentT == b.segNo() - 1) {
		float x = aP*t*t + bP*t + cP;
		return glm::vec3(x, 1 - x, 0);
	}
	float f1 = 0, f3 = 0;
	float x = aP*t*t + bP*t + cP;
	float f2 = cP - x;
	if (t > 0.5) {
		f3 = x;
		f1 = 0.05f;
	}
	else {
		f3 = 0.05f;
		f1 = x;
	}
	return glm::vec3(f1, f2, f3);
}*/

//we gonna divide the rest between next and current
glm::vec3 Bezier2D::calcWeight(int segmentT, int segmentS, float t, float s)
{
	float f1 = 0, f3 = 0;
	if (t>0.5)
		f3 = (1.0f - 4.0f*(1.0f - t)*t)*(1.0f - t) / 2.0f + (1.0f - 4.0f*(1.0f - t)*t)*t / 2.0f;
	else
		f1 = (1.0f - 4.0f*(1.0f - t)*t)*(1.0f - t) / 2.0f + (1.0f - 4.0f*(1.0f - t)*t)*t / 2.0f;
	float f2 = (2.0f*(1.0f - t)*(t + 0.0f) + 0.5f);
	return glm::vec3(f1, f2, f3);
}

Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s) {
	mat4 segmentTBP = b.GetSegment(segmentT);
	mat4 surface[SEG_CON_PTS];
	gen_surface(surface, segmentTBP, segmentS);

	vec4 pos = calc_bezier_point2D(surface, t, s);
	vec3 pos3(pos.x, pos.y, pos.z);

	Vertex out(pos3, vec2(t, s), calc_bezier_point2D_get_normal(segmentT, pos3, t), color);
	return out;
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s) {
	mat4 segmentTBP = b.GetSegment(segmentT);
	mat4 surface[SEG_CON_PTS];
	gen_surface(surface, segmentTBP, segmentS);

	vec4 pos = calc_bezier_point2D(surface, t, s);
	vec3 pos3(pos.x, pos.y, pos.z);

	return calc_bezier_point2D_get_normal(segmentT, pos3, t);
}

void Bezier2D::moveByVector(mat4 *seg, vec3 toAdd) {
	for (int i = 0; i < SEG_CON_PTS; i++) {
		(*seg)[i].x += toAdd.x;
		(*seg)[i].y += toAdd.y;
		(*seg)[i].z += toAdd.z;
	}
}

void Bezier2D::gen_surface(mat4 *gen_surface, mat4 segmentT, int segmentS) {
	for (int i = 0; i < SEG_CON_PTS; i++) {
		vec3 p0 = Bezier1D::v4to3(segmentT[i]);

		vec3 radius = p0 - this->first;
		float angleToRotate = angle_mine_rad(radius, axis);
		float numericRad = glm::length(radius)*glm::sin(angleToRotate);
		radius = this->first + axis*(glm::length(radius)*glm::cos(angleToRotate));

		mat4 requiredSeg = segmentCircleParts[segmentS];
		scaleMat(&requiredSeg, numericRad);
		moveByVector(&requiredSeg, radius);
		for (int j = 0; j < SEG_CON_PTS; j++)
			gen_surface[i][j] = requiredSeg[j];		
	}
}

//perfectBall
IndexedModel Bezier2D::genBall(int resT, int resS, int circularSubdivision) {
	Bezier1D tempB1;
	Bezier2D B2(tempB1, circularSubdivision, vec3(1, 0, 0), vec3(0, 0, 0));
	mat4 rotator = glm::rotate(90.f, vec3(0, 1, 0));
	mat4 *segmentCircleParts = B2.segmentCircleParts;
	std::vector<mat4> segs;
	for (int i = 0; i < circularSubdivision/2; i++) {
		mat4 temp = segmentCircleParts[i];
		rotateMat(&temp, &rotator);
		segs.push_back(temp);
	}
	Bezier1D b1(segs);
	B2.b = b1;
	return B2.GetSurface(resT, resS);
}