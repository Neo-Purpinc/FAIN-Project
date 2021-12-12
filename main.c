
/*===============================================================*\

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "Polygone.h"
#include "Image.h"
#include "utils.h"

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
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN))
	{
		I_focusPoint(img,x,img->_height-y);
		switch(getMode(polygone)){
			case INSERT:
				addPoint(polygone,new_Point(x,img->_height-y));
				if(polygone->isFilled)
					toggleFilled(polygone);
			break;
			case VERTEX:
			break;
			case EDGE:
			break;
		}
		
	}
	else if(button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)
	{
		I_focusPoint(img,x,img->_height-y);
		switch(getMode(polygone)){
			case EDGE:
			createPointBetweenTwoPoints(polygone);
			break;
			case VERTEX:
			break;
			case INSERT:
			break;
		}
	}
	drawPolygone(img,polygone);
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27 : exit(1); break;
		case 'z' : I_zoom(img,2.0); break;
		case 'Z' : I_zoom(img,0.5); break;
		case 'i' : I_zoomInit(img); setMode(polygone,INSERT); break;
		case 'v' : 
			setMode(polygone,VERTEX);
		break;
		case 'e' :
			setMode(polygone,EDGE);
			selectPreviousPoint(polygone);
		break;
		case 'f' : 
			if(!isPolygoneEmpty(polygone)){
				toggleFilled(polygone);
			}
		case 'c' : 
			if(!isPolygoneEmpty(polygone))
			{
				toggleClosed(polygone);
				if(!polygone->isClosed && polygone->points->selected == polygone->points->dernier)
					selectNextPoint(polygone);
			}
		break;
		case 127:
			removeSelectedPoint(polygone);
		break;
		default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
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

	int d = 10;

	switch(key)
	{
	case GLUT_KEY_UP    : 
		// I_move(img,0,d);
		if(getMode(polygone)==VERTEX)
			deplacerSelectedPoint(polygone,0,d);
		break;
	case GLUT_KEY_DOWN  :
		// I_move(img,0,-d);
		if(getMode(polygone)==VERTEX)
			deplacerSelectedPoint(polygone,0,-d);
		break;
	case GLUT_KEY_LEFT  : 
		// I_move(img,d,0);
		if(getMode(polygone)==VERTEX)
			deplacerSelectedPoint(polygone,-d,0);
		break;
	case GLUT_KEY_RIGHT :
		// I_move(img,-d,0);
		if(getMode(polygone)==VERTEX)
			deplacerSelectedPoint(polygone,d,0);
		break;
	case 104:
		if(getMode(polygone)==VERTEX || getMode(polygone)==EDGE){
			Point p = polygone->points->selected->point;
			selectNextPoint(polygone);
			if(getMode(polygone)==VERTEX && polygone->points->selected == polygone->points->dernier->precedent && equalPoints(p,polygone->points->selected->point))
				selectLastPoint(polygone);
		}
	break;
	case 105:
		if(getMode(polygone)==VERTEX || getMode(polygone)==EDGE){
			Point p = polygone->points->selected->point;
			selectPreviousPoint(polygone);
			if(getMode(polygone)==VERTEX && polygone->points->selected == polygone->points->dernier->precedent && equalPoints(p,polygone->points->selected->point))
				selectLastPoint(polygone);
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
			original = I_read(argv[1]);
			largeur = original->_width;
			hauteur = original->_height;
			img = original;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			original = I_new(largeur,hauteur);
			img = original;
		}
		int windowPosX = 2000, windowPosY = 700;
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
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
