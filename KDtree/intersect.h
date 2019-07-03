#pragma once
#include "kdtree.h"
#include <mesh.h>
#include <stdlib.h>
#include <list>

class intersect
{

public:
	//constructor
	intersect(std::vector<glm::vec3> shape);

	/*
	* Return the tree root node
	* will prepare minimal bounding box of intersection, if one exist. 
	* iterative function
	*/
	std::vector<IndexedModel> isIntersect(glm::mat4 *transMe, glm::mat4 *transOther, intersect other);

	/*
	* the model bounding box
	*/
	IndexedModel intersect::getBoundingBox();
	
	static vec4 v3to4(vec3 v) {
		return vec4(v.x, v.y, v.z, 1);
	}

	static vec3 v4to3(vec4 v) {
		return vec3(v.x, v.y, v.z);
	}
protected:
	std::vector<float> boundbox;
	std::vector<glm::vec3> boundboxvec;
	Kdtree kd;

private:
	std::vector<IndexedModel> makeBoxesIndexModels(std::vector<std::vector<glm::vec3>> intersect_boxes);

	bool isEqual(std::vector<glm::vec3> &boxvec, std::vector<glm::vec3> &boxvec2);
	//will not add duplicates
	void insert_box(std::vector<std::vector<glm::vec3>> *boxes, std::vector<glm::vec3> boxvec, glm::mat4 *transmat, glm::vec3 color);

	//will be used to find if separating panel is exist
	int isThereSeparatingPanel(std::vector<glm::vec3> box1, std::vector<glm::vec3> box2);

	void merge(std::vector<std::vector<glm::vec3>> *a, std::vector<std::vector<glm::vec3>> *b);

	int intersect::intersectWithOther(Node * nextother, int axis, std::vector<float> &boundingboxcopy);

	void nodesIntersectValitate(Node * next, int axis, Node * other, int depth, std::vector<std::vector<glm::vec3>> * output, std::vector<glm::vec3> &boxvec, std::vector<glm::vec3> &boxvec2);

	void intersect::rec_is_intersect(Node *current, Node *other,
		int depth, std::vector<std::vector<glm::vec3>> *output);

	std::vector<glm::vec3> bound_vec_to_boundbox(std::vector<float> boundbox);

	IndexedModel intersect::boxVertexesToIndexModel(std::vector<glm::vec3> intesect_box, glm::vec3 color);
};