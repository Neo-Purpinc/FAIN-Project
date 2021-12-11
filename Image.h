/*====================================================*\
  Arash HABIBI
  Image.h
\*====================================================*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "Ppm.h"
#include "utils.h"

//-----------------------------------------------------

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);
void C_check(Color c, char *message);

//-----------------------------------------------------

typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

Image* I_new(int _width, int _height);
Image* I_read(char *ppmfilename);
void I_copy(Image *image,Image *sortie);

void I_fill       (Image *img, Color c);
void I_checker    (Image *img, Color c1, Color c2, int step);

void I_changeColor(Image *img, Color c);
void I_plot       (Image *img, int x, int y);
void I_plotColor  (Image *img, int x, int y, Color c);

void I_focusPoint (Image *img, int x, int y);
void I_zoomInit   (Image *img);
void I_zoom       (Image *img, double zoom_coef);
void I_move       (Image *img, int x, int y);

void I_draw       (Image *img);

void I_bresenhamOrigin(Image *img, int x, int y);
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);
void I_bresenhamColor(Image *img, int xA, int yA, int xB, int yB, Color c);
void I_remplissage4(Image *img_in, Image *img_out, int x_germe, int y_germe);
void I_remplissage8(Image *img_in, Image *img_out, int x_germe, int y_germe);

Color getColor(Image *img, int x, int y);
bool equalColors(Color a, Color b);
bool isInImage(Image *img, int x, int y);

#endif



