#include "ObiektFizyczny.h"
#include <algorithm>
#include "Gracz.h"
#include "Kamera.h"
#include "Mapa.h"

ObiektFizyczny::ObiektFizyczny(Mapa* mapa, Vec3 _pozycja, TypyObiektow _typObiektu)
{ // po czesci spelnia funkcje fabryki, wypelnia tak obiekt aby byl roznego typu od stringu _typObiektu
	
	// deklaracja gówna
	wielkoscPola = Vec3(30.0f, 40.0f, 10.0f);
	wielkoscKamienia = Vec3(7.0f, 4.0f, 7.0);
	// koniec deklaracji gowna
	
	wskNaAkcje = &ObiektFizyczny::akcjaPusta;
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
		case typPalma1:
			// taki troche hack... bo niby obiekt fizyczny a szescian nei ma wielkosci wiec nie ma kolizji
			szescianAABBmax = szescianAABBmin = szescianAABBmin = Vec3(0,0,0);
			wskNaRysuj = &ObiektFizyczny::rysujPalme;
			break;
		case typPalma2:
			// taki troche hack... bo niby obiekt fizyczny a szescian nei ma wielkosci wiec nie ma kolizji
			szescianAABBmax = szescianAABBmin = szescianAABBmin = Vec3(0,0,0);
			wskNaRysuj = &ObiektFizyczny::rysujPalme;
			break;
	}

	model = &(mapa->model[typObiektu]);
	tekstura = &(mapa->tekstura[typObiektu]);
}
void ObiektFizyczny::rysuj()
{
	glColor3f(1.0f,1.0f,1.0f);
	ObiektFizyczny* ob = this;
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
	dzieci.push_back(obiekt);

	// zakomentowalem ponizsze aby moc normalnie wczytywac drzewa ktore sa w backgroundzie

	//if(	szescianAABBmax.x >= obiekt->szescianAABBmin.x 
	//	&&
	//	szescianAABBmin.x <= obiekt->szescianAABBmax.x 
	//	&&
	//	szescianAABBmax.y >= obiekt->szescianAABBmin.y
	//	&&
	//	szescianAABBmin.y <= obiekt->szescianAABBmax.y
	//	&&
	//	szescianAABBmax.z >= obiekt->szescianAABBmin.z 
	//	&&
	//	szescianAABBmin.z <= obiekt->szescianAABBmax.z)
	//{ // jezeli jest kolizja, czyli jezeli obiekt dodawany zawiera sie w obiekcie do ktorego chce go dodac
	//  // to go dodaje
	//	
	//}
	//else
	//{
	//	printf("dodajObiekt - probowalem dodac do obiektu taki obiekt(%.0f,%.0f,%.0f) ktory sie w nim(%.0f,%.0f,%.0f) nie zawiera\n"
	//		,obiekt->pozycja.x, obiekt->pozycja.y, obiekt->pozycja.z, this->pozycja.x, this->pozycja.y, this->pozycja.z);
	//}
}

#pragma region metody do wsk na rysuj
void ObiektFizyczny::rysujPole()
{ // pola nie rysuje
}
void ObiektFizyczny::rysujPalme()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, *tekstura);
		glPushMatrix();
			glTranslatef(pozycja.x, pozycja.y, pozycja.z);
			glCallList(*model);
		glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void ObiektFizyczny::rysujKamien()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, *tekstura);
	glPushMatrix();
		Vec3 srodek = zwrocSrodek();
		glTranslatef(srodek.x, srodek.y, srodek.z);
		glCallList(*model);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
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
	gracz->zmienStanKlawisza('w', true);
	gracz->zmienStanKlawisza('s', true);
	gracz->zmienStanKlawisza('a', false);
	gracz->zmienStanKlawisza('d', false);
	

	Vec3 cel = gracz->getPozycja();
	cel.x -= 45.0f;
	kamera->idzDo(cel, 15000, gracz->getPozycja()); // nie dzia³a tutaj 15 sekund?? 
	kamera->sledzGracza(*gracz, true);
}
#pragma endregion
