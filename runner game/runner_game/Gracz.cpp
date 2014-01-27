#include "Gracz.h"

// MUSZE TUTAJ KONIECZNIE ZMIENIC ABY WSPOLZEDNE CZLOWIEKA  !!!!!!!!!
// BYLY JEGO LEWYM DOLNYM PUNKTEM JAK W RESZCIE OBIEKTOW !!!!!!!!!
// teraz wspolzedna sa srodkiem 
#include <math.h>
#include "Mapa.h"

Gracz::Gracz()
{
	mapa = new Mapa();
	naZiemi = true;
	szereokoscGracza = 3.0f; // szereokosc, grubosc, wysokosc solidCuba ktory jest graczem
	wysokoscGracza = 10.0f;
	gruboscGracza = 3.0f;

	// pozycja srodka gracza
	itNaAktualnePole = mapa->zwrocItNaPolePoczatkowe();
	itNaKolejnePole = itNaAktualnePole;
	itNaKolejnePole++;
	pozycja.x = 15.0f;
	pozycja.y = wysokoscGracza / 2;
	pozycja.z = (*itNaAktualnePole)->pozycja.z;
	TorDocelowy = pozycja;

	kierunek.x = 0.0f;
	kierunek.y = 0.0f;
	kierunek.z = 1.0f;

	predkosc = Vec3(1.0f,0.0f,0.3f);

	szescianAABB = Vec3(szereokoscGracza,wysokoscGracza,gruboscGracza);
	szescianAABBmin.x = pozycja.x - szescianAABB.x/2;
	szescianAABBmin.y = pozycja.y - szescianAABB.y/2;
	szescianAABBmin.z = pozycja.z - szescianAABB.z/2;

	szescianAABBmax.x = pozycja.x + szescianAABB.x/2;
	szescianAABBmax.y = pozycja.y + szescianAABB.y/2;
	szescianAABBmax.z = pozycja.z + szescianAABB.z/2;
}
#pragma region sety i gety
void Gracz::setXPozycja(float _x)
{
	pozycja.x = _x;
	szescianAABBmin.x = pozycja.x - szescianAABB.x/2;
	szescianAABBmax.x = pozycja.x + szescianAABB.x/2;
}
void Gracz::setYPozycja(float _y)
{
	pozycja.y = _y;
	szescianAABBmin.y = pozycja.y - szescianAABB.y/2;
	szescianAABBmax.y = pozycja.y + szescianAABB.y/2;
}
void Gracz::setZPozycja(float _z)
{
	pozycja.z = _z;
	szescianAABBmin.z = pozycja.z - szescianAABB.z/2;
	szescianAABBmax.z = pozycja.z + szescianAABB.z/2;
}
void Gracz::setPozycja(Vec3 nowaWartosc)
{
	pozycja = nowaWartosc;
	szescianAABBmin = pozycja - szescianAABB/2;
	szescianAABBmax = pozycja + szescianAABB/2;
}
Vec3 Gracz::getPozycja()
{
	return pozycja;
}
void Gracz::dodajPozycja(Vec3 wektorDoDodania)
{
	pozycja += wektorDoDodania;
	szescianAABBmin += wektorDoDodania;
	szescianAABBmax += wektorDoDodania;
}
#pragma endregion
///////////////////////////////////////

void Gracz::rysuj()
{
	glPushMatrix();
	glTranslatef(getPozycja().x, getPozycja().y, getPozycja().z);
		glScalef(szereokoscGracza, wysokoscGracza, gruboscGracza);
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
		glScalef(
			szescianAABBmax.x - szescianAABBmin.x,
			szescianAABBmax.y - szescianAABBmin.y, 
			szescianAABBmax.z - szescianAABBmin.z);
		glutWireCube(1.0f);
	glPopMatrix();
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

			// usuwam jedno pole oraz generuje jedno do przodu
			list<ObiektFizyczny*>::iterator itPoczatek = mapa->zwrocItNaPolePoczatkowe();
			if(getPozycja().z > (*itPoczatek)->pozycja.z + 30) // nie kasuje sie od razu tylko jak gracz odjedzie od poczatkowego troche
				mapa->generujPola(1);
		}
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
	case typPole:
		glPushMatrix();
			glColor3f(1.0f,0.0f,0.0f);
			glTranslatef(obiekt->pozycja.x,obiekt->pozycja.y,obiekt->pozycja.z);
			glutSolidCube(1.0f);
		glPopMatrix();
		break;
	case typKamien:
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
	predkosc.y += mapa->grawitacja;
	dodajPozycja(kierunek * predkosc);
	if(getPozycja().y - wysokoscGracza/2 < 0.0f)
	{
		setYPozycja(wysokoscGracza/2);
		predkosc.y = 0.0f;
		naZiemi = true;
	}

	//kierunek prosto (0,0,1)
	if(kierunek.x != 0.0f //je¿eli ide w bok (nie ide prosto)
		&& getPozycja().x < TorDocelowy.x + predkosc.x// jezeli przekraczam lewy rog mapy
		&& getPozycja().x > TorDocelowy.x - predkosc.x) // jezeli przekraczam prawy rog mapy
	{ // to 
		setXPozycja(TorDocelowy.x);
		kierunek.x = 0.0f;
		kierunek.z = 1.0f;
	}
}
void Gracz::zmienTor(int _kierunekRuchu)
{ // w lewo 0, w prawo 1
	//if(TorDocelowy.x == getPozycja().x) // jezeli nigdzie sie aktualnie nie przesuwam
	//{
		if(_kierunekRuchu == 0)
		{ // ide w lewo
			if(getPozycja().x < 30.0f && TorDocelowy.x < 30.0f)
			{
				kierunek.x = 1.0f;
				TorDocelowy.x += 15.0f;
			}
		}
		else
		{// ide w prawo
			if(getPozycja().x > 0.0f && TorDocelowy.x > 0.0f)
			{
				kierunek.x = -1.0f;
				TorDocelowy.x += -15.0f;
			}
		}
	//}
}
void Gracz::skocz()
{
	if(naZiemi)
	{
		kierunek.y = 1.0f;
		predkosc.y = 1.2f;
		naZiemi = false;
	}
}

