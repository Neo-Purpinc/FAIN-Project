#include "Liste.h"

Point createPoint(int x, int y){
    Point p;
    p.x = x;
    p.y = y;
    return p;
}
bool equalPoints(Point p1, Point p2){
    // Deux point sont égaux si les coordonnées x et y sont égales
    return p1.x == p2.x && p1.y == p2.y;
}

Liste createListe(){
    // Création d'une liste vide
    Liste l = (Liste)malloc(sizeof(StructListe));
    // Si l'allocation a échoué
    if(l == NULL){
        printf("Erreur d'allocation de la liste\n");
        exit(EXIT_FAILURE);
    }
    // Initialisation de la liste
    l->premier = NULL;
    l->dernier = NULL;
    l->selected = NULL;
    l->taille = 0;
    return l;
}
void freeListe(Liste l){
    // Libération de la liste
    // On parcourt la liste et on libère chaque point
    ElementListe courant = l->premier;
    // Tant qu'il y a des éléments à libérer
    while(courant != NULL){
        // On libère le point courant
        ElementListe tmp = courant;
        // On passe au point suivant
        courant = courant->suivant;
        free(tmp);
    }
    free(l);
}
void addPoint(Liste l, Point p){
    // Ajout d'un point à la liste
    // Création d'un nouvel élément
    ElementListe nouveau = malloc(sizeof(StructElementListe));
    // Si l'allocation a échoué

    if (l == NULL || nouveau == NULL)
    {
        printf("Erreur d'allocation du nouveau point\n");
        exit(EXIT_FAILURE);
    }
    // Initialisation du nouvel élément
    nouveau->point = p;
    nouveau->index = l->taille;
    nouveau->suivant = NULL;
    nouveau->precedent = l->dernier; // peut être null
    // Si la liste est vide
    if(l->premier == NULL){
        // On ajoute le nouvel élément en tant que premier et dernier élément
        l->premier = nouveau; 
        l->dernier = nouveau;
    }
    else{
        // Sinon on ajoute le nouvel élément en tant que dernier élément
        // et on relie le précédent dernier élément au nouvel élément
        l->dernier->suivant = nouveau;
        l->dernier = nouveau;
    }
    // On incrémente la taille de la liste
    l->taille++;
    // On sélectionne le nouvel élément
    l->selected = nouveau;
}
void insertByX(Liste l, Point p){
    // Insertion d'un point dans la liste par ordre croissant des coordonnées x
    ElementListe courant = l->premier;
    // Tant qu'il y a des éléments à parcourir et que la coordonnée x du point à insérer est supérieure à celle du point courant
    while(courant != NULL && courant->point.x < p.x){
        // On passe au point suivant
        courant = courant->suivant;
    }
    // Si la liste est vide ou si la coordonnée x du point à insérer est inférieure à celle du premier point
    if(courant == NULL){
        // On ajoute le point en tant que premier élément
        addPoint(l, p);
    }
    else{
        // Sinon on crée un nouvel élément
        ElementListe nouveau = malloc(sizeof(StructElementListe));
        // Si l'allocation a échoué
        if (nouveau == NULL){
            printf("Erreur d'allocation du nouveau point\n");
            exit(EXIT_FAILURE);
        }
        // Initialisation du nouvel élément
        nouveau->point = p;
        nouveau->index = l->taille;
        nouveau->suivant = courant;
        nouveau->precedent = courant->precedent;
        // Si le point à insérer est le premier de la liste
        if(courant->precedent == NULL){
            // On ajoute le nouvel élément en tant que premier élément
            l->premier = nouveau;
        }
        else{
            // Sinon on relie le précédent élément au nouvel élément
            courant->precedent->suivant = nouveau;
        }
        // On relie le nouvel élément au suivant
        courant->precedent = nouveau;
        // On incrémente la taille de la liste
        l->taille++;
        // On sélectionne le nouvel élément
        l->selected = nouveau;
    }
}
void printListe(Liste l){
    // Affichage de la liste
    // On parcourt la liste et on affiche chaque point
    ElementListe courant = l->premier;
    // Tant qu'il y a des éléments à afficher
    while(courant != NULL){
        // On affiche le point courant
        printf("(%d, %d) ", courant->point.x, courant->point.y);
        // On passe au point suivant
        courant = courant->suivant;
    }
    printf("\n");
}

Point getPointBetweenTwoPoints(Point a, Point b)
{
    // Création d'un point au milieu de deux autres points
    Point p;
    // On calcule la moyenne des coordonnées x et y
    p.x = (a.x + b.x) / 2;
    p.y = (a.y + b.y) / 2;
    return p;
}