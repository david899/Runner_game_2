#include "ObiektFizyczny.h"
#include <algorithm>
#include "Gracz.h"
#include "Kamera.h"



ObiektFizyczny::ObiektFizyczny(Vec3 _pozycja, TypyObiektow _typObiektu)
{ // po czesci spelnia funkcje fabryki, wypelnia tak obiekt aby byl roznego typu od stringu _typObiektu
	
	// deklaracja gówna
	wielkoscPola = Vec3(30.0f, 40.0f, 10.0f);
	wielkoscKamienia = Vec3(5.0f, 5.0f, 5.0f);
	wskNaAkcje = &ObiektFizyczny::akcjaPusta;
	// koniec deklaracji szamba
	
	pozycja = _pozycja;
	typObiektu = _typObiektu;
	switch(typObiektu)
	{
		case typPole:
			szescianAABBmin = _pozycja;
			szescianAABBmax = szescianAABBmin + wielkoscPola;
			szescianAABB = wielkoscPola;
			wskNaRysuj = &ObiektFizyczny::rysujPole;
			break;
		case typKamien:
			szescianAABBmin = _pozycja;
			szescianAABBmax = szescianAABBmin + wielkoscKamienia;
			szescianAABB = wielkoscKamienia;
			wskNaRysuj = &ObiektFizyczny::rysujKamien;
			break;
	}
}
void ObiektFizyczny::rysuj()
{
	(this->*wskNaRysuj)(); // rysuje aktualny obiekt
	if(dzieci.empty() == false)
	{
		for(list<ObiektFizyczny*>::iterator it = dzieci.begin(); it != dzieci.end(); it++)
		{
			(*it)->rysuj();
		}
	}
}
void ObiektFizyczny::debugRysuj()
{
	// wspolzedne srodka
	Vec3 srodek = zwrocSrodekAABB();
	// rysowanie aktualnego
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(srodek.x, srodek.y, srodek.z);
		glScalef(
			szescianAABB.x,
			szescianAABB.y,
			szescianAABB.z);
		glutWireCube(1.0f);
	glPopMatrix();
	// innych rysuje
	if(dzieci.empty() == false)
	{
		for(list<ObiektFizyczny*>::iterator it = dzieci.begin(); it != dzieci.end(); it++)
		{
			(*it)->debugRysuj();
		}
	}
}
bool ObiektFizyczny::sprawdzKolizje(Gracz* gracz)
{
	// obiekt fizyczny sprawdza czy kolizja wystapila, jezeli wystapila to:
	// a) obiekt ma dzieci: nie robie nic z kolizja z tym obiektem, sprawdzam kolizje
	// w glab, czyli czy dzieci maja kolizje (ktores z nich musi miec)
	// b) obiekt nie ma dzieci: to znaczy ze to z nim koliduje, jest najmniejsza czescia
	// z drzewa wiec dodaje AKTUALNY(kolidujacy) obiekt do obiektow kolidujacych z graczem
	// gracz to potem obsluguje
	bool temp = (this->typObiektu == typKamien);
	if(	szescianAABBmax.x >= gracz->szescianAABBmin.x 
		&&
		szescianAABBmin.x <= gracz->szescianAABBmax.x 
		&&
		szescianAABBmax.y >= gracz->szescianAABBmin.y
		&&
		szescianAABBmin.y <= gracz->szescianAABBmax.y
		&&
		szescianAABBmax.z >= gracz->szescianAABBmin.z 
		&&
		szescianAABBmin.z <= gracz->szescianAABBmax.z)
	{
		// kolizja wystapila

		(this->*wskNaAkcje)(gracz, gracz->kamera);
		if(dzieci.empty())
		{
			// wykonuje swoja kolizje
			gracz->obiektyKolidujace.push(this);
		}
		else
		{
			//jezeli sa to wykonuje kolizje dzieci
			list<ObiektFizyczny*>::iterator it = dzieci.begin();
			for(it = dzieci.begin(); it != dzieci.end(); it++)
			{
				(*it)->sprawdzKolizje(gracz);
			}
			
		}
		return true;
	}
	return false;
	// nie ma kolizji
	// nie robie nic, przechodzi mi do kolejnego obiektu
}
Vec3 ObiektFizyczny::zwrocSrodek()
{
	switch(typObiektu)
	{
		case typPole:
			return Vec3(pozycja.x + wielkoscPola.x/2, pozycja.y + wielkoscPola.y/2, pozycja.z + wielkoscPola.z/2);
			break;
		case typKamien:
			return Vec3(pozycja.x + wielkoscKamienia.x/2, pozycja.y + wielkoscKamienia.y/2, pozycja.z + wielkoscKamienia.z/2);
			break;

		default: 
			return Vec3(0,0,0);
			printf("Uwaga! Sprobowalem dostac srodek od obiektu ktorego nie ma w switchu, wynik - srodek = 0,0,0");
			break;
	}
	
}
Vec3 ObiektFizyczny::zwrocSrodekAABB()
{
	Vec3 zwracany = Vec3();
	zwracany.x = szescianAABBmin.x + (szescianAABBmax.x - szescianAABBmin.x)/2;
	zwracany.y = szescianAABBmin.y + (szescianAABBmax.y - szescianAABBmin.y)/2;
	zwracany.z = szescianAABBmin.z + (szescianAABBmax.z - szescianAABBmin.z)/2;
	return zwracany;
}
void ObiektFizyczny::dodajObiekt(ObiektFizyczny* obiekt)
{
	if(	szescianAABBmax.x >= obiekt->szescianAABBmin.x 
		&&
		szescianAABBmin.x <= obiekt->szescianAABBmax.x 
		&&
		szescianAABBmax.y >= obiekt->szescianAABBmin.y
		&&
		szescianAABBmin.y <= obiekt->szescianAABBmax.y
		&&
		szescianAABBmax.z >= obiekt->szescianAABBmin.z 
		&&
		szescianAABBmin.z <= obiekt->szescianAABBmax.z)
	{ // jezeli jest kolizja, czyli jezeli obiekt dodawany zawiera sie w obiekcie do ktorego chce go dodac
	  // to go dodaje
		dzieci.push_back(obiekt);
	}
	else
	{
		printf("dodajObiekt - probowalem dodac do obiektu taki obiekt(%.0f,%.0f,%.0f) ktory sie w nim(%.0f,%.0f,%.0f) nie zawiera\n"
			,obiekt->pozycja.x, obiekt->pozycja.y, obiekt->pozycja.z, this->pozycja.x, this->pozycja.y, this->pozycja.z);
	}
	
}

