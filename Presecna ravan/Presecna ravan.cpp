#include <stdio.h>
#include "freeglut.h"

#define KOCKA			1
#define TORUS			2
#define OKTAEDAR		3
#define TETRAEDAR		4
#define SFERA			5

GLboolean prikazPresecneRavni = GL_TRUE;

// Mis
GLboolean	leftDown = GL_FALSE;
GLboolean	rightDown = GL_FALSE;
GLint		leftX, leftY, rightX, rightY;

// Pozicija i osvetljenje
GLfloat		xRotacijaTast = 0.0;
GLfloat		yRotacijaTast= 0.0;
GLfloat		localSystemx = -55.0;
GLfloat		localSystemy = 35.0;
GLfloat		xPolozajObjekta = -55.0;
GLfloat		yPolozajObjekta = 35.0;
GLfloat		PozicijaSvetla[] = {5.0, 4.0, 7.0, 1.0};

GLdouble	clipPlane[4] = {-1.0, -1.0, -1.0, 3};
GLdouble	interakcija = 0.0;

GLint Height = 500;
GLint Width = 600;

GLint prikazObjekta = TORUS;

static void MouseMove(int x, int y)
{
	if(leftDown)
	{
		// obrtanje objekta i lokalnog koordinatnog sistema pomocu levog tastera misa 

		xPolozajObjekta += (x - leftX) * 0.2;
		yPolozajObjekta += (y - leftY) * 0.2;

		localSystemx += (x - leftX) * 0.2;
		localSystemy += (y - leftY) * 0.2;

		leftX = x;
		leftY = y;
	}

	if(rightDown)
	{
		// pomeranje presecne ravni pomocu desnog tastera misa

		clipPlane[3] += (x - rightX) * 0.03;

		interakcija += (x - rightX) * 0.01;

		rightX = x;
	}
}

static void MouseDownUp(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			leftDown = GL_TRUE;
			leftX = x;
			leftY = y;
		}
		else if(state == GLUT_UP)
		{
			leftDown = GL_FALSE;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			rightDown = GL_TRUE;
			rightX = x;
		}
		else if(state == GLUT_UP)
		{
			rightDown = GL_FALSE;
		}
	}
}

static void SpecialKeys(int key, int x, int y)
{
    switch (key)
	{
		case GLUT_KEY_LEFT:
				yRotacijaTast -= 2.0;
			break;
		case GLUT_KEY_RIGHT:
				yRotacijaTast += 2.0;
			break;
		case GLUT_KEY_DOWN:
				xRotacijaTast += 2.0;
			break;
		case GLUT_KEY_UP:
				xRotacijaTast -= 2.0;
			break;
	}
}

static void key(unsigned char key, int x, int y)
{
    switch(key)
    {
	case '\040': // oktalni kod tastature za taster Space
		switch(prikazObjekta)
		{
			default:
			case SFERA:
				prikazObjekta = KOCKA;
				break;
			case KOCKA:
				prikazObjekta = TORUS;
				break;
			case TORUS:
				prikazObjekta = OKTAEDAR;
				break;
			case OKTAEDAR:
				prikazObjekta = TETRAEDAR;
				break;
			case TETRAEDAR:
				prikazObjekta = SFERA;
				break;
		}

		break;
	case 'p':
	case 'P':
		prikazPresecneRavni = !prikazPresecneRavni;
		break;
		
		case '\033': // oktalni kod tastature za taster Esc
		exit(0);
		break;
	}
}

void drawCurrentObject(void)
{
	switch(prikazObjekta)
	{
		default:
		case SFERA:
			glutSolidSphere(0.9, 25, 25);
			break;
		case KOCKA:
			glutSolidCube(1);
			break;
		case TORUS:
			glutSolidTorus(0.2, 0.9, 25, 50);
			break;
		case OKTAEDAR:
			glutSolidOctahedron();
			break;
		case TETRAEDAR:
			glutSolidTetrahedron();
			break;
	}
}

static void drawClipPlane(GLfloat koordinata, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(yRotacijaTast, 0.0, 1.0, 0.0);
	glRotatef(xRotacijaTast, 1.0, 0.0, 0.0);
	glTranslatef(interakcija, interakcija, interakcija);

	glBegin(GL_TRIANGLE_STRIP);

	    glColor4f(red, green, blue, alpha);
		glNormal3f(0.6, 0.6, 0.6); 
		glVertex3f(0.0, koordinata, 0.0);

	    glColor4f(red, green, blue, alpha);
		glNormal3f(0.6, 0.6, 0.6); 
		glVertex3f(koordinata, 0.0, 0.0);

		glColor4f(red, green, blue, alpha);
		glNormal3f(0.6, 0.6, 0.6);
		glVertex3f(0.0, 0.0, koordinata);

	glEnd();

	// Crtanje vektora
	glBegin(GL_LINES);

		glVertex3f(0.0, koordinata, 0.0);
		glVertex3f(-0.5, 2.5, -0.5);
		glVertex3f(-0.5, 2.5, -0.5);
		glVertex3f(-0.3, 2.5, -0.3);

		glVertex3f(koordinata, 0.0, 0.0);
		glVertex3f(2.5, -0.5, -0.5);
		glVertex3f(2.5, -0.5, -0.5);
		glVertex3f(2.5, -0.3, -0.3);

		glVertex3f(0.0, 0.0, koordinata);
		glVertex3f(-0.5, -0.5, 2.5);
		glVertex3f(-0.5, -0.5, 2.5);
		glVertex3f(-0.3, -0.3, 2.5);

	glEnd();
}

