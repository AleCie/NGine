#pragma once

class Color
{
public:
	Color();
	Color(float r, float g, float b, float a);

	float GetR();
	float GetG();
	float GetB();
	float GetA();

	static Color Red();
	static Color Green();
	static Color Blue();

	static Color Black();
	static Color White();

private:

	float R, G, B, A;
};