#include "Mapa.h"
#include "ObiektFizyczny.h"
#include "ObjLoader.h"
#include "TexLoader.h"

Mapa::Mapa()
{
	grawitacja = -0.05f;
	// ===================================================================================
	// standardowym i sztywnym poczatkiem mapy jest faza 1 gry, czyli bieg przez jaskinie
	// taki tutorial o stalej dlugosci, tutaj zaczyna sie definiowanie tych pol, wypelnianie mapy:
	// ===================================================================================
	Vec3 pozycjaPoczatkowa = Vec3(0,0,0);
	// tworzy x liczbe pustych pol, potem wypelnia losowo 
	for(int i = 0; i < iloscPolStartowych; i++)
	{
		ObiektFizyczny* pole = new ObiektFizyczny(this,pozycjaPoczatkowa,typPole);
		pozycjaPoczatkowa.z += pole->wielkoscPola.z; //odleglosc miedzy polami = ich wielkosci
		wypelnijPoleLosowo(pole);
		wektorPol.push_back(pole);
	}
	// na pozycji 5 -> na 5 polu dodaje akcje pierwsza
	list<ObiektFizyczny*>::iterator it = wektorPol.begin();
	advance(it,5);
	(*it)->dodajAkcje(1);
}
void Mapa::rysujBackground()
{
	glEnable(GL_TEXTURE_2D);


	// rysuje jaskinie ===============================
	glBindTexture(GL_TEXTURE_2D, tekstura[typJaskinia]);
	glPushMatrix();
		glTranslatef(15.0f, 0.0f, -15.0f);
		glCallList(model[typJaskinia]);
	glPopMatrix();

	// rysuje skrzynie ==============================
	glBindTexture(GL_TEXTURE_2D, tekstura[typSkrzynia]);
	glPushMatrix();
		glTranslatef(15.0f, 0.0f, -5.0f);
		glCallList(model[typSkrzynia]);
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);
}
void Mapa::rysuj()
{
	// rysuje pola fizyczne
	list<ObiektFizyczny*>::iterator it = wektorPol.begin();
	for(it = wektorPol.begin(); it != wektorPol.end(); it++)
	{
		(*it)->rysuj();
	}
	// rysuje background nie bioracy udzialu w kolizji
	rysujBackground();
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
		nowePole = new ObiektFizyczny(this,pozycjaNowegoPola, typPole);
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
		nowePole = new ObiektFizyczny(this,pozycjaNowegoPola, typPole);
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
	z = RandomFloat(poleDoWypelnienia->szescianAABBmin.z, poleDoWypelnienia->szescianAABBmax.z);
	ObiektFizyczny* obiekt = new ObiektFizyczny(this,Vec3(x,y,z),typKamien);
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
void Mapa::WczytajModeleOrazTekstury()
{
	//wczytuje tekstury tylko raz podczas tworzenia, pozniej obiekty maja dostep do tego
	model[typGracz] = LoadObj("Resources//Modele//Gracz.obj");
	tekstura[typGracz] = LoadTexture("Resources//Tekstury//Gracz.bmp",GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR); 
	model[typSkrzynia] = LoadObj("Resources//Modele//skrzynia.obj");
	tekstura[typSkrzynia] = LoadTexture("Resources//Tekstury//Skrzynia.bmp",GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR); 
	model[typJaskinia] = LoadObj("Resources//Modele//jaskinia.obj");
	tekstura[typJaskinia] = LoadTexture("Resources//Tekstury//Jaskinia.bmp",GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR); 
}