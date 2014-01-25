#ifndef _POLE_H
#define _POLE_H

#include "Obiekt.h"
#include <vector>
#include <GL\freeglut.h>

using namespace std;

class Pole
{
public:
	//zmienne
	Vec3 pozycja;	// wspolzedna pola jest definiowana jako jego przedni, lewy, dolny róg!
	vector<Obiekt> obiektyPrzylaczone;

	//konstruktory,rzeciazenia
	bool operator==(const Pole & _pole);
	Pole();
	Pole(float x, float y, float z);
	Pole(Vec3 _pozycja);

	//metody
	Vec3 getSrodek();
	void dodajObiekt();
	void sprKolizje();
	void debugRysuj();
	void rysuj();
};
#endif