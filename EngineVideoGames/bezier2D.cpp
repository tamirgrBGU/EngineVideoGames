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

glm::vec3 color(glm::vec3(0.2f, 0.1f, 0.90f));
IndexedModel Bezier2D::GetSurface(int resT, int resS) {

	IndexedModel model;
	mat4** surfaces = new mat4*[circularSubdivision];
	for (int i = 0; i < circularSubdivision; ++i)
		surfaces[i] = new mat4[SEG_CON_PTS];

	for (int segmentTindx = 0; segmentTindx < b.segNo(); segmentTindx++) {
		mat4 segmentT = b.GetSegment(segmentTindx);
		for (int segmentSindx = 0; segmentSindx < circularSubdivision; segmentSindx++) {
			gen_surface(surfaces[segmentSindx], segmentT, segmentSindx);
		}
		for (int t = 0; t < resT; t++) {
			float tPart = t / float(resT);
			for (int segmentSindx = 0; segmentSindx < circularSubdivision; segmentSindx++) {
				for (int s = 0; s < resS; s++) {
					float sPart = s / float(resS);
					vec4 pos = calc_bezier_point2D(surfaces[segmentSindx], tPart, sPart);
					vec3 pos3(pos.x, pos.y, pos.z);
					//printf("%d %d %f %f <%f %f %f>\n", segmentTindx, segmentSindx, tPart, sPart, pos.x, pos.y, pos.z);
					model.positions.push_back(pos3);
					model.colors.push_back(color);
					vec3 normal(0, pos.y, pos.z);
					model.normals.push_back(normal);// calc_bezier_point2D_get_normal(surfaces[segmentSindx], tPart, sPart));
					model.texCoords.push_back(vec2(tPart, sPart));
				}
			}
			//printf("%d/%d.%d\n", t, resT, segmentTindx);
		}
	}

	for (int segmentSindx = 0; segmentSindx < circularSubdivision; segmentSindx++) {
		for (int s = 0; s < resS; s++) {
			float sPart = s / float(resS);
			vec4 pos = calc_bezier_point2D(surfaces[segmentSindx], 1, sPart);
			vec3 pos3(pos.x, pos.y, pos.z);
			//printf("%d %d %f %f <%f %f %f>\n", segmentTindx, segmentSindx, tPart, sPart, pos.x, pos.y, pos.z);
			model.positions.push_back(pos3);
			model.colors.push_back(color);
			vec3 normal(0, pos.y, pos.z);
			model.normals.push_back(normal);// calc_bezier_point2D_get_normal(surfaces[segmentSindx], 1, sPart));
			model.texCoords.push_back(vec2(1, sPart));
		}
	}

	for (int i = 0; i < circularSubdivision; ++i)
		delete(surfaces[i]);
	delete(surfaces);

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
	mat4 surface[SEG_CON_PTS];
	gen_surface(surface, segmentTBP, segmentS);

	vec4 pos = calc_bezier_point2D(surface, t, s);
	vec3 pos3(pos.x, pos.y, pos.z);

	Vertex out(pos3, vec2(t, s), calc_bezier_point2D_get_normal(surface, t, s), color);
	return out;
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s) {
	mat4 segmentTBP = b.GetSegment(segmentT);
	mat4 surface[SEG_CON_PTS];
	gen_surface(surface, segmentTBP, segmentS);

	return calc_bezier_point2D_get_normal(surface, t, s);
}

void Bezier2D::moveByVector(mat4 *seg, float toAdd) {
	for (int i = 0; i < SEG_CON_PTS; i++) {
		(*seg)[i].x += toAdd;
	}
}

void Bezier2D::gen_surface(mat4 *gen_surface, mat4 segmentT, int segmentS) {
	for (int i = 0; i<SEG_CON_PTS; i++) {
		vec4 radius = segmentT[i];
		vec3 v3(0, radius.y, radius.z);
		float numericRad = glm::length(v3);
		mat4 requiredSeg = segmentCircleParts[segmentS];
		scaleMat(&requiredSeg, numericRad);
		moveByVector(&requiredSeg, radius.x);
		for (int j = 0; j<SEG_CON_PTS; j++) {
			gen_surface[j][i] = requiredSeg[j];
		}
	}
	//dumpMat4(*gen_surface, segmentS);
}