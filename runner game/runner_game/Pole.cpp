#include "Pole.h"

bool Pole::operator==(const Pole & _pole)
{
	return (this->pozycja == _pole.pozycja) ? true : false;
}
Pole::Pole()
{
	pozycja.x = -1;
	pozycja.y = -1;
	pozycja.z = -1;
}
Pole::Pole(float x, float y, float z)
{
	pozycja.x = x;
	pozycja.y = y;
	pozycja.z = z;
}
Pole::Pole(Vec3 _pozycja)
{
	pozycja = _pozycja;
}


Vec3 Pole::getSrodek()
{
	Vec3 zwracany = Vec3(pozycja.x + 5.0f, pozycja.y + 5.0f, pozycja.z + 5.0f);
	return zwracany;
}
void Pole::dodajObiekt()
{
}
void Pole::sprKolizje()
{
	// tutaj po prostu leci po wszystkich obiektach z listy i u¿ywa ich funkcje o.sprawdzKolizje();
}

void Pole::debugRysuj()
{ // tylko do debugu!
	glPushMatrix();
		Vec3 srodek = getSrodek();// rysuje od pozycji srodka
		glTranslatef(srodek.x, srodek.y, srodek.z);
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireCube(10.0f);
	glPopMatrix();
}
void Pole::rysuj()
{// rysuje wszystkie obiekty podlaczone pod to pole
}