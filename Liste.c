#include "Liste.h"

Point createPoint(int x, int y){
    Point p;
    p.x = x;
    p.y = y;
    return p;
}
bool equalPoints(Point p1, Point p2){
    return p1.x == p2.x && p1.y == p2.y;
}

Liste createListe(){
    Liste l = (Liste)malloc(sizeof(StructListe));
    l->premier = NULL;
    l->dernier = NULL;
    l->selected = NULL;
    l->taille = 0;
    return l;
}
void freeListe(Liste l){
    ElementListe courant = l->premier;
    while(courant != NULL){
        ElementListe tmp = courant;
        courant = courant->suivant;
        free(tmp);
    }
    free(l);
}
void addPoint(Liste l, Point p){
    ElementListe nouveau = malloc(sizeof(StructElementListe));
    if (l == NULL || nouveau == NULL)
        exit(EXIT_FAILURE);
    nouveau->point = p;
    nouveau->index = l->taille;
    nouveau->suivant = NULL;
    nouveau->precedent = l->dernier; // peut être null
    if(l->premier == NULL){
        l->premier = nouveau;
        l->dernier = nouveau;
    }
    else{
        l->dernier->suivant = nouveau;
        l->dernier = nouveau;
    }
    l->taille++;
    l->selected = nouveau;
}
void insertByX(Liste l, Point p){
    ElementListe courant = l->premier;
    while(courant != NULL && courant->point.x < p.x){
        courant = courant->suivant;
    }
    if(courant == NULL){
        addPoint(l, p);
    }
    else{
        ElementListe nouveau = malloc(sizeof(StructElementListe));
        if (l == NULL || nouveau == NULL)
            exit(EXIT_FAILURE);
        nouveau->point = p;
        nouveau->index = l->taille;
        nouveau->suivant = courant;
        nouveau->precedent = courant->precedent;
        if(courant->precedent == NULL){
            l->premier = nouveau;
        }
        else{
            courant->precedent->suivant = nouveau;
        }
        courant->precedent = nouveau;
        l->taille++;
        l->selected = nouveau;
    }
}
void sortListe(Liste l){
    Liste l2 = createListe();
    ElementListe courant = l->premier;
    for(int i = 0; i < l->taille; i++){
        insertByX(l2, courant->point);
        courant = courant->suivant;
    }
    l = l2;
}
void printListe(Liste l){
    ElementListe courant = l->premier;
    while(courant != NULL){
        printf("(%d, %d) ", courant->point.x, courant->point.y);
        courant = courant->suivant;
    }
    printf("\n");
}