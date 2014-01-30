#ifndef _KAMERA_H
#define _KAMERA_H

#include "Gracz.h"
#include "Vec3.h"

class Kamera
{
private:
	//zmienne
	bool patrzeNaGracza;
	bool sledzeGracza;
	Vec3 manKierunek; // do manualnego ruchu
	float manPredkosc; // tu tez
	Vec3 poprzedniaPozycjaGracza;
	//dodac trzeci wektor pochylenia kamery? 
	
public:
	//zmienne
	static const int odchylenieX = 0, odchylenieY = 25, odchylenieZ = -30; // do ustalania pozycji za graczem
	Vec3 pozycja;
	Vec3 cel;
	Vec3 przesuniecie; // ta i kolejna to zmienne do wykorzystania TYLKO! do funkcji idzDo, a dokladnie onTimer
	int iloscPrzesuniec;
	Vec3 zwrocCel();
	
	
	//metody
	Kamera();
	void idzDo(Gracz gracz, int _milisekundy, bool _patrzNaGracza, bool _sledzGracza);
	void idzDo(Vec3 _pozycjaDocelowa, int _milisekundy, bool _patrzNaGracza);
	void idzDo(Vec3 _pozycjaDocelowa, int _milisekundy, Vec3 _cel);
	void sledzGracza(Gracz gracz, bool idzDoGracza);
	void stopSledzGracza();
	void Update(Gracz gracz);
	void manualnyRuch(Vec3 _kierunek, float _predkosc);
	void patrzNaGracza(bool wybor);
	private: void plynnePrzesuniecieKamery(int _milisekundy, Vec3 _pozycjaDocelowa);
};
#endif