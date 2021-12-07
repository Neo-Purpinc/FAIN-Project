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

int is_empty(Pile pile){
    int bool = 0;
    if(pile->premier == NULL){
        bool = 1;
    }
    return bool;
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
    return l;
}

int is_empty_liste(Liste l){
    int bool = 0;
    if(l->premier == NULL){
        bool = 1;
    }
    return bool;
}

void ajouterPointFin(Liste l, Point p){
    ElementListe nouveau = malloc(sizeof(StructElementListe));
    if (l == NULL || nouveau == NULL)
        exit(EXIT_FAILURE);
    nouveau->point = p;
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
}

void ajouterPointDebut(Liste l, Point p){
    ElementListe nouveau = malloc(sizeof(StructElementListe));
    if (l == NULL || nouveau == NULL)
        exit(EXIT_FAILURE);
    nouveau->point = p;
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
    int i = 0;
    ElementListe courant = l->premier;
    while(courant != NULL){
        i++;
        courant = courant->suivant;
    }
    return i;
}