#include "Fraction.h"

using std::ostream;
using std::istream;

Fraction::Fraction(long molecule, long denominator) throw(std::invalid_argument)
{
	getSimplestFraction(molecule, denominator);
	this->m_iMolecule = molecule;
	this->m_iDenominator = denominator;
}

ostream& operator<<(ostream &os, const Fraction &fraction)
{
	os << fraction.m_iMolecule << '/' << fraction.m_iDenominator;
	return os;
}

istream& operator>>(istream &is, Fraction &fraction)
{
	char cOperator;
	is >> fraction.m_iMolecule >> operator >> fraction.m_iDenominator;
	return is;
}

#include <cmath>

Fraction operator+(const Fraction &left, Fraction right) noexcept
{
	if (left.m_iMolecule == 0)
		return right;
	else if (right.m_iMolecule == 0)
		return left;
	// ����������ĸ����С������
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// ������ͨ�ֲ������֮��
	right.m_iMolecule *= commonNumber / right.m_iDenominator;
	right.m_iMolecule += commonNumber / left.m_iDenominator*left.m_iMolecule;
	// ���ͨ�ֺ�ķ�ĸ
	right.m_iDenominator = commonNumber;
	// ����������ʽ
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// ����������
	return right;
}

Fraction operator-(const Fraction &left, Fraction right) noexcept
{
	// ����Ϊ��
	if (right.m_iMolecule == 0)
		return left;
	// ������Ϊ��
	else if (left.m_iMolecule == 0)
	{
		right.m_iMolecule = -right.m_iMolecule;
		return right;
	}
	// ����������ĸ����С������
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// ������ͨ�ֲ������֮��
	right.m_iMolecule *= commonNumber / right.m_iDenominator;
	right.m_iMolecule = commonNumber / left.m_iDenominator*left.m_iMolecule - right.m_iMolecule;
	// ���ͨ�ֺ�ķ�ĸ
	right.m_iDenominator = commonNumber;
	// ����������ʽ
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// ����������
	return right;
}

Fraction operator*(const Fraction &left, Fraction right) noexcept
{
	if (left.m_iMolecule == 0 || right.m_iMolecule == 0)
		return 0;
	// �������
	right.m_iMolecule *= left.m_iMolecule;
	// ��ĸ���
	right.m_iDenominator *= left.m_iDenominator;
	// ����������ʽ
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// ����������
	return right;
}

Fraction operator/(const Fraction &left, Fraction right) throw(std::invalid_argument)
{
	// ����Ϊ��
	if (right.m_iMolecule == 0)
		throw std::invalid_argument("The divisor cannot be zero!\n");
	// ����������Ϊ��
	if (left.m_iMolecule == 0)
		return 0;
	// ����ʱ������Ž������
	long lNumber = left.m_iMolecule*right.m_iDenominator;
	// ��������ĸ
	right.m_iDenominator = left.m_iDenominator*right.m_iMolecule;
	// ��Ž������
	right.m_iMolecule = lNumber;
	// getSimplestFraction�����д˲���
	/*if (right.m_iDenominator < 0)
	{
		right.m_iDenominator = -right.m_iDenominator;
		right.m_iMolecule = -right.m_iMolecule;
	}*/
	// ����������ʽ
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// ����������
	return right;
}

Fraction& operator+=(Fraction &left, const Fraction &right) noexcept
{
	if (left.m_iMolecule == 0)
		return left = std::move(right);
	else if (right.m_iMolecule == 0)
		return left;
	// ����������ĸ����С������
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// ������ͨ�ֲ������֮��
	left.m_iMolecule *= commonNumber / left.m_iDenominator;
	left.m_iMolecule += commonNumber / right.m_iDenominator*right.m_iMolecule;
	// ���ͨ�ֺ�ķ�ĸ
	left.m_iDenominator = commonNumber;
	// ����������ʽ
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// ����������
	return left;
}

