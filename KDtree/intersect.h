#pragma once
#include "kdtree.h"
#include <mesh.h>

//will prepare minimal bounding box of intersection, if one exist. 
//iterative function
std::vector<std::vector<glm::vec3>> areIntersecting(std::vector<glm::vec3> shape1, std::vector<glm::vec3> shape2);

//will be use to find the first bounding box
std::vector<float> findthightbox(std::vector<glm::vec3> positions);

//will be used to find if separating panel is exist
int isThereSeparatingPanel(std::vector<glm::vec3> box1, std::vector<glm::vec3> box2);