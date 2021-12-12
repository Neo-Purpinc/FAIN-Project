#include "Polygone.h"

Polygone createPolygone() {
    Polygone p = malloc(sizeof(PolygoneStruct));
    p->isClosed = false;
    p->isFilled = false;
    p->points = new_Liste();
    p->mode = INSERT;
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
void addPoint(Polygone p, Point point) { ajouterPointFin(p->points, point); }

void fillLine(Image *img, Liste l, int y){
    printf("Début de fillLine\n");
    printf("sizeL = %d\n", size(l));
    for(int i = 0; i<size(l); i+=2){
        if(l->premier == NULL){
            printf("l->premier == NULL\n");
            break;
        }
        if(l->premier->suivant == NULL){
            printf("l->premier->suivant == NULL\n");
            break;
        }
        if(size(l) != 1)
            I_bresenham(img,l->premier->point.x,y,l->premier->suivant->point.x,y);
        printf("Segment tracé");
        l->premier = l->premier->suivant->suivant;
    }
    printf("Fin de fillLine\n");
}
bool intersectionsAux(int y, Point a, Point b, Point* res){
    printf("Début de l'intersection\n");
    if((a.y>y && b.y>y) || (a.y<y && b.y<y)){
        printf("C'est faux\n");
        return false;
    }
    Point c, d;
    c.x = 0;
    c.y = y;
    d.x = 1;
    d.y = y;
    printf("C'est vrai\n");
    res->x=((a.x*b.y - a.y*b.x)*(c.x-d.x) - (a.x-b.x)*(c.x*d.y - c.y*d.x))/((a.x-b.x)*(c.y-d.y) - (a.y-b.y)*(c.x-d.x))+1;
    printf("Division par 0 ?");
    res->y=y;
    printf("Intersection trouvée\n");
    return true;
}
Liste getIntersections(Polygone p, int y){
    Liste listeIntersections = new_Liste();
    ElementListe courant = p->points->premier;
    Point tmp;
    int yPrevious, yNext;
    bool isEqual;
    bool aux;
    while(courant->suivant != NULL){
        isEqual = false;
        printf("Avant follow\n");
        ElementListe follow = courant->suivant;
        printf("Avant if !intersectionsAux\n");
        if(!(aux=intersectionsAux(y,courant->point,follow->point,&tmp))){
            printf("Après if !intersectionsAux\n");
            ajouterPointFin(listeIntersections,tmp);
            if(equalPoints(courant->point,tmp)){
                printf("Cas 1\n");
                isEqual = true;
                yPrevious = courant->precedent->point.y;
                yNext = courant->point.y;
            } else if (equalPoints(follow->point,tmp)){
                printf("Cas 2\n");
                isEqual = true;
                yPrevious = courant->point.y;
                yNext = follow->point.y;
            }
            if(isEqual && ((yPrevious > y && yNext > y) || (yPrevious < y && yNext < y))){
                printf("Cas 3\n");
                ajouterPointFin(listeIntersections,tmp);
            }
        }
        printf("On sort du if\n");
        courant = courant->suivant;
    }
    isEqual = false;
    courant = p->points->premier;
    if(!(aux=intersectionsAux(y,courant->point,courant->suivant->point,&tmp))){
        printf("Cas 4\n");
        ajouterPointFin(listeIntersections,tmp);
        if(equalPoints(courant->point,tmp)){
            printf("Cas 5\n");
            isEqual = true;
            yPrevious = courant->precedent->point.y;
            yNext = courant->point.y;
        } else if (equalPoints(courant->suivant->point,tmp)){
            printf("Cas 6\n");
            isEqual = true;
            yPrevious = courant->point.y ;
            yNext = courant->suivant->point.y;
        }
        if(isEqual && ((yPrevious > y && yNext > y) || (yPrevious < y && yNext < y))){
            printf("Cas 7\n");
            ajouterPointFin(listeIntersections,tmp);
        }
    }
    return sortListeByX(listeIntersections);
}
void drawBrokenLine(Image *img, Polygone p){
	ElementListe courant = p->points->premier;
	while(courant->suivant != NULL){
		ElementListe follow = courant->suivant;
		I_bresenham(img,courant->point.x,courant->point.y,follow->point.x,follow->point.y);
		courant = courant->suivant;
	}
}

float distance(int x1, int y1, int x2, int y2){ return sqrt(pow(x1-x2,2)+pow(y1-y2,2));}
int minimum(int a, int b){ return a<b?a:b; }

int getYmin(Polygone p){
    ElementListe courant = p->points->premier;
    int ymin = courant->point.y;
    while(courant->suivant != NULL){
        if(courant->point.y < ymin)
            ymin = courant->point.y;
        courant = courant->suivant;
    }
    return ymin;
}
int getYmax(Polygone p){
    ElementListe courant = p->points->premier;
    int ymax = courant->point.y;
    while(courant->suivant != NULL){
        if(courant->point.y > ymax)
            ymax = courant->point.y;
        courant = courant->suivant;
    }
    return ymax;
}
int closestVertex(int x, int y, Polygone p){
    ElementListe courant = p->points->premier;
    float min = distance(x,y,courant->point.x,courant->point.y);
    int index = 0;
    while(courant->suivant != NULL){
        ElementListe follow = courant->suivant;
        float dist = distance(x,y,follow->point.x,follow->point.y);
        if(dist < min){
            min = dist;
            index = follow->index;
        }
        courant = courant->suivant;
    }
    return index;
}

int closestEdge(int x, int y, Polygone p){
    ElementListe courant = p->points->premier;
    float min = distance(x,y,courant->point.x,courant->point.y);
    int index = 0,index2 = 0;
    while(courant->suivant != NULL){
        ElementListe follow = courant->suivant;
        float dist = distance(x,y,follow->point.x,follow->point.y);
        if(dist < min){
            index2 = index;
            min = dist;
            index = follow->index;
        }
        courant = courant->suivant;
    }
    return (minimum(index,index2));
}
void drawSelectedPoint(Image *img, Polygone p){
    Color red = C_new(255,0,0);
    I_plotColor(img,p->points->selected->point.x,p->points->selected->point.y,red);
    I_plotColor(img,p->points->selected->point.x+1,p->points->selected->point.y,red);
    I_plotColor(img,p->points->selected->point.x,p->points->selected->point.y+1,red);
    I_plotColor(img,p->points->selected->point.x-1,p->points->selected->point.y,red);
    I_plotColor(img,p->points->selected->point.x,p->points->selected->point.y-1,red);
    I_plotColor(img,p->points->selected->point.x+1,p->points->selected->point.y+1,red);
    I_plotColor(img,p->points->selected->point.x-1,p->points->selected->point.y-1,red);
    I_plotColor(img,p->points->selected->point.x-1,p->points->selected->point.y+1,red);
    I_plotColor(img,p->points->selected->point.x+1,p->points->selected->point.y-1,red);
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
    Liste listeIntersections;
    int ymin = getYmin(p);
    int ymax = getYmax(p);
    printf("ymin : %d, ymax : %d\n",ymin,ymax);
    for(int y = ymin; y<=ymax; y++){
        printf("avant getIntersections\n");
        listeIntersections = getIntersections(p,y);
        printf("apres getIntersections\n");
        printf("avant fillLine\n");
        fillLine(img,listeIntersections,y);
        printf("apres fillLine\n");
    }
}
void drawSelectedEdge(Image *img, Polygone p){
    Color red = C_new(255,0,0);
    if(p->points->taille>2){
        if(p->points->selected->suivant != NULL){
            ElementListe courant = p->points->selected;
            ElementListe follow = courant->suivant;
            I_bresenhamColor(img,courant->point.x,courant->point.y,follow->point.x,follow->point.y,red);
        }
        else{
            I_bresenhamColor(img,p->points->selected->point.x,p->points->selected->point.y,p->points->premier->point.x,p->points->premier->point.y,red);
        }
    }
}
void drawPolygoneClosed(Image *img, Polygone p){
    if(p->isFilled)
        drawPolygoneFilled(img,p);
    else
        drawPolygoneBordered(img,p);
}
void drawPolygone(Image *img, Polygone p){
    I_fill(img,C_new(0,0,0));
    if(p->isClosed)
        drawPolygoneClosed(img,p);
    else
        drawBrokenLine(img,p);
    if(getMode(p) == VERTEX)
        drawSelectedPoint(img,p);
    else if(getMode(p) == EDGE)
        drawSelectedEdge(img,p);
}

void selectNextPoint(Polygone p){
    if(p->points->selected->suivant != NULL){
        p->points->selected = p->points->selected->suivant;
        if(!p->isClosed && p->points->selected->suivant == NULL && getMode(p) == EDGE)
            p->points->selected = p->points->premier;
    }
    else{
        p->points->selected = p->points->premier;
    }
}

void selectPreviousPoint(Polygone p){
    if(p->points->selected->precedent != NULL)
        p->points->selected = p->points->selected->precedent;
    else{
        if(!p->isClosed && getMode(p) == EDGE)
            p->points->selected = p->points->dernier->precedent;
        else
            p->points->selected = p->points->dernier;
    }
}

void removeSelectedPoint(Polygone p){
    ElementListe courant = p->points->selected;
    selectNextPoint(p);
    if(courant->precedent != NULL){
        courant->precedent->suivant = courant->suivant;
    }
    else{
        p->points->premier = courant->suivant;
    }
    if(courant->suivant != NULL){
        courant->suivant->precedent = courant->precedent;
    }
    else{
        p->points->dernier = courant->precedent;
    }
    free(courant);
    p->points->taille--;
}

void deplacerSelectedPoint(Polygone p, int dx, int dy){
    p->points->selected->point.x += dx;
    p->points->selected->point.y += dy;
}
Point getPointBetweenTwoPoints(Point a, Point b){
    Point p;
    p.x = (a.x+b.x)/2;
    p.y = (a.y+b.y)/2;
    return p;
}
void createPointBetweenTwoPoints(Polygone p){
    ElementListe courant = p->points->selected;
    ElementListe follow = courant->suivant;
    ElementListe newPoint = malloc(sizeof(ElementListe));
    newPoint->point = getPointBetweenTwoPoints(courant->point,follow->point);
    newPoint->suivant = follow;
    newPoint->precedent = courant;
    newPoint->index = p->points->taille;
    courant->suivant = newPoint;
    follow->precedent = newPoint;
    p->points->taille++;
}

void selectLastPoint(Polygone p){
    p->points->selected = p->points->dernier;
}

void setMode(Polygone p, enum mode mode){
    p->mode = mode;
}

enum mode getMode(Polygone p){
    return p->mode;
}