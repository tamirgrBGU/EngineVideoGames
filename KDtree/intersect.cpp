#include "intersect.h"
#include <stdlib.h>
#include <list>

std::vector<float> findthightbox(std::vector<glm::vec3> positions) {
	std::vector<float> critical_point;
	critical_point.push_back(positions[0].x);	critical_point.push_back(positions[0].x);
	critical_point.push_back(positions[0].y);	critical_point.push_back(positions[0].y);
	critical_point.push_back(positions[0].z);	critical_point.push_back(positions[0].z);

	for (unsigned int i = 1; i < positions.size(); i++) {
		if (critical_point[0] > positions[i].x)
			critical_point[0] = positions[i].x;
		if (critical_point[1] < positions[i].x)
			critical_point[1] = positions[i].x;
		if (critical_point[2] > positions[i].y)
			critical_point[2] = positions[i].y;
		if (critical_point[3] < positions[i].y)
			critical_point[3] = positions[i].y;
		if (critical_point[4] > positions[i].z)
			critical_point[4] = positions[i].z;
		if (critical_point[5] < positions[i].z)
			critical_point[5] = positions[i].z;
	}

	return critical_point;
}

int isThereSeparatingPanel(std::vector<glm::vec3> box1, std::vector<glm::vec3> box2) {
	glm::vec3 PA = (box1[1] + box1[7]) / 2.0f;//coordinate position of the center of A
	glm::vec3 Ax = box1[2] - box1[1];//unit vector representing the x - axis of A
	glm::vec3 Ay = box1[5] - box1[1];// unit vector representing the y - axis of A
	glm::vec3 Az = box1[0] - box1[1];// unit vector representing the z - axis of A
	float WA = glm::length(Ax);// half width of A(corresponds with the local x - axis of A)
	Ax = Ax / WA;//normalize
	WA /= 2.0f;
	float HA = glm::length(Ay);//half height of A(corresponds with the local y - axis of A)
	Ay = Ay / HA;
	HA /= 2.0f;
	float DA = glm::length(Az);//half depth of A(corresponds with the local z - axis of A)
	Az = Az / DA;
	DA /= 2.0f;
	glm::vec3 PB = (box2[1] + box2[7]) / 2.0f;//coordinate position of the center of B
	glm::vec3 Bx = box2[2] - box2[1];//unit vector representing the x - axis of B
	glm::vec3 By = box2[5] - box2[1];//unit vector representing the y - axis of B
	glm::vec3 Bz = box2[0] - box2[1];//unit vector representing the z - axis of B
	float WB = glm::length(Bx);// half width of B(corresponds with the local x - axis of B)
	Bx = Bx / WB;//normalize
	WB /= 2.0f;
	float HB = glm::length(By);// half height of B(corresponds with the local y - axis of B)
	By = By / HB;
	HB /= 2.0f;
	float DB = glm::length(Bz);// half depth of B(corresponds with the local z - axis of B)
	Bz = Bz / DB;
	DB /= 2.0f;
	glm::vec3 T = PB - PA;
	float cond, res;
	// L = Ax
	cond = abs(glm::dot(T, Ax));
	res = WA + WB * abs(glm::dot(Ax, Bx)) + HB * abs(glm::dot(Ax, By)) + DB * abs(glm::dot(Ax, Bz));
	if (cond > res)
		return 1;
	// L = Ay
	cond = abs(glm::dot(T, Ay));
	res = HA + WB * abs(glm::dot(Ay, Bx)) + HB * abs(glm::dot(Ay, By)) + DB * abs(glm::dot(Ay, Bz));
	if (cond > res)
		return 2;
	// L = Az
	cond = abs(glm::dot(T, Az));
	res = DA + WB * abs(glm::dot(Az, Bx)) + HB * abs(glm::dot(Az, By)) + DB * abs(glm::dot(Az, Bz));
	if (cond > res)
		return 3;
	// L = Bx
	cond = abs(glm::dot(T, Bx));
	res = WB + WA * abs(glm::dot(Ax, Bx)) + HA * abs(glm::dot(Ay, Bx)) + DA * abs(glm::dot(Az, Bx));
	if (cond > res)
		return 4;
	// L = By
	cond = abs(glm::dot(T, Ay));
	res = HB + WA * abs(glm::dot(Ax, By)) + HA * abs(glm::dot(Ay, By)) + DA * abs(glm::dot(Az, By));
	if (cond > res)
		return 5;
	// L = Bz
	cond = abs(glm::dot(T, Az));
	res = DB + WA * abs(glm::dot(Ax, Bz)) + HA * abs(glm::dot(Ay, Bz)) + DA * abs(glm::dot(Az, Bz));
	if (cond > res)
		return 6;
	// L = Ax x Bx
	glm::vec3 AxCrsBx = glm::cross(Ax, Bx);
	cond = abs(glm::dot(T, AxCrsBx));
	res  = WA * abs(glm::dot(Ax, AxCrsBx)) + HA * abs(glm::dot(Ay, AxCrsBx)) + DA * abs(glm::dot(Az, AxCrsBx)) +
		   WB * abs(glm::dot(Bx, AxCrsBx)) + HB * abs(glm::dot(By, AxCrsBx)) + DB * abs(glm::dot(Bz, AxCrsBx));
	if (cond > res)
		return 7;
	return 0;
}

