#include "Complex.h"

Complex operator+(const Complex &left, const Complex &right)
{
	return Complex(left.realPart + right.realPart, left.imaginaryPart + right.imaginaryPart);
}

Complex operator-(const Complex &left, const Complex &right)
{
	return Complex(left.realPart - right.realPart, left.imaginaryPart - right.imaginaryPart);
}

Complex operator*(const Complex &left, const Complex &right)
{
	return Complex(left.realPart*right.realPart - left.imaginaryPart*right.imaginaryPart, \
		left.realPart*right.imaginaryPart + left.imaginaryPart*right.realPart);
}

Complex operator/(const Complex &left, const Complex &right)
{
	double denominator = right.realPart*right.realPart + right.imaginaryPart*right.imaginaryPart;
	return Complex((left.realPart*right.realPart + left.imaginaryPart*right.imaginaryPart) / denominator, \
		(-left.realPart*right.imaginaryPart + left.imaginaryPart*right.realPart) / denominator);
}

Complex& operator+=(Complex &left, const Complex &right)
{
	left.realPart += right.realPart;
	left.imaginaryPart += right.imaginaryPart;
	return left;
}

Complex& operator-=(Complex &left, const Complex &right)
{
	left.realPart -= right.realPart;
	left.imaginaryPart -= right.imaginaryPart;
	return left;
}

Complex& operator*=(Complex &left, const Complex &right)
{
	return left = Complex(left.realPart*right.realPart - left.imaginaryPart*right.imaginaryPart, \
		left.realPart*right.imaginaryPart + left.imaginaryPart*right.realPart);
}

Complex& operator/=(Complex &left, const Complex &right)
{
	double denominator = right.realPart*right.realPart + right.imaginaryPart*right.imaginaryPart;
	return left = Complex((left.realPart*right.realPart + left.imaginaryPart*right.imaginaryPart) / denominator, \
		(-left.realPart*right.imaginaryPart + left.imaginaryPart*right.realPart) / denominator);
}

std::ostream & operator <<(std::ostream &output, const Complex &complex)
{
	if (complex.imaginaryPart > 0)
		output << complex.realPart << '+' << complex.imaginaryPart << 'i';
	else if (complex.imaginaryPart == 0)
		output << complex.realPart;
	else
		output << complex.realPart << complex.imaginaryPart << 'i';
	return output;
}

std::istream & operator >>(std::istream &input, Complex &complex)
{
	char imaginaryUnit;
	return input >> complex.realPart >> complex.imaginaryPart >> imaginaryUnit;
}