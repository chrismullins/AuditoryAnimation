#pragma once
#include "RGBColor.h"

// This is a simple data structure for describing the material of an object
class Material
{
public:
	RGBColor ambient;
	RGBColor diffuse;
	RGBColor specular;
	RGBColor reflective;

	float phong_exponent;

	Material(RGBColor amb, RGBColor diff, RGBColor spec, RGBColor refl);
	Material();
	~Material(void);
};