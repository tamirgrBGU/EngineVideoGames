
#include "levelTheme.h"
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <random>

char *floorTexImg = "floor.jpg";	char *wallTexImg  = "wall.jpg";
char *sharedObjNames[] = { "../res/objs/cave.obj" };
const int sharedSize = sizeof(sharedObjNames) / sizeof(char*);
char *objNames[] = { "fruit.obj", "tree.obj" };
const char *path = "../res/objs-ordered/levels/%s/%s";
static const int subNameSize = 256;
struct theme *ThemeHolder::genTheme(char *name) {
	struct theme *out = new theme();
	out->themeName = (char *) malloc(subNameSize);
	strcpy_s(out->themeName, subNameSize, name);

	out->filepath = (char **)calloc(sizeof(void *), size);

	unsigned int i = 0;
	for (; i < sharedSize; i++) {
		out->filepath[i] = (char *)malloc(subNameSize);
		strcpy_s(out->filepath[i], subNameSize, sharedObjNames[i]);
	}
	for (;  i < size; i++) {
		out->filepath[i] = (char *) malloc(subNameSize);
		sprintf_s(out->filepath[i], subNameSize, path, name, objNames[i - sharedSize]);
	}

	//out->wallTex		 = -1;
	//out->floorTex		 = -1;
	out->uploadedObj	 = (void **) calloc(sizeof(void *), size);
	out->computedKDtrees = (void **) calloc(sizeof(void *), size);
	return out;
}

char *themeFilePath = "../res/objs-ordered/levels/themeList";
static const int maxWidth = 1024;
char buf[maxWidth];
int ThemeHolder::readInitFile() {
	std::ifstream myfile(themeFilePath);
	if (myfile.is_open())
	{
		while (myfile.getline(buf, maxWidth, '\n'))
			if (buf[0] != 0) {
				printf("%s\n", buf);
				struct theme *t = genTheme(buf);
				themes.push_back(t);
			}

		myfile.close();
		return 0;
	}
	printf("Unable to open file <%s>\n", themeFilePath);
	return -1;
}

void ThemeHolder::loadTex(Scene *scn)
{
	for (int i = 0; i < (signed)themes.size(); i++) {
		int texId = scn->textures.size();
		sprintf_s(buf, maxWidth, path, themes[i]->themeName, floorTexImg);
		scn->AddTexture(buf);
		themes[i]->floorTex = texId;
		sprintf_s(buf, maxWidth, path, themes[i]->themeName, wallTexImg);
		scn->AddTexture(buf);
		themes[i]->wallTex = texId+1;
	}
}

ThemeHolder::ThemeHolder(Scene *scn, int size, int firstTheme) {
	current = firstTheme;
	this->size = size;
	readInitFile();
	loadTex(scn);

	printf("%d themes loaded\n", themes.size());
}

struct theme *ThemeHolder::getCurrentTheme() {
	//printf("theme %d \n", current);
	return themes[current];
}

void ThemeHolder::swapThemes(int next) {
	for (int i = 0; i < sharedSize; i++) {
		themes[next]->uploadedObj[i]	 = themes[current]->uploadedObj[i];
		themes[next]->computedKDtrees[i] = themes[current]->computedKDtrees[i];
	}
	current = next;
}

ThemeHolder::~ThemeHolder(){
	for (int i = 0; i < (signed) themes.size(); i++) {
		delete themes[i]->themeName;
		for (int j = 0; j < size; j++)
			delete themes[i]->filepath[j];		
		delete themes[i]->uploadedObj;
		delete themes[i]->computedKDtrees;
		delete themes[i];
	}
	themes.clear();
}