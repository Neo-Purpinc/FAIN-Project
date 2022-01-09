
/*===============================================================*\

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include "Image.h"
#include "Polygone.h"
Image *img, *original;
Polygone polygone;


//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);
	I_draw(img);
	glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	// Si on clic avec le bouton gauche de la souris
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN))
	{
		switch(polygone->mode){
			case INSERT:
				// En mode INSERT, un clic ajoute un point
				addVertex(polygone,createPoint(x,img->_height-y));
			break;
			case VERTEX:
				// En mode VERTEX, un clic sélectionne le point le plus proche
				closestVertex(x,img->_height-y,polygone);
			break;
			case EDGE:
				// En mode EDGE, un clic sélectionne l'arête la plus proche
				closestEdge(x,img->_height-y,polygone);
			break;
		}
		
	}
	// Sinon si on appuie avec la molette et qu'on est en mode EDGE
	// On crée un point au milieu des deux extrémités de l'arête
	else if(button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN && polygone->mode==EDGE)
			createPointBetweenTwoPoints(polygone);
	// On redessine toujours le polygone après l'avoir mis à jour
	drawPolygone(img,polygone);
	glutPostRedisplay();
}

// ----------------------------------------------------------------
// Cette fonction permet de réagir pour le drag and drop
// ----------------------------------------------------------------
void mouse_move_CB(int x,int y){
	// Si on est en mode VERTEX et qu'un déplacement est possible
	// On modifie la position du point
	if(polygone->mode==VERTEX && x>=0 && x<img->_width && y>=0 && y<img->_height)
		 	dragAndDrop(polygone,x,img->_height-y);
	// On redessine toujours le polygone après l'avoir mis à jour
	drawPolygone(img,polygone);
}
//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27 : 
			// En appuyant sur Echap, on quitte le programme 
			exit(1);
		break;
		case 'i' : 
			// En appuyant sur i, on passe en mode INSERT
			polygone->mode = INSERT; 
		break;
		case 'v' : 
			// En appuyant sur v, on passe en mode VERTEX
			polygone->mode = VERTEX;
		break;
		case 'e' :
			// En appuyant sur e, on passe en mode EDGE
			polygone->mode = EDGE;
			// Si on passe en mode EDGE lorsque le polygone est une ligne brisée et que le point sélectionné était le dernier point,
			if(polygone->points->selected == polygone->points->dernier && !polygone->isClosed){
				// On sélectionne l'avant dernier point pour dessiner l'EDGE entre l'avant dernier et le dernier point (celui d'après étant caché car ligne brisée)
				selectPreviousPoint(polygone);
			}
		break;
		case 'f' : 
			// En appuyant sur f, on passe en mode FILL
			if(!isPolygoneEmpty(polygone)){
				toggleFilled(polygone);
				if(!polygone->isClosed && polygone->isFilled)
					toggleClosed(polygone);
			}
		break;
		case 'c' :
			// En appuyant sur c, on passe en mode CLOSE
			if(!isPolygoneEmpty(polygone))
			{
				toggleClosed(polygone);
				if(!polygone->isClosed && polygone->points->selected == polygone->points->dernier && polygone->mode == EDGE)
					selectPreviousPoint(polygone);
			}
		break;
		case 127:
			// En appuyant sur la touche suppr, on supprime le point sélectionné
			// Si le polygone est vide, on ne fait rien
			if(polygone->mode == VERTEX && polygone->points->taille > 0){
				removeSelectedPoint(polygone);
				// Si on a supprimée le dernier point, on passe en mode INSERT
				if(polygone->points->taille == 0)
					polygone->mode = INSERT;
			}
		break;
		default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
	// On On redessine toujours le polygone après l'avoir mis à jour
	drawPolygone(img,polygone);
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait
// que l'utilisateur presse une touche spéciale
// (F1, F2 ... F12, home, end, insert, haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 5;

	switch(key)
	{
	// On appuie sur la flèche du haut
	case GLUT_KEY_UP    : 
		// Si on est en mode VERTEX et qu'un déplacement est possible
		if(polygone->mode==VERTEX && polygone->points->selected->point.y+d<img->_height){
			// On déplace le point sélectionné
			moveSelectedPoint(polygone,0,d);
		}
		break;
	// On appuie sur la flèche du bas
	case GLUT_KEY_DOWN  :
		// Si on est en mode VERTEX et qu'un déplacement est possible
		if(polygone->mode==VERTEX && polygone->points->selected->point.y-d>=0){
			// On déplace le point sélectionné
			moveSelectedPoint(polygone,0,-d);
		}
		break;
	// On appuie sur la flèche de gauche
	case GLUT_KEY_LEFT  : 
		// Si on est en mode VERTEX et qu'un déplacement est possible
		if(polygone->mode==VERTEX && polygone->points->selected->point.x-d>=0){
			// On déplace le point sélectionné
			moveSelectedPoint(polygone,-d,0);
		}
		break;
	// On appuie sur la flèche de droite
	case GLUT_KEY_RIGHT :
		// Si on est en mode VERTEX et qu'un déplacement est possible
		if(polygone->mode==VERTEX && polygone->points->selected->point.x+d<img->_width){
			// On déplace le point sélectionné
			moveSelectedPoint(polygone,d,0);
		}
		break;
	// On appuie sur la touche PageUp
	case 104:
		// Si on est en mode VERTEX ou EDGE
		if(polygone->mode==VERTEX || polygone->mode==EDGE){
			// On récupère le point sélectionné
			Point p = polygone->points->selected->point;
			// On sélectionne le suivant
			selectNextPoint(polygone);
			if(polygone->mode==VERTEX && polygone->points->selected == polygone->points->dernier->precedent && equalPoints(p,polygone->points->selected->point)){
				// Nécessaire pour les cas où le polygone est fermé et qu'on l'ouvre avec un point sélectionné qui pose problème (Edge etc.)
				selectLastPoint(polygone);
			}
		}
	break;
	// On appuie sur la touche PageDown
	case 105:
		// Si on est en mode VERTEX ou EDGE
		if(polygone->mode==VERTEX || polygone->mode==EDGE){
			// On récupère le point sélectionné
			Point p = polygone->points->selected->point;
			// On sélectionne le précédent
			selectPreviousPoint(polygone);
			if(polygone->mode==VERTEX && polygone->points->selected == polygone->points->dernier->precedent && equalPoints(p,polygone->points->selected->point)){
				selectLastPoint(polygone);
			}
		}
	break;
	default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	drawPolygone(img,polygone); 
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if((argc!=3)&&(argc!=2))
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\nou",argv[0]);
		fprintf(stderr,"\t: %s <ppmfilename> \n\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if(argc==2)
		{
			// On récupère l'image passé en paramètre
			original = I_read(argv[1]);
			largeur = original->_width;
			hauteur = original->_height;
			img = original;
		}
		else
		{
			// Sinon on crée une image vide
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			original = I_new(largeur,hauteur);
			img = original;
		}
		int windowPosX = 1050, windowPosY = 200;
		polygone = createPolygone();
		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,0,hauteur,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();
		freePolygone(polygone);
		return 0;
	}
}