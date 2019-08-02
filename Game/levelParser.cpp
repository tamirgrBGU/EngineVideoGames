#include "levelParser.h"
#include "windows.h"
#include <fstream>
#include <iostream>
#include <random>
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

static const char * pathPtr = "%s\\zone%d.csv";
std::vector<std::string*> * scanDir(const char* path) {
	std::vector<std::string*> * names = new std::vector<std::string*>();
	char buf[1024]; int i = 0;

	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);
	printf(pwd);
	printf("\n");

	sprintf_s(buf, pathPtr, path, i++);
	while (fileExists(buf)) {
		printf("found level %s\n", buf);
		std::string* name = new std::string(buf);
		names->push_back(name);
		sprintf_s(buf, pathPtr, path, i++);
	}

	return names;
}

void leveGenerator::init(const char * path) {
	levelName = scanDir(path);
	levelGround = new std::vector<modelWrapper>();
	walls = new std::vector<modelWrapper>();
	fallWalls = new std::vector<modelWrapper>();
	stairsWalls = new std::vector<modelWrapper>();
	stairs = new std::vector<modelWrapper>();
	specialObj = new std::vector<struct objLocation>();
}

//Will ensure that the last delimeter was eof 
int nextDelimeterCSV(char* buf, int current, int del) {
	if (buf[current-1] == '\0')
		return -1;
	while ((buf[current] != del) & (buf[current] != '\0'))
		current++;
	return current;
}

