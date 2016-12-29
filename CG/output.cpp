#include <gl/glut.h>
#include <gl/GL.h>
#include <fstream>
#include <iostream>
#include <FreeImage.h>
using namespace std;

extern int window_width;
extern int window_height;

void saveImage()
{
	cout << "2" << endl;
	BYTE* pixels = new BYTE[3 * window_width * window_height];
	
	glReadPixels(0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	// Convert to FreeImage format & save to file
	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, window_width, window_height, 3 * window_width,
		24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	FreeImage_Save(FIF_BMP, image, "F:/test.bmp", 0);

	// Free resources
	FreeImage_Unload(image);
	delete[] pixels;
}

void output()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	saveImage();

}