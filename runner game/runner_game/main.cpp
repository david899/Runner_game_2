#pragma region TODO
/*
 TODO ///////////////////////////////////////////////////
- uzupelnij przeliczanie na biegunowy
- cala kolizja 
- doczytywanie sie oraz czyszczenie pol na mapie
- singleton - gracz, mapa, sprawdzKolizje, kamera itd?
- kamera powinna moc sie latwiej odnosic do gracza zeby nie przekazywac caly czas go jako argument, mo¿e jakiœ globalny get zwracajacy wskaznik na gracza?
- kamera wziac pod uwage ze nie do konca dobrze dziala gdy wysle polecenie plynnego lotu gdy jest wykonywany inny plynny lot
- uzupelnij update kamery
- get cel, pozycja czy nie da sie zrobic tak zeby wyrzucalo consty ?
- dopasowac reszte projektu i budowe plikow .h i .cpp tak jak pokazal dr Bazyluk (do przeklepania troche, oprocz samych includow musze pozmieniac klasy na wskazniki do nich)
- przemysl czy nie wsadziæ gdzieœ wskaŸników zamiast zwyk³ych obiektów

 UWAGI //////////////////////////////////////////////////
- debugRysuj - zamiast obiektow rysuje ich sfery kolizji

 DOWIEDZ SIE
- w sprawdzKolizje przy itNaAktualnePole++ dobrze byloby sprawdzac czy ma to znaczenie, bo inaczej wywala sie gdy natrafia na koniec wektora (it wiekszy niz ostatni element wektora)
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
#include "Bitmap.h"
#include "Vec3.h"
#include "Gracz.h"
#include "Kamera.h"
#include "Mapa.h"
#include "Obiekt.h"
#include "Pole.h"
#include "ObiektFizyczny.h"

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

void OnReshape(int width, int height) 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(60.0f, (float) width / height, .01f, 100.0f);
}

#pragma endregion
using namespace std;

Gracz gracz = Gracz();
Kamera kamera = Kamera();
double czasGry = 0.0; // aktualny czas (a dokladniej - czas z ostatniego wywolania Update())
bool debug = false;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 360);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Runner v0.1");

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
	kamera.sledzGracza(gracz,true);
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
		if (key == 'w') 
			gracz.dodajPredkosc(0.1f);
		if (key == 's')
			gracz.predkosc = 0.0f;
		if (key == 'a')
			gracz.zmienTor(kierunek_ruchu::w_lewo);
		if (key == 'd')
			gracz.zmienTor(kierunek_ruchu::w_prawo);

	#pragma region sterowanie kamera
		Vec3 cel = gracz.pozycja;
		if(key == 49)// 1 na numercznej
		{
			kamera.idzDo(gracz,3000,true,true);
		}
		if (key == 50) // 2 na numerycznej
		{
			cel.y += 15.0f;
			kamera.idzDo(cel,3000, gracz.pozycja);
		}
		if (key == 51) // 3
		{
			cel.x += 15.0f;
			kamera.idzDo(cel,3000,gracz.pozycja);
		}
		if(key == 52) // 4
		{
			cel.x -= 15.0f;
			kamera.idzDo(cel,3000,gracz.pozycja);
		}
	#pragma endregion
	}
	void OnSpecialKeyDown(int key, int x, int y) 
	{

	}
	void OnSpecialKeyUp(int key, int x, int y) 
	{
		if(key = GLUT_KEY_F1)
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
	
}

void rysujSfereNieba()
{

}
void rysujSciane()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBindTexture(GL_TEXTURE_2D, texSkrzynka_przod); nie mam tej zmiennej juz
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(.5f, .5f, .5f);
		glTranslatef(4.5f, 2.0f, 3.0f);
			glBegin(GL_QUADS);
				glTexCoord2f( 0.0f,  1.0f);
				glVertex3f(-1.0f,  1.0f,  0.0f);
			
				glTexCoord2f( 0.0f,  0.0f);
				glVertex3f(-1.0f, -1.0f,  0.0f);
			
				glTexCoord2f( 1.0f,  0.0f);
				glVertex3f( 1.0f, -1.0f,  0.0f);
			
				glTexCoord2f( 1.0f,  1.0f);
				glVertex3f( 1.0f,  1.0f,  0.0f);
			
				glTexCoord2f( 0.0f,  0.0f);
				glVertex3f(-1.0f, -1.0f,  0.0f);
			
				glTexCoord2f( 0.0f,  1.0f);
				glVertex3f(-1.0f,  1.0f,  0.0f);
			
				glTexCoord2f( 1.0f,  1.0f);
				glVertex3f( 1.0f,  1.0f,  0.0f);
			
				glTexCoord2f( 1.0f,  0.0f);
				glVertex3f( 1.0f, -1.0f,  0.0f);
			glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
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
	#pragma endregion
	
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();

}

GLuint LoadTexture(char * file, int magFilter, int minFilter) 
{// Funkcja odczytuj¹ca bitmapê i tworz¹ca na jej podstawie teksturê z zadanym rodzajem filtracji
	// Odczytanie bitmapy
	Bitmap *tex = new Bitmap();
	if (!tex->loadBMP(file)) {
		printf("ERROR: Cannot read texture file \"%s\".\n", file);
		return -1;
	}
	// Utworzenie nowego id wolnej tekstury
	GLuint texId;
	glGenTextures(1, &texId);
	// "Bindowanie" tekstury o nowoutworzonym id
	glBindTexture(GL_TEXTURE_2D, texId);
	// Okreœlenie parametrów filtracji dla tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); // Filtracja, gdy tekstura jest powiêkszana
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); // Filtracja, gdy tekstura jest pomniejszana
	// Wys³anie tekstury do pamiêci karty graficznej zale¿nie od tego, czy chcemy korzystaæ z mipmap czy nie
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST) {
		// Automatyczne zbudowanie mipmap i wys³anie tekstury do pamiêci karty graficznej
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->width, tex->height, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	else {
		// Wys³anie tekstury do pamiêci karty graficznej 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	// Zwolnienie pamiêci, usuniêcie bitmapy z pamiêci - bitmapa jest ju¿ w pamiêci karty graficznej
	delete tex;
	// Zwrócenie id tekstury
	return texId;
}


