#include "Polygone.h"


Polygone createPolygone()
{
    Polygone p = malloc(sizeof(PolygoneStruct));
    p->isClosed = false;
    p->isFilled = false;
    p->points = createListe();
    p->mode = INSERT;
    return p;
}

bool isPolygoneEmpty(Polygone p)
{
    return p->points->taille == 0;
}
void toggleClosed(Polygone p)
{
    p->isClosed = !p->isClosed;
}
void toggleFilled(Polygone p)
{
    p->isFilled = !p->isFilled;
}
void addVertex(Polygone p, Point point)
{ 
    addPoint(p->points, point);
}
bool isBetween(int y1, int y, int y2){
    return (y1 < y && y <= y2) || (y1 >= y && y > y2);
}
int intersectionsAux(int y, Point a, Point b)
{
    int res;
    if (!isBetween(a.y,y,b.y) || (a.y == b.y))
        res = -1;
    else if (a.x == b.x)
        res = a.x;
    else
        res=((a.x*b.y - a.y*b.x)*(-1) - (a.x-b.x)*(-y))/(-(a.y-b.y)*(-1));
    return res;
}
Liste getIntersections(Polygone p, int y)
{
    Liste listeIntersections = createListe();
    ElementListe courant = p->points->premier;
    Point tmp;
    tmp.y = y;
    while (courant->suivant != NULL)
    {
        tmp.x = intersectionsAux(y, courant->point, courant->suivant->point);
        if(tmp.x != -1) insertByX(listeIntersections, tmp);
        courant = courant->suivant;
    }
    tmp.x = intersectionsAux(y, courant->point, p->points->premier->point);
    if(tmp.x != -1) insertByX(listeIntersections, tmp);
    return listeIntersections;
}
void drawBrokenLine(Image *img, Polygone p)
{
    Color c = C_new(0, 0, 255);
    ElementListe courant = p->points->premier;
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y,c);
        courant = courant->suivant;
    }
}
float distance(int x1, int y1, int x2, int y2) { return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); }
int minimum(int a, int b) { return a < b ? a : b; }
int getYmin(Polygone p)
{
    ElementListe courant = p->points->premier;
    int ymin = courant->point.y;
    while (courant != NULL)
    {
        if (courant->point.y < ymin)
            ymin = courant->point.y;
        courant = courant->suivant;
    }
    return ymin;
}
int getYmax(Polygone p)
{
    ElementListe courant = p->points->premier;
    int ymax = courant->point.y;
    while (courant != NULL)
    {
        if (courant->point.y > ymax)
            ymax = courant->point.y;
        courant = courant->suivant;
    }
    return ymax;
}
void closestVertex(int x, int y, Polygone p)
{
    ElementListe courant = p->points->premier, index = courant;
    float min = distance(x, y, courant->point.x, courant->point.y);
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        float dist = distance(x, y, follow->point.x, follow->point.y);
        if (dist < min)
        {
            min = dist;
            index = follow;
        }
        courant = courant->suivant;
    }
    p->points->selected = index;
}

