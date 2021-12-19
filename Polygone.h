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
void addVertex(Polygone, Point);
bool isPolygoneEmpty(Polygone);
void toggleClosed(Polygone);
void toggleFilled(Polygone);
void drawPolygone(Image*, Polygone);
void closestVertex(int, int, Polygone);
void closestEdge(int, int, Polygone);
void selectNextPoint(Polygone);
void selectPreviousPoint(Polygone);
void selectLastPoint(Polygone);
void removeSelectedPoint(Polygone);
void moveSelectedPoint(Polygone, int, int);
void createPointBetweenTwoPoints(Polygone);
void freePolygone(Polygone);
void dragAndDrop(Polygone, int, int);
#endif