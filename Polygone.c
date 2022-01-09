#include "Polygone.h"

// POLYGONE
Polygone createPolygone()
{
    // Création d'un polygone vide
    // On alloue la mémoire pour le polygone
    Polygone p = malloc(sizeof(PolygoneStruct));
    // Si l'allocation a échoué
    if (p == NULL)
    {
        printf("Erreur d'allocation du polygone\n");
        exit(EXIT_FAILURE);
    }
    // Initialisation du polygone
    p->isClosed = false;
    p->isFilled = false;
    p->points = createListe();
    p->mode = INSERT;
    return p;
}
bool isPolygoneEmpty(Polygone p)
{
    // Retourne vrai si le polygone est vide
    return p->points->taille == 0;
}
void toggleClosed(Polygone p)
{
    // Inverse l'état de fermeture du polygone
    p->isClosed = !p->isClosed;
}
void toggleFilled(Polygone p)
{
    // Inverse l'état de remplissage du polygone
    p->isFilled = !p->isFilled;
}
void addVertex(Polygone p, Point point)
{ 
    // Ajoute un point à la liste des points du polygone
    addPoint(p->points, point);
}
void freePolygone(Polygone p)
{ 
    // Libère la liste des points du polygone
    freeListe(p->points);
    // Libère le polygone
    free(p);
}

// AUXILIAIRES
bool isBetween(int y1, int y, int y2){
    // Retourne vrai si y est entre y1 et y2
    return (y1 < y && y <= y2) || (y1 >= y && y > y2);
}
int intersectionsAux(int y, Point a, Point b)
{
    // Retourne la coordonnée x de l'intersection entre la ligne y et le segment [a,b]
    int res;
    if (!isBetween(a.y,y,b.y) || (a.y == b.y)){
        // Si y n'est pas entre les coordonnées y de a et b ou si a et b sont sur la même ligne
        res = -1;
    }
    else if (a.x == b.x){
        // Sinon si a et b sont sur la même colonne
        res = a.x;
    }
    else{
        // Sinon
        res=((a.x*b.y - a.y*b.x)*(-1) - (a.x-b.x)*(-y))/(-(a.y-b.y)*(-1));
    }
    return res;
}
Liste getIntersections(Polygone p, int y)
{
    // Initialisation de la liste des intersections
    Liste listeIntersections = createListe();
    // On parcourt la liste des points du polygone
    ElementListe courant = p->points->premier;
    Point tmp;
    tmp.y = y;
    // Tant qu'il y a des points à parcourir
    while (courant->suivant != NULL)
    {
        // On récupère les coordonnées x de l'intersection entre la ligne y et le segment [courant,courant->suivant]
        tmp.x = intersectionsAux(y, courant->point, courant->suivant->point);
        // Si l'intersection est valide
        // on l'ajoute à la liste des intersections dans l'ordre croissant des coordonnées x
        if(tmp.x != -1) insertByX(listeIntersections, tmp);
        // On passe au point suivant
        courant = courant->suivant;
    }
    // On fait ca entre le dernier et le premier point également
    tmp.x = intersectionsAux(y, courant->point, p->points->premier->point);
    if(tmp.x != -1) insertByX(listeIntersections, tmp);
    // Puis on retourne la liste des intersections
    return listeIntersections;
}
float distance(int x1, int y1, int x2, int y2) { 
    // Retourne la distance entre deux points
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
int minimum(int a, int b) {
    // Retourne le minimum entre deux nombres
    return a < b ? a : b;
}
int getYmin(Polygone p)
{
    // Retourne la coordonnée y minimale du polygone
    // Initialisation au premier point
    ElementListe courant = p->points->premier;
    int ymin = courant->point.y;
    // Parcours de la liste
    while (courant != NULL)
    {
        // Si la coordonnée y du point courant est plus petite que ymin
        if (courant->point.y < ymin){
            // On met à jour ymin
            ymin = courant->point.y;
        }
        // On passe au point suivant
        courant = courant->suivant;
    }
    return ymin;
}
int getYmax(Polygone p)
{
    // Retourne la coordonnée y maximale du polygone
    // Initialisation au premier point
    ElementListe courant = p->points->premier;
    int ymax = courant->point.y;
    // Parcours de la liste
    while (courant != NULL)
    {
        // Si la coordonnée y du point courant est plus grande que ymax
        if (courant->point.y > ymax){
            // On met à jour ymax
            ymax = courant->point.y;
        }
        // On passe au point suivant
        courant = courant->suivant;
    }
    return ymax;
}
void closestVertex(int x, int y, Polygone p)
{
    // Trouve le point le plus proche du point (x,y)
    // Initialisation au premier point
    ElementListe courant = p->points->premier, index = courant;
    float min = distance(x, y, courant->point.x, courant->point.y);
    // Parcours de la liste
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        float dist = distance(x, y, follow->point.x, follow->point.y);
        // Si la distance entre le point de coordonnées (x,y) et le point suivant est plus petite que la distance minimale actuelle
        if (dist < min)
        {
            // On met à jour la distance minimale
            min = dist;
            // On met à jour l'index
            index = follow;
        }
        // On passe au point suivant
        courant = courant->suivant;
    }
    // On sélectionne le point dans la liste à la place index, c'est le plus proche du point (x,y)
    p->points->selected = index;
}
void closestEdge(int x, int y, Polygone p)
{  
    // Trouve la ligne la plus proche du point (x,y)
    // On cherche le point le plus proche du point (x,y) et on suppose
    // que l'arête la plus proche est celle sortante du point le plus proche
    closestVertex(x, y, p);
    // On gère le cas où c'est le dernier et qu'on est en mode EDGE non fermé
    if (p->points->selected == p->points->dernier && p->mode == EDGE && !p->isClosed)
    {
        p->points->selected = p->points->selected->precedent;
    }
}

