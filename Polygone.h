#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include "utils.h"
#include "Image.h"

typedef struct {
    bool isClosed;
    bool isFilled;
    Liste points;
    enum mode mode;
} PolygoneStruct, *Polygone;

Polygone createPolygone();
void setMode(Polygone p, enum mode mode);
enum mode getMode(Polygone p);
void addPoint(Polygone p, Point point);
bool isPolygoneEmpty(Polygone p);
void toggleClosed(Polygone p);
void toggleFilled(Polygone p);
void drawPolygone(Image *img, Polygone p);
int closestVertex(int x, int y, Polygone p);
int closestEdge(int x, int y, Polygone p);
void deletePolygone(Polygone p);
void selectNextPoint(Polygone p);
void selectPreviousPoint(Polygone p);
void selectLastPoint(Polygone p);
void removeSelectedPoint(Polygone p);
void deplacerSelectedPoint(Polygone p, int dx, int dy);
void selectNextEdge(Polygone p);
void selectPreviousEdge(Polygone p);
void createPointBetweenTwoPoints(Polygone p);
#endif