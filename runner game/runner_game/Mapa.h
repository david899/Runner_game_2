#ifndef _MAPA_H
#define _MAPA_H
#include <vector>
#include <random>
#include <cstdlib>
using namespace std;
class ObiektFizyczny;

class Mapa
{
public:
	//zmienne
	vector<ObiektFizyczny*> wektorPol;
	
	//konstruktory
	Mapa();
	
	//metody
	void rysuj();
	void debugRysuj();
	void generujPola(int ilePol);
	void generujPola(int ileWygenerowac, int ileUsunac);
	vector<ObiektFizyczny*>::iterator zwrocItNaPolePoczatkowe();
	float RandomFloat(float min, float max);
private:
	static const int iloscPolStartowych = 30  ;
	void wypelnijPoleLosowo(ObiektFizyczny* pole);
};
#endif