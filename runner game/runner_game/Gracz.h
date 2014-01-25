#ifndef _GRACZ_H
#define _GRACZ_H

#include "ObiektFizyczny.h"
#include "Vec3.h"
#include <GL\freeglut.h>
#include <stack>
#include "Pole.h"

using namespace std;
class ObiektFizyczny;
class Mapa;
class Gracz 
{
public: 
	//zmienne 
	Vec3 pozycja; // pozycja gracza jest jego srodkowym punktem (srodek z x,y,z)
	Vec3 kierunek;
	float predkosc;
	Vec3 szescianAABB;
	Vec3 szescianAABBmin;
	Vec3 szescianAABBmax;
	float graczX, graczY, graczZ; //szerekosc, dlugosc, wysokosc
	stack<ObiektFizyczny*> obiektyKolidujace;
	Mapa* mapa;
private:
	vector<ObiektFizyczny*>::iterator itNaAktualnePole;

	//konstruktory
public:
	Gracz();

	//metody
public:
	void rysuj();
	void debugRysuj();
	void dodajPredkosc(float _predkosc);
	void update();
	Vec3 zwrocSrodekAABB();
	void sprawdzKolizje();
	void reakcjaNakolizje(ObiektFizyczny* obiekt);
};
#endif