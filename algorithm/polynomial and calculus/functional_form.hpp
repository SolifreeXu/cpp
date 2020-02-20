#ifndef FUNCTIONAL_FORM_HPP
#define FUNCTIONAL_FORM_HPP

#include "Polynomial.hpp"
/* For all lambda functions below, if you are not clear how they work, please refer to the given example for details. */

/*
 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
 * has (f1+f2)(m) as first, and (d(f1+f2)/dx)(m) as second.
 */
auto evalSum = [](auto f1, auto f2) {
	return [=](auto m) {
		auto p1 = f1(m);
		auto p2 = f2(m);
		auto first = p1.first + p2.first;
		auto second = p1.second + p2.second;
		return std::make_pair(first, second);
	};
};

/*
 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
 * has (f1-f2)(m) as first, and (d(f1-f2)/dx)(m) as second.
 */
auto evalSub = [](auto f1, auto f2) {
	return [=](auto m) {
		auto p1 = f1(m);
		auto p2 = f2(m);
		auto first = p1.first - p2.first;
		auto second = p1.second - p2.second;
		return std::make_pair(first, second);
	};
};

/*
 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
 * has (f1*f2)(m) as first, and (d(f1*f2)/dx)(m) as second.
 */
auto evalMul = [](auto f1, auto f2) {
	return [=](auto m) {
		auto p1 = f1(m);
		auto p2 = f2(m);
		auto first = p1.first*p2.first;
		auto second = p1.second*p2.first + p1.first*p2.second;
		return std::make_pair(first, second);
	};
};

/*
 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
 * has (f1/f2)(m) as first, and (d(f1/f2)/dx)(m) as second.
 */
auto evalDiv = [](auto f1, auto f2) {
	return [=](auto m) {
		auto p1 = f1(m);
		auto p2 = f2(m);
		auto first = p1.first / p2.first;
		auto numerator = p1.second*p2.first - p1.first*p2.second;
		auto denominator = p2.first*p2.first;
		auto second = numerator / denominator;
		return std::make_pair(first, second);
	};
};

/*
 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
 * has (f1(f2))(m) as first, and (d(f1(f2))/dx)(m) as second.
 */
auto evalComp = [](auto f1, auto f2) {
	return [=](auto m) {
		auto p1 = f1(m);
		auto p2 = f2(p1.first);
		auto first = p2.first;
		auto second = p2.second*p1.second;
		return std::make_pair(first, second);
	};
};

/*
 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
 * has [(f)(m)]^exp as first, and (d(f^exp)/dx)(m) as second.
 */
auto evalPow = [](auto f, int exp) {
	return [=](auto m) {
		auto p = f(m);
		auto first = pow(p.first, exp);
		auto second = exp * pow(p.first, exp - 1) * p.second;
		return std::make_pair(first, second);
	};
};

#endif
