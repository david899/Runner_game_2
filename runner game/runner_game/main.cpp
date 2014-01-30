#pragma region TODO
/*
 TODO ///////////////////////////////////////////////////
- uzupelnij przeliczanie na biegunowy
- singleton - gracz, mapa, sprawdzKolizje, kamera itd?
- kamera powinna moc sie latwiej odnosic do gracza zeby nie przekazywac caly czas go jako argument, mo¿e jakiœ globalny get zwracajacy wskaznik na gracza?
- kamera wziac pod uwage ze nie do konca dobrze dziala gdy wysle polecenie plynnego lotu gdy jest wykonywany inny plynny lot
- przemysl czy nie wsadziæ gdzieœ wskaŸników zamiast zwyk³ych obiektów

 UWAGI //////////////////////////////////////////////////
- debugRysuj - rysuje ich szesciany kolizji

 DOWIEDZ SIE
 */
#pragma endregion
 
/*
 STEROWANIE 
 f1 debug view
 f2 swiatlo
 spacja dodaj predkosc
 1,2,3,4 - zmienia perspektywe
*/

#pragma region deklaracje funkcji

#include <math.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "Vec3.h"
#include "Gracz.h"
#include "Kamera.h"
#include "Mapa.h"
#include "ObiektFizyczny.h"

void rysujSkydoome();
GLuint stworzSkydome();
void OnRender();
void OnReshape(int, int);
void OnKeyPress(unsigned char, int, int);
void OnKeyDown(unsigned char, int, int);
void OnKeyUp(unsigned char, int, int);
void OnSpecialKeyPress(int, int, int);
void OnSpecialKeyDown(int, int, int);
void OnSpecialKeyUp(int, int, int);
void OnMouseMove(int, int);
void Update(int);
void przesunTimerFunc(int);
GLuint LoadTexture(char *,int,int);
class Pole;
#pragma endregion

using namespace std;

Kamera kamera = Kamera();
Gracz gracz = Gracz(&kamera);

double czasGry = 0.0; // aktualny czas (a dokladniej - czas z ostatniego wywolania Update())
bool debug = false;
GLuint skydoome;
GLuint _skyTexture;
float skydomeRotate = 0.0f;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 360);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Runner v0.2");

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyPress);
	glutKeyboardUpFunc(OnKeyUp);
	glutSpecialFunc(OnSpecialKeyPress);
	glutSpecialUpFunc(OnSpecialKeyUp);
	glutTimerFunc(17, Update, 0);

	glEnable(GL_DEPTH_TEST);

	float gl_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gl_amb);

	//glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	// Moje inity
	skydoome = stworzSkydome();
	kamera.idzDo(Vec3(0.0f, 8.0f, 10.0f),0, gracz.getPozycja()); // pozycja poczatkowa kamery
	kamera.patrzNaGracza(true);

	// musi byc ta kolejnosc bo inaczej gracz pobierze puste wskazniki na modele
	gracz.mapa->WczytajModeleOrazTekstury();
	glEnable(GL_LIGHT0);


	glutMainLoop();
	return 0;
}
#pragma region Obsluga wejscia

	bool keystate[256];
	bool keystate_special[256];

	void OnKeyDown(unsigned char key, int x, int y) 
	{
		if (key == 27) 
			glutLeaveMainLoop();
	}
	void OnKeyUp(unsigned char key, int x, int y) 
	{
		keystate[key] = false;
		gracz.obslugaKlawiszy(key);

	#pragma region sterowanie kamera
		Vec3 cel = gracz.getPozycja();
		if(key == 49)// 1 na numercznej
		{
			kamera.idzDo(gracz,3000,true,true);
		}
		if (key == 50) // 2 na numerycznej
		{
			cel.y += 15.0f;
			kamera.idzDo(cel,3000, gracz.getPozycja());
			kamera.sledzGracza(gracz,true);
		}
		if (key == 51) // 3
		{
			cel.x += 45.0f;
			kamera.idzDo(cel,15000,gracz.getPozycja());
			kamera.sledzGracza(gracz,true);
		}
		if(key == 52) // 4
		{
			cel.x -= 45.0f;
			kamera.idzDo(cel,3000,gracz.getPozycja());
			kamera.sledzGracza(gracz,true);
		}
	#pragma endregion
	}
	void OnSpecialKeyDown(int key, int x, int y) 
	{

	}
	void OnSpecialKeyUp(int key, int x, int y) 
	{
		if(key == GLUT_KEY_F1)
			debug = !debug;
		keystate_special[key] = false;
	}
	void OnKeyPress(unsigned char key, int x, int y) 
		{
		if (!keystate[key]) {
			OnKeyDown(key, x, y);
		}
		keystate[key] = true;
	}
	void OnSpecialKeyPress(int key, int x, int y) {
		if (!keystate_special[key]) {
			OnSpecialKeyDown(key, x, y);
		}
		keystate_special[key] = true;
	}

