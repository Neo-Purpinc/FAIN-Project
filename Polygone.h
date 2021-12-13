#ifndef _POLYGONE_H_
#define _POLYGONE_H_


#include "Image.h"
#include "Liste.h"
#include <float.h>

typedef enum { INSERT, VERTEX, EDGE } Mode;
typedef struct {
    bool isClosed;
    bool isFilled;
    Liste points;
    Mode mode;
} PolygoneStruct, *Polygone;

Polygone createPolygone();
void addVertex(Polygone p, Point point);
bool isPolygoneEmpty(Polygone p);
void toggleClosed(Polygone p);
void toggleFilled(Polygone p);
void drawPolygone(Image *img, Polygone p);
void closestVertex(int x, int y, Polygone p);
void closestEdge(int x, int y, Polygone p);
void selectNextPoint(Polygone p);
void selectPreviousPoint(Polygone p);
void selectLastPoint(Polygone p);
void removeSelectedPoint(Polygone p);
void moveSelectedPoint(Polygone p, int dx, int dy);
void selectNextEdge(Polygone p);
void selectPreviousEdge(Polygone p);
void createPointBetweenTwoPoints(Polygone p);
void selectPointByIndex(Polygone p, int index);
void freePolygone(Polygone p);
void dragAndDrop(Polygone p, int x, int y);
#endif