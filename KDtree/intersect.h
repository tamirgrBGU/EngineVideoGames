#pragma once
#include "kdtree.h"
#include <mesh.h>


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
	std::vector<IndexedModel> isIntersect(glm::mat4 transMe, glm::mat4 transOther, intersect other);

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
	void insert_box(std::vector<std::vector<glm::vec3>> &boxes, std::vector<glm::vec3> boxvec);

	//will minimize the boxes that is not intesect with others
	void checkminiboxes(std::vector<std::vector<glm::vec3>> &intersect_boxes1, std::vector<std::vector<glm::vec3>> &intersect_boxes2, glm::mat4 transMe, glm::mat4 transOther);
	void finilizeBoxes(std::vector<std::vector<glm::vec3>> &intersect_boxes1, std::vector<std::vector<glm::vec3>> &intersect_boxes2);

	//will be use to find the first bounding box
	std::vector<float> findthightbox(std::vector<glm::vec3> positions);

	//will be used to find if separating panel is exist
	int isThereSeparatingPanel(std::vector<glm::vec3> box1, std::vector<glm::vec3> box2);

	void merge(std::vector<std::vector<glm::vec3>> *a, std::vector<std::vector<glm::vec3>> *b);

	std::vector<std::vector<glm::vec3>> rec_is_intersect(Node *current, std::vector<float> *boundingbox, std::vector<glm::vec3> *intersectwith, int depth);

	std::vector<glm::vec3> bound_vec_to_boundbox(std::vector<float> boundbox);

	IndexedModel intersect::boxVertexesToIndexModel(std::vector<glm::vec3> intesect_box);
};