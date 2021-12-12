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
void addVertex(Polygone p, Point point) { addPoint(p->points, point); }
bool intersectionsAux(int y, Point a, Point b, Point *res)
{
    if ((a.y > y && b.y > y) || (a.y < y && b.y < y))
    {
        return false;
    }
    Point c, d;
    c.x = 0;
    c.y = y;
    d.x = 1;
    d.y = y;
    res->x = ((a.x * b.y - a.y * b.x) * (c.x - d.x) - (a.x - b.x) * (c.x * d.y - c.y * d.x)) / ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x)) + 1;
    res->y = y;
    return true;
}
Liste getIntersections(Polygone p, int y)
{
    Liste listeIntersections = createListe();
    ElementListe courant = p->points->premier, follow = courant->suivant;
    Point pointCourant, pointFollow, tmp;
    int yPrevious, yNext;
    bool isEqual;
    while (courant != NULL)
    {
        isEqual = false;
        follow = courant->suivant;
        if(follow == NULL)
        {
            follow = p->points->premier;
        }
        pointCourant = courant->point;
        pointFollow = follow->point;
        if(intersectionsAux(y, pointCourant, pointFollow, &tmp))
        {
            addPoint(listeIntersections, tmp);
            if (equalPoints(pointCourant, tmp))
            {
                isEqual = true;
                yPrevious = courant->precedent->point.y;
                yNext = pointCourant.y;
            }
            else if (equalPoints(pointFollow, tmp))
            {
                isEqual = true;
                yPrevious = pointCourant.y;
                yNext = pointFollow.y;
            }
            if (isEqual && ((yPrevious > y && yNext > y) || (yPrevious < y && yNext < y)))
            {
                addPoint(listeIntersections, tmp);
            }
        }
        courant = courant->suivant;
    }
    return listeIntersections;
}
void drawBrokenLine(Image *img, Polygone p)
{
    ElementListe courant = p->points->premier;
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y,C_new(255,255,255));
        courant = courant->suivant;
    }
}
float distance(int x1, int y1, int x2, int y2) { return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); }
int minimum(int a, int b) { return a < b ? a : b; }
int getYmin(Polygone p)
{
    ElementListe courant = p->points->premier;
    int ymin = courant->point.y;
    while (courant->suivant != NULL)
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
    while (courant->suivant != NULL)
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
float shortestDistancePointSegment(int x, int y, Point a, Point b)
{
    // a et b sont les extrémités du segment
    // x et y sont le point à tester
    // Déterminer l'équation y = ax + b
    float a1 = (b.y - a.y) / (b.x - a.x);
    float b1 = a.y - a1 * a.x;
    float dist = abs(-a1 * x + y - b1) / sqrt(pow(a1, 2) + 1);
    return dist;
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
    I_plotColor(img, p->points->selected->point.x, p->points->selected->point.y, red);

    I_plotColor(img, p->points->selected->point.x + 1, p->points->selected->point.y, red);
    I_plotColor(img, p->points->selected->point.x, p->points->selected->point.y + 1, red);
    I_plotColor(img, p->points->selected->point.x - 1, p->points->selected->point.y, red);
    I_plotColor(img, p->points->selected->point.x, p->points->selected->point.y - 1, red);

    I_plotColor(img, p->points->selected->point.x + 1, p->points->selected->point.y + 1, red);
    I_plotColor(img, p->points->selected->point.x - 1, p->points->selected->point.y - 1, red);
    I_plotColor(img, p->points->selected->point.x - 1, p->points->selected->point.y + 1, red);
    I_plotColor(img, p->points->selected->point.x + 1, p->points->selected->point.y - 1, red);

    I_plotColor(img, p->points->selected->point.x + 2, p->points->selected->point.y, red);
    I_plotColor(img, p->points->selected->point.x - 2, p->points->selected->point.y, red);
    I_plotColor(img, p->points->selected->point.x, p->points->selected->point.y + 2, red);
    I_plotColor(img, p->points->selected->point.x, p->points->selected->point.y - 2, red);
    I_plotColor(img, p->points->selected->point.x + 2, p->points->selected->point.y + 1, red);
    I_plotColor(img, p->points->selected->point.x - 2, p->points->selected->point.y + 1, red);
    I_plotColor(img, p->points->selected->point.x + 2, p->points->selected->point.y - 1, red);
    I_plotColor(img, p->points->selected->point.x - 2, p->points->selected->point.y - 1, red);
    I_plotColor(img, p->points->selected->point.x + 1, p->points->selected->point.y + 2, red);
    I_plotColor(img, p->points->selected->point.x - 1, p->points->selected->point.y + 2, red);
    I_plotColor(img, p->points->selected->point.x + 1, p->points->selected->point.y - 2, red);
    I_plotColor(img, p->points->selected->point.x - 1, p->points->selected->point.y - 2, red);
}
void drawPolygoneBordered(Image *img, Polygone p)
{
    ElementListe courant = p->points->premier;
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y, C_new(255, 255, 255));
        courant = courant->suivant;
    }
    I_bresenham(img, p->points->dernier->point.x, p->points->dernier->point.y, p->points->premier->point.x, p->points->premier->point.y, C_new(255, 255, 255));
}
void drawPolygoneFilled(Image *img, Polygone p)
{
    Liste listeIntersections;
    int ymin = getYmin(p);
    int ymax = getYmax(p);
    for (int y = ymin; y <= ymax; y++)
    {
        listeIntersections = getIntersections(p, y);
        sortListe(listeIntersections);
        listeIntersections->dernier->suivant = listeIntersections->premier;
        for (int i = 0; i < listeIntersections->taille; i += 2)
        {
            Point a = listeIntersections->premier->point;
            Point b = listeIntersections->premier->suivant->point;
            for (int x = a.x; x < b.x; x++)
            {
                I_plotColor(img, x, y, C_new(255, 255, 255));
            }
            listeIntersections->premier = listeIntersections->premier->suivant->suivant;
        }
    }
}
void drawSelectedEdge(Image *img, Polygone p)
{
    Color red = C_new(255, 0, 0);
    if (p->points->taille > 2)
    {
        if (p->points->selected->suivant != NULL)
        {
            ElementListe courant = p->points->selected;
            ElementListe follow = courant->suivant;
            I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y, red);
        }
        else
        {
            I_bresenham(img, p->points->selected->point.x, p->points->selected->point.y, p->points->premier->point.x, p->points->premier->point.y, red);
        }
    }
}
void drawPolygoneClosed(Image *img, Polygone p)
{
    if (p->isFilled)
        drawPolygoneFilled(img, p);
    else
        drawPolygoneBordered(img, p);
}
void drawPolygone(Image *img, Polygone p)
{
    I_fill(img, C_new(0, 0, 0));
    if (p->isClosed)
        drawPolygoneClosed(img, p);
    else
        drawBrokenLine(img, p);
    if (p->mode == VERTEX)
        drawSelectedPoint(img, p);
    else if (p->mode == EDGE)
        drawSelectedEdge(img, p);
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
    printf("Point non trouve\n");
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