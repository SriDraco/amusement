/*
 * Track.cpp: A class that draws the train and its track.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Track.h"
#include <stdio.h>
#include <windows.H>
#include <FL/math.H>
#include <FL/glu.h>
#include "libtarga.h"

 // The control points for the track spline.
const int   Track::TRACK_NUM_CONTROLS = 4;
const float Track::TRACK_CONTROLS[TRACK_NUM_CONTROLS][3] =
{ { -20.0, -20.0, -18.0 }, { 20.0, -20.0, 40.0 },
  { 20.0, 20.0, -18.0 }, { -20.0, 20.0, 40.0 } };

// The carriage energy and mass
const float Track::TRAIN_ENERGY = 250.0f;

// Normalize a 3d vector.
static void
Normalize_3(float v[3])
{
	double  l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	if (l == 0.0)
		return;

	v[0] /= (float)l;
	v[1] /= (float)l;
	v[2] /= (float)l;
}


// Destructor
Track::~Track(void)
{
	if (initialized)
	{
		glDeleteLists(track_list, 1);
		glDeleteLists(train_list, 1);
	}
}


// Initializer. Would return false if anything could go wrong.
bool
Track::Initialize(void)
{
	CubicBspline    refined(3, true);
	int		    n_refined;
	float	    p[3];
	int		    i;
	ubyte   *image_data;
	int	    image_height, image_width;
	

	// Create the track and rail splines.
	track = new CubicBspline(3, true);
	for (i = 0; i < TRACK_NUM_CONTROLS; i++) {
		track->Append_Control(TRACK_CONTROLS[i]);
	}

	// Refine it down to a fixed tolerance. This means that any point on
	// the track that is drawn will be less than 0.1 units from its true
	// location. In fact, it's even closer than that.
	track->Refine_Tolerance(refined, 0.1f);
	n_refined = refined.N();

	// Create the track 
	// Create the display list for the track - just a set of line segments
	// We just use curve evaluated at integer paramer values, because the
	// subdivision has made sure that these are good enough.
	track_list = glGenLists(1);
	glNewList(track_list, GL_COMPILE);
	glColor3f(1.0f, 1.0, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= n_refined; i++)
	{
		refined.Evaluate_Point((float)i, p);
		glVertex3fv(p);
	}
	glEnd();
	glEndList();

	// Set up the train. NOTE: The
	// x-axis will be aligned to point along the track. The origin of the
	// train is assumed to be at the bottom of the train.
	// Now do the geometry. Create the display list.
	// Load the image for the texture. The texture file has to be in
	// a place where it will be found.
	if (!(image_data = (ubyte*)tga_load("railFront.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "track::Initialize: Couldn't load railFront.tga\n");
	}

	// This creates a texture object and binds it, so the next few operations
	// apply to this texture.
	glGenTextures(1, &texture_obj_front);
	glBindTexture(GL_TEXTURE_2D, texture_obj_front);

	// This sets a parameter for how the texture is loaded and interpreted.
	// basically, it says that the data is packed tightly in the image array.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// This sets up the texture with high quality filtering. First it builds
	// mipmaps from the image data, then it sets the filtering parameters
	// and the wrapping parameters. 
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// This says what to do with the texture. Modulate will multiply the
	// texture by the underlying color.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	train_list = glGenLists(1);
	glNewList(train_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the train texture.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj_front);
	glBegin(GL_QUADS);
	//Front
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5f, 0.5f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5f, -0.5f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.5f, -0.5f, 0.0f);
	glScalef(M_PI, M_PI, M_PI);
	
	//Back
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.5f, 0.5f, 1.0f);
	glVertex3f(-1.5f, 0.5f, 0.0f);
	glVertex3f(-1.5f, -0.5f, 0.0f);
	glVertex3f(-1.5f, -0.5f, 1.0f);
	glScalef(M_PI, M_PI, M_PI);
	glEnd();
	
	//Load top texture
	if (!(image_data = (ubyte*)tga_load("railTop.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "track::Initialize: Couldn't load railTop.tga\n");
	}

	// This creates a texture object and binds it, so the next few operations
	// apply to this texture.
	glGenTextures(1, &texture_obj_top);
	glBindTexture(GL_TEXTURE_2D, texture_obj_top);

	// This sets a parameter for how the texture is loaded and interpreted.
	// basically, it says that the data is packed tightly in the image array.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// This sets up the texture with high quality filtering. First it builds
	// mipmaps from the image data, then it sets the filtering parameters
	// and the wrapping parameters. We want the grass to be repeated over the
	// ground.
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// This says what to do with the texture. Modulate will multiply the
	// texture by the underlying color.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//Up
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.5f, -0.5f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.5f, -0.5f, 1.0f);
	glScalef(M_PI, M_PI, M_PI);
	glEnd();
	//Load side texture
	if (!(image_data = (ubyte*)tga_load("railSide.tga", &image_width, &image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "track::Initialize: Couldn't load railSide.tga\n");
	}

	// This creates a texture object and binds it, so the next few operations
	// apply to this texture.
	glGenTextures(1, &texture_obj_top);
	glBindTexture(GL_TEXTURE_2D, texture_obj_top);

	// This sets a parameter for how the texture is loaded and interpreted.
	// basically, it says that the data is packed tightly in the image array.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// This sets up the texture with high quality filtering. First it builds
	// mipmaps from the image data, then it sets the filtering parameters
	// and the wrapping parameters. We want the grass to be repeated over the
	// ground.
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// This says what to do with the texture. Modulate will multiply the
	// texture by the underlying color.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
	//Down
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.5f, -0.5f, 0.0f);
	glVertex3f(-1.5f, -0.5f, 0.0f);
	glVertex3f(-1.5f, 0.5f, 0.0f);
	glVertex3f(1.5f, 0.5f, 0.0f);
	glScalef(M_PI, M_PI, M_PI);
	//Right
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5f, 0.5f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5f, 0.5f, 1.0f);
	glScalef(M_PI, M_PI, M_PI);
	//Left
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5f, -0.5f, 0.0f);
	glTexCoord2f(-1.0, 0.0); glVertex3f(1.5f, -0.5f, 1.0f);
	glTexCoord2f(-1.0, -1.0); glVertex3f(-1.5f, -0.5f, 1.0f);
	glTexCoord2f(0.0, -1.0); glVertex3f(-1.5f, -0.5f, 0.0f);
	glScalef(M_PI, M_PI, M_PI);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();
	

	initialized = true;

	return true;
}


// Draw
void
Track::Draw(void)
{
	float   posn[3];
	float   tangent[3];
	double  angle;
	
	if (!initialized)
		return;

	glPushMatrix();

	// Draw the track
	glCallList(track_list);

	glPushMatrix();

	// Figure out where the train is
	track->Evaluate_Point(posn_on_track, posn);

	// Translate the train to the point
	glTranslatef(posn[0], posn[1], posn[2]);
	
	// ...and what it's orientation is
	track->Evaluate_Derivative(posn_on_track, tangent);
	Normalize_3(tangent);

	// Rotate it to point along the track, but stay horizontal
	angle = atan2(tangent[1], tangent[0]) * 180.0 / M_PI;
	glRotatef((float)angle, 0.0f, 0.0f, 1.0f);

	// Another rotation to get the tilt right.
	angle = asin(-tangent[2]) * 180.0 / M_PI;
	glRotatef((float)angle, 0.0f, 1.0f, 0.0f);

	// Draw the train
	glCallList(train_list);

	glPopMatrix();
	glPopMatrix();
}


void
Track::Update(float dt)
{
	float   point[3];
	float   deriv[3];
	double  length;
	double  parametric_speed;

	if (!initialized)
		return;

	// First we move the train along the track with its current speed.

	// Get the derivative at the current location on the track.
	track->Evaluate_Derivative(posn_on_track, deriv);

	// Get its length.
	length = sqrt(deriv[0] * deriv[0] + deriv[1] * deriv[1] + deriv[2] * deriv[2]);
	if (length == 0.0)
		return;

	// The parametric speed is the world train speed divided by the length
	// of the tangent vector.
	parametric_speed = speed / length;

	// Now just evaluate dist = speed * time, for the parameter.
	posn_on_track += (float)(parametric_speed * dt);

	// If we've just gone around the track, reset back to the start.
	if (posn_on_track > track->N())
		posn_on_track -= track->N();

	// As the second step, we use conservation of energy to set the speed
	// for the next time.
	// The total energy = z * gravity + 1/2 speed * speed, assuming unit mass
	track->Evaluate_Point(posn_on_track, point);
	if (TRAIN_ENERGY - 9.81 * point[2] < 0.0)
		speed = 0.0;
	else
		speed = (float)sqrt(2.0 * (TRAIN_ENERGY - 9.81 * point[2]));
}

void
Track::Ride() {
	double cameraPosn[3];
	float trainPosn[3];
	double cameraLookAt[3];
	float   tangent[3];
	
	// Figure out where the train is
	track->Evaluate_Point(posn_on_track, trainPosn);

	// ...and what it's orientation is
	track->Evaluate_Derivative(posn_on_track, tangent);

	// Put the camera at the right place
	cameraPosn[0] = trainPosn[0];
	cameraPosn[1] = trainPosn[1];
	cameraPosn[2] = trainPosn[2] + 2;

	// Look at the right place. (next point on track + up a little bit)
	cameraLookAt[0] = trainPosn[0] + tangent[0];
	cameraLookAt[1] = trainPosn[1] + tangent[1];
	cameraLookAt[2] = trainPosn[2];
	if (tangent[2] > trainPosn[2])
		cameraLookAt[2] += tangent[2];
	else
		cameraLookAt[2] += 0.8 * tangent[2];

	// Update camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosn[0], cameraPosn[1], cameraPosn[2],
		cameraLookAt[0], cameraLookAt[1], cameraLookAt[2],
		0.0, 0.0, 1.0);

	return;
}