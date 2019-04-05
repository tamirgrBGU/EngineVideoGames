#include "bezier2D.h"


Bezier2D::Bezier2D(void)
{
}

mat4 *segmentCircleParts;
Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision) {
	this->b = b;
	this->axis = axis;
	this->circularSubdivision = circularSubdivision;
	segmentCircleParts = new mat4[circularSubdivision];
	initParts(segmentCircleParts, axis);
}

Bezier2D::~Bezier2D(void)
{
	b.~Bezier1D();
	if (segmentCircleParts != nullptr)
		free(segmentCircleParts);
}

glm::mat4 *Bezier2D::TestHelper() {
	return segmentCircleParts;
}

IndexedModel Bezier2D::GetSurface(int resT, int resS) {
	IndexedModel model;
	for (int segmentTindx = 0; segmentTindx < b.segNo(); segmentTindx++) {
		mat4 segmentT = b.GetSegment(segmentTindx);
		for (int t = 0; t < resT + 1; t++) {
			for (int segmentSindx = 0; segmentSindx < circularSubdivision; segmentSindx++) {
				float tPart = t / ((float)resT);
				mat4 segmentS = genSeg(this->b.calc_bezier_point(tPart, segmentT), segmentSindx);
				vec4 vx4= this->b.calc_bezier_point(tPart, segmentT);
				for (int s = 0; s < resS; s++) {
					float sPart = s / ((float)resS);
					vec4 pos = calc_bezier_point2D(segmentT, segmentS, tPart, sPart);
					vec3 pos3(pos.x, pos.y, pos.z);
					
					vec4 ds = calc_bezier_point_derivateS(segmentT, segmentS, tPart, sPart);
					vec3 ds3(ds.x, ds.y, ds.z);

					vec4 dt = calc_bezier_point_derivateT(segmentT, segmentS, tPart, sPart);
					vec3 dt3(dt.x, dt.y, dt.z);

					model.positions.push_back(pos3);
					model.colors.push_back(vec3(1.0f, 0.5f, 0.31f));
					model.normals.push_back(glm::cross(ds3, dt3));
					model.texCoords.push_back(vec2(t,s));
				}
			}
		}
	}

	int pointIndex = 0;
	int fullCycle = resS*circularSubdivision;
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

	//while (true);
	return model;
}						//generates model for rendering using MeshConstructor::initMeshs

Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s) {
	mat4 segmentTBP = b.GetSegment(segmentT);
	mat4 segmentSBP = genSeg(this->b.calc_bezier_point(t, segmentTBP), segmentS);

	vec4 pos = calc_bezier_point2D(segmentSBP, segmentSBP, t, s);
	vec3 pos3(pos.x, pos.y, pos.z);

	vec4 ds = calc_bezier_point_derivateS(segmentSBP, segmentSBP, t, s);
	vec3 ds3(ds.x, ds.y, ds.z);
	vec4 dt = calc_bezier_point_derivateT(segmentTBP, segmentSBP, t, s);
	vec3 dt3(dt.x, dt.y, dt.z);

	Vertex out(pos3, vec2(0), glm::cross(ds3, dt3), vec3(0));
	return out;
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s) {
	mat4 segmentTBP = b.GetSegment(segmentT);
	mat4 segmentSBP = genSeg(this->b.calc_bezier_point(t, segmentTBP), segmentS);

	vec4 ds = calc_bezier_point_derivateS(segmentSBP, segmentSBP, t, s);
	vec3 ds3(ds.x, ds.y, ds.z);
	vec4 dt = calc_bezier_point_derivateT(segmentTBP, segmentSBP, t, s);
	vec3 dt3(dt.x, dt.y, dt.z);
	return glm::cross(ds3, dt3);
}

void Bezier2D::moveByVector(mat4 *seg, float toAdd) {
	for (int i = 0; i < 4; i++) {
		(*seg)[i].x += toAdd;
	}
}

glm::mat4 Bezier2D::genSeg(vec4 radius, int segmentS) {
	vec3 v3(0, radius.y, radius.z);
	float numericRad = glm::length(v3);
	mat4 requiredSeg;
	requiredSeg=(segmentCircleParts[segmentS]);
	scaleMat(&requiredSeg, numericRad);
	/*printf("_%f_%f_<%f, %f, %f, %f> <%f, %f, %f, %f> <%f, %f, %f, %f> <%f, %f, %f, %f>\n", numericRad, radius.x, requiredSeg[0].x, requiredSeg[0].y, requiredSeg[0].z, requiredSeg[0].w,
		requiredSeg[1].x, requiredSeg[1].y, requiredSeg[1].z, requiredSeg[1].w, requiredSeg[2].x, requiredSeg[2].y, requiredSeg[2].z, requiredSeg[2].w, requiredSeg[3].x, requiredSeg[3].y, requiredSeg[3].z, requiredSeg[3].w);*/
	moveByVector(&requiredSeg, radius.x);
	return requiredSeg;
}