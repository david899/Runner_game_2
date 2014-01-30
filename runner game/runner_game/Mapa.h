#ifndef _MAPA_H
#define _MAPA_H
#include <list>
#include <random>
#include <cstdlib>
#include <map>

using namespace std;
class ObiektFizyczny;
enum TypyObiektow;

class Mapa
{
public:
	//zmienne
	list<ObiektFizyczny*> wektorPol;
	float grawitacja;
	map<TypyObiektow,int> model;
	map<TypyObiektow,int> tekstura;
	
	//konstruktory
	Mapa();
	
	//metody
	void rysuj();
	void debugRysuj();
	void generujPola(int ilePol);
	void generujPola(int ileWygenerowac, int ileUsunac);
	list<ObiektFizyczny*>::iterator zwrocItNaPolePoczatkowe();
	float RandomFloat(float min, float max);
	void WczytajModeleOrazTekstury();
	void rysujBackground();
private:
	static const int iloscPolStartowych = 15;
	void wypelnijPoleLosowo(ObiektFizyczny* poleDoWypelnienia);
};
#endif