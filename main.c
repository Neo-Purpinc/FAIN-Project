
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
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN))
	{
		I_focusPoint(img,x,img->_height-y);
		switch(polygone->mode){
			case INSERT:
				addVertex(polygone,createPoint(x,img->_height-y));
			break;
			case VERTEX:
				closestVertex(x,img->_height-y,polygone);
			break;
			case EDGE:
				closestEdge(x,img->_height-y,polygone);
			break;
		}
		
	}
	else if(button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)
	{
		I_focusPoint(img,x,img->_height-y);
		switch(polygone->mode){
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

void mouse_move_CB(int x,int y){
	switch (polygone->mode)
	{
	case VERTEX: dragAndDrop(polygone,x,img->_height-y); break;
	case EDGE: 
		selectNextPoint(polygone);
		dragAndDrop(polygone,x,img->_height-y);
		selectPreviousPoint(polygone);
	break;
	case INSERT:
	break;
	}
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
		case 27 : exit(1); break;
		case 'z' : I_zoom(img,2.0); break;
		case 'Z' : I_zoom(img,0.5); break;
		case 'I' : I_zoomInit(img); break;
		case 'i' : polygone->mode = INSERT; break;
		case 'v' : 
			polygone->mode = VERTEX;
		break;
		case 'e' :
			polygone->mode = EDGE;
			if(polygone->points->selected == polygone->points->dernier && !polygone->isClosed)
				selectPreviousPoint(polygone);
		break;
		case 'f' : 
			if(!isPolygoneEmpty(polygone)){
				toggleFilled(polygone);
				if(!polygone->isClosed && polygone->isFilled)
					toggleClosed(polygone);
			}
		break;
		case 'c' :
			if(!isPolygoneEmpty(polygone))
			{
				toggleClosed(polygone);
				if(!polygone->isClosed && polygone->points->selected == polygone->points->dernier && polygone->mode == EDGE)
					selectPreviousPoint(polygone);
			}
		break;
		case 127:
			if(polygone->mode == VERTEX)
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

	int d = 5;

	switch(key)
	{
	case GLUT_KEY_UP    : 
		// I_move(img,0,d);
		if(polygone->mode==VERTEX && polygone->points->selected->point.y+d<img->_height-1)
			moveSelectedPoint(polygone,0,d);
		break;
	case GLUT_KEY_DOWN  :
		// I_move(img,0,-d);
		if(polygone->mode==VERTEX && polygone->points->selected->point.y-d>1)
			moveSelectedPoint(polygone,0,-d);
		break;
	case GLUT_KEY_LEFT  : 
		// I_move(img,d,0);
		if(polygone->mode==VERTEX && polygone->points->selected->point.x-d>1)
			moveSelectedPoint(polygone,-d,0);
		break;
	case GLUT_KEY_RIGHT :
		// I_move(img,-d,0);
		if(polygone->mode==VERTEX && polygone->points->selected->point.x+d<img->_width-1)
			moveSelectedPoint(polygone,d,0);
		break;
	case 104:
		if(polygone->mode==VERTEX || polygone->mode==EDGE){
			Point p = polygone->points->selected->point;
			selectNextPoint(polygone);
			if(polygone->mode==VERTEX && polygone->points->selected == polygone->points->dernier->precedent && equalPoints(p,polygone->points->selected->point))
				selectLastPoint(polygone);
		}
	break;
	case 105:
		if(polygone->mode==VERTEX || polygone->mode==EDGE){
			Point p = polygone->points->selected->point;
			selectPreviousPoint(polygone);
			if(polygone->mode==VERTEX && polygone->points->selected == polygone->points->dernier->precedent && equalPoints(p,polygone->points->selected->point))
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