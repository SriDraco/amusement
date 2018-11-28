//Ferris Wheel, the class to handle initializing, drawing, and riding the ferris wheel
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <math.h>
const int FWHEEL_RING = 1, FWHEEL_TROLLEY = 2, FWHEEL_TOP = 3;
class FerrisWheel {
private:
	int wheelRingColor;
	int wheelTrolleyColor;
	GLdouble wheelWidth;
	GLdouble wheelSpin;
	void drawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions);
	void drawWagons(void);
public:
	bool Initialize(void);
	void Draw(void);
};