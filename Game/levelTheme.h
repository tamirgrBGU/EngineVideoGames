#pragma once
#include "scene.h"
#include <vector>
#include <fstream>

struct theme {
	char *themeName;
	int floorTex;
	int wallTex;
	//game will use the following two variables to chache objects
	char **filepath;
	void **uploadedObj;
	void **computedKDtrees;
};

class ThemeHolder
{
	std::vector<struct theme *> themes;
	int size;	int current;
public:
	ThemeHolder(Scene *scn, int size, int firstTheme);
	~ThemeHolder();
	struct theme *getCurrentTheme();
	void swapThemes(int next);
private:
	struct theme *genTheme(char *name);
	int readInitFile();
	void loadTex(Scene *scn);
};