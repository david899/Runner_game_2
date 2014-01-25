#ifndef _VEC3_H
#define _VEC3_H

class Vec3
{
public:
	// zmienne
	float x, y, z;
	// konstruktory, przeciazenia
	Vec3();
	Vec3(float _x, float _y, float _z);
	Vec3(const Vec3 &wektor);
	bool operator==(const Vec3 &wektor);
	Vec3 operator+(const Vec3 &wektor);
	Vec3 operator-(const Vec3 &wektor);
	Vec3 operator*(const float &liczba);
	Vec3 operator*(const Vec3 &wektor);
	Vec3 operator/(const float &liczba);
	Vec3 operator/(const int &liczba);
	void operator+=(const Vec3 &wektor);
	void operator-=(const Vec3 &wektor);
	

	/////////////////////////////////////////////////////
	//metody
	static Vec3 Vec3::zwrocWektor(Vec3 skad, Vec3 dokad)
	{
		// przyjmuje punkty, zwraca wektor z nich
		return Vec3(dokad - skad);
	}
	void dodajKat(float kat, char wybor_x_y_z);
};
#endif