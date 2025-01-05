#pragma once

#include <iostream>

class Complex
{
	double realPart;
	double imaginaryPart;

	friend std::ostream & operator <<(std::ostream &output, const Complex &complex);
	friend std::istream & operator >>(std::istream &input, Complex &complex);
public:
	Complex(const double real = 0, const double imag = 0)
		:realPart(real), imaginaryPart(imag) {}
	friend Complex operator+(const Complex &left, const Complex &right);
	friend Complex operator-(const Complex &left, const Complex &right);
	friend Complex operator*(const Complex &left, const Complex &right);
	friend Complex operator/(const Complex &left, const Complex &right);
	friend Complex& operator+=(Complex &left, const Complex &right);
	friend Complex& operator-=(Complex &left, const Complex &right);
	friend Complex& operator*=(Complex &left, const Complex &right);
	friend Complex& operator/=(Complex &left, const Complex &right);
};