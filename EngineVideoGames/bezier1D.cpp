#include "bezier1D.h"
#include <string>
#include <iostream>

Bezier1D::Bezier1D(void)
{

	glm::mat4 Model = glm::mat4(1.0);

	glm::vec4 Position0 = glm::vec4(0.0, 1.0, 0.0, 0.0);
	glm::vec4 Position1 = glm::vec4(1.0, 2.0, 0.0, 0.0);
	glm::vec4 Position2 = glm::vec4(2.0, 3.0, 0.0, 0.0);
	glm::vec4 Position3 = glm::vec4(0.0, 4.0, 0.0, 0.0);


	Model[0] = Position0;
	Model[1] = Position1;
	Model[2] = Position2;
	Model[3] = Position3;
	segments.push_back(Model);

	Position0 = glm::vec4(0.0, 4.0, 0.0, 0.0);
	Position1 = glm::vec4(1.0, 5.0, 0.0, 0.0);
	Position2 = glm::vec4(1.0, 6.0, 0.0, 0.0);
	Position3 = glm::vec4(0.0, 7.0, 0.0, 0.0);

	Model[0] = Position0;
	Model[1] = Position1;
	Model[2] = Position2;
	Model[3] = Position3;
	segments.push_back(Model);

	Position0 = glm::vec4(0.0, 7.0, 0.0, 0.0);
	Position1 = glm::vec4(-1.0, 8.0, 0.0, 0.0);
	Position2 = glm::vec4(-1.0, 9.0, 0.0, 0.0);
	Position3 = glm::vec4(0.0, 10.0, 0.0, 0.0);

	Model[0] = Position0;
	Model[1] = Position1;
	Model[2] = Position2;
	Model[3] = Position3;
	segments.push_back(Model);

}
IndexedModel Bezier1D::GetLine(int resT) {

	printf("%s\n", "test");
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

	std::vector<unsigned int> axisIndices;

	for (int i = 0; i < segments.size()*(resT - 1); i++)
	{
		axisIndices.push_back(i);
		axisIndices.push_back(i + 1);
	}


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
	
	segments[segment][indx][0] = newPosition[0];
	segments[segment][indx][1] = newPosition[1];
	segments[segment][indx][2] = newPosition[2];

	sector = segments[segment];

	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << sector[j][i] << " ";
		std::cout << std::endl;
	}

	//glm::mat4 transmat(1.0);


	/*transmat[0] = glm::vec4(0, 0, 0, 0);
	transmat[1] = glm::vec4(0, 0, 0, 0);
	transmat[2] = glm::vec4(0, 0, 0, 0);
	transmat[3] = glm::vec4(0, 0, 0, 0);
	transmat[indx] = diff;*/

	//segments[segment] = segments[segment] + transmat;
	//if (indx == 0)
	//	(segments[segment - 1])[3] = newPosition;
	//if (preserveC1)
	//{
	//	glm::vec4 diff(newPosition - oldpos);
	//	switch (indx)
	//	{
	//	case 0:
	//		segments[segment - 1][2] += diff;
	//		segments[segment][1] += diff;
	//		break;
	//	case 1:
	//		if (segment != 0)
	//			segments[segment - 1][2] -= diff;
	//		break;
	//	case 2:
	//		if (segment != segments.size())
	//			segments[segment + 1][1] -= diff;
	//		break;
	//	}

	//}

}

Bezier1D::~Bezier1D(void)
{
}
