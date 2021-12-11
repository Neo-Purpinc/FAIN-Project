#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Image.h"

// STRUCTURES
// - Point
typedef struct {
    int x;
    int y;
} Point;

// - Pile
typedef struct StructElementPile StructElementPile;
struct StructElementPile
{
    Point point;
    StructElementPile *suivant;
};
typedef StructElementPile *ElementPile;

typedef struct 
{
    ElementPile premier;
} StructPile, *Pile;

// - Liste
typedef struct StructElementListe StructElementListe;
struct StructElementListe
{
    Point point;
    int index;
    StructElementListe *precedent;
    StructElementListe *suivant;
};
typedef StructElementListe *ElementListe;

typedef struct 
{
    ElementListe premier;
    ElementListe dernier;
} StructListe, *Liste;

// FONCTIONS
// - Point
Point new_Point(int x,int y);

// - Pile
Pile new_Pile();
bool is_empty(Pile pile);
void empiler(Pile pile, Point p);
Point depiler(Pile pile);

// - Liste
Liste new_Liste();
bool is_empty_liste(Liste liste);
void ajouterPointDebut(Liste liste,Point p);
void ajouterPointFin(Liste liste,Point p);
void afficherListe(Liste liste);
void removePoint(Liste liste,Point p);
int size(Liste l);
#endif