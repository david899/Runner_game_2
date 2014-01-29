#ifndef _TEXLOADER_H
#define _TEXLOADER_H

#include <GL/freeglut.h>
#include "Bitmap.h"

GLuint LoadTexture(char * file, int magFilter, int minFilter) 
{// Funkcja odczytuj�ca bitmap� i tworz�ca na jej podstawie tekstur� z zadanym rodzajem filtracji
	// Odczytanie bitmapy
	Bitmap *tex = new Bitmap();
	if (!tex->loadBMP(file)) {
		printf("ERROR: Cannot read texture file \"%s\".\n", file);
		return -1;
	}
	// Utworzenie nowego id wolnej tekstury
	GLuint texId;
	glGenTextures(1, &texId);
	// "Bindowanie" tekstury o nowoutworzonym id
	glBindTexture(GL_TEXTURE_2D, texId);
	// Okre�lenie parametr�w filtracji dla tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); // Filtracja, gdy tekstura jest powi�kszana
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); // Filtracja, gdy tekstura jest pomniejszana
	// Wys�anie tekstury do pami�ci karty graficznej zale�nie od tego, czy chcemy korzysta� z mipmap czy nie
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST) {
		// Automatyczne zbudowanie mipmap i wys�anie tekstury do pami�ci karty graficznej
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->width, tex->height, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	else {
		// Wys�anie tekstury do pami�ci karty graficznej 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	// Zwolnienie pami�ci, usuni�cie bitmapy z pami�ci - bitmapa jest ju� w pami�ci karty graficznej
	delete tex;
	// Zwr�cenie id tekstury
	return texId;
}

#endif