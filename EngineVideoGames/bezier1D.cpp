#include "bezier1D.h"
#include <string>
#include <iostream>

Bezier1D::Bezier1D(void)
{
	numberOfPoints = 10;

	glm::mat4 Model = glm::mat4(1.0);

	glm::vec4 Position0 = glm::vec4(0.0, 0.0, 0.0, 0.0);
	glm::vec4 Position1 = glm::vec4(1.0, 0.0, 0.0, 0.0);
	glm::vec4 Position2 = glm::vec4(2.0, 0.0, 0.0, 0.0);
	glm::vec4 Position3 = glm::vec4(3.0, 0.0, 0.0, 0.0);


	Model[0] = Position0;
	Model[1] = Position1;
	Model[2] = Position2;
	Model[3] = Position3;
	segments.push_back(Model);

	Position0 = glm::vec4(3.0, 0.0, 0.0, 0.0);
	Position1 = glm::vec4(4.0, 0.0, 0.0, 0.0);
	Position2 = glm::vec4(5.0, 0.0, 0.0, 0.0);
	Position3 = glm::vec4(6.0, 0.0, 0.0, 0.0);

	Model[0] = Position0;
	Model[1] = Position1;
	Model[2] = Position2;
	Model[3] = Position3;
	segments.push_back(Model);

	Position0 = glm::vec4(6.0, 0.0, 0.0, 0.0);
	Position1 = glm::vec4(7.0, 0.0, 0.0, 0.0);
	Position2 = glm::vec4(8.0, 0.0, 0.0, 0.0);
	Position3 = glm::vec4(9.0, 0.0, 0.0, 0.0);

	Model[0] = Position0;
	Model[1] = Position1;
	Model[2] = Position2;
	Model[3] = Position3;
	segments.push_back(Model);

}
IndexedModel Bezier1D::GetLine(int resT) {

	std::vector<LineVertex> axisVertices;
	glm::mat4 sector = segments[0];
	for (int s = 0; s < segments.size(); s++)
	{
		for (int i = 0; i < resT; i++)
		{
			double t = (double)i / (double)resT;
			axisVertices.push_back(GetVertex(s, t));
		}
	}
	axisVertices.push_back(GetVertex(segments.size() - 1, 1));
	std::vector<unsigned int> axisIndices;

	for (int i = 0; i < axisVertices.size() - 1; i++)
	{
		axisIndices.push_back(i);
		axisIndices.push_back(i + 1);
	}
	axisIndices.push_back(axisVertices.size());

	IndexedModel model;
	for (unsigned int i = 0; i < axisVertices.size(); i++)
	{
		model.positions.push_back(*axisVertices[i].GetPos());
		model.colors.push_back(*axisVertices[i].GetColor());

	}
	for (unsigned int i = 0; i < axisIndices.size(); i++)
		model.indices.push_back(axisIndices[i]);

	return model;

}

LineVertex Bezier1D::GetVertex(int segment, float t) {
	LineVertex CP3 = GetControlPoint(segment, 0);
	LineVertex CP2 = GetControlPoint(segment, 1);
	LineVertex CP1 = GetControlPoint(segment, 2);
	LineVertex CP0 = GetControlPoint(segment, 3);



	glm::vec3 myvertex(
		(CP0.GetPos()->x * t*t*t) + (CP1.GetPos()->x * 3 * t*t*(1 - t)) + (CP2.GetPos()->x * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->x * (1 - t)*(1 - t)*(1 - t)), // point x
		(CP0.GetPos()->y * t*t*t) + (CP1.GetPos()->y * 3 * t*t*(1 - t)) + (CP2.GetPos()->y * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->y * (1 - t)*(1 - t)*(1 - t)), //point y
		(CP0.GetPos()->z * t*t*t) + (CP2.GetPos()->z * 3 * t*t*(1 - t)) + (CP2.GetPos()->z * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->z * (1 - t)*(1 - t)*(1 - t))  // point z
	);

	LineVertex output(myvertex, glm::vec3(0, 0, 1));
	return output;

}
LineVertex Bezier1D::GetControlPoint(int seg, int index)
{
	if (seg == segments.size()) {
		seg--;
		index = 3;
	}
	glm::mat4 sector = segments[seg];
	glm::vec3 thepoint0(sector[index]);
	LineVertex output(thepoint0, glm::vec3(0, 0, 1));
	return output;
}

void Bezier1D::MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition) {
	if (segment == 0 && indx == 0)
		return;

	glm::mat4 sector = segments[segment];
	glm::vec4 oldpos(sector[indx]);
	glm::vec4 diff(newPosition - oldpos);



	segments[segment][indx] = newPosition;
	if (indx == 0)
		(segments[segment - 1])[3] = newPosition;
	if (preserveC1)
	{
		switch (indx)
		{
		case 0:
			segments[segment - 1][2] += diff;
			segments[segment][1] += diff;
			break;
		case 1:
			if (segment != 0)
				segments[segment - 1][2] -= diff;
			break;
		case 2:
			if (segment != segments.size() - 1)
				segments[segment + 1][1] -= diff;
			break;
		}

	}

}


glm::vec3  Bezier1D::GetVelosity(int segment, float t) {
	glm::vec3 CP3 = *GetControlPoint(segment, 3).GetPos();
	glm::vec3 CP2 = *GetControlPoint(segment, 2).GetPos();
	glm::vec3 CP1 = *GetControlPoint(segment, 1).GetPos();
	glm::vec3 CP0 = *GetControlPoint(segment, 0).GetPos();
	float tp0 = (1 - t)*(1 - t);
	float tp1 = -3 * t*t + 4 * t - 1;
	float tp2 = 3 * t*t - 2 * t;
	float tp3 = t * t;

	return CP3 * tp3 + CP2 * tp2 + CP1 * tp1 + CP0 * tp0;
	//	glm::vec3 myvertex(
	//	(CP0.GetPos()->x * 3 * t*t) + (CP1.GetPos()->x * 6 * t*t*(1 - t)) + (CP2.GetPos()->x * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->x * (1 - t)*(1 - t)*(1 - t)), // point x
	//	(CP0.GetPos()->y * t*t*t) + (CP1.GetPos()->y * 3 * t*t*(1 - t)) + (CP2.GetPos()->y * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->y * (1 - t)*(1 - t)*(1 - t)), //point y
	//	(CP0.GetPos()->z * t*t*t) + (CP2.GetPos()->z * 3 * t*t*(1 - t)) + (CP2.GetPos()->z * 3 * t*(1 - t)*(1 - t)) + (CP3.GetPos()->z * (1 - t)*(1 - t)*(1 - t))  // point z
	//);

	//return myvertex;
}

Bezier1D::~Bezier1D(void)
{
}
