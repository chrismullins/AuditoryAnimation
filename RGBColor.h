#pragma once
#include "math.h"

class RGBColor
{
public:
	float red;
	float green;
	float blue;
	RGBColor();
	RGBColor(float r, float g, float b);

	friend RGBColor operator+(RGBColor& op1, RGBColor& op2);
	friend RGBColor operator*(RGBColor& op1, RGBColor& op2);
	float operator[](int channel);

	friend RGBColor operator-(RGBColor& op1, RGBColor& op2);

	friend RGBColor operator*(RGBColor& op, float scale);
	friend RGBColor operator*(float scale, RGBColor& op);

	RGBColor operator/(float divisor);
	RGBColor power(float power);

	~RGBColor(void);
};

