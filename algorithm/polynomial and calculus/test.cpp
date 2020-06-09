#include "Polynomial.hpp"

#include <iostream>

int main()
{
	using std::vector;
	vector<double> v1{ 0, 1, 0, 1 };
	vector<double> v2{ 0, 2, 3, 1 };
	using namespace form;
	auto result = add(Polynomial<double>(v1), Polynomial<double>(v2))(2);

	using std::fabs;
	using std::cout;
	if (fabs(result.first - 34) <= 0.001 \
		&& fabs(result.second - 39) <= 0.001)
		cout << "Correct!\n";
	else cout << "Wrong!\n";

	//get the value and derviative using the functional composition forms
	double point = 0.1;
	Polynomial<double> p1{ 1, 2, 3 }, p2{ 1, 2, 3 }, \
		p3{ 1, 2, 3 }, p4{ 1, 2, 3 }, p5{ 1, 2, 3 }, p6{ 1, 2, 3 };
	auto expression = divide(subtract(power(compose(p1, p2), 3), p3), add(p4, multiply(p5, p6)));
	result = expression(point);

	//get the value and derivative using the alternative ways
	using std::pow;
	double numerator = pow(p2.evaluate(p1.evaluate(point)), 3.0) - p3.evaluate(point);
	double denominator = p4.evaluate(point) + p5.evaluate(point) * p6.evaluate(point);
	double value = numerator / denominator;

	double delta = 0.0000001;
	point += delta;
	numerator = pow(p2.evaluate(p1.evaluate(point)), 3.0) - p3.evaluate(point);
	denominator = p4.evaluate(point) + p5.evaluate(point) * p6.evaluate(point);
	double gradient = (numerator / denominator - value) / delta;

	if (fabs(result.first - value) <= 0.001 \
		&& fabs(result.second - gradient) <= 0.001)
		cout << "Correct!\n";
	else cout << "Wrong!\n";
	return 0;
}
