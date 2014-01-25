#ifndef _OBIEKT_H
#define _OBIEKT_H

#include <GL\freeglut.h>
#include "Vec3.h"

class Obiekt
{
public:
	//zmienne
	Vec3 pozycja;
	enum rodzajeKolizji kolizja;
	//metody
	Obiekt::Obiekt(Vec3 _pozycja, rodzajeKolizji _kolizja);
	void Obiekt::rysuj();
};

#endif