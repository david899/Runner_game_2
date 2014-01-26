#include "Gracz.h"

// MUSZE TUTAJ KONIECZNIE ZMIENIC ABY WSPOLZEDNE CZLOWIEKA  !!!!!!!!!
// BYLY JEGO LEWYM DOLNYM PUNKTEM JAK W RESZCIE OBIEKTOW !!!!!!!!!
// teraz wspolzedna sa srodkiem 
#include <math.h>
#include "Mapa.h"

Gracz::Gracz()
{
	mapa = new Mapa();
	graczX = 3.0f; // szereokosc, grubosc, wysokosc solidCuba ktory jest graczem
	graczY = 16.0f;
	graczZ = 3.0f;

	// pozycja srodka gracza
	itNaAktualnePole = mapa->zwrocItNaPolePoczatkowe();
	itNaKolejnePole = itNaAktualnePole;
	itNaKolejnePole++;
	pozycja.x = 15.0f;
	pozycja.y = graczY / 2;
	pozycja.z = (*itNaAktualnePole)->pozycja.z;
	zmianaToruCel = pozycja;

	kierunek.x = 0.0f;
	kierunek.y = 0.0f;
	kierunek.z = 1.0f;

	predkosc = 0.0f;

	szescianAABB = Vec3(graczX,graczY,graczZ);
	szescianAABBmin.x = pozycja.x - szescianAABB.x/2;
	szescianAABBmin.y = pozycja.y - szescianAABB.y/2;
	szescianAABBmin.z = pozycja.z - szescianAABB.z/2;

	szescianAABBmax.x = pozycja.x + szescianAABB.x/2;
	szescianAABBmax.y = pozycja.y + szescianAABB.y/2;
	szescianAABBmax.z = pozycja.z + szescianAABB.z/2;
}

///////////////////////////////////////

void Gracz::rysuj()
{
	glPushMatrix();
		glTranslatef(pozycja.x, pozycja.y, pozycja.z);
		glScalef(graczX, graczY, graczZ);
		glColor3f(1.0f,0.0f,0.0f);
		glutSolidCube(1.0f);
	glPopMatrix();
}
void Gracz::debugRysuj()
{
	// wspolzedne srodka
	Vec3 srodek = Vec3();
	srodek = zwrocSrodekAABB();
	// rysowanie aktualnego
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(srodek.x, srodek.y, srodek.z);
		/*glScalef(
			szescianAABB.x,
			szescianAABB.y,
			szescianAABB.z);*/
		glScalef(
			szescianAABBmax.x - szescianAABBmin.x,
			szescianAABBmax.y - szescianAABBmin.y, 
			szescianAABBmax.z - szescianAABBmin.z);
		glutWireCube(1.0f);
	glPopMatrix();
}
void Gracz::dodajPredkosc(float _predkosc)
{
	predkosc += _predkosc;
}
Vec3 Gracz::zwrocSrodekAABB()
{
	Vec3 zwracany = Vec3();
	zwracany.x = szescianAABBmin.x + (szescianAABBmax.x - szescianAABBmin.x)/2;
	zwracany.y = szescianAABBmin.y + (szescianAABBmax.y - szescianAABBmin.y)/2;
	zwracany.z = szescianAABBmin.z + (szescianAABBmax.z - szescianAABBmin.z)/2;
	return zwracany;
}
void Gracz::sprawdzKolizje()
{
	// sprawdzam kolizje od aktualnego pola oraz od kolejnego (aby rozwiazac problem gdy jestem na dwoch polach
	// jednoczesnie. Jezeli nie ma kolizji na aktualnym to go zwiekszam(przesuwam do kolejnego)
	
	if((*itNaAktualnePole)->sprawdzKolizje(this) == true) // sprawdzenie aktualnego
	{
		(*itNaKolejnePole)->sprawdzKolizje(this); // sprawdzenie kolejnego pola
	}
	else
	{
		++itNaAktualnePole == mapa->wektorPol.end();
		if(++itNaKolejnePole == mapa->wektorPol.end())
		{
			itNaAktualnePole--;
			itNaKolejnePole--;
		}
		else
		{
			(*itNaAktualnePole)->sprawdzKolizje(this);
			(*itNaKolejnePole)->sprawdzKolizje(this);
		}
		mapa->generujPola(1);
		
	}
	// wykonuje reakcje na kolizje ktora zostala dodana powyzej
	while(obiektyKolidujace.empty() == false)
	{
		reakcjaNakolizje(obiektyKolidujace.top());
		obiektyKolidujace.pop();
	}
}
void Gracz::reakcjaNakolizje(ObiektFizyczny* obiekt)
{
	switch(obiekt->typObiektu)
	{
	case TypyObiektow::typPole:
		//printf("Wszedlem na pole z = %f",obiekt->zwrocSrodek().z);
		glPushMatrix();
			glColor3f(1.0f,0.0f,0.0f);
			glTranslatef(obiekt->pozycja.x,obiekt->pozycja.y,obiekt->pozycja.z);
			glutSolidCube(1.0f);
		glPopMatrix();
		break;
	case TypyObiektow::typKamien:
		glPushMatrix();
			glColor3f(1.0f,0.0f,0.0f);
			glTranslatef(obiekt->pozycja.x,obiekt->pozycja.y,obiekt->pozycja.z);
			glutSolidCube(1.0f);
		glPopMatrix();
		break;
	}
}
void Gracz::update()
{
	pozycja += kierunek * predkosc;
	szescianAABBmin += kierunek * predkosc;
	szescianAABBmax += kierunek * predkosc;

	//printf("gracz.x = %.0f\n",pozycja.x);
	printf("gracz.z = %.0f\n",pozycja.z);
	//printf("kierunek = (%.0f,%.0f,%.0f)\n",kierunek.x,kierunek.y,kierunek.z);

	//kierunek prosto (0,0,1)
	if(kierunek.x != 0.0f && pozycja.x < zmianaToruCel.x + predkosc && pozycja.x > zmianaToruCel.x - predkosc)
	{
		pozycja.x = zmianaToruCel.x;
		kierunek.x = 0.0f;
		kierunek.z = 1.0f;
		predkosc /= 2.0;
	}
	
}
void Gracz::zmienTor(int _kierunekRuchu)
{ // w lewo 0, w prawo 1
	if(zmianaToruCel.x == pozycja.x) // jezeli nigdzie sie aktualnie nie przesuwam
	{
		if(_kierunekRuchu == 0)
		{ // ide w lewo
			kierunek.dodajKat(45.0f,'y');
			zmianaToruCel.x += 15.0f;
			predkosc *= 2.0;
		}
		else
		{// ide w prawo
			kierunek.dodajKat(-45.0f,'y');
			zmianaToruCel.x += -15.0f;
			predkosc *= 2.0;
		}
	}
}