//std::vector<glm::vec3> floats_to_vec(std::vector<float> boundbox) {
//	std::vector<glm::vec3> vec;
//	vec.push_back(vec3(boundbox[0], 0, 0));
//	vec.push_back(vec3(boundbox[1], 0, 0));
//	vec.push_back(vec3(0, boundbox[2], 0));
//	vec.push_back(vec3(0, boundbox[3], 0));
//	vec.push_back(vec3(0, 0, boundbox[4]));
//	vec.push_back(vec3(0, 0, boundbox[5]));
//	return vec;
//}

std::vector<glm::vec3> bound_vec_to_boundbox(std::vector<float> boundbox) {
	std::vector<glm::vec3> vec;
	vec.push_back(vec3(boundbox[0], boundbox[2], boundbox[5]));
	vec.push_back(vec3(boundbox[0], boundbox[2], boundbox[4]));
	vec.push_back(vec3(boundbox[1], boundbox[2], boundbox[4]));
	vec.push_back(vec3(boundbox[1], boundbox[2], boundbox[5]));
	vec.push_back(vec3(boundbox[0], boundbox[3], boundbox[5]));
	vec.push_back(vec3(boundbox[0], boundbox[3], boundbox[4]));
	vec.push_back(vec3(boundbox[1], boundbox[3], boundbox[4]));
	vec.push_back(vec3(boundbox[1], boundbox[3], boundbox[5]));
	return vec;
}

void merge(std::vector<std::vector<glm::vec3>> *a, std::vector<std::vector<glm::vec3>> *b) {
	for (int i = 0; i < b->size(); i++) 
		a->push_back((*b)[i]);
}

std::vector<std::vector<glm::vec3>> rec_is_intersect(Node *current, std::vector<float> *boundingbox, std::vector<glm::vec3> *intersectwith, int depth) {
	std::vector<std::vector<glm::vec3>> output;
	//printf("depth %d\n", depth);

	int axis = depth % 3;
	std::vector<glm::vec3> boxvec = bound_vec_to_boundbox(*boundingbox);
	std::vector<float> boundingboxcopy = *boundingbox;
	bool done_flag = 0;

	Node *next = current->right;
	if (!next) {
		//printf("2");
		output.push_back(boxvec);
		if (!(current->left))
			return output;
	}
	else {
		boundingboxcopy[axis * 2] = current->data[axis];
		int res = isThereSeparatingPanel(bound_vec_to_boundbox(boundingboxcopy), *intersectwith);
		//printf("resa %d\n", res);
		if (res == 0) {
			if (boundingboxcopy[axis * 2] == boundingboxcopy[axis * 2 + 1]) {
				done_flag = 1;
				//printf("0");
				//output.push_back(boxvec);//will be done by max places that will get to null continuation afterwards
			}
			else {
				//printf("4");
				std::vector<std::vector<glm::vec3>> outa = rec_is_intersect(next, &boundingboxcopy, intersectwith, depth + 1);
				//printf("1");
				merge(&output, &outa);
			}
		}
	}


	next = current->left;
	if (!next) {
		//printf("3");
		output.push_back(boxvec);
	}
	else {
		boundingboxcopy[axis * 2]	  = (*boundingbox)[axis * 2];//retrive the last value
		boundingboxcopy[axis * 2 + 1] = current->data[axis];
		int res = isThereSeparatingPanel(bound_vec_to_boundbox(boundingboxcopy), *intersectwith);
		//printf("resb %d\n", res);
		if (res == 0) {
			if (boundingboxcopy[axis * 2] == boundingboxcopy[axis * 2 + 1]) {
				//if (done_flag) return output;
				//printf("0");
				//output.push_back(boxvec);//will be done by max places that will get to null continuation afterwards
			}
			else { 
				//printf("5");
				std::vector<std::vector<glm::vec3>> outb = rec_is_intersect(next, &boundingboxcopy, intersectwith, depth + 1);
				//printf("1");
				merge(&output, &outb);
			}
		}
	}

	return output;
}

std::vector<std::vector<glm::vec3>> areIntersecting(std::vector<glm::vec3> shape1, std::vector<glm::vec3> shape2) {
	std::vector<float> boundbox1 = findthightbox(shape1);
	std::vector<float> boundbox2 = findthightbox(shape2);

	std::vector<glm::vec3> boundboxvec1 = bound_vec_to_boundbox(boundbox1);
	std::vector<glm::vec3> boundboxvec2 = bound_vec_to_boundbox(boundbox2);

	int res = isThereSeparatingPanel(boundboxvec1, boundboxvec2);
	//printf("res %d\n", res);
	if (res > 0)
		return std::vector<std::vector<glm::vec3>>();

	Kdtree kd;	kd.makeTree(shape1);
	Kdtree kd2;	kd2.makeTree(shape2);

	std::vector<std::vector<glm::vec3>> intersect_boxes1 = rec_is_intersect(kd.getRoot(), &boundbox1, &boundboxvec2, 0);
	std::vector<std::vector<glm::vec3>> intersect_boxes2 = rec_is_intersect(kd2.getRoot(), &boundbox2, &boundboxvec1, 0);

	//minimize_both();
	merge(&intersect_boxes1, &intersect_boxes2);
	return intersect_boxes1;
}