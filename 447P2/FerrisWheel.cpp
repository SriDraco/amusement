/* Implementation of FerrisWheel*/
#include "FerrisWheel.h"
//Helper functions:
void
setMaterials(int materialIndex) {
	float materialGrey[] = { 0.8,0.8,0.8 }, materialWhite[] = { 0.2,0.2,0.2 };
	float ringMaterialColours[][3] = { {0.8,0.8,0.0},{0.33,0.33,0.33}, {0.9,0.4,0.4},
								{0.5450,0.27,0}, {0.36,0.36,0.36} },
		ringMaterialLightBr[][3] = { {0,0,0},{0.3,0.3,0.3},{0,0,0},{0,0,0},{0.0,0.0,0.0} };
	float trolMaterialColours[][3] = { {0.2,0.2,0.8},{0.8,0.8,0.0},{0.33,0.33,0.33},
								{0.9,0.4,0.4} ,{0.5450,0.27,0} },
		trolMaterialLightBr[] = { 0.1,0.1,0.1 };

	switch (materialIndex) {
	case 0:
	default:
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialGrey);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialWhite);
		break;
	case FWHEEL_RING:
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ringMaterialColours[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ringMaterialLightBr[0]);
		break;
	case FWHEEL_TROLLEY:
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, trolMaterialColours[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, trolMaterialLightBr);
		break;
	}
}
// Class functions

bool
FerrisWheel::Initialize() {
	wheelRingColor = wheelTrolleyColor = 0;
	wheelWidth = 8.0;
	wheelSpin = 0.0;
	return true;
}
void
FerrisWheel::Draw() {
	int num = 12;
	GLUquadricObj *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);

	glPushMatrix();

	// initialise lights
	GLfloat light_post1[] = { 0.0,50.0,0.0,1.0 }, 
		    whiteSpecularLight[] = { 0.5, 0.5, 0.5 }, blackAmbientLight[] = { 0.3, 0.3, 0.3 }, 
		    whiteDiffuseLight[] = { 0.6, 0.6, 0.6 }, mShininess[] = { 128 }, twoModel[] = { GL_TRUE };
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, blackAmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT1, GL_POSITION, light_post1);

	// Start the ring
	twoModel[0] = GL_FALSE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	setMaterials(FWHEEL_RING);
	glTranslatef(0.0, 0.0, wheelWidth);
	glRotatef(-wheelSpin, 0, 0, 1);
	drawCylinder(0, 0, 0, 0, -55, 20, 1.5, 8);
	glTranslatef(0.0, 0.0, -wheelWidth * 2);
	drawCylinder(0, 0, 0, 0, -55, -20, 1.5, 8);
	glTranslatef(0.0, 0.0, wheelWidth * 2);
	glRotatef(wheelSpin, 0, 0, 1);
	glutSolidTorus(1.0, 35.0, 16, 64);

	twoModel[0] = GL_TRUE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	gluDisk(quadric, 0.0, 10.0, 10.0, 1);
	glTranslatef(0.0, 0.0, -wheelWidth * 2);
	glutSolidTorus(1.0, 35.0, 16, 64);
	gluDisk(quadric, 0.0, 10.0, 10.0, 1);
	glTranslatef(0.0, 0.0, wheelWidth);

	twoModel[0] = GL_FALSE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);

	// Draw the wagons
	for (int i = 0; i < num; i++)
	{
		glEnable(GL_LIGHTING);
		glPushMatrix();
		glRotatef(360 * i / num, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glTranslatef(0.0, 0.0, wheelWidth);
		drawCylinder(0.0, 45.0, 0.0, 0.0, 45.0, -5.0, 1.0, 12);
		drawCylinder(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		glTranslatef(0.0, 0.0, -wheelWidth * 2);
		drawCylinder(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		drawCylinder(0.0, 45.0, 0.0, 0.0, 45.0, 5.0, 1.0, 12);
		glTranslatef(0.0, 0.0, wheelWidth);
		glDisable(GL_LIGHTING);
		glTranslatef(0.0, -45.0, 0.0);
		glRotatef(-wheelSpin - (360 * i / num) - sin(wheelSpin / 10.0)*10.0, 0.0, 0.0, 1.0);
		setMaterials(FWHEEL_TROLLEY);
		drawWagons();
		setMaterials(FWHEEL_RING);
		glTranslatef(0.0, 45.0, 0.0);
		glPopMatrix();

	}
	setMaterials(0);
	glPopMatrix();
	glDisable(GL_LIGHT1);
}
void
FerrisWheel::drawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions) {
	GLUquadricObj *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	float vx = x2 - x1, vy = y2 - y1, vz = z2 - z1;
	if (vz == 0) vz = .0001;
	float v = sqrt(vx*vx + vy * vy + vz * vz);
	float ax = 57.2957795*acos(vz / v);
	if (vz < 0.0)
		ax = -ax;
	float rx = -vy * vz;
	float ry = vx * vz;
	glPushMatrix();
	glTranslatef(x1, y1, z1);
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);
	gluQuadricOrientation(quadric, GLU_INSIDE);
	gluDisk(quadric, 0.0, radius, subdivisions, 1);
	glTranslatef(0, 0, v);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluDisk(quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
	gluDeleteQuadric(quadric);
}
void
FerrisWheel::drawWagons()
{
	GLdouble wagonSize = 5.0;
	double eqn[] = { 0.0, -1.0, 0.0, -3.5 };
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_CLIP_PLANE0);
	for (int c = 0; c < 4; c++)
	{
		glRotatef((90 / 4.0), 0.0, 1.0, 0.0);
		glNormal3f(1.0, 0.0, 0.0);
		glutWireCube(wagonSize*2.0);
	}
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glScalef(2.0, 3.0, 2.0);
	setMaterials(0);
	setMaterials(FWHEEL_TROLLEY);
	glScalef(1 / 2.0, 1 / 3.0, 1 / 2.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPopMatrix();
	glTranslatef(0.0, wagonSize, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(5.0, 5.0, 6, 6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -2 * wagonSize, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadricObj *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluDisk(quadric, 0.0, 6.0, 16.0, 1.0);
	gluDeleteQuadric(quadric);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glPopMatrix();
}