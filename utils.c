#include "utils.h"

Point new_Point(int x, int y){
    Point p;
    p.x = x;
    p.y = y;
    return p;
}
Pile new_Pile()
{
    Pile p = (Pile)malloc(sizeof(StructPile));
    p->premier = NULL;
    return p;
}

bool is_empty(Pile pile){
    return pile->premier == NULL;
}
void empiler(Pile pile, Point p)
{
    ElementPile nouveau = malloc(sizeof(StructElementPile));
    if (pile == NULL || nouveau == NULL)
        exit(EXIT_FAILURE);
    nouveau->point = p;
    nouveau->suivant = pile->premier;
    pile->premier = nouveau;
}

Point depiler(Pile pile)
{
    if (pile == NULL || pile->premier == NULL)
        exit(EXIT_FAILURE);
    ElementPile first = pile->premier;
    Point p = first->point;
    pile->premier = pile->premier->suivant;
    free(first);
    return p;
}

Liste new_Liste(){
    Liste l = (Liste)malloc(sizeof(StructListe));
    l->premier = NULL;
    l->dernier = NULL;
    l->selected = NULL;
    l->taille = 0;
    return l;
}

bool is_empty_liste(Liste l){
    return l->taille==0;
}

void ajouterPointFin(Liste l, Point p){
    ElementListe nouveau = malloc(sizeof(StructElementListe));
    if (l == NULL || nouveau == NULL)
        exit(EXIT_FAILURE);
    nouveau->point = p;
    nouveau->index = size(l);
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

void ajouterPointDebut(Liste l, Point p){
    ElementListe nouveau = malloc(sizeof(StructElementListe));
    if (l == NULL || nouveau == NULL)
        exit(EXIT_FAILURE);
    nouveau->point = p;
    nouveau->index = size(l);
    nouveau->suivant = l->premier;
    nouveau->precedent = NULL;
    if(l->premier == NULL){
        l->premier = nouveau;
        l->dernier = nouveau;
    }
    else{
        l->premier->precedent = nouveau;
        l->premier = nouveau;
    }
    l->taille++;
    l->selected = nouveau;
}

void afficherListe(Liste l){
    ElementListe courant = l->premier;
    while(courant != NULL){
        printf("(%d, %d) ", courant->point.x, courant->point.y);
        courant = courant->suivant;
    }
    printf("\n");
}
int size(Liste l){
    return l->taille;
}

bool equalPoints(Point p1, Point p2){
    return p1.x == p2.x && p1.y == p2.y;
}
// Create a function insertByX that insert a Point p in an Liste l by its position x ascending
void insertByX(Liste l, Point p){
    ElementListe courant = l->premier;
    while(courant != NULL && courant->point.x < p.x){
        courant = courant->suivant;
    }
    if(courant == NULL){
        ajouterPointFin(l, p);
    }
    else{
        ElementListe nouveau = malloc(sizeof(StructElementListe));
        if (l == NULL || nouveau == NULL)
            exit(EXIT_FAILURE);
        nouveau->point = p;
        nouveau->index = size(l);
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

Liste sortListeByX(Liste l){
    Liste l2 = new_Liste();
    ElementListe courant = l->premier;
    for(int i = 0; i < size(l); i++){
        insertByX(l2, courant->point);
        courant = courant->suivant;
    }
    return l2;
}