#include "intersect.h"

//default constructor
intersect::intersect(std::vector<glm::vec3> &shape) {
	kd.makeTree(shape);
	boundbox = kd.getRoot()->boundbox;
	boundboxvec = bound_vec_to_boundbox(boundbox);
}

//copy constructor
intersect::intersect(Node *Node) {
	kd = Kdtree(Node);
	boundbox = kd.getRoot()->boundbox;
	boundboxvec = bound_vec_to_boundbox(boundbox);
}

//default constructor
IndexedModel intersect::getBoundingBox() {
	return boxVertexesToIndexModel(boundboxvec, glm::vec3(0,0,0));
}

glm::mat4 *currentTransMe;
glm::mat4 *currentTransOther;

/*
*  Return the tree root node
*/
std::vector<IndexedModel> coloredBoxesOutput;
int maxDep;
std::vector<IndexedModel> intersect::isIntersect(glm::mat4 *transMe, glm::mat4 *transOther, intersect &other) {
	std::vector<glm::vec3> boundboxvec1 = bound_vec_to_boundbox(boundbox);
	std::vector<glm::vec3> boundboxvec2 = bound_vec_to_boundbox(other.boundbox);

	currentTransMe  = transMe;
	currentTransOther = transOther;

	int res = isThereSeparatingPanel(boundboxvec1, boundboxvec2);
	//printf("res %d\n", res);

	if (res > 0)
		return std::vector<IndexedModel>();

	std::vector<std::vector<glm::vec3>> intersect_boxes;
	coloredBoxesOutput.clear();
	maxDep = kd.max_depth < other.kd.max_depth ? kd.max_depth : other.kd.max_depth;
	if (0 >= maxDep - 1) {
		IndexedModel a;
		coloredBoxesOutput.push_back(a);
	}
	else
		rec_is_intersect(kd.getRoot(), other.kd.getRoot(), 0, &intersect_boxes);
	//intersect_boxes.pop_back(); intersect_boxes.pop_back();
	return coloredBoxesOutput;
}

