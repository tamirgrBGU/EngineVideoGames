#pragma once
#include "../KDtree/intersect.h"

void addObj(int x, int y, void (*onIntersect)(void), std::vector<glm::vec3> shape);
void addSnakeHead(std::vector<glm::vec3> shape);
void isIntersectSnakeHead(glm::mat4 *tranSnake);