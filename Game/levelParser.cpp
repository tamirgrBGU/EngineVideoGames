#include "levelParser.h"
#include "windows.h"
#include <fstream>
#include <iostream>

leveGenerator::leveGenerator(int i) { 
	currentLevel = i;
	init(levelDirName);
	parseLevel(i);
}

int fileExists(TCHAR * file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(file, &FindFileData);
	int found = handle != INVALID_HANDLE_VALUE;
	//printf("%s %d\n", file , found);
	if (found)
	{
		//FindClose(&handle); this will crash
		FindClose(handle);
	}
	return found;
}

std::vector<std::string*> * scanDir(const char* path) {
	std::vector<std::string*> * names = new std::vector<std::string*>();
	char buf[1024]; int i = 0;

	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);
	printf(pwd);
	printf("\n");

	sprintf_s(buf, "%s\\zone%d.txt", path, i++);
	while (fileExists(buf)){
		printf("found level %s\n", buf);
		std::string* name = new std::string(buf);
		names->push_back(name);
		sprintf_s(buf, "%s\\zone%d.txt", path, i++);
	} 

	return names;
}

void leveGenerator::init(const char * path) {
	levelName=scanDir(path);
	levelGround = new std::vector<IndexedModel>();
	walls	    = new std::vector<IndexedModel>();
	stairs		= new std::vector<IndexedModel>();
	specialObj  = new std::vector<struct objLocation>();
}

int nextDelimeter(char* buf, int current, int del) {
	if (buf[current] == '\0')
		return -1;
	while (buf[current] != del)
		current++;
	return current;
}

int nextDelimeter(char* buf, int current, int limit, int del) {
	while ((buf[current] != del) & (current < limit))
		current++;
	if (current == limit)
		return -1;
	return current;
}

int getInt(char* buf, int first, int end) {
	int i = 0;
	while (first < end) {
		i = 10 * i + buf[first] - '0';
		first++;
	}
	return i;
}

int parseFileLine(char* buf, int linenum, std::vector<struct objLocation> *Objs) {
	//printf("<%s>\n", buf);
	int i = 0, point, del, x=0;
	del = nextDelimeter(buf, i, ',');
	while (del > -1) {
		struct objLocation obj;
		point = nextDelimeter(buf, i, del, '.');
		//printf("%d %d\n", del, point);
		if (point > -1) {
			int dash = nextDelimeter(buf, i, '-');

			obj.type  = getInt(buf, i, dash);
			obj.direction = getInt(buf, dash+1, point);
			obj.level = getInt(buf, point+1, del);
		}
		else {
			obj.type = -1;
			obj.direction = -1;
			obj.level = getInt(buf, i, del);
		}
		obj.x = float(x++);
		obj.y = float(linenum);
		Objs->push_back(obj);
		i = del + 1;
		del = nextDelimeter(buf, i, ',');
	}
	return x;
}

/*
	square
	a	b
	c	d
*/
IndexedModel create_square(vec3 a, vec3 b, vec3 c, vec3 d) {
	IndexedModel square;
	square.positions.push_back(a);	square.positions.push_back(b);
	square.positions.push_back(c);	square.positions.push_back(d);
	vec3 norm = glm::cross(c - a, d - b);
	square.normals.push_back(norm);	square.normals.push_back(norm);
	square.normals.push_back(norm);	square.normals.push_back(norm);
	vec3 color(0.6, 0.6, 0.6);
	square.colors.push_back(color);	square.colors.push_back(color);
	square.colors.push_back(color);	square.colors.push_back(color);
	vec3 zero(0);
	square.weights.push_back(zero);	square.weights.push_back(zero);
	square.weights.push_back(zero);	square.weights.push_back(zero);
	square.texCoords.push_back(vec2(0, 0));	square.texCoords.push_back(vec2(1, 0));
	square.texCoords.push_back(vec2(1, 1));	square.texCoords.push_back(vec2(0, 1));
	square.indices.push_back(0);	square.indices.push_back(1);	square.indices.push_back(2);
	square.indices.push_back(2);	square.indices.push_back(3);	square.indices.push_back(0);
	return square;
}

