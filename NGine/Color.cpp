#include "Color.h"

Color::Color()
{
}

Color::Color(float r, float g, float b, float a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

float Color::GetR()
{
	return R;
}

float Color::GetG()
{
	return G;
}

float Color::GetB()
{
	return B;
}

float Color::GetA()
{
	return A;
}

Color Color::Red()
{
	return Color(1, 0, 0, 1);
}

Color Color::Green()
{
	return Color(0, 1, 0, 1);
}

Color Color::Blue()
{
	return Color(0, 0, 1, 1);
}

Color Color::Black()
{
	return Color(0, 0, 0, 1);
}

Color Color::White()
{
	return Color(1, 1, 1, 1);
}
