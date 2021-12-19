# Fondements et Algorithmes de l’Imagerie Numérique

# Projet : Saisie et remplissage d’un polygone 2D

L’objectif de ce projet est d’écrire un programme en C ou C++ permettant de dessiner un polygone plan et de le remplir.

## 1) Afficher une ligne brisée (2p)

Écrire un programme qui, à partir d’un ensemble de coordonnées de points dans le plan (spécifiées directement dans le code source),
affiche une ligne brisée qui relie ces points par des droites de Bresenham dans l’ordre de leur spécification.

## 2) Dessin à la souris (2p)

Écrire un programme où chaque clique de la souris ajoute un nouveau sommet en ce point et, le cas échéant, le relie au sommet précédent.
Lorsqu’on presse la touche ’c’ (comme dans close), la ligne brisée ouverte devient un polygone fermé.
Lorsqu’on presse de nouveau la touche ’c’, le polygone fermé redevient une ligne brisée ouverte.

![FIGURE 1](http://url/to/img.png) FIGURE 1 – À gauche, un polygone fermé et, à droite, le même polygone rempli par scanline.

## 3) Remplissage scan-line (6p)

Écrire une nouvelle version du programme précédent qui grâce à la touche ’f’ (comme dans fill) permet de remplir le polygone fermé avec la méthode scanline.

## 4) Insérer et supprimer des sommets (3p)

Trouver une structure de données adéquate permettant de représenter les lignes brisées et les polygones fermés et qui permette également d’insérer des sommets et d’en supprimer. 

## 5) Sélection par clavier (2p)

Écrire une nouvelle version du programme précédent qui admette trois modes :

**insert (touche i)** 

Dans le mode append le programme fonctionne comme auparavant : chaque clique de la souris avec le bouton gauche crée un nouveau sommet et le relie au dernier point crée.

**vertex (touche v)** 

Dans le mode vertex, un sommet et un seul est supposé sélectionné. Le sommet sélectionné est visualisé par un signe distinctif (par exemple un petit carré autour du sommet).
— La touche page suivante (respectivement page précédente) permet de sélectionner le sommet suivant (respectivement précédant) le sommet sélectionné.
— Les touches haut, bas, droite, gauche permettent de déplacer le sommet sélectionné dans la direction correspondante.
— La touche suppr permet d’éliminer le sommet sélectionné.

**edge (touche e)** 

Dans le mode edge, une arête et une seule est supposée sélectionnée. Cette arête est visualisée par un signe distinctif. On peut imaginer qu’elle soit dessinée d’une couleur différente des autres arêtes.
— La touche page suivante (respectivement page précédente) permet de sélectionner l’arête suivant (respectivement précédant) l’arête sélectionnée.
— un clique avec le bouton du milieu permet de couper l’arête sélectionnée en deux en insérant un nouveau sommet entre ses extrémités.

## 6) Sélection par souris (2p)

**Sommet le plus proche**

Écrire une fonction *closestVertex* qui, étant donnée une position(x, y)et une ligne brisée ou un polygone fermé, retourne l’indice du sommet le plus proche de (x, y).

**Arête la plus proche**

Écrire une fonction *closestEdge* qui, étant donnée une position(x, y)et une ligne brisée ou un polygone fermé, retourne l’indice de l’arête la plus proche de (x, y).

Écrire une nouvelle version du programme précédent qui réalise les mêmes actions sauf qu’on n’utilise plus les
touches page précédente et page suivante pour changer les sélections.
— en mode vertex le fait de cliquer avec le bouton gauche revient à sélectionner le sommet le plus proche ;
— en mode edge le fait de cliquer avec le bouton gauche revient à sélectionner l’arête la plus proche.
Pour simplifier on admettra que l’arête la plus proche est adjacente au sommet le plus proche.
