#include "Obiekt.h"

// tu bedzie rozbudowana struktura teraz tylko jeden przykladowy typ
// tutaj mozna wykorzystaæ fabrykê lub inny wzorzec, sprawdz notatki z angry birds tam jest konstruktor na fabryke ktora od danego slowa wyrzucala wlasciwy typ

Obiekt::Obiekt(Vec3 _pozycja, rodzajeKolizji _kolizja)
{
	pozycja = _pozycja;
	kolizja = _kolizja;
}

void Obiekt::rysuj()
{
	glutWireCube(10.0f);
}
