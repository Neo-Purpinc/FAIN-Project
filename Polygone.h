#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include "utils.h"
#include "Image.h"

typedef struct {
    bool isClosed;
    bool isFilled;
    Liste points;
} PolygoneStruct, *Polygone;

Polygone createPolygone();
void addPoint(Polygone p, Point point);
bool isPolygoneEmpty(Polygone p);
void toggleClosed(Polygone p);
void toggleFilled(Polygone p);
void drawPolygone(Image *img, Polygone p);
void drawPolygoneClosed(Image *img, Polygone p);
void drawPolygoneBordered(Image *img, Polygone p);
void drawPolygoneFilled(Image *img, Polygone p);
void drawBrokenLine(Image *img,Polygone p);

#endif