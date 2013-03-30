#include "RGBColor.h"


RGBColor::RGBColor(void) :
	red(0.0),
	green(0.0),
	blue(0.0)
{
}

RGBColor::RGBColor(float r, float g, float b) :
	red(r),
	green(g),
	blue(b)
{
}

RGBColor operator-(RGBColor& op1, RGBColor& op2)
{
	return RGBColor(op1[0] - op2[0], op1[1] - op2[1], op1[2] - op2[2]);
}

//For accessing color values
float RGBColor::operator[](int index)
{
	switch(index)
	{
	case 0:
		return red;
	case 1:
		return green;
	case 2:
		return blue;
	default:
		return 0.0;
	}
}



RGBColor operator+(RGBColor& op1, RGBColor& op2)
{
	return RGBColor(op1[0] + op2[0], op1[1] + op2[1], op1[2] + op2[2]);
}

RGBColor operator*(RGBColor& op1, RGBColor& op2)
{
	return RGBColor(op1[0] * op2[0], op1[1] * op2[1], op1[2] * op2[2]);
}

RGBColor operator*( RGBColor& op, float scale)
{
	return RGBColor(op[0] * scale, op[1] * scale, op[2] * scale);
}

RGBColor operator*(float scale, RGBColor& op)
{
	return RGBColor(op[0] * scale, op[1] * scale, op[2] * scale);
}


RGBColor RGBColor::operator/(float divisor)
{
	return RGBColor(red / divisor, green / divisor, blue / divisor);
}

RGBColor RGBColor::power(float power)
{
	return RGBColor(pow(red, power), pow(green, power), pow(blue, power));
}


RGBColor::~RGBColor(void)
{
}
