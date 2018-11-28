// Textures.cpp
// Loads targa images as textures with error checking and the like
// KT

#include "Textures.h"
#include "libtarga.h"
#include <iostream>

using namespace std;

 
Textures * Textures::Load_Image(char * filename) {
	unsigned char   *temp_data;
	Textures	    *temp_image;
	Textures	    *result;
	int		        width, height;

	if (!filename)
	{
		cout << "No filename given." << endl;
		return NULL;
	}

	temp_data = (unsigned char*)tga_load(filename, &width, &height, TGA_TRUECOLOR_32);
	if (!temp_data)
	{
		cout << "TGA Error: %s\n", tga_error_string(tga_get_last_error());
		width = height = 0;
		return NULL;
	}
	temp_image = new Textures(width, height, temp_data);
	free(temp_data);

	result = temp_image->Reverse_Rows();

	delete temp_image;

	return result;
}
Textures* Textures::Reverse_Rows(void)
{
	unsigned char   *dest = new unsigned char[width * height * 4];
	Textures	    *result;
	int 	        i, j;

	if (!data)
		return NULL;

	for (i = 0; i < height; i++)
	{
		int in_offset = (height - i - 1) * width * 4;
		int out_offset = i * width * 4;

		for (j = 0; j < width; j++)
		{
			dest[out_offset + j * 4] = data[in_offset + j * 4];
			dest[out_offset + j * 4 + 1] = data[in_offset + j * 4 + 1];
			dest[out_offset + j * 4 + 2] = data[in_offset + j * 4 + 2];
			dest[out_offset + j * 4 + 3] = data[in_offset + j * 4 + 3];
		}
	}

	result = new Textures(width, height, dest);
	delete[] dest;
	return result;
}// Reverse_Rows
Textures::Textures(int w, int h, unsigned char *d)
{
	int i;

	width = w;
	height = h;
	data = new unsigned char[width * height * 4];

	for (i = 0; i < width * height * 4; i++)
		data[i] = d[i];
}
Textures::Textures() : width(0), height(0), data(NULL)
{}
Textures::~Textures()
{
	if (data)
		delete[] data;
}