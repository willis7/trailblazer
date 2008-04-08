#include "Ship.h"
#include <string.h>
#include <GL\glut.h>
#include <iostream>
using namespace std;

// Picking Stuff 
#define RENDER					1
#define SELECT					2
#define BUFFSIZE				64

GLuint selectBuff[BUFFSIZE];
GLint hits;
int mode = RENDER;
int cursorX,cursorY;

#define MAX_SHIPS				10
//point reduction for missing
#define DIFFICULTY				30	

//Window Properties
static int h=800,w=800;

Ship *fleet[MAX_SHIPS];
int score =0;
int miss = 50;

//Functions Prototypes
void drawScene(void);
void handleResize(int width,int height);
void init(void);
void createList(void);
void startPicking(void);
void stopPicking(void);
void mouseStuff(int button, int state, int x, int y);
void processHits(GLint hits, GLuint buffer[]);
void update(int value);

//----------------------
// Init Enemies:
//----------------------
void initEnemies()
{
	int min = 0;
	int max = 600;

	for(int i=0;i<MAX_SHIPS;i++)
	{
		Ship *s = new Ship("blaster.obj","blaster.bmp");
		s->setPosition((rand()%(max-min)-300), (rand()%(max-min)-300), -1500);
		s->setSpeed(rand()%(40-0)+20);
		s->setPoints(100);
		fleet[i] = s;
	}
}
//----------------------
// Main
//----------------------
void main(int argc,char **argv)
{	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(w,h);	
	glutInitWindowPosition(100,100);			
	glutCreateWindow("Tailblazer");

	init();
	initEnemies();
	glutReshapeFunc(handleResize);
	glutDisplayFunc(drawScene);
	glutTimerFunc(100, update, 0);	
	glutMouseFunc(mouseStuff);

	glutMainLoop();

	for(int i=0;i<MAX_SHIPS;i++)
	{
		delete(fleet[i]);
	}
}
//----------------------
// Display Text string
//----------------------
void renderBitmapString(float x, float y,char *text)
{  
	int i=0;	
	glRasterPos2i(x,y);

	while(text[i]) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
		++i;
	}
}
//----------------------
// Set/Reset Ortho:
// this is done for the purpose of the fonts
//----------------------
void setOrthographicProjection() 
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glScalef(1, -1, 1);
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
//----------------------
// Scene
//----------------------
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	char str[10];
	itoa(score,str,10);

	//Disable lighting in order to show the text
	glDisable(GL_LIGHTING);
	glPushMatrix();	
		glColor3f(1.0f,1.0f,1.0f);
		setOrthographicProjection();
		glLoadIdentity();
		renderBitmapString(20,20,"Score");
		renderBitmapString(20,50,str);
		resetPerspectiveProjection();
		glutWireSphere(5,10,20);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);

	if (mode == SELECT)
	{
		startPicking();
	}
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	createList();

	if (mode == SELECT) 
		stopPicking();
	else
	{
		glutSwapBuffers();
	}
}
//----------------------
// Init
//----------------------
void init(void)
{
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 
	GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f }; 	

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); 
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHT1); 
	glEnable(GL_DEPTH_TEST); 
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}
//----------------------
// Update enemies
//----------------------
void update(int value)
{
	//if ships have gone off the screen and are not flagged as dead...reset
	for(int i=0;i<MAX_SHIPS;i++)
	{
		if(fleet[i]->getPosition('z') >= 0 ||fleet[i]->status() == true)
		{
			fleet[i]->Reset(-1500);
			fleet[i]->reducePoints(DIFFICULTY);
		}
	}

	for(int i=0;i<MAX_SHIPS;i++)
	{
		fleet[i]->move();
	}

	glutPostRedisplay();    
	glutTimerFunc(60, update, 0);
}
//----------------------
// Mouse
//----------------------
void mouseStuff(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	cursorX = x;
	cursorY = y;
	mode = SELECT;
}
//----------------------
// Resizing
//----------------------
void handleResize(int w1, int h1)
{
	float ratio;

	h = h1;
	w = w1;

	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
    glViewport(0, 0, w, h);
	gluPerspective(45,ratio,0.1,1500);
	glMatrixMode(GL_MODELVIEW);

}
//---------------
// Picking Stuff
//---------------
void startPicking()
{
	GLint viewport[4];
	float ratio;

	glSelectBuffer(BUFFSIZE,selectBuff);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(cursorX,viewport[3]-cursorY,5,5,viewport);
	ratio = (viewport[2]+0.0) / viewport[3];
	gluPerspective(45,ratio,0.1,1500);	
	glMatrixMode(GL_MODELVIEW);
}
void processHits(GLint hits, GLuint buffer[], int sw)
{
   GLint i, j, numberOfNames = 0;
   GLuint names, *ptr, minZ,*ptrNames;

   ptr = (GLuint *)buffer;
   minZ = 0xffffffff;

   for (i = 0; i < hits; i++)
   {	
      names = *ptr;
	  ptr++;
	  if (*ptr < minZ)
	  {
		  numberOfNames = names;
		  minZ = *ptr;
		  ptrNames = ptr+2;
	  }	  
	  ptr += names+2;
	}
   if (numberOfNames > 0) 
   {
	  cout <<"You picked item  "<< endl;
	  ptr = ptrNames;
	  for (j = 0; j < numberOfNames; j++,ptr++)
	  { 
		  cout << *ptr <<endl;
		  fleet[*ptr]->kill();
		  score = score+ fleet[*ptr]->getPoints();
		  cout << "Score " << score << endl;
	  }
	}
   else{
	 cout << "you didnt select anything!" << endl; 
	 score = score- miss;
	 cout << "Score " << score << endl;
   }
}

void stopPicking() 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	hits = glRenderMode(GL_RENDER);
	processHits(hits,selectBuff,0);

	mode = RENDER;

}
//---------------
// List of pickable objects
//---------------
void createList()
{
	for(int i= 0; i< MAX_SHIPS; i++)
	{
		if(fleet[i]->status() == false)
		{
			glPushMatrix();
			glTranslatef(fleet[i]->getPosition('x'), fleet[i]->getPosition('y'), fleet[i]->getPosition('z'));
			glLoadName(i);
			fleet[i]->draw();
			glPopMatrix();
		}
	}
}

