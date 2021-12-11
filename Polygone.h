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
int closestVertex(int x, int y, Polygone p);
int closestEdge(int x, int y, Polygone p);
void deletePolygone(Polygone p);

#endif