void closestEdge(int x, int y, Polygone p)
{
    closestVertex(x, y, p);
    if (p->points->selected == p->points->dernier && p->mode == EDGE && !p->isClosed)
    {
        p->points->selected = p->points->selected->precedent;
    }
}
void drawSelectedPoint(Image *img, Polygone p)
{
    Color red = C_new(255, 0, 0);
    Point point = p->points->selected->point;
    point.x = p->points->selected->point.x;
    point.y = p->points->selected->point.y;
    I_plotColor(img, point.x, point.y, red);
    int voisins[20][2] = { {point.x+1, point.y},{point.x,point.y+1},{point.x-1,point.y},{point.x,point.y-1},
                    {point.x+1,point.y+1},{point.x-1,point.y+1},{point.x+1,point.y-1},{point.x-1,point.y-1},
                    {point.x+2,point.y},{point.x,point.y+2},{point.x-2,point.y},{point.x,point.y-2},
                    {point.x+2,point.y+1},{point.x-2,point.y+1},{point.x+2,point.y-1},{point.x-2,point.y-1},
                    {point.x+1,point.y+2},{point.x+1,point.y-2},{point.x-1,point.y+2},{point.x-1,point.y-2} };
    for(int i = 0; i<20; i++){
        if(voisins[i][0]>=0 && voisins[i][0]<img->_width && voisins[i][1]>=0 && voisins[i][1]<img->_height)
            I_plotColor(img, voisins[i][0], voisins[i][1], red);
    }
}
void drawPolygoneBordered(Image *img, Polygone p)
{
    Color c = C_new(0, 0, 255);
    ElementListe courant = p->points->premier;
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y, c);
        courant = courant->suivant;
    }
    I_bresenham(img, p->points->dernier->point.x, p->points->dernier->point.y, p->points->premier->point.x, p->points->premier->point.y,c);
}
void drawPolygoneFilled(Image *img, Polygone p)
{
    Liste listeIntersections;
    Color c = C_new(255, 255, 255);
    int ymin = getYmin(p);
    int ymax = getYmax(p);
    for (int y = ymin; y <= ymax; y++)
    {
        listeIntersections = getIntersections(p, y);
        ElementListe courant = listeIntersections->premier;
        while(courant!=NULL){
            Point a = courant->point;
            Point b = b = courant->suivant->point;
            if(courant->suivant==NULL)
                printf("ERROR\n");
            I_bresenham(img, a.x, a.y, b.x, b.y, c);
            courant = courant->suivant->suivant;
        }
    }
}
void drawSelectedEdge(Image *img, Polygone p)
{
    Color c = C_new(255, 0, 0);
    if (p->points->taille > 1)
    {
        if (p->points->selected->suivant != NULL)
        {
            ElementListe courant = p->points->selected;
            ElementListe follow = courant->suivant;
            I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y, c);
        }
        else
        {
            I_bresenham(img, p->points->selected->point.x, p->points->selected->point.y, p->points->premier->point.x, p->points->premier->point.y, c);
        }
    }
}
void drawPolygoneClosed(Image *img, Polygone p)
{
    if (p->isFilled && p->points->taille > 2)
        drawPolygoneFilled(img, p);
    drawPolygoneBordered(img, p);
}
void drawPolygone(Image *img, Polygone p)
{
    I_fill(img, C_new(0, 0, 0));
    if(p->points->taille != 0){
        if (p->isClosed)
            drawPolygoneClosed(img, p);
        else
            drawBrokenLine(img, p);
        if (p->mode == VERTEX)
            drawSelectedPoint(img, p);
        else if (p->mode == EDGE)
            drawSelectedEdge(img, p);
    }
}
void selectNextPoint(Polygone p)
{
    if (p->points->selected->suivant != NULL)
    {
        p->points->selected = p->points->selected->suivant;
        if (!p->isClosed && p->points->selected->suivant == NULL && p->mode == EDGE)
            p->points->selected = p->points->premier;
    }
    else
    {
        p->points->selected = p->points->premier;
    }
}
void selectPreviousPoint(Polygone p)
{
    if (p->points->selected->precedent != NULL)
        p->points->selected = p->points->selected->precedent;
    else
    {
        if (!p->isClosed && p->mode == EDGE)
            p->points->selected = p->points->dernier->precedent;
        else
            p->points->selected = p->points->dernier;
    }
}
void removeSelectedPoint(Polygone p)
{
    ElementListe courant = p->points->selected;
    selectNextPoint(p);
    if (courant->precedent != NULL)
    {
        courant->precedent->suivant = courant->suivant;
    }
    else
    {
        p->points->premier = courant->suivant;
    }
    if (courant->suivant != NULL)
    {
        courant->suivant->precedent = courant->precedent;
    }
    else
    {
        p->points->dernier = courant->precedent;
    }
    free(courant);
    p->points->taille--;
}
void moveSelectedPoint(Polygone p, int dx, int dy)
{
    
    p->points->selected->point.x += dx;
    p->points->selected->point.y += dy;
}
Point getPointBetweenTwoPoints(Point a, Point b)
{
    Point p;
    p.x = (a.x + b.x) / 2;
    p.y = (a.y + b.y) / 2;
    return p;
}
void createPointBetweenTwoPoints(Polygone p)
{
    ElementListe courant = p->points->selected;
    ElementListe follow = courant->suivant;
    if (follow == NULL)
    {
        addVertex(p, getPointBetweenTwoPoints(courant->point, p->points->premier->point));
    }
    else
    {
        ElementListe newPoint = (ElementListe)malloc(sizeof(StructElementListe));
        newPoint->point = getPointBetweenTwoPoints(courant->point, follow->point);
        newPoint->suivant = follow;
        newPoint->precedent = courant;
        p->points->taille++;
        newPoint->index = p->points->taille;
        courant->suivant = newPoint;
        follow->precedent = newPoint;
        p->points->selected = newPoint;
    }
}
void selectPointByIndex(Polygone p, int index)
{
    ElementListe courant = p->points->premier;
    while (courant->suivant != NULL)
    {
        if (courant->index == index)
        {
            p->points->selected = courant;
            return;
        }
        courant = courant->suivant;
    }
    if (courant->index == index)
    {
        p->points->selected = courant;
        return;
    }
}
void selectLastPoint(Polygone p)
{
    p->points->selected = p->points->dernier;
}

void freePolygone(Polygone p)
{ 
    freeListe(p->points);
    free(p);
}

void dragAndDrop(Polygone p, int x, int y){
    p->points->selected->point.x = x;
    p->points->selected->point.y = y;
}