#include "Polygone.h"


Polygone createPolygone() {
    Polygone p = malloc(sizeof(PolygoneStruct));
    p->isClosed = false;
    p->isFilled = false;
    p->points = new_Liste();
    return p;
}

bool isPolygoneEmpty(Polygone p) {
    return size(p->points) == 0;
}

void toggleClosed(Polygone p) {
    p->isClosed = !p->isClosed;
}
void toggleFilled(Polygone p) {
    p->isFilled = !p->isFilled;
}
void addPoint(Polygone p, Point point) {
    ajouterPointFin(p->points, point);
}
void drawPolygone(Image *img, Polygone p){
    Color c = {0, 0, 0};
    I_fill(img,c);
    if(p->isClosed)
        drawPolygoneClosed(img,p);
    else
        drawBrokenLine(img,p);
}

void drawPolygoneClosed(Image *img, Polygone p){
    if(p->isFilled)
        drawPolygoneFilled(img,p);
    else
        drawPolygoneBordered(img,p);
}


void drawPolygoneBordered(Image *img, Polygone p){
    ElementListe courant = p->points->premier;
	while(courant->suivant != NULL){
		ElementListe follow = courant->suivant;
		I_bresenham(img,courant->point.x,courant->point.y,follow->point.x,follow->point.y);
		courant = courant->suivant;
	}
    I_bresenham(img,p->points->dernier->point.x,p->points->dernier->point.y,p->points->premier->point.x,p->points->premier->point.y);
}

void drawPolygoneFilled(Image *img, Polygone p){

}

void drawBrokenLine(Image *img, Polygone p){
	ElementListe courant = p->points->premier;
	while(courant->suivant != NULL){
		ElementListe follow = courant->suivant;
		I_bresenham(img,courant->point.x,courant->point.y,follow->point.x,follow->point.y);
		courant = courant->suivant;
	}
}
