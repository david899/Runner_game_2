#include "Vec3.h"

Vec3::Vec3() //newralgicznie, nie zmieniac
{
	x = 0;
	y = 0;
	z = 0;
}
Vec3::Vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
Vec3::Vec3(const Vec3 &wektor)
{
	x = wektor.x;
	y = wektor.y;
	z = wektor.z;
}
bool Vec3::operator==(const Vec3 &wektor)
{
	if( this->x == wektor.x 
		&& 
		this->y == wektor.y
		&&
		this->z == wektor.z )
		return true;
		
	else
		return false;
}
Vec3 Vec3::operator+(const Vec3 &wektor)
{
	Vec3 wynik;
	wynik.x = this->x + wektor.x;
	wynik.y = this->y + wektor.y;
	wynik.z = this->z + wektor.z;
	return wynik;
}
Vec3 Vec3::operator-(const Vec3 &wektor)
{
	Vec3 wynik;
	wynik.x = this->x - wektor.x;
	wynik.y = this->y - wektor.y;
	wynik.z = this->z - wektor.z;
	return wynik;
}
Vec3 Vec3::operator*(const float &liczba)
{
	Vec3 wynik;
	wynik.x = this->x * liczba;
	wynik.y = this->y * liczba;
	wynik.z = this->z * liczba;
	return wynik;
}
Vec3 Vec3::operator*(const Vec3 &wektor)
{
	Vec3 wynik;
	wynik.x = this->x * wektor.x;
	wynik.y = this->y * wektor.y;
	wynik.z = this->z * wektor.z;
	return wynik;
}
Vec3 Vec3::operator/(const float &liczba)
{
	Vec3 wynik;
	wynik.x = this->x / liczba;
	wynik.y = this->y / liczba;
	wynik.z = this->z / liczba;
	return wynik;
}
Vec3 Vec3::operator/(const int &liczba)
{
	Vec3 wynik;
	wynik.x = this->x / liczba;
	wynik.y = this->y / liczba;
	wynik.z = this->z / liczba;
	return wynik;
}
void Vec3::operator+=(const Vec3 &wektor)
{
	this->x += wektor.x;
	this->y += wektor.y;
	this->z += wektor.z;
}
void Vec3::operator-=(const Vec3 &wektor)
{
	this->x -= wektor.x;
	this->y -= wektor.y;
	this->z -= wektor.z;
}

/////////////////////////////////////////////////////
// metody

/*void Vec3::dodajKat(float kat, char wybor_x_y_z)
{
	float r = biegunowy_promien(wybor_x_y_z);
	float phi = biegunowy_phi(wybor_x_y_z);
	if(wybor_x_y_z == 'x')
	{
		return 
	}
}
float Vec3::biegunowy_promien(char wybor_x_y_z)
{
	if(wybor_x_y_z == 'x')
		return sqrt(pow(y,2)+pow(z,2));
	if(wybor_x_y_z == 'y')
		return sqrt(pow(x,2)+pow(z,2));
	if(wybor_x_y_z == 'z')
		return sqrt(pow(x,2)+pow(y,2));
}
float Vec3::biegunowy_phi(char wybor_x_y_z)
{	
	if(wybor_x_y_z == 'x')
		return atan2(z,y);
	if(wybor_x_y_z == 'y')
		return atan2(z,x);
	if(wybor_x_y_z == 'z')
		return atan2(y,x);
}
void Vec3::biegunowy_na_kartez(float r, float phi, char wybor_x_y_z)
{
	if(wybor_x_y_z == 'x')
	{
		y = r*cos(phi);
		z = 
	}
}*/