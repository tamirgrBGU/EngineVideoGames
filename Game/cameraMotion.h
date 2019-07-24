#pragma once
#include "game.h"

void initCameraMotion(Game *obj, MovableGLM *tail, float z);
void updateCam();
void switchCamMode();