#include "Polynomial.hpp"
#include "functional_form.hpp"

#include <cmath>
#include <iostream>

int main()
{
	std::vector<double> v1{ 0, 1, 0, 1 };
	std::vector<double> v2{ 0, 2, 3, 1 };
	Polynomial<double> pv1(v1), pv2(v2);
	auto result = evalSum(pv1, pv2)(2);
	if (fabs(result.first - 34) <= 0.001 \
		&& fabs(result.second - 39) <= 0.001)
		std::cout << "Success!\n";
	else std::cout << "Error!\n";

	double point = 0.1;

	//get the value and derviative using the functional composition forms
	Polynomial<double> p1{ 1, 2, 3 }, p2{ 1, 2, 3 }, \
		p3{ 1, 2, 3 }, p4{ 1, 2, 3 }, p5{ 1, 2, 3 }, p6{ 1, 2, 3 };
	auto g = evalDiv(evalSub(evalPow(evalComp(p1, p2), 3), p3), evalSum(p4, evalMul(p5, p6)));
	result = g(point);

	//get the value and derivative using the alternative ways
	double epsilon = 0.0000001;
	double numerator = pow(p2.evaluate(p1.evaluate(point)), 3.0) - p3.evaluate(point);
	double denominator = p4.evaluate(point) + p5.evaluate(point) * p6.evaluate(point);
	double value = numerator / denominator;
	point += epsilon;
	numerator = pow(p2.evaluate(p1.evaluate(point)), 3.0) - p3.evaluate(point);
	denominator = p4.evaluate(point) + p5.evaluate(point) * p6.evaluate(point);
	double grad = (numerator / denominator - value) / epsilon;
	if (fabs(result.first - value) <= 0.001 \
		&& fabs(result.second - grad) <= 0.001)
		std::cout << "Success!\n";
	else std::cout << "Error!\n";
}
