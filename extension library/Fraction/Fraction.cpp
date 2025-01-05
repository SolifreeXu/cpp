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
	// 求两分数分母的最小公倍数
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// 两分数通分并求分子之和
	right.m_iMolecule *= commonNumber / right.m_iDenominator;
	right.m_iMolecule += commonNumber / left.m_iDenominator*left.m_iMolecule;
	// 存放通分后的分母
	right.m_iDenominator = commonNumber;
	// 求分数的最简式
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// 返回运算结果
	return right;
}

Fraction operator-(const Fraction &left, Fraction right) noexcept
{
	// 减数为零
	if (right.m_iMolecule == 0)
		return left;
	// 被减数为零
	else if (left.m_iMolecule == 0)
	{
		right.m_iMolecule = -right.m_iMolecule;
		return right;
	}
	// 求两分数分母的最小公倍数
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// 两分数通分并求分子之差
	right.m_iMolecule *= commonNumber / right.m_iDenominator;
	right.m_iMolecule = commonNumber / left.m_iDenominator*left.m_iMolecule - right.m_iMolecule;
	// 存放通分后的分母
	right.m_iDenominator = commonNumber;
	// 求分数的最简式
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// 返回运算结果
	return right;
}

Fraction operator*(const Fraction &left, Fraction right) noexcept
{
	if (left.m_iMolecule == 0 || right.m_iMolecule == 0)
		return 0;
	// 分子相乘
	right.m_iMolecule *= left.m_iMolecule;
	// 分母相乘
	right.m_iDenominator *= left.m_iDenominator;
	// 求分数的最简式
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// 返回运算结果
	return right;
}

Fraction operator/(const Fraction &left, Fraction right) throw(std::invalid_argument)
{
	// 除数为零
	if (right.m_iMolecule == 0)
		throw std::invalid_argument("The divisor cannot be zero!\n");
	// 被除数分子为零
	if (left.m_iMolecule == 0)
		return 0;
	// 用临时变量存放结果分子
	long lNumber = left.m_iMolecule*right.m_iDenominator;
	// 计算结果分母
	right.m_iDenominator = left.m_iDenominator*right.m_iMolecule;
	// 存放结果分子
	right.m_iMolecule = lNumber;
	// getSimplestFraction中已有此步骤
	/*if (right.m_iDenominator < 0)
	{
		right.m_iDenominator = -right.m_iDenominator;
		right.m_iMolecule = -right.m_iMolecule;
	}*/
	// 求分数的最简式
	Fraction::getSimplestFraction(right.m_iMolecule, right.m_iDenominator);
	// 返回运算结果
	return right;
}

Fraction& operator+=(Fraction &left, const Fraction &right) noexcept
{
	if (left.m_iMolecule == 0)
		return left = std::move(right);
	else if (right.m_iMolecule == 0)
		return left;
	// 求两分数分母的最小公倍数
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// 两分数通分并求分子之和
	left.m_iMolecule *= commonNumber / left.m_iDenominator;
	left.m_iMolecule += commonNumber / right.m_iDenominator*right.m_iMolecule;
	// 存放通分后的分母
	left.m_iDenominator = commonNumber;
	// 求分数的最简式
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// 返回运算结果
	return left;
}

Fraction& operator-=(Fraction &left, const Fraction &right) noexcept
{
	// 减数为零
	if (right.m_iMolecule == 0)
		return left;
	// 被减数为零
	else if (left.m_iMolecule == 0)
	{
		left = std::move(right);
		left.m_iMolecule = -left.m_iMolecule;
		return left;
	}
	// 求两分数分母的最小公倍数
	long commonNumber = Fraction::minCommonMultiple(left.m_iDenominator, right.m_iDenominator);
	// 两分数通分并求分子之差
	left.m_iMolecule *= commonNumber / left.m_iDenominator;
	left.m_iMolecule -= commonNumber / right.m_iDenominator*right.m_iMolecule;
	// 存放通分后的分母
	left.m_iDenominator = commonNumber;
	// 求分数的最简式
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// 返回运算结果
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
	// 分子相乘
	left.m_iMolecule *= right.m_iMolecule;
	// 分母相乘
	left.m_iDenominator *= right.m_iDenominator;
	// 求分数的最简式
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// 返回运算结果
	return left;
}

Fraction& operator/=(Fraction &left, const Fraction &right) throw(std::invalid_argument)
{
	// 除数为零
	if (right.m_iMolecule == 0)
		throw std::invalid_argument("The divisor cannot be zero!\n");
	// 被除数分子为零
	if (left.m_iMolecule == 0)
		return left;
	// 用临时变量存放结果分子
	long lNumber = left.m_iMolecule*right.m_iDenominator;
	// 计算结果分母
	left.m_iDenominator = left.m_iDenominator*right.m_iMolecule;
	// 存放结果分子
	left.m_iMolecule = lNumber;
	// getSimplestFraction中已有此步骤
	/*if (left.m_iDenominator < 0)
	{
		left.m_iDenominator = -left.m_iDenominator;
		left.m_iMolecule = -left.m_iMolecule;
	}*/
	// 求分数的最简式
	Fraction::getSimplestFraction(left.m_iMolecule, left.m_iDenominator);
	// 返回运算结果
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
	if (first < second)	//当first小于second时实现两值互换
	{
		first ^= second;
		second ^= first;
		first ^= second;
	}
	int temp = first%second;	//first对second取余赋给temp
	while (temp != 0)	//当temp不为0时执行循环体语句
	{
		first = second;	//将second赋给first
		second = temp;	//将temp的值赋给second
		temp = first%second;	//继续取余并赋给temp
	}
	return second;
}

long Fraction::minCommonMultiple(long first, long second)
{
	return first / maxCommonDivisor(first, second)*second;
}

void Fraction::getSimplestFraction(long &molecule, long &denominator) throw(std::invalid_argument)
{
	// 分母为零
	if (denominator == 0)
		throw std::invalid_argument("The denominator cannot be zero!\n");
	// 分子为零
	if (molecule == 0)
	{
		molecule = 0;
		denominator = 1;
		return;
	}
	// 分母小于零
	if (denominator < 0)
	{
		molecule = -molecule;
		denominator = -denominator;
	}
	long temp = maxCommonDivisor(std::abs(molecule), denominator);
	molecule /= temp;
	denominator /= temp;
}