// DESSIN
void drawBrokenLine(Image *img, Polygone p)
{
    // Dessine une ligne brisée bleu
    Color c = C_new(0, 0, 255);
    // On parcours la liste des points du polygone et on les relie entre eux
    ElementListe courant = p->points->premier;
    while (courant->suivant != NULL)
    {
        ElementListe follow = courant->suivant;
        I_bresenham(img, courant->point.x, courant->point.y, follow->point.x, follow->point.y,c);
        courant = courant->suivant;
    }
}
void drawSelectedPoint(Image *img, Polygone p)
{
    // Dessine le point sélectionné et son voisinage en rouge
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
    // Comme pour la ligne brisée mais on relie le dernier et le premier point aussi
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
    // Dessine un polygone rempli en blanc en utilisant l'algorithme de scanline
    // On prépare une liste qui contiendra les intersections
    Liste listeIntersections;
    Color c = C_new(255, 255, 255);
    // On récupère les valeurs minimales et maximales en y du polygone
    int ymin = getYmin(p);
    int ymax = getYmax(p);
    // On parcours les lignes de l'image 
    for (int y = ymin; y <= ymax; y++)
    {
        // on récupère la liste trié en X des intersections pour la ligne y
        listeIntersections = getIntersections(p, y);
        // On parcourt la liste des intersections
        ElementListe courant = listeIntersections->premier;
        // Tant qu'il y a des intersections
        while(courant!=NULL){
            // On relie l'intersection courante à la suivante
            Point a = courant->point;
            Point b = b = courant->suivant->point;
            if(courant->suivant==NULL)
                printf("ERROR\n");
            I_bresenham(img, a.x, a.y, b.x, b.y, c);
            // Une fois sur deux
            courant = courant->suivant->suivant;
        }
    }
}
void drawSelectedEdge(Image *img, Polygone p)
{
    // Dessine l'arête sélectionnée en rouge si il y a au moins un point avec gestion des cas limites
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
    // Dessine un polygone fermé
    // Si il y a plus de 2 points et que l'attribut Rempli est à true, on dessine le polygone rempli
    // Sinon on dessine le polygone fermé vide
    if (p->isFilled && p->points->taille > 2)
        drawPolygoneFilled(img, p);
    else
        drawPolygoneBordered(img, p);
}
void drawPolygone(Image *img, Polygone p)
{
    // Dessine le polygone en fonction de son type
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

// SELECTIONS
void selectNextPoint(Polygone p)
{
    // Sélectionne le point suivant l'actuel sélectionné
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
    // Sélectionne le point précédent l'actuel sélectionné
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
void selectLastPoint(Polygone p)
{
    // Sélectionne le dernier point
    p->points->selected = p->points->dernier;
}

// ACTIONS SUR LES POINTS
void removeSelectedPoint(Polygone p)
{
    // Supprime le point sélectionné
    ElementListe courant = p->points->selected;
    selectNextPoint(p);
    // Si le point sélectionné n'est pas le premier
    if (courant->precedent != NULL)
    {
        // On relie le point précédent au point suivant
        courant->precedent->suivant = courant->suivant;
    }
    else
    {
        // Sinon le point sélectionné était le premier et celui qui le suit devient le nouveau premier après sa suppression
        p->points->premier = courant->suivant;
    }
    // Si le point sélectionné n'est pas le dernier
    if (courant->suivant != NULL)
    {
        // On relie le point suivant au point précédent
        courant->suivant->precedent = courant->precedent;
    }
    else
    {
        // Sinon le point sélectionné était le dernier et celui qui le précédait devient le nouveau dernier après sa suppression
        p->points->dernier = courant->precedent;
    }
    // On supprime le point
    free(courant);
    // On diminue la taille de la liste
    p->points->taille--;
}
void moveSelectedPoint(Polygone p, int dx, int dy)
{
    // Déplace le point sélectionné de dx et dy
    p->points->selected->point.x += dx;
    p->points->selected->point.y += dy;
}
void createPointBetweenTwoPoints(Polygone p)
{
    // Crée un point entre le point sélectionné et le point suivant
    ElementListe courant = p->points->selected;
    ElementListe follow = courant->suivant;
    // Si le point sélectionné est le dernier
    if (follow == NULL)
    {
        // On crée un point entre le dernier et le premier
        addVertex(p, getPointBetweenTwoPoints(courant->point, p->points->premier->point));
    }
    else
    {
        // Sinon on crée un point entre le point sélectionné et le point suivant
        ElementListe newPoint = (ElementListe)malloc(sizeof(StructElementListe));
        newPoint->point = getPointBetweenTwoPoints(courant->point, follow->point);
         p->points->taille++;
        newPoint->index = p->points->taille;
        // On mets à jour les liens entre les points
        newPoint->suivant = follow;
        newPoint->precedent = courant;
        courant->suivant = newPoint;
        follow->precedent = newPoint;
        p->points->selected = newPoint;
    }
}
void dragAndDrop(Polygone p, int x, int y){
    // Déplace le point sélectionné en x et y
    p->points->selected->point.x = x;
    p->points->selected->point.y = y;
}