/*
triangle
	a
c	b
*/
IndexedModel create_triangle(vec3 a, vec3 b, vec3 c) {
	IndexedModel triangle;
	triangle.positions.push_back(a);	triangle.positions.push_back(b);	triangle.positions.push_back(c);
	vec3 norm = glm::cross(b - a, b - c);
	triangle.normals.push_back(norm);	triangle.normals.push_back(norm);	triangle.normals.push_back(norm);
	vec3 color(0.6, 0.6, 0.6);
	triangle.colors.push_back(color);	triangle.colors.push_back(color);	triangle.colors.push_back(color);
	vec3 zero(0);
	triangle.weights.push_back(zero);	triangle.weights.push_back(zero);	triangle.weights.push_back(zero);
	triangle.texCoords.push_back(vec2(0, 0));	triangle.texCoords.push_back(vec2(1, 0)); triangle.texCoords.push_back(vec2(1, 1));
	triangle.indices.push_back(0);	triangle.indices.push_back(1);	triangle.indices.push_back(2);
	return triangle;
}


IndexedModel create_UVtriangle(float xa, float ya, float za, float yc, float zc) {
	return create_triangle(vec3(xa, ya, za), vec3(xa, yc, za), vec3(xa, yc, zc));
}

IndexedModel create_Vtriangle(float xa, float ya, float za, float yc, float zc) {
	return create_triangle(vec3(xa, ya, za), vec3(xa, ya, zc), vec3(xa, yc, zc));
}

IndexedModel create_Htriangle(float xa, float ya, float za, float xc, float zc) {
	return create_triangle(vec3(xa, ya, za), vec3(xa, ya, zc), vec3(xc, ya, zc));
}

IndexedModel create_UHtriangle(float xa, float ya, float za, float xc, float zc) {
	return create_triangle(vec3(xa, ya, za), vec3(xc, ya, za), vec3(xc, ya, zc));
}

IndexedModel create_wall_square(float xa, float ya, float za, float xc, float yc, float zc) {
	return create_square(vec3(xa, ya, za), vec3(xc, ya, za), vec3(xc, yc, zc), vec3(xa, yc, zc));
}

IndexedModel create_Hstairs_square(float xa, float ya, float za, float xc, float yc, float zc) {
	return create_square(vec3(xa, ya, za), vec3(xa, yc, za), vec3(xc, yc, zc), vec3(xc, ya, zc));
}

IndexedModel create_Hwall_square(float xa, float ya, float za, float xc, float zc) {
	return create_square(vec3(xa, ya, za), vec3(xc, ya, za), vec3(xc, ya, zc), vec3(xa, ya, zc));
}

IndexedModel create_Vwall_square(float xa, float ya, float za, float yc, float zc) {
	return create_square(vec3(xa, ya, za), vec3(xa, yc, za), vec3(xa, yc, zc), vec3(xa, ya, zc));
}


IndexedModel create_ground_square(float xa, float ya, float xc, float yc, float z) {
	return create_square(vec3(xa, ya, z), vec3(xc, ya, z), vec3(xc, yc, z), vec3(xa, yc, z));
}

void inline setWalls(const struct objConnected objC, std::vector<IndexedModel>* walls) {
	struct objLocation obj = objC.me;
	if (objC.down != nullptr) {//below wall
		struct objLocation obj2 = objC.down->me;
		if ((obj.level != obj2.level) & (obj2.type != 0)) {
			if (obj.level > obj2.level)
				walls->push_back(create_Hwall_square(obj.x, obj.y + allscale, obj.level * zscale, obj.x + allscale, obj2.level * zscale));
			else
				walls->push_back(create_Hwall_square(obj.x, obj.y + allscale, obj2.level * zscale, obj.x + allscale, obj.level * zscale));
		}
	}
	if (objC.right != nullptr) {//right wall
		struct objLocation obj2 = objC.right->me;
		if ((obj.level != obj2.level) & (obj2.type != 0)) {
			if (obj.level > obj2.level)
				walls->push_back(create_Vwall_square(obj.x + allscale, obj.y + allscale, obj.level * zscale, obj.y, obj2.level * zscale));
			else
				walls->push_back(create_Vwall_square(obj.x + allscale, obj.y + allscale, obj2.level * zscale, obj.y, obj.level * zscale));
		}
	}
}

