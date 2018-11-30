/*
 * Buildings.h: Texture mapped buildings with subdivided roofs
 *
 *  --KH
 */

#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_
#include "SubDSphere.h"

class Buildings {
private:
	GLubyte display_list;
	GLuint  texture_obj;
	SubDSphere roof;

public:
	void Draw(bool smooth, unsigned int num);
	void	smoothRoof(unsigned int n);
	bool    Initialize(unsigned int num);
};



#endif