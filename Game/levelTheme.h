#pragma once
#include "scene.h"
#include <vector>
#include <fstream>

struct theme {
	char *themeName;
	std::vector<int> texNo;
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
	int getTex(int type) {
		return themes[current]->texNo[type];
	};
	struct theme *getCurrentTheme() {
		//printf("theme %d \n", current);
		return themes[current];
	}
	void swapThemes(int next);
private:
	struct theme *genTheme(char *name);
	int readInitFile();
	void loadTex(Scene *scn);
};