void setStairs(const struct objConnected objC, std::vector<IndexedModel>* walls, std::vector<IndexedModel>* stairs)
{
	struct objLocation obj = objC.me;
	//create special squere at x and y;	//add it to a seperate list so it will be able to avoid coliding with them
	if (obj.direction == 0) {//up ^ ||
		stairs->push_back(create_wall_square(obj.x + allscale, obj.y + allscale, obj.level * zscale, obj.x, obj.y, (obj.level + 1) * zscale));
		if (objC.right!=nullptr && (objC.right->me.level > obj.level))
			walls->push_back(create_UVtriangle(obj.x, obj.y, (obj.level + 1) * zscale, obj.y + allscale, obj.level * zscale));
		else
			walls->push_back(create_Vtriangle(obj.x, obj.y, (obj.level + 1) * zscale, obj.y + allscale, obj.level * zscale));
		if (objC.left != nullptr && (objC.left->me.level > obj.level))
			walls->push_back(create_UVtriangle(obj.x + allscale, obj.y, (obj.level + 1) * zscale, obj.y + allscale, obj.level * zscale));
		else
			walls->push_back(create_Vtriangle(obj.x + allscale, obj.y, (obj.level + 1) * zscale, obj.y + allscale, obj.level * zscale));
	}
	else if (obj.direction == 1) {//right > =
		stairs->push_back(create_Hstairs_square(obj.x + allscale, obj.y + allscale, (obj.level + 1) * zscale, obj.x, obj.y, obj.level * zscale));
		if (objC.up != nullptr && (objC.up->me.level > obj.level))
			walls->push_back(create_UHtriangle(obj.x + allscale, obj.y, (obj.level + 1) * zscale, obj.x, obj.level * zscale));
		else
			walls->push_back(create_Htriangle(obj.x + allscale, obj.y, (obj.level + 1) * zscale, obj.x, obj.level * zscale));

		if (objC.down != nullptr && (objC.down->me.level > obj.level))
			walls->push_back(create_UHtriangle(obj.x + allscale, obj.y + allscale, (obj.level + 1) * zscale, obj.x, obj.level * zscale));
		else
			walls->push_back(create_Htriangle(obj.x + allscale, obj.y + allscale, (obj.level + 1) * zscale, obj.x, obj.level * zscale));
	}
	else if (obj.direction == 2) {//down u ||
		stairs->push_back(create_wall_square(obj.x, obj.y + allscale, (obj.level + 1) * zscale, obj.x + allscale, obj.y, obj.level * zscale));
		if (objC.right != nullptr && (objC.right->me.level > obj.level))
			walls->push_back(create_UVtriangle(obj.x, obj.y + allscale, (obj.level + 1) * zscale, obj.y, obj.level * zscale));
		else
			walls->push_back(create_Vtriangle(obj.x, obj.y + allscale, (obj.level + 1) * zscale, obj.y, obj.level * zscale));
		if (objC.left != nullptr && (objC.left->me.level > obj.level))
			walls->push_back(create_UVtriangle(obj.x + allscale, obj.y + allscale, (obj.level + 1) * zscale, obj.y, obj.level * zscale));
		else
			walls->push_back(create_Vtriangle(obj.x + allscale, obj.y + allscale, (obj.level + 1) * zscale, obj.y, obj.level * zscale));
	}
	else {//left < =
		stairs->push_back(create_Hstairs_square(obj.x, obj.y, (obj.level + 1) * zscale, obj.x + allscale, obj.y + allscale, obj.level * zscale));

		if (objC.up != nullptr && (objC.up->me.level > obj.level))
			walls->push_back(create_UHtriangle(obj.x, obj.y + allscale, (obj.level + 1) * zscale, obj.x + allscale, obj.level * zscale));
		else
			walls->push_back(create_Htriangle(obj.x, obj.y, (obj.level + 1) * zscale, obj.x + allscale, obj.level * zscale));

		if (objC.down != nullptr && (objC.down->me.level > obj.level))
			walls->push_back(create_UHtriangle(obj.x, obj.y + allscale, (obj.level + 1) * zscale, obj.x + allscale, obj.level * zscale));
		else
			walls->push_back(create_Htriangle(obj.x, obj.y + allscale, (obj.level + 1) * zscale, obj.x + allscale, obj.level * zscale));
	}
}

