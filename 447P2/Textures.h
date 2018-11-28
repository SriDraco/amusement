// Textures.h
// For loading targa images as textures -- a header file
// KT

#ifndef _TEXTURES_H_
#define _TEXTURES_H_
class Textures {

	Textures(void);
	Textures(int w, int h, unsigned char *d);
	~Textures(void);
	static Textures* Load_Image(char*);
	Textures* Reverse_Rows(void);

	int		width;	    
	int		height;	   
	unsigned char	*data;
};
#endif