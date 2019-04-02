#include "bezier2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


Bezier2D::Bezier2D(void)
{
	b = Bezier1D();
	circularSubdivision = 4;
}
Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision) {
	this->b = b;
	this->circularSubdivision = circularSubdivision;
}

IndexedModel Bezier2D::GetSurface(int resT, int resS) {
	float t_step = 1 / (float)resT;
	float s_step = 1 / (float)resS;
	unsigned int i = 0;
	IndexedModel ans = IndexedModel();
	std::vector<glm::vec3> colors = std::vector<glm::vec3>();
	std::vector<glm::vec2> texCoords = std::vector<glm::vec2>();
	std::vector<glm::vec3> normals  = std::vector<glm::vec3>();
	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	std::vector<glm::vec3> weights = std::vector<glm::vec3>();
	std::vector<glm::ivec3> joint_indices = std::vector<glm::ivec3>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	for (int T=0; T<b.numOfSegments;T++){
		for (int S = 0; S < this->circularSubdivision; S++) {
			for (float s = 0; s < 1; s = s + s_step) {
				for (float t = 0; t < 1; t = t + t_step) {
					Vertex p0 = this->GetVertex(T, S, t, s);
					Vertex p1 = this->GetVertex(T, S, t + t_step, s);
					Vertex p2 = this->GetVertex(T, S, t + t_step, s+s_step);
					Vertex p3 = this->GetVertex(T, S, t , s + s_step);
					//push p0
					positions.push_back(*p0.GetPos());
					colors.push_back(*p0.GetColor());
					normals.push_back(*p0.GetNormal());
					texCoords.push_back(glm::vec2(0, 0));
					weights.push_back(glm::vec3(1,1,1)); // not sure
					joint_indices.push_back(glm::ivec3(1, 1, 1)); // not sure

					//push p1
					positions.push_back(*p1.GetPos());
					colors.push_back(*p1.GetColor());
					normals.push_back(*p1.GetNormal());
					texCoords.push_back(glm::vec2(1, 0));
					weights.push_back(glm::vec3(1, 1, 1)); // not sure
					joint_indices.push_back(glm::ivec3(1, 1, 1)); // not sure

					//push p2
					positions.push_back(*p2.GetPos());
					colors.push_back(*p2.GetColor());
					normals.push_back(*p2.GetNormal());
					texCoords.push_back(glm::vec2(1, 1));
					weights.push_back(glm::vec3(1, 1, 1)); // not sure
					joint_indices.push_back(glm::ivec3(1, 1, 1)); // not sure

					//push p3
					positions.push_back(*p3.GetPos());
					colors.push_back(*p3.GetColor());
					normals.push_back(*p3.GetNormal());
					texCoords.push_back(glm::vec2(0, 1));
					weights.push_back(glm::vec3(1, 1, 1)); // not sure
					joint_indices.push_back(glm::ivec3(1, 1, 1)); // not sure

					for (int j = 0; j < 4; j++) {
						indices.push_back(i);
						i++;
					}
				}
			}
		}
	}
	ans.positions = positions;
	ans.colors = colors;
	ans.texCoords = texCoords;
	ans.normals = normals;
	ans.weights = weights;
	ans.joint_indices = joint_indices;
	ans.indices = indices;
	return ans;
}



Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s) {
	glm::vec4 loc = glm::vec4(*b.GetVertex(segmentT,t).GetPos(), 1);
	float angle = (360 / circularSubdivision)*segmentS + (360 / circularSubdivision)*s;
	glm::vec4 newloc = loc * glm::rotate(angle, glm::vec3(1, 0, 0));
	return Vertex(glm::vec3(newloc), glm::vec2(0, 0), GetNormal(segmentT, segmentS, t, s), *b.GetVertex(segmentT, t).GetColor());
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s) {
	glm::vec4 normal_2d = glm::vec4(glm::cross(this->b.GetVelosity(segmentT, t), glm::vec3(0, 0, 1)),0);
	float angle = (360 / circularSubdivision)*segmentS + (360 / circularSubdivision)*s;
	glm::vec4 normal_3d = normal_2d * glm::rotate(angle, glm::vec3(1, 0, 0));
	return glm::vec3(normal_3d);
}

Bezier2D::~Bezier2D(void)
{
}
