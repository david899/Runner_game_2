#ifndef _GRACZ_H
#define _GRACZ_H

#include "ObiektFizyczny.h"
#include "Vec3.h"
#include <GL\freeglut.h>
#include <stack>
#include "Pole.h"
#include <list>

using namespace std;
class ObiektFizyczny;
class Mapa;
class Gracz 
{
public: 
	//zmienne 
	Vec3 kierunek;
	Vec3 predkosc;
	Vec3 szescianAABB;
	Vec3 szescianAABBmin;
	Vec3 szescianAABBmax;
	float szereokoscGracza, wysokoscGracza, gruboscGracza; //szerekosc, dlugosc, wysokosc
	stack<ObiektFizyczny*> obiektyKolidujace;
	Mapa* mapa;
	Vec3 TorDocelowy;
	bool naZiemi;
private:
	list<ObiektFizyczny*>::iterator itNaAktualnePole;
	list<ObiektFizyczny*>::iterator itNaKolejnePole;
	Vec3 pozycja; // pozycja gracza jest jego srodkowym punktem (srodek z x,y,z)
	// wazna rzecz o pozycji - odwolywac sie do niej JEDYNIE przez funkcje, bo funkcja zalatwia
	// synchronizowanie polozenia szescianuAABB wraz z polozeniem

	//konstruktory
public:
	Gracz();

	//metody
public:
	void setXPozycja(float _x);
	void setYPozycja(float _y);
	void setZPozycja(float _z);
	void setPozycja(Vec3 nowaWartosc);
	Vec3 getPozycja();
	void dodajPozycja(Vec3 wektorDoDodania);
	void rysuj();
	void debugRysuj();
	void update();
	Vec3 zwrocSrodekAABB();
	void sprawdzKolizje();
	void reakcjaNakolizje(ObiektFizyczny* obiekt);
	void zmienTor(int kierunek);
	void skocz();
};
#endif