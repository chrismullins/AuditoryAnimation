#include "Material.h"

Material::Material(RGBColor amb, RGBColor diff, RGBColor spec, RGBColor refl)
{
	ambient = amb;
	diffuse = diff;
	specular = spec;
	reflective = refl;
}

Material::Material()
{
	diffuse = RGBColor();
	ambient = RGBColor();
	reflective = RGBColor();
	
	phong_exponent = 0.0;
}

Material::~Material(void)
{
}