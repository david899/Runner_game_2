#ifndef _OBIEKTFIZYCZNY_H
#define _OBIEKTFIZYCZNY_H
#include <list>
#include <stack>
#include <GL\freeglut.h>
#include <stdio.h>
#include "Vec3.h"
#include "TypyRunner.h"

class Gracz;
class Kamera;
class Mapa;

using namespace std;

class ObiektFizyczny
{
public:
	// zmienne
	Vec3 pozycja;
	Vec3 szescianAABB; // wielkosc szescianu 
	Vec3 szescianAABBmin;
	Vec3 szescianAABBmax;
	void (ObiektFizyczny::*wskNaRysuj)();
	void (ObiektFizyczny::*wskNaAkcje)(Gracz* gracz, Kamera* kamera); // metoda spod tego wskaŸnika wywo³a siê kiedy wykryje kolizje z tym ob. fiz
	list<ObiektFizyczny*> dzieci;
	TypyObiektow typObiektu;
	Vec3 wielkoscPola;
	Vec3 wielkoscKamienia;
	int *model, *tekstura;

	// metody
	void rysuj();
	void debugRysuj(); // debugRysuj - zamiast obiektow rysuje ich sfery kolizji
	bool sprawdzKolizje(Gracz* gracz);
	Vec3 zwrocSrodek();
	Vec3 zwrocSrodekAABB();
	void dodajObiekt(ObiektFizyczny* obiekt);
	void dodajAkcje(int i);
	// konstruktory
	ObiektFizyczny(Mapa* mapa, Vec3 _pozycja, TypyObiektow _typObiektu); // po czesci spelnia funkcje fabryki, wypelnia tak obiekt aby byl roznego typu od stri);
	
private:
	// te funkcje nie moga byc widoczne dalej, wywolywalne tylko przez wskazniki w rysuj
	void rysujPole(); 
	void rysujKamien();
	void rysujPalme();

	//to sa akcje, czyli takie skrypty ktore beda sie wywolywac gdy wykryje kolizje z tym ob. fiz.
	void akcjaFaza1Pocz(Gracz* gracz, Kamera* kamera);
	void akcjaPusta(Gracz* gracz, Kamera* kamera);
	void akcjaFaza2Pocz(Gracz* gracz, Kamera* kamera);
};

#endif