void initGroundModel(std::vector<IndexedModel>* levelGround, 
					 std::vector<IndexedModel>* stairs,
					 std::vector<IndexedModel>* walls,
					 std::vector<struct objLocation>* specialObj,
					 const std::vector<struct objConnected> vec){
	levelGround->clear();
	stairs->clear();
	walls->clear();
	specialObj->clear();

	for (unsigned int i = 0; i < vec.size(); i++) {
		struct objConnected objC = vec[i];
		struct objLocation *obj = &objC.me;
		//fix obj x and y to real location, level represents z
		obj->x *= allscale;
		obj->y *= allscale;

		//check left and right for vertical squres
		//add all vertical squeres seperate list so it will be able to avoid coliding with them
		
		if (obj->type != 0)
			setWalls(objC, walls);
		
		if (obj->type == 0) {//stairs
			setStairs(objC, walls, stairs);
		}
		else { //spacial index model (not square :) )
			//if (obj->type == -1) {//create squere at x and y;
			levelGround->push_back(create_ground_square(obj->x, obj->y, obj->x + allscale, obj->y + allscale, obj->level * zscale));
			if (obj->type > 0) {
				obj->level *= zscale;
				specialObj->push_back(*obj);
			}
		}
	}
}

std::vector<struct objConnected> toConnectedVec(int width, std::vector<struct objLocation> &vec) {
	int size = vec.size();
	std::vector<struct objConnected> out;
	for (int i = 0; i < size; i++) {
		struct objConnected a;
		a.me = vec[i];
		a.down  = nullptr;		a.up   = nullptr;
		a.right = nullptr;		a.left = nullptr;
		out.push_back(a);
	}
		
	for (int i = 0; i < size; i++) {
		struct objConnected *a = &out[i];
		if (i + width < size)	a->down = &out[i + width];
		if (i - width > -1)		a->up   = &out[i - width];

		//printf("%d %d %d %f\n", i+1, i, width, a->me.x);
		if (i + 1 < size && (i + 1) % width != 0)	a->right = &out[i + 1];
		if (i > 1 && (i - 1) % width != 0) 			a->left  = &out[i - 1];
	}
	return out;
}

const int maxWidth = 1024;
int leveGenerator::parseLevel(int i) {
	if (!levelName)
		return -1;

	printf("levels loaded %d\n", levelName->size());
	if (i < 0 || (unsigned) i >= levelName->size())
		return -2;

	char buf[maxWidth];
	sprintf_s(buf, "%s", (*levelName)[i]->c_str());

	std::ifstream myfile(buf);
	if (myfile.is_open())
	{
		std::vector<struct objLocation> vec;
		int line = 0;
		myfile.getline(buf, maxWidth, '\n');
		int width = parseFileLine(buf, line++, &vec);
		while (myfile.getline(buf, maxWidth, '\n')) {
			int widthT = parseFileLine(buf, line++, &vec);
			if(width != widthT)
				printf("bad line %d %d|%d\n", line - 1, widthT, width);
		}
		
		myfile.close();
		initGroundModel(levelGround, stairs, walls, specialObj, toConnectedVec(width, vec));
		return 0;
	}
	printf("Unable to open file <%s>\n", buf);
	return -3;
}


struct objMap leveGenerator::getLevel(int i) {
	if (currentLevel != i) {
		if(parseLevel(i))
			return struct objMap() = { 0, 0, 0, 0 };
		currentLevel = i;
	}
	struct objMap out = {
		levelGround, stairs,
		walls, specialObj};
	return out;
}

leveGenerator::~leveGenerator(void){
	levelGround->clear();	delete levelGround;
	stairs->clear();
	walls->clear();
	specialObj->clear();	delete specialObj;

	for (auto &obj : *levelName) delete obj;
	levelName->clear();
	delete levelName;

}