#include "Fraction.h"

#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

Fraction generate(long upperLimit)
{
	static std::default_random_engine random(time(NULL));
	static std::uniform_int_distribution<long> distribution(0, upperLimit);
	long molecule = distribution(random);
	long denominator = distribution(random) + 1;
	if (distribution(random) % 2)
		molecule = -molecule;
	return Fraction(molecule, denominator);
}

using std::cout;
using std::move;

int main()
{
	Fraction left = move(generate(10000)), right = move(generate(10000)), buff, result;
	std::random_device builder;
	switch (builder() % 4)
	{
	case 0:
		buff = move(left + right);
		cout << left << " + ";
		if (right < 0)
			cout << '(' << right << ')';
		else
			cout << right;
		cout << " = ";
		break;
	case 1:
		buff = move(left - right);
		cout << left << " - ";
		if (right < 0)
			cout << '(' << right << ')';
		else
			cout << right;
		cout << " = ";
		break;
	case 2:
		buff = move(left*right);
		cout << left << " * ";
		if (right < 0)
			cout << '(' << right << ')';
		else
			cout << right;
		cout << " = ";
		break;
	case 3:
		buff = move(left / right);
		cout << left << " / ";
		if (right < 0)
			cout << '(' << right << ')';
		else
			cout << right;
		cout << " = ";
		break;
	}
	std::cin >> result;
	if (result == buff)
		cout << "正确！\n";
	else
		cout << "错误！\n";

	return 0;
}
