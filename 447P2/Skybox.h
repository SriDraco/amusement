// Skybox, the texture mapped sky
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <math.h>
const int SKY_FRONT = 0, SKY_RIGHT = 1, SKY_LEFT = 2, SKY_BACK = 3, SKY_UP = 4, SKY_DOWN = 5;
class Skybox {
private:
	GLint sky[6];

public:
	void Initialize(void);
	void Draw(void);
};