#pragma region metody do wsk na rysuj
void ObiektFizyczny::rysujPole()
{ // pola nie rysuje
}
void ObiektFizyczny::rysujKamien()
{
	glPushMatrix();
		Vec3 srodek = zwrocSrodek();
		glColor3f(0.674f, 0.647f, 0.647f);
		glTranslatef(srodek.x, srodek.y, srodek.z);
		glScalef(wielkoscKamienia.x, wielkoscKamienia.y, wielkoscKamienia.z);
		glutSolidSphere(0.5f,15,15); // tutaj rysuje nie od 1.0f tylk 0.5f poniewaz skaluje do PROMIENIA nie cieciwy!!
	glPopMatrix();
}
#pragma endregion
#pragma region metody do wsk na akcje

void ObiektFizyczny::dodajAkcje(int i)
{ // brzydkie to to ale nie mam czasu na kombinowanie czegos ladniejszego wiec roboczo
	switch(i)
	{
		case 1:
			wskNaAkcje = &ObiektFizyczny::akcjaFaza1Pocz;
			break;
	}
	
}
void ObiektFizyczny::akcjaPusta(Gracz* gracz, Kamera* kamera)
{ // niiiiiiic
}
void ObiektFizyczny::akcjaFaza1Pocz(Gracz* gracz, Kamera* kamera)
{
	gracz->zmienStanKlawisza('a', false);
	gracz->zmienStanKlawisza('d', false);

	Vec3 cel = gracz->getPozycja();
	cel.x -= 45.0f;
	kamera->idzDo(cel, 15000, gracz->getPozycja());
	kamera->sledzGracza(*gracz, true);
}
#pragma endregion
