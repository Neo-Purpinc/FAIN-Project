#ifndef _LISTE_H_
#define _LISTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// STRUCTURES
// - Point
typedef struct {
    int x;
    int y;
} Point;

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
    ElementListe selected;
    int taille;
} StructListe, *Liste;

// FONCTIONS
// - Point
Point createPoint(int x,int y);
bool equalPoints(Point p1, Point p2);

// - Liste
Liste createListe();
void freeListe(Liste liste);
void addPoint(Liste liste,Point p);
void sortListe(Liste l);
void printListe(Liste liste);

#endif