int nextDelimeter(char* buf, int current, int limit, int del) {
	while ((buf[current] != del) & (current < limit) & (buf[current] != '\0'))
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
	int i = 0, point, del, x = 0;
	del = nextDelimeterCSV(buf, i, ',');
	while (del > -1) {
		struct objLocation obj;
		point = nextDelimeter(buf, i, del, '.');
		//printf("%d %d\n", del, point);
		if (point > -1) {
			int dash = nextDelimeter(buf, i, point, '-');
			if (dash != -1) {
				obj.type = getInt(buf, i, dash);
				obj.direction = getInt(buf, dash + 1, point);
				obj.level = getInt(buf, point + 1, del);
			}
			else
				printf("point at %d does not have dash!\n", point);
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
		del = nextDelimeterCSV(buf, i, ',');
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
	vec3 norm = glm::cross(c - b, c - a);
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
	vec3 norm = glm::cross(c - a, c - b);
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

inline void genMW(modelWrapper& mw, struct objLocation obj) {
	mw.x = obj.x;
	mw.y = obj.y;
	mw.z = obj.z;
	mw.level = obj.level;
}

inline void setTheSmallOnObj(struct objLocation &obj, struct objLocation &obj2) {
	if (obj.level > obj2.level) {
		struct objLocation t = obj;
		obj = obj2;
		obj2 = t;
	}
}

inline bool setUpMw(modelWrapper &mw, struct objLocation &obj, struct objLocation &obj2) {
	if ((obj.level != obj2.level) & (obj2.type != 0)) {
		genMW(mw, obj2);
		setTheSmallOnObj(obj, obj2);
		mw.z = obj.z;
		mw.level = obj.level;
		return true;
	}
	return false;
}

inline void pushWall(modelWrapper& mw, struct objLocation& highObj, IndexedModel& model, std::vector<modelWrapper>* walls, std::vector<modelWrapper>* fallWalls) {
	mw.model = model;
	walls->push_back(mw);
	mw.z = highObj.z;
	mw.level = highObj.level;
	fallWalls->push_back(mw);
}

//will build visible and invisible walls
//warning the visible walls are used as invisible walls, if the level diff is too high it can be invisible long wall (high z scale)!
void setWalls(const struct objConnected objC, std::vector<modelWrapper>* walls, std::vector<modelWrapper>* fallWalls) {
	struct objLocation obj;	modelWrapper mw;
	if (objC.down != nullptr) {//below wall
		obj = objC.me;
		struct objLocation obj2 = objC.down->me; 
		if (setUpMw(mw, obj, obj2))
			pushWall(mw, obj2,
				create_Hwall_square(0, 0, (obj2.level - obj.level) * zscale, allscale, 0), walls, fallWalls);
	}
	if (objC.right != nullptr) {//right wall
		obj = objC.me;
		struct objLocation obj2 = objC.right->me;
		if (setUpMw(mw, obj, obj2)) 
			pushWall(mw, obj2, 
				create_Vwall_square(0, allscale, (obj2.level - obj.level) * zscale, 0, 0), walls, fallWalls);
	}
}

//invisible walls to identify that the snake is on the stairs
inline void pushInvWalls(modelWrapper& mw, float xBoost, float yBoost, const struct objConnected *nobjC, std::vector<modelWrapper>* stairWalls, IndexedModel& im) {
	mw.model = im;
	mw.x += xBoost;
	mw.y += yBoost;
	stairWalls->push_back(mw);
	if (nobjC) {
		struct objLocation obj = nobjC->me;
		mw.x = obj.x + xBoost;
		mw.y = obj.y + yBoost;
		mw.z = obj.z;
		mw.level = obj.level;
	}
	stairWalls->push_back(mw);
}

void setStairs(const struct objConnected objC, std::vector<modelWrapper>* walls, std::vector<modelWrapper>* stairs, std::vector<modelWrapper>* stairWalls)
{
	struct objLocation obj = objC.me;
	modelWrapper mw;
	mw.x = obj.x;
	mw.y = obj.y;
	mw.z = obj.z;
	mw.level = obj.level;
	//create special squere at x and y;	//add it to a seperate list so it will be able to avoid coliding with them
	if (obj.direction == 0) {//up ^ ||
		mw.model = create_wall_square(allscale, allscale, 0, 0, 0, zscale);
		stairs->push_back(mw);
		//side walls to the stairs
		if (objC.right != nullptr && (objC.right->me.level > obj.level))
			mw.model = create_UVtriangle(0, 0, zscale, allscale, 0);
		else
			mw.model = create_Vtriangle(0, 0, zscale, allscale, 0);
		walls->push_back(mw);
		if (objC.left != nullptr && (objC.left->me.level > obj.level))
			mw.model = create_UVtriangle(allscale, 0, zscale, allscale, 0);
		else
			mw.model = create_Vtriangle(allscale, 0, zscale, allscale, 0);
		walls->push_back(mw);
		pushInvWalls(mw, 0, allscale, objC.up, stairWalls, create_wall_square(0, 0, zscale, allscale, 0, 0));
	}
	else if (obj.direction == 1) {//right > =
		mw.model = create_Hstairs_square(allscale, allscale, zscale, 0, 0, 0);
		stairs->push_back(mw);
		//side walls to the stairs
		if (objC.up != nullptr && (objC.up->me.level > obj.level))
			mw.model = create_UHtriangle(allscale, 0, zscale, 0, 0);
		else
			mw.model = create_Htriangle(allscale, 0, zscale, 0, 0);
		walls->push_back(mw);
		if (objC.down != nullptr && (objC.down->me.level > obj.level))
			mw.model = create_UHtriangle(allscale, allscale, zscale, 0, 0);
		else
			mw.model = create_Htriangle(allscale, allscale, zscale, 0, 0);
		walls->push_back(mw);
		pushInvWalls(mw, 0, 0, objC.right, stairWalls, create_Vwall_square(0, allscale, zscale, 0, 0));
	}
	else if (obj.direction == 2) {//down u ||
		mw.model = create_wall_square(0, allscale, zscale, allscale, 0, 0);
		stairs->push_back(mw);
		//side walls to the stairs
		if (objC.right != nullptr && (objC.right->me.level > obj.level))
			mw.model = create_UVtriangle(0, allscale, zscale, 0, 0);
		else
			mw.model = create_Vtriangle(0, allscale, zscale, 0, 0);
		walls->push_back(mw);
		if (objC.left != nullptr && (objC.left->me.level > obj.level))
			mw.model = create_UVtriangle(allscale, allscale, zscale, 0, 0);
		else
			mw.model = create_Vtriangle(allscale, allscale, zscale, 0, 0);
		walls->push_back(mw);
		pushInvWalls(mw, 0, 0, objC.down, stairWalls, create_wall_square(0, 0, zscale, allscale, 0, 0));
	}
	else {//left < =
		mw.model = create_Hstairs_square(0, 0, zscale, allscale, allscale, 0);
		stairs->push_back(mw);
		//side walls to the stairs
		if (objC.up != nullptr && (objC.up->me.level > obj.level))
			mw.model = create_UHtriangle(0, 0, zscale, allscale, 0);
		else
			mw.model = create_Htriangle(0, 0, zscale, allscale, 0);
		walls->push_back(mw);
		if (objC.down != nullptr && (objC.down->me.level > obj.level))
			mw.model = create_UHtriangle(0, allscale, zscale, allscale, 0);
		else
			mw.model = create_Htriangle(0, allscale, zscale, allscale, 0);
		walls->push_back(mw);
		pushInvWalls(mw, allscale, 0, objC.left, stairWalls, create_Vwall_square(0, allscale, zscale, 0, 0));
	}
}

std::random_device rd;   // non-deterministic generator
std::mt19937 gen(rd());  // to seed mersenne twister.
						 // replace the call to rd() with a
						 // constant value to get repeatable
						 // results.
static const int maxObjToChooseFrom = 2;
static const int probToGet = 4 * maxObjToChooseFrom;

void initGroundModel(std::vector<modelWrapper>* levelGround,
	std::vector<modelWrapper>* stairs,
	std::vector<modelWrapper>* stairsWalls,
	std::vector<modelWrapper>* walls,
	std::vector<modelWrapper>* fallWalls,
	std::vector<struct objLocation>* specialObj,
	std::vector<struct objConnected> vec) {
	levelGround->clear();
	stairs->clear();
	stairsWalls->clear();
	walls->clear();
	specialObj->clear();

	for (unsigned int i = 0; i < vec.size(); i++) {
		//warning do not edit vector or else or the pointers will blow to hell!
		struct objConnected *objC = &(vec[i]);
		struct objLocation  *obj  = &(objC->me);
		//fix obj x and y to real location, level represents z
		obj->x *= allscale;
		obj->y *= allscale;
		obj->z = obj->level * zscale;
	}

	for (unsigned int i = 0; i < vec.size(); i++) {
		struct objConnected objC = vec[i];
		struct objLocation *obj  = &objC.me;

		//check left and right for vertical squres
		//add all vertical squeres seperate list so it will be able to avoid coliding with them
		
		if (obj->type == 0)
			setStairs(objC, walls, stairs, stairsWalls);

		else { //spacial index model (not square :) )
			setWalls(objC, walls, fallWalls);
			  //if (obj->type == -1) {//create squere at x and y;
			modelWrapper mw; genMW(mw, *obj);
			mw.model = create_ground_square(0, allscale, allscale, 0, 0);
			levelGround->push_back(mw);
			if (obj->type > 0)
				specialObj->push_back(*obj);
			else
			{
				unsigned int x = gen();
				x = x % probToGet;
				if (x < maxObjToChooseFrom) {
					obj->type = 2 + x;
					specialObj->push_back(*obj);
				}
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
		a.down = nullptr;		a.up = nullptr;
		a.right = nullptr;		a.left = nullptr;
		out.push_back(a);
	}

	for (int i = 0; i < size; i++) {
		struct objConnected *a = &out[i];
		if (i + width < size)	a->down = &out[i + width];
		if (i - width > -1)		a->up = &out[i - width];

		//printf("%d %d %d %f\n", i+1, i, width, a->me.x);
		if (i + 1 < size && (i + 1) % width != 0)	a->right = &out[i + 1];
		if (i > 1 && (i - 1) % width != 0) 			a->left = &out[i - 1];
	}
	return out;
}

const int maxWidth = 1024;
int leveGenerator::parseLevel(int i) {
	if (!levelName)
		return -1;

	printf("levels loaded %d\n", levelName->size());
	if (i < 0 || (unsigned)i >= levelName->size())
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
			if (buf[0] != 0) {
				int widthT = parseFileLine(buf, line++, &vec);
				if (width != widthT)
					printf("bad line %d %d|%d\n", line - 1, widthT, width);
			}
		}

		myfile.close();
		initGroundModel(levelGround, stairs, stairsWalls, walls, fallWalls, specialObj, toConnectedVec(width, vec));
		return 0;
	}
	printf("Unable to open file <%s>\n", buf);
	return -3;
}


struct objMap leveGenerator::getLevel(int i) {
	if (currentLevel != i) {
		if (parseLevel(i)) {
			printf("WARNing!! no such level\n");
			return struct objMap() = { 0, 0, 0, 0, 0, 0 };
		}
		currentLevel = i;
	}
	struct objMap out = {
		levelGround,
		stairs,	stairsWalls, 
		walls,	fallWalls, 
		specialObj };

	return out;
}

leveGenerator::~leveGenerator(void) {
	levelGround->clear();	delete levelGround;
	stairs->clear();		delete stairs;
	walls->clear();			delete walls;
	stairsWalls->clear();	delete stairsWalls;
	fallWalls->clear();		delete fallWalls;
	specialObj->clear();	delete specialObj;

	for (auto &obj : *levelName) delete obj;
	levelName->clear();
	delete levelName;

}