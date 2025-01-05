#pragma once

#include <iostream>
#include <exception>

class Fraction
{
	long m_iMolecule;
	long m_iDenominator;	// 分母总是正整数

	friend std::ostream& operator<<(std::ostream &os, const Fraction &fraction);
	friend std::istream& operator>>(std::istream &is, Fraction &fraction);
public:
	Fraction(long molecule = 0, long denominator = 1) throw(std::invalid_argument);
	Fraction(const Fraction &fraction) \
		:m_iMolecule(fraction.m_iMolecule), m_iDenominator(fraction.m_iDenominator){}
	friend Fraction operator+(const Fraction &left, Fraction right) noexcept;
	friend Fraction operator-(const Fraction &left, Fraction right) noexcept;
	friend Fraction operator*(const Fraction &left, Fraction right) noexcept;
	friend Fraction operator/(const Fraction &left, Fraction right) throw(std::invalid_argument);
	friend Fraction& operator+=(Fraction &left, const Fraction &right) noexcept;
	friend Fraction& operator-=(Fraction &left, const Fraction &right) noexcept;
	friend Fraction& operator*=(Fraction &left, const Fraction &right) noexcept;
	friend Fraction& operator/=(Fraction &left, const Fraction &right) throw(std::invalid_argument);
	friend bool operator>(const Fraction &left, const Fraction &right);
	friend bool operator<(const Fraction &left, const Fraction &right);
	friend bool operator==(const Fraction &left, const Fraction &right);
	friend bool operator>=(const Fraction &left, const Fraction &right);
	friend bool operator<=(const Fraction &left, const Fraction &right);
	//operator double(){ return (double)m_iMolecule / m_iDenominator; }
	static long maxCommonDivisor(long first, long second);
	static long minCommonMultiple(long first, long second);
	static void getSimplestFraction(long &molecule, long &denominator) throw(std::invalid_argument);
};
