/*
 * CS559 Maze Project
 *
 * Class header file for the WorldWindow class. The WorldWindow is
 * the window in which the viewer's view of the world is displayed.
 *
 * (c) Stephen Chenney, University of Wisconsin at Madison, 2001-2002
 *
 */

#ifndef _WORLDWINDOW_H_
#define _WORLDWINDOW_H_

#include <FL/fl.H>
#include <FL/Fl_Gl_Window.H>
#include "Ground.h"
#include "Track.h"
#include "Buildings.h"
#include "FerrisWheel.h"


// Subclass the Fl_Gl_Window because we want to draw OpenGL in here.
class WorldWindow : public Fl_Gl_Window {
public:
	// Constructor takes window position and dimensions, the title.
	WorldWindow(int x, int y, int w, int h, char *label);

	// draw() method invoked whenever the view changes or the window
	// otherwise needs to be redrawn.
	void	draw(void);

	// Event handling method. Uses only mouse events.
	int	handle(int);

	// Update the world according to any events that have happened since
	// the last time this method was called.
	bool	Update(float);

private:
	// Objects in the world
	Ground		ground;		// The ground object.
	Track		traintrack;	// The train and track.
	Buildings	building[4];// The buildings in the world
	FerrisWheel FWheel;		// Ferris Wheel

	// Object Meta variables
	bool		riding;		// Are you riding the train?
	bool		smooth;		// Are the subdivions smooth?
	bool		corner1;	// Looking at building 1
	bool		corner2;	// Looking at building 2
	bool		corner3;	// Looking at building 3
	bool		corner4;	// Looking at building 4
	bool		wheelView;	// Looking at wheel

	// Viewer Variables
	static const double FOV_X; // The horizontal field of view.

	float	phi;	// Viewer's inclination angle.
	float	theta;	// Viewer's azimuthal angle.
	float	dist;	// Viewer's distance from the look-at point.
	float	x_at;	// The x-coord to look at.
	float	y_at;	// The y-coord to look at. z_at is assumed 2.0.

	int     button;	// The mouse button that is down, -1 if none.
	int	x_last;	// The location of the most recent mouse event
	int	y_last;
	int	x_down; // The location of the mouse when the button was pushed
	int	y_down;
	float   phi_down;   // The view inclination angle when the mouse
				// button was pushed
	float   theta_down; // The view azimuthal angle when the mouse
				// button was pushed
	float	dist_down;  // The distance when the mouse button is pushed.
	float	x_at_down;  // The x-coord to look at when the mouse went down.
	float	y_at_down;  // The y-coord to look at when the mouse went down.

	// Mouse function
	void	Drag(float);	// The function to call for mouse drag events
};

#endif