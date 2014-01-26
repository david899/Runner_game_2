#include "Mapa.h"
#include "ObiektFizyczny.h"

Mapa::Mapa()
{
	Vec3 pos = Vec3(0,0,0);
	Vec3 przesuniecie = Vec3(0,0,10.0f);
	// tworzy int iloscPolStartowych pol na przod oraz wypelnia je losowo 
	for(int i = 0; i < iloscPolStartowych; i++)
	{
		ObiektFizyczny* pole = new ObiektFizyczny(pos,TypyObiektow::typPole);
		pos += przesuniecie;
		wypelnijPoleLosowo(pole);
		wektorPol.push_back(pole);
	}
	// w powyzszej petli powineinem jeszcze dodawac inne obiekty do moich pol
}
void Mapa::rysuj()
{
	list<ObiektFizyczny*>::iterator it = wektorPol.begin();
	for(it = wektorPol.begin(); it != wektorPol.end(); it++)
	{
		(*it)->rysuj();
	}
}
void Mapa::debugRysuj()
{
	list<ObiektFizyczny*>::iterator it = wektorPol.begin();
	for(it = wektorPol.begin(); it != wektorPol.end(); it++)
	{
		(*it)->debugRysuj();
	}
}
void Mapa::generujPola(int ilePol)
{
	// metoda kasuje pola za graczem oraz dodaje pola przed graczem w ilosci - int ilePol 
	list<ObiektFizyczny*>::iterator it;
	Vec3 pozycjaNowegoPola = Vec3();
	Vec3 przesuniecie = Vec3(0,0,10.0f);
	ObiektFizyczny* nowePole;
	for(int i = 0; i < ilePol; i++)
	{
		it = wektorPol.begin();
		wektorPol.erase(it);
		it = wektorPol.end();
		--it;
		pozycjaNowegoPola = (*it)->pozycja + przesuniecie;
		nowePole = new ObiektFizyczny(pozycjaNowegoPola, TypyObiektow::typPole);
		wypelnijPoleLosowo(nowePole);	// wypelniam losowymi obiektami (moj element losowosci, 
										//pozniej trzeba dopasowac tak aby mapa byla "przejezdna"
		wektorPol.push_back(nowePole);
	}

}
void Mapa::generujPola(int ileWygenerowac, int ileUsunac)
{
	// metoda kasuje pola za graczem i dodaje nowe w zaleznosci od argumentow
	list<ObiektFizyczny*>::iterator it;
	Vec3 pozycjaNowegoPola = Vec3();
	Vec3 przesuniecie = Vec3(0,0,10.0f);
	ObiektFizyczny* nowePole;
	for(int i = 0; i < ileUsunac; i++)
	{
		it = wektorPol.begin();
		wektorPol.erase(it);
	}
	for(int i = 0; i < ileWygenerowac; i++)
	{
		it = wektorPol.end();
		pozycjaNowegoPola = (*it)->pozycja + przesuniecie;
		nowePole = new ObiektFizyczny(pozycjaNowegoPola, TypyObiektow::typPole);
		wypelnijPoleLosowo(nowePole);	// wypelniam losowymi obiektami (moj element losowosci, 
										//pozniej trzeba dopasowac tak aby mapa byla "przejezdna"
		wektorPol.push_back(nowePole);
	}
}
void Mapa::wypelnijPoleLosowo(ObiektFizyczny* poleDoWypelnienia)
{
	float x,y,z;
	x = RandomFloat(poleDoWypelnienia->szescianAABBmin.x, poleDoWypelnienia->szescianAABBmax.x);
	y = RandomFloat(poleDoWypelnienia->szescianAABBmin.y, poleDoWypelnienia->szescianAABBmax.y);
	//y = 0.0f;
	z = RandomFloat(poleDoWypelnienia->szescianAABBmin.z, poleDoWypelnienia->szescianAABBmax.z);
	ObiektFizyczny* obiekt = new ObiektFizyczny(Vec3(x,y,z),TypyObiektow::typKamien);
	poleDoWypelnienia->dodajObiekt(obiekt);
}
list<ObiektFizyczny*>::iterator Mapa::zwrocItNaPolePoczatkowe()
{
	list<ObiektFizyczny*>::iterator it = wektorPol.begin();
	return it;
}
float Mapa::RandomFloat(float min, float max)
{ 
	int r = rand(); 
	float fraction = ((float) r / RAND_MAX) * (max - min); 
	return min + fraction;
}