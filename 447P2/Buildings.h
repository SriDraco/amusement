/*
 * Buildings.h: Texture mapping some rectangles, a header file
 *
 *  --KT
 */

#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_

class Buildings {
private:
	GLubyte display_list;
	GLuint  texture_obj;


public:
	void Draw(void);
	bool    Initialize(void);
};



#endif