int intersect::isThereSeparatingPanel(std::vector<glm::vec3> &box1, std::vector<glm::vec3> &box2) {
	glm::vec3 frontleftdown = v4to3(*currentTransMe * v3to4(box1[0]));
	glm::vec3 backleftdown = v4to3(*currentTransMe * v3to4(box1[1]));
	glm::vec3 backrightdown = v4to3(*currentTransMe * v3to4(box1[2]));
	glm::vec3 backleftup  = v4to3(*currentTransMe * v3to4(box1[5]));
	glm::vec3 frontupright = v4to3(*currentTransMe * v3to4(box1[7]));
	glm::vec3 PA = (backleftdown + frontupright) / 2.0f;//coordinate position of the center of A
	glm::vec3 Ax = backrightdown - backleftdown;//unit vector representing the x - axis of A
	glm::vec3 Ay = backleftup  - backleftdown;// unit vector representing the y - axis of A
	glm::vec3 Az = frontleftdown - backleftdown;// unit vector representing the z - axis of A
	float WA = glm::length(Ax);// half width of A(corresponds with the local x - axis of A)
	Ax = Ax / WA;//normalize
	WA /= 2.0f;
	float HA = glm::length(Ay);//half height of A(corresponds with the local y - axis of A)
	Ay = Ay / HA;
	HA /= 2.0f;
	float DA = glm::length(Az);//half depth of A(corresponds with the local z - axis of A)
	Az = Az / DA;
	DA /= 2.0f;

	frontleftdown= v4to3(*currentTransOther * v3to4(box2[0]));
	backleftdown = v4to3(*currentTransOther * v3to4(box2[1]));
	backrightdown= v4to3(*currentTransOther * v3to4(box2[2]));
	backleftup  = v4to3(*currentTransOther * v3to4(box2[5]));
	frontupright = v4to3(*currentTransOther * v3to4(box2[7]));
	glm::vec3 PB = (backleftdown + frontupright) / 2.0f;//coordinate position of the center of B
	glm::vec3 Bx = backrightdown - backleftdown;//unit vector representing the x - axis of B
	glm::vec3 By = backleftup - backleftdown;//unit vector representing the y - axis of B
	glm::vec3 Bz = frontleftdown - backleftdown;//unit vector representing the z - axis of B
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

	float Rxx = glm::dot(Ax, Bx);
	float Rxy = glm::dot(Ax, By);
	float Rxz = glm::dot(Ax, Bz);
	float Ryx = glm::dot(Ay, Bx);
	float Ryy = glm::dot(Ay, By);
	float Ryz = glm::dot(Ay, Bz);
	float Rzx = glm::dot(Az, Bx);
	float Rzy = glm::dot(Az, By);
	float Rzz = glm::dot(Az, Bz);
	float cond, res;
	// L = Ax
	cond = abs(glm::dot(T, Ax));
	res = WA + abs(WB * Rxx) + abs(HB * Rxy) + abs(DB * Rxz);
	if (cond > res)
		return 1;
	// L = Ay
	cond = abs(glm::dot(T, Ay));
	res = HA + abs(WB * Ryx) + abs(HB * Ryy) + abs(DB * Ryz);
	if (cond > res)
		return 2;
	// L = Az
	cond = abs(glm::dot(T, Az));
	res = DA + abs(WB * Rzx) + abs(HB * Rzy) + abs(DB * Rzz);
	if (cond > res)
		return 3;
	// L = Bx
	cond = abs(glm::dot(T, Bx));
	res = WB + abs(WA * Rxx) + abs(HA * Ryx) + abs(DA * Rzx);
	if (cond > res)
		return 4;
	// L = By
	cond = abs(glm::dot(T, By));
	res = HB + abs(WA * Rxy) + abs(HA * Ryy) + abs(DA * Rzy);
	if (cond > res)
		return 5;
	// L = Bz
	cond = abs(glm::dot(T, Bz));
	res = DB + abs(WA * Rxz) + abs(HA * Ryz) + abs(DA * Rzz);
	if (cond > res)
		return 6;
	// L = Ax x Bx
	cond = abs(glm::dot(T, Az)*Ryx - glm::dot(T, Ay)*Rzx);
	res = abs(HA*Rzx) + abs(DA*Ryx) + abs(HB*Rxz) + abs(DB*Rxy);
	if (cond > res)
		return 7;
	// L = Ax x By
	cond = abs(glm::dot(T, Az)*Ryy - glm::dot(T, Ay)*Rzy);
	res = abs(HA*Rzy) + abs(DA*Ryy) + abs(WB*Rxz) + abs(DB*Rxx);
	if (cond > res)
		return 8;
	// L = Ax x Bz
	cond = abs(glm::dot(T, Az)*Ryz - glm::dot(T, Ay)*Rzz);
	res = abs(HA*Rzz) + abs(DA*Ryz) + abs(WB*Rxy) + abs(HB*Rxx);
	if (cond > res)
		return 9;
	// L = Ay x Bx
	cond = abs(glm::dot(T, Ax)*Rzx - glm::dot(T, Az)*Rxx);
	res = abs(WA*Rzx) + abs(DA*Rxx) + abs(HB*Ryz) + abs(DB*Ryy);
	if (cond > res)
		return 10;
	// L = Ay x By
	cond = abs(glm::dot(T, Ax)*Rzy - glm::dot(T, Az)*Rxy);
	res = abs(WA*Rzy) + abs(DA*Rxy) + abs(WB*Ryz) + abs(DB*Ryx);
	if (cond > res)
		return 11;
	// L = Ay x Bz
	cond = abs(glm::dot(T, Ax)*Rzz - glm::dot(T, Az)*Rxz);
	res = abs(WA*Rzz) + abs(DA*Rxz) + abs(WB*Ryy) + abs(HB*Ryx);
	if (cond > res)
		return 12;
	// L = Az x Bx
	cond = abs(glm::dot(T, Ay)*Rxx - glm::dot(T, Ax)*Ryx);
	res = abs(WA*Ryx) + abs(HA*Rxx) + abs(HB*Rzz) + abs(DB*Rzy);
	if (cond > res)
		return 13;
	// L = Az x By
	cond = abs(glm::dot(T, Ay)*Rxy - glm::dot(T, Ax)*Ryy);
	res = abs(WA*Ryy) + abs(HA*Rxy) + abs(WB*Rzz) + abs(DB*Rzx);
	if (cond > res)
		return 14;
	// L = Az x Bz
	cond = abs(glm::dot(T, Ay)*Rxz - glm::dot(T, Ax)*Ryz);
	res = abs(WA*Ryz) + abs(HA*Rxz) + abs(WB*Rzy) + abs(HB*Rzx);
	if (cond > res)
		return 15;
	return 0;
}

