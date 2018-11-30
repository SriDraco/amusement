/*
 * Buildings.cpp: Texture mapping some rectangles
 *
 *  --KT
 */
#include <FL/glu.H>
#include "Buildings.h"
#include <windows.H>
#include <GL/glut.h>
#include <FL/math.H>
#include "libtarga.h"
#include <stdio.h>

bool
Buildings::Initialize(unsigned int num) {
	
	ubyte   *image_data;
	int	    image_height, image_width;

	// Load the image for the texture. The texture file has to be in
	// a place where it will be found.
	switch (num)
	{
	case 0:
	default:
		if (!(image_data = (ubyte*)tga_load("brick.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
		{
			fprintf(stderr, "Buildings::Initialize: Couldn't load brick.tga\n");
			return false;
		}
		break;
	case 1:
		if (!(image_data = (ubyte*)tga_load("stone.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
		{
			fprintf(stderr, "Buildings::Initialize: Couldn't load brick.tga\n");
			return false;
		}
		break;
	case 2:
		if (!(image_data = (ubyte*)tga_load("wood.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
		{
			fprintf(stderr, "Buildings::Initialize: Couldn't load brick.tga\n");
			return false;
		}
		break;
	case 3:
		if (!(image_data = (ubyte*)tga_load("broken.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
		{
			fprintf(stderr, "Buildings::Initialize: Couldn't load brick.tga\n");
			return false;
		}
		break;
	}
	glGenTextures(1, &texture_obj);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image_data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	free(image_data);
	roof.Initialize();
	return true;
}

void 
Buildings::Draw(bool smooth,  unsigned int num) {
	glPushMatrix();
	int cornerTest = 45;
	switch (num) {
	case 0:
	default:
		glTranslatef(cornerTest, -cornerTest, 1);
		break;
	case 1:
		glTranslatef(-cornerTest, -cornerTest, 1);
		break;
	case 2:
		glTranslatef(-cornerTest, cornerTest, 1);
		break;
	case 3:
		glTranslatef(cornerTest, cornerTest, 1);
		break;
	};
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glPushMatrix();
	glScalef(5, 5, 3);
	glBegin(GL_QUADS);
	// Top face
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(.50f, .50f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-.50f, .50f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.50f, .50f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(.50f, .50f, 1.0f);
	
	// Bottom face
	glNormal3f(0, -1.0f, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(.50f, -.50f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-.50f, -.50f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.50f, -.50f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(.50f, -.50f, -1.0f);

	// Front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(.50f, .50f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-.50f, .50f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.50f, -.50f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(.50f, -.50f, 1.0f);

	// Back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(.50f, -.50f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-.50f, -.50f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.50f, .50f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(.50f, .50f, -1.0f);

	// Left face 
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-.5f, 0.5f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-.5f, -0.5f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.5f, -0.5f, 1.0f);

	// Right face
	glNormal3f(1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -1.0f);
	glEnd();
	//Add Roof (subdivided sphere)
	GLfloat trans[3];
	trans[0] = 0;
	trans[1] = 0;
	trans[2] = 1.5f;
	roof.Draw(trans, 0, 0, smooth);
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void
Buildings::smoothRoof(unsigned int n)
{
	roof.Subdivide(n);
}