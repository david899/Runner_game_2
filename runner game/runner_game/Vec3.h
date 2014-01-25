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
	/*void dodajKat(float kat, char wybor_x_y_z)
	{
		float r = biegunowy_promien(wybor_x_y_z);
		float phi = biegunowy_phi(wybor_x_y_z);
		if(wybor_x_y_z == 'x')
		{
			return 
		}
	}
	float biegunowy_promien(char wybor_x_y_z)
	{
		if(wybor_x_y_z == 'x')
			return sqrt(pow(y,2)+pow(z,2));
		if(wybor_x_y_z == 'y')
			return sqrt(pow(x,2)+pow(z,2));
		if(wybor_x_y_z == 'z')
			return sqrt(pow(x,2)+pow(y,2));
	}
	float biegunowy_phi(char wybor_x_y_z)
	{	
		if(wybor_x_y_z == 'x')
			return atan2(z,y);
		if(wybor_x_y_z == 'y')
			return atan2(z,x);
		if(wybor_x_y_z == 'z')
			return atan2(y,x);
	}
	void biegunowy_na_kartez(float r, float phi, char wybor_x_y_z)
	{
		if(wybor_x_y_z == 'x')
		{
			y = r*cos(phi);
			z = 
		}
	}*/
};
#endif