Fraction& operator-=(Fraction &left, const Fraction &right) noexcept
{
	// ����Ϊ��
	if (right.m_iMolecule == 0)
		return left;
	// ������Ϊ��
	else if (left.m_iMolecule == 0)
	{
		left = std::move(right);
		left.m_iMolecule = -left.m_iMolecule;
		return left;
	}
	// ����������ĸ����С������
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// ������ͨ�ֲ������֮��
	left.m_iMolecule *= commonNumber / left.m_iDenominator;
	left.m_iMolecule -= commonNumber / right.m_iDenominator*right.m_iMolecule;
	// ���ͨ�ֺ�ķ�ĸ
	left.m_iDenominator = commonNumber;
	// ����������ʽ
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// ����������
	return left;
}

Fraction& operator*=(Fraction &left, const Fraction &right) noexcept
{
	if (left.m_iMolecule == 0 || right.m_iMolecule == 0)
	{
		left.m_iMolecule = 0;
		left.m_iDenominator = 1;
		return left;
	}
	// �������
	left.m_iMolecule *= right.m_iMolecule;
	// ��ĸ���
	left.m_iDenominator *= right.m_iDenominator;
	// ����������ʽ
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// ����������
	return left;
}

Fraction& operator/=(Fraction &left, const Fraction &right) throw(std::invalid_argument)
{
	// ����Ϊ��
	if (right.m_iMolecule == 0)
		throw std::invalid_argument("The divisor cannot be zero!\n");
	// ����������Ϊ��
	if (left.m_iMolecule == 0)
		return left;
	// ����ʱ������Ž������
	long lNumber = left.m_iMolecule*right.m_iDenominator;
	// ��������ĸ
	left.m_iDenominator = left.m_iDenominator*right.m_iMolecule;
	// ��Ž������
	left.m_iMolecule = lNumber;
	// getSimplestFraction�����д˲���
	/*if (left.m_iDenominator < 0)
	{
		left.m_iDenominator = -left.m_iDenominator;
		left.m_iMolecule = -left.m_iMolecule;
	}*/
	// ����������ʽ
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// ����������
	return left;
}

bool operator>(const Fraction &left, const Fraction &right)
{
	return (left - right).m_iMolecule > 0;
}

bool operator<(const Fraction &left, const Fraction &right)
{
	return (left - right).m_iMolecule < 0;
}

bool operator==(const Fraction &left, const Fraction &right)
{
	return (left - right).m_iMolecule == 0;
}

bool operator>=(const Fraction &left, const Fraction &right)
{
	return (left - right).m_iMolecule >= 0;
}

bool operator<=(const Fraction &left, const Fraction &right)
{
	return (left - right).m_iMolecule <= 0;
}

long Fraction::maxCommonDivisor(long first, long second)
{
	if (first == 0 || second == 0)
		return -1;
	if (first < second)	//��firstС��secondʱʵ����ֵ����
	{
		first ^= second;
		second ^= first;
		first ^= second;
	}
	int temp = first%second;	//first��secondȡ�ำ��temp
	while (temp != 0)	//��temp��Ϊ0ʱִ��ѭ�������
	{
		first = second;	//��second����first
		second = temp;	//��temp��ֵ����second
		temp = first%second;	//����ȡ�ಢ����temp
	}
	return second;
}

long Fraction::minCommonMultiple(long first, long second)
{
	return first / maxCommonDivisor(first, second)*second;
}

void Fraction::getSimplestFraction(long &molecule, long &denominator) throw(std::invalid_argument)
{
	// ��ĸΪ��
	if (denominator == 0)
		throw std::invalid_argument("The denominator cannot be zero!\n");
	// ����Ϊ��
	if (molecule == 0)
	{
		molecule = 0;
		denominator = 1;
		return;
	}
	// ��ĸС����
	if (denominator < 0)
	{
		molecule = -molecule;
		denominator = -denominator;
	}
	long temp = maxCommonDivisor(std::abs(molecule), denominator);
	molecule /= temp;
	denominator /= temp;
}
