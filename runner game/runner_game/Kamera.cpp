#include "Kamera.h"

// globalna funkcja kamery
void przesunTimerFunc(int wskaznikNaKamere)
{
	// funkcja potrzebna do plynnego przesuniecia, do metody idzDo
	if(wskaznikNaKamere != 0)
	{
		Kamera* kamera = (Kamera*)wskaznikNaKamere;
		if(kamera->iloscPrzesuniec > 0)
		{
			glutTimerFunc(17, przesunTimerFunc, wskaznikNaKamere);
			kamera->pozycja += kamera->przesuniecie;
			kamera->iloscPrzesuniec--;
		}
	}
	else
	{
		printf("Blad, probowalem wywolac przesun funkcje bez podania wskaznika na kamere");
	}
}

Kamera::Kamera()
{
	pozycja = Vec3(0,0,0);
	cel = Vec3(0,0,0);
		
	sledzeGracza = false;
	patrzeNaGracza = false;
	manKierunek; // wektor zerowy
	manPredkosc = 0.0f;
	poprzedniaPozycjaGracza = Vec3(0,0,0);
}

////////////////////////////////////////////////////
Vec3 Kamera::zwrocCel()
{
	return cel;
}
void Kamera::idzDo(Gracz gracz, int _milisekundy, bool _patrzNaGracza, bool _sledzGracza)
{
	sledzeGracza = _sledzGracza;
	patrzeNaGracza = _patrzNaGracza;
	Vec3 pozycjaZaGraczem = Vec3(	gracz.getPozycja().x + odchylenieX, 
									gracz.getPozycja().y + odchylenieY, 
									gracz.getPozycja().z + odchylenieZ);
	if(_milisekundy <= 0)
	{
		pozycja = pozycjaZaGraczem;
	}
	else
	{	
		plynnePrzesuniecieKamery(_milisekundy, pozycjaZaGraczem);
	}
}
void Kamera::idzDo(Vec3 _pozycjaDocelowa, int _milisekundy, bool _patrzNaGracza)
{
	sledzeGracza = false;
	patrzeNaGracza = _patrzNaGracza;
	if(_milisekundy <= 0)
	{
		pozycja = _pozycjaDocelowa;
	}
	else
	{	
		plynnePrzesuniecieKamery(_milisekundy, _pozycjaDocelowa);
	}	
}
void Kamera::idzDo(Vec3 _pozycjaDocelowa, int _milisekundy, Vec3 _cel)
{
	sledzeGracza = false;
	patrzeNaGracza = false;
	cel = _cel;
	if(_milisekundy <= 0)
	{
		pozycja = _pozycjaDocelowa;
	}
	else
	{
		plynnePrzesuniecieKamery(_milisekundy, _pozycjaDocelowa);
	}
		
}
void Kamera::patrzNaGracza(bool wybor)
{
	patrzeNaGracza = wybor;
}
void Kamera::sledzGracza(Gracz gracz, bool idzDoGracza)
{
	cel = gracz.getPozycja();
	patrzeNaGracza = true;

	if(idzDoGracza = true)
	{
		sledzeGracza = true;
	}
}
void Kamera::stopSledzGracza()
{
	sledzeGracza = false;
	patrzeNaGracza = false;
}
void Kamera::Update(Gracz gracz)
{// obsluga ruchu kamery, automatycznego sledzenia gracza jezeli tak jest ustawiona
	if(sledzeGracza == true)
	{
		Vec3 przesuniecie;
		przesuniecie.z = gracz.getPozycja().z - poprzedniaPozycjaGracza.z;
		pozycja += przesuniecie;
		
	}
	if(patrzeNaGracza == true)
	{
		// cel = gracz.getPozycja(); // wtedy poruszam kamera gdy skacze itd
		Vec3 przesuniecie;
		przesuniecie.z = gracz.getPozycja().z - poprzedniaPozycjaGracza.z;
		cel += przesuniecie;
	}
	// jezeli nie ma obu powyzszych to jest inne sterowanie, np klawiszami, jak klawisze nie zmieniaja pozycji to b�dzie sta�
	else if (sledzeGracza == false)
	{
		pozycja += manKierunek * manPredkosc;
	}
	poprzedniaPozycjaGracza = gracz.getPozycja();
}
void Kamera::manualnyRuch(Vec3 _kierunek, float _predkosc)
{ // musze tutaj przesy�a� zera je�eli chce zatrzyma�, najlepiej w funkcji jak puszczam klawisz
	manKierunek = _kierunek;
	manPredkosc = _predkosc;
}

void Kamera::plynnePrzesuniecieKamery(int _milisekundy, Vec3 _pozycjaDocelowa)
{
	iloscPrzesuniec = _milisekundy/17;
	przesuniecie = Vec3::zwrocWektor(pozycja, _pozycjaDocelowa) / iloscPrzesuniec; // dziele na tyle odcinkow, ile milisekund podalem / 17 aby nie robic mniejszej ilosci niz moj update(co 17 milisekund)
	przesunTimerFunc((int)this);
}
