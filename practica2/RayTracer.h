#ifndef RayTracer_h

#define RayTracer_h

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include <GL/glut.h> //OpenGL Utility Toolkits

#include "Scene.h"

using namespace glm;

Scene *scene;

void Render();

#endif

