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
Buildings::Initialize(void) {
	
	ubyte   *image_data;
	int	    image_height, image_width;

	// Load the image for the texture. The texture file has to be in
	// a place where it will be found.
	if (!(image_data = (ubyte*)tga_load("brick.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Buildings::Initialize: Couldn't load .tga\n");
		return false;
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

	return true;
}

void 
Buildings::Draw(void) {

	glTranslatef(1.5f, 0.0f, 2.0f);  // Move right and into the screen
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	   // Top face (y = 1.0f)
	   // Define vertices in counter-clockwise (CCW) order with normal pointing out
	//glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	//glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	//glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	//glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	//glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}