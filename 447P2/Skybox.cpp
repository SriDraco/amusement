#include "Skybox.h"
GLuint 
LoadBMP(const char *fileName)
{
	FILE *file;
	unsigned char header[54], *data;
	unsigned int dataPos, size, width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);				//Windows BMP begin with 54 byte header
	dataPos = *(int*)&(header[0x0A]);	//dec10, Actual BMP data
	size = *(int*)&(header[0x22]);	//dec34, BMP Size
	width = *(int*)&(header[0x12]);	//dec18, Image Width
	height = *(int*)&(header[0x16]);	//dec22, Image Height
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);				//Generate (allocate) 1 texture name
	glBindTexture(GL_TEXTURE_2D, texture);	//Bind the 2D texture



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//MAG filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//MIN filter

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); //target, level, internalFormat, width, height,border, format, type, data 
	return texture;
}

void
Skybox::Initialize() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	sky[SKY_DOWN] = 0;
	sky[SKY_FRONT] = LoadBMP("front.bmp");
	sky[SKY_BACK] = LoadBMP("back.bmp");
	sky[SKY_RIGHT] = LoadBMP("right.bmp");
	sky[SKY_LEFT] = LoadBMP("left.bmp");
	sky[SKY_UP] = LoadBMP("up.bmp");
}
void
Skybox::Draw() {
	float x = 1.0f, y = 0.0f, z = 0.0f, width = 250, height = 250, length = 250;
	glDisable(GL_DEPTH_TEST);
	x = x - width / 2;
	y = y - length / 2;
	z = z - height / 2;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sky[SKY_UP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + length, z + height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + length, z + height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + height);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + length, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + length, z + height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + height);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky[SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + length, z + height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + length, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z + height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + length, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + length, z + height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + length, z + height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + length, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky[SKY_DOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + length, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + length, z);
	glEnd();
	glDisable(GL_TEXTURE_2D);	
}