static void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	{
	glTranslatef(1.5, -1.1, 0.0);
	glRotatef(localSystemy, 0.0, 1.0, 0.0);
	glRotatef(localSystemx, 1.0, 0.0, 0.0);
	
	glBegin(GL_LINES);

		// Crtanje x ose
	    glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.3, 0.0, 0.0);
		glVertex3f(0.3, 0.0, 0.0);
		glVertex3f(0.2, 0.0, 0.05);
		glVertex3f(0.3, 0.0, 0.0);
		glVertex3f(0.2, 0.0, -0.05);

		// Crtanje y ose
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.3, 0.0);
		glVertex3f(0.0, 0.3, 0.0);
		glVertex3f(0.05, 0.2, 0.0);
		glVertex3f(0.0, 0.3, 0.0);
		glVertex3f(-0.05, 0.2, 0.0);

		// Crtanje z ose
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.3);
		glVertex3f(0.0, 0.0, 0.3);
		glVertex3f(0.05, 0.0, 0.2);
		glVertex3f(0.0, 0.0, 0.3);
		glVertex3f(-0.05, 0.0, 0.2);

		// Crtanje y
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(0.1, 0.2, 0.0);
		glVertex3f(0.15, 0.3, 0.0);
		glVertex3f(0.1, 0.3, 0.0);
		glVertex3f(0.125, 0.25, 0.0);

		// Crtanje x
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.25, 0.1, 0.0);
		glVertex3f(0.3, 0.15, 0.0);
		glVertex3f(0.3, 0.1, 0.0);
		glVertex3f(0.25, 0.15, 0.0);

		// Crtanje z
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-0.1, 0.1, 0.3);
		glVertex3f(-0.15, 0.1, 0.3);
		glVertex3f(-0.1, 0.03, 0.3);
		glVertex3f(-0.15, 0.03, 0.3);
		glVertex3f(-0.15, 0.03, 0.3);
		glVertex3f(-0.1, 0.1, 0.3);

	glEnd();
	}

	// Crtanje i rotacija presecne ravni sa geometrijskim telom
	
	glEnable(GL_CLIP_PLANE0);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(yRotacijaTast, 0.0, 1.0, 0.0);
	glRotatef(xRotacijaTast, 1.0, 0.0, 0.0);
	glClipPlane(GL_CLIP_PLANE0, clipPlane);

	glRotatef(xPolozajObjekta, 1.0, 0.0, 0.0);
	glRotatef(yPolozajObjekta, 0.0, 1.0, 0.0);

	// Definisanje boje i zaklanjanje povrsine geometrijskog tela u preseku sa ravni
	glEnable(GL_CULL_FACE);
	glColor3f(1.0, 0.0, 0.0);
	drawCurrentObject();
	glCullFace(GL_FRONT);

	// Ukljucivanje testa matrice
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawCurrentObject();
	glStencilFunc(GL_EQUAL, 1, 1);
	glCullFace(GL_BACK);

	// Onemogucavanje efekta zaklanjanja povrsine i presecne ravni
	glDisable(GL_CULL_FACE);
	glDisable(GL_CLIP_PLANE0);

	drawClipPlane(3.0, 1.0, 0.0, 0.0, 0.0);

	// Ponovo postavljamo funkciju poredjenja
	glStencilFunc(GL_NOTEQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	if(prikazPresecneRavni)
	{
		// Crtanje providne presecne ravni
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawClipPlane(3.0, 0.0, 1.0, 1.0, 0.2);
		glDisable(GL_BLEND);
	}

	// Iskljucivanje testa matrice
	glDisable(GL_STENCIL_TEST);

	glutSwapBuffers();
}

static void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLfloat) width / (GLfloat) height, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

static void InitMaterials(void)
{
    float refleksija[] = {0.7, 0.6, 0.6};

    glLightfv(GL_LIGHT0, GL_POSITION, PozicijaSvetla);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, refleksija);
    glMaterialf(GL_FRONT, GL_SHININESS, 70.0);

	glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(Width, Height);
	glutCreateWindow(argv[0]);

	InitMaterials();

	glutKeyboardFunc(key);
	glutSpecialFunc(SpecialKeys);
	glutMotionFunc(MouseMove);
	glutMouseFunc(MouseDownUp);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutIdleFunc(draw);

	glClearColor(0.1, 0.1, 0.1, 0.1); 

	glEnable(GL_DEPTH_TEST); // Ukljucivanje bafera dubine

	glutMainLoop();
	return 0;
}