inline std::vector<glm::vec3> intersect::bound_vec_to_boundbox(std::vector<float> &boundbox) {
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

void intersect::merge(std::vector<std::vector<glm::vec3>> *a, std::vector<std::vector<glm::vec3>> *b) {
	for (unsigned int i = 0; i < b->size(); i++) 
		a->push_back((*b)[i]);
}

bool intersect::isEqual(std::vector<glm::vec3> &boxvec1, std::vector<glm::vec3> &boxvec2) {
	if (boxvec1.size() > boxvec2.size())
		return false;
	bool out = 1;
	for (unsigned int i = 0; i < boxvec1.size(); i++)
		out &= boxvec1[i] == boxvec2[i];
	return out;
}

void intersect::insert_box(std::vector<std::vector<glm::vec3>> *boxes, std::vector<glm::vec3> boxvec, glm::mat4 *transmat, glm::vec3 color) {
	for (std::vector<glm::vec3> box : *boxes)
		if(intersect::isEqual(box, boxvec)) return;

	for (unsigned int i = 0; i < boxvec.size(); i++)
		boxvec[i] = v4to3(*transmat * v3to4(boxvec[i]));

	coloredBoxesOutput.push_back(boxVertexesToIndexModel(boxvec, color));
	boxes->push_back(boxvec);
}

int intersect::intersectWithOther(Node * nextother, int axis, std::vector<float> &boundingboxcopy)
{
	if (nextother) {
		std::vector<float> &boundingboxcopyother = nextother->boundbox;
		int res = isThereSeparatingPanel(bound_vec_to_boundbox(boundingboxcopy), bound_vec_to_boundbox(boundingboxcopyother));
		if (res == 0) {
			if (boundingboxcopyother[axis * 2] == boundingboxcopyother[axis * 2 + 1])
				return 64;//we do not want to do the recursion on a plane
		}
		return res;
	}
	return 65;
}

void intersect::nodesIntersectValitate(Node * next, int axis, Node * other,
	int depth, std::vector<std::vector<glm::vec3>> * output, std::vector<glm::vec3> &boxvec, std::vector<glm::vec3> &boxvec2)
{
	bool intersect_with = 0;// 1;
	if (next) {
		if (next->boundbox[axis * 2] == next->boundbox[axis * 2 + 1])
			intersect_with = 1;//we do not want to keep recursion on a plane	
		else {
			int res1 = intersectWithOther(other->left, axis, next->boundbox);
			if (res1 == 0)
				rec_is_intersect(next, other->left, depth + 1, output);
			int res2 = intersectWithOther(other->right, axis, next->boundbox);
			if (res2 == 0)
				rec_is_intersect(next, other->right, depth + 1, output);
			intersect_with = (res1 & res2) > 0;
		}
	}
	if (intersect_with && ((signed)depth >= maxDep-1)) {//none of the children are intersecting - add perants
		insert_box(output, boxvec, currentTransMe,	  glm::vec3(0.2, 0.2, 1));
		insert_box(output, boxvec2, currentTransOther, glm::vec3(1, 0.2, 0.2));
	}
}

//in iteration level the boxes are intersecting we will assure it happen in the children too
void intersect::rec_is_intersect(Node *current, Node *other,
			int depth, std::vector<std::vector<glm::vec3>> *output) {
	int axis = depth % 3;
	std::vector<glm::vec3> boxvec = bound_vec_to_boundbox(current->boundbox);
	std::vector<glm::vec3> boxvec2 = bound_vec_to_boundbox( other->boundbox);
	std::vector<float> boundingboxcopy   = current->boundbox;
	std::vector<float> boundingboxcopyother =  other->boundbox;
	
	nodesIntersectValitate(current->left, axis, other, depth, output, boxvec, boxvec2);
	nodesIntersectValitate(current->right, axis, other, depth, output, boxvec, boxvec2);
}

IndexedModel intersect::boxVertexesToIndexModel (std::vector<glm::vec3> &intesect_box, glm::vec3 color){	
	Vertex vertices[] =
	{
		Vertex(intesect_box[1], glm::vec2(1, 0), glm::vec3(0, 0, -1), color),
		Vertex(intesect_box[5], glm::vec2(0, 0), glm::vec3(0, 0, -1), color),
		Vertex(intesect_box[6], glm::vec2(0, 1), glm::vec3(0, 0, -1), color),
		Vertex(intesect_box[2], glm::vec2(1, 1), glm::vec3(0, 0, -1), color),

		Vertex(intesect_box[0], glm::vec2(1, 0), glm::vec3(0, 0, 1), color),
		Vertex(intesect_box[4], glm::vec2(0, 0), glm::vec3(0, 0, 1), color),
		Vertex(intesect_box[7], glm::vec2(0, 1), glm::vec3(0, 0, 1), color),
		Vertex(intesect_box[3], glm::vec2(1, 1), glm::vec3(0, 0, 1), color),

		Vertex(intesect_box[1], glm::vec2(0, 1), glm::vec3(0, -1, 0), color),
		Vertex(intesect_box[0], glm::vec2(1, 1), glm::vec3(0, -1, 0), color),
		Vertex(intesect_box[3], glm::vec2(1, 0), glm::vec3(0, -1, 0), color),
		Vertex(intesect_box[2], glm::vec2(0, 0), glm::vec3(0, -1, 0), color),

		Vertex(intesect_box[5], glm::vec2(0, 1), glm::vec3(0, 1, 0), color),
		Vertex(intesect_box[4], glm::vec2(1, 1), glm::vec3(0, 1, 0), color),
		Vertex(intesect_box[7], glm::vec2(1, 0), glm::vec3(0, 1, 0), color),
		Vertex(intesect_box[6], glm::vec2(0, 0), glm::vec3(0, 1, 0), color),

		Vertex(intesect_box[1], glm::vec2(1, 1), glm::vec3(-1, 0, 0), color),
		Vertex(intesect_box[0], glm::vec2(1, 0), glm::vec3(-1, 0, 0), color),
		Vertex(intesect_box[4], glm::vec2(0, 0), glm::vec3(-1, 0, 0), color),
		Vertex(intesect_box[5], glm::vec2(0, 1), glm::vec3(-1, 0, 0), color),

		Vertex(intesect_box[2], glm::vec2(1, 1), glm::vec3(1, 0, 0), color),
		Vertex(intesect_box[3], glm::vec2(1, 0), glm::vec3(1, 0, 0), color),
		Vertex(intesect_box[7], glm::vec2(0, 0), glm::vec3(1, 0, 0), color),
		Vertex(intesect_box[6], glm::vec2(0, 1), glm::vec3(1, 0, 0), color)
	};
	
	unsigned int indices[] = {0, 1, 2,
							 0, 2, 3,

							 6, 5, 4,
							 7, 6, 4,

							 10, 9, 8,
							 11, 10, 8,

							 12, 13, 14,
							 12, 14, 15,

							 16, 17, 18,
							 16, 18, 19,

							 22, 21, 20,
							 23, 22, 20
	             };

	IndexedModel model;
	
	for(unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for(unsigned int i = 0; i < 36; i++)
    model.indices.push_back(indices[i]);
	
	return model;
}

std::vector<IndexedModel> intersect::makeBoxesIndexModels(std::vector<std::vector<glm::vec3>> &intersect_boxes) {
	std::vector<IndexedModel> models;
	for (std::vector<glm::vec3> intesect_box : intersect_boxes)
		models.push_back(boxVertexesToIndexModel(intesect_box, glm::vec3(0, 0, 0)));
	return models;
}