#pragma endregion

void Update(int id) 
{ // update gry w taki sposob aby uniezale¿niæ update gry od mo¿liwoœci sprzêtu
	glutTimerFunc(17, Update, 0);
	
	czasGry = glutGet(GLUT_ELAPSED_TIME); // Ile milisekund uplynelo od momentu uruchomienia programu?

	gracz.update();
	kamera.Update(gracz);
	skydomeRotate+= 0.03f;
}

void OnRender() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		kamera.pozycja.x, kamera.pozycja.y, kamera.pozycja.z,
		kamera.zwrocCel().x, kamera.zwrocCel().y, kamera.zwrocCel().z,
		0.0f, 1.0f, 0.0f
	);

	#pragma region Swiatlo

		float l0_amb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float l0_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float l0_spe[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float l0_pos[] = { 0.0f, 5.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, l0_amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_dif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, l0_spe);
		glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);

	#pragma endregion
	

	#pragma region Rysuje tutaj

		gracz.sprawdzKolizje();
		gracz.mapa->rysuj();
		gracz.rysuj();
		if(debug)
		{
			gracz.mapa->debugRysuj();
 			gracz.debugRysuj();
		}

		rysujSkydoome();
	#pragma endregion
	
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();

}


void OnReshape(int width, int height) 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(60.0f, (float) width / height, .01f, 500.0f);
}

GLuint stworzSkydome()
{
	int Np = 36;
	float radius = 20.0f;
	float PI = 3.14;

	_skyTexture = LoadTexture("Resources\\Tekstury\\Skydome.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	GLuint _displayListId;
	_displayListId = glGenLists(1);
	glNewList(_displayListId, GL_COMPILE);
	glBegin(GL_QUAD_STRIP);
			for (int j = -1; j < Np / 2; ++j) {
				for (int i = 0; i <= Np; ++i) {

					float t1 = PI * j / Np - PI / 2;
					float t2 = PI * (j+1) / Np - PI / 2;
					float p = PI * 2.0f * i / Np;

					float tx1 = sin(t1) * cos(p);
					float tx2 = sin(t2) * cos(p);
					float tz1 = sin(t1) * sin(p);
					float tz2 = sin(t2) * sin(p);
					float ty1 = cos(t1);
					float ty2 = cos(t2);
					
					float tr1 = -sin(t1);
					float tr2 = -sin(t2);

					float x1 = radius * tx1;
					float x2 = radius * tx2;
					float z1 = radius * tz1;
					float z2 = radius * tz2;
					float y1 = radius * ty1;
					float y2 = radius * ty2;

					glTexCoord2f(.5f + .5f * tr2 * tx2, .5f + .5f * tr2 * tz2);
					glNormal3f(x2, y2, z2);
					glVertex3f(x2, y2, z2);
					
					glTexCoord2f(.5f + .5f * tr1 * tx1, .5f + .5f * tr1 * tz1);
					glNormal3f(x1, y1, z1);
					glVertex3f(x1, y1, z1);

				}
			}
		glEnd();
	glEndList();
	return _displayListId;
}
void rysujSkydoome()
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

		glColor3f(1.0, 1.0f, 1.0f);
		glTranslatef(gracz.getPozycja().x, 0.0f, gracz.getPozycja().z);
		glRotatef(skydomeRotate, 0.0f, 1.0f, 0.0f);
		glBindTexture(GL_TEXTURE_2D, _skyTexture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glScalef(10.0f, 10.0f, 10.0f);
		glCallList(skydoome);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
}