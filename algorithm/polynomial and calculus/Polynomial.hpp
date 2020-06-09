#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <cstddef>
#include <cmath>

#include <utility>
#include <tuple>
#include <vector>

template <typename Type>
class Polynomial
{
	static constexpr auto sort = [](auto left, auto right)
	{
		if (left < right)
			return std::make_tuple(left, right);
		else
			return std::make_tuple(right, left);
	};
	std::vector<Type> coefficients;
public:
	/* Default constructor */
	Polynomial() {}

	/* Constructor for a polynomial that's a constant number */
	Polynomial(Type constant)
	{
		coefficients.push_back(constant);
	}

	/* Copying constructor with lvalue reference */
	Polynomial(const Polynomial<Type>& another)
		: coefficients(another.coefficients) {}

	/* Moving constructor with rvalue reference */
	Polynomial(Polynomial<Type>&& another)
		: coefficients(std::move(another.coefficients)) {}

	/* Given an lvalue reference of vector of coefficients */
	Polynomial(const std::vector<Type>& coefficients)
		: coefficients(coefficients) {}

	/* Given an rvalue reference of a vector of coefficients */
	Polynomial(std::vector<Type>&& coefficients)
		: coefficients(std::move(coefficients)) {}

	/* Given an array of coefficients with size of the array */
	Polynomial(const Type *coefficients, size_t number)
		: coefficients(coefficients, coefficients + number) {}

	/* Given an initializer_list */
	Polynomial(const std::initializer_list<Type>& coefficients)
		: coefficients(coefficients) {}

	/* Virtual destructor */
	virtual ~Polynomial() {}

	/* Copying assignment operator with lvalue reference */
	Polynomial<Type>& operator=(const Polynomial<Type>& another)
	{
		coefficients = another.coefficients;
		return *this;
	}

	/* Moving assignment operator with rvalue reference */
	Polynomial<Type>& operator=(Polynomial<Type>&& another)
	{
		coefficients = std::move(another.coefficients);
		return *this;
	}

	/* Assignment operator with initializer_list */
	Polynomial<Type>& operator=(const std::initializer_list<Type>& coefficients)
	{
		this->coefficients = coefficients;
		return *this;
	}

	/*
	 * Below are operations of polynomials.
	 * They are quite self-explanatory.
	 */

	Polynomial<Type> operator+(const Polynomial<Type>& rhs) const
	{
		auto&& lcs = coefficients;
		auto lsize = lcs.size();
		auto&& rcs = rhs.coefficients;
		auto rsize = rcs.size();
		auto [min_size, max_size] = sort(lsize, rsize);

		decltype(coefficients) coefficients;
		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients.push_back(lcs[index] + rcs[index]);
		while (index < lsize)
			coefficients.push_back(lcs[index++]);
		while (index < rsize)
			coefficients.push_back(rcs[index++]);
		return Polynomial(std::move(coefficients));
	}

	Polynomial<Type> operator-(const Polynomial<Type>& rhs) const
	{
		auto&& lcs = coefficients;
		auto lsize = lcs.size();
		auto&& rcs = rhs.coefficients;
		auto rsize = rcs.size();
		auto [min_size, max_size] = sort(lsize, rsize);
		
		decltype(coefficients) coefficients;
		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients.push_back(lcs[index] - rcs[index]);
		while (index < lsize)
			coefficients.push_back(lcs[index++]);
		while (index < rsize)
			coefficients.push_back(-rcs[index++]);
		return Polynomial(std::move(coefficients));
	}

	Polynomial<Type> operator*(const Polynomial<Type>& rhs) const
	{
		auto&& lcs = coefficients;
		auto lsize = lcs.size();
		auto&& rcs = rhs.coefficients;
		auto rsize = rcs.size();
		auto max_size = lsize + rsize;
		max_size = max_size > 0 ? max_size - 1 : 0;

		std::vector<Type> coefficients(max_size);
		for (decltype(lsize) i = 0; i < lsize; ++i)
			for (decltype(rsize) j = 0; j < rsize; ++j)
				coefficients[i + j] += lcs[i] * rcs[j];
		return Polynomial(coefficients);
	}
	
	Polynomial<Type>& operator+=(const Polynomial<Type>& rhs)
	{
		auto&& rcs = rhs.coefficients;
		auto rsize = rcs.size();
		auto [min_size, max_size] = sort(coefficients.size(), rsize);

		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients[index] += rcs[index];
		while (index < rsize)
			coefficients.push_back(rcs[index++]);
		return *this;
	}

	Polynomial<Type>& operator-=(const Polynomial<Type>& rhs)
	{
		auto&& rcs = rhs.coefficients;
		auto rsize = rcs.size();
		auto [min_size, max_size] = sort(coefficients.size(), rsize);

		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients[index] -= rcs[index];
		while (index < rsize)
			coefficients.push_back(-rcs[index++]);
		return *this;
	}

	Polynomial<Type>& operator*=(const Polynomial<Type>& rhs)
	{
		return *this = *this * rhs;
	}

	/*
	 * This function evaluates the polynomial at "parameter",
	 * and returns the value of evaluation.
	 * For example, evaluating x+3 at parameter=7 gives 10.
	 */
	Type evaluate(Type parameter) const
	{
		Type sum = 0;
		auto size = coefficients.size();
		for (decltype(size) index = 0; index < size; ++index)
			sum += std::pow(parameter, index)*coefficients[index];
		return sum;
	}

	/*
	 * This function takes the derivative of the polynomial,
	 * and returns a new polynomial of its derivative.
	 * For example, calling this function for x^2
	 * will return a polynomial 2x. 求导
	 */
	Polynomial<Type> derivative() const
	{
		auto size = coefficients.size();
		decltype(coefficients) coefficients;
		coefficients.reserve(size > 0 ? size - 1 : 0);

		for (decltype(size) index = 1; index < size; ++index)
			coefficients.push_back(index * this->coefficients[index]);
		return Polynomial(std::move(coefficients));
	}

	/*
	* This function returns a pair of:
	* 1. the value (evaluation) of the polynomial at "parameter".
	* 2. the value of the first-order derivative of the polynomial at "parameter".
	* For example, calling this operator for x^2-2x at parameter=4
	* will return (8, 6). 返回原函数值以及一阶导值
	*/
	std::pair<Type, Type> operator()(Type parameter) const
	{
		Type first = evaluate(parameter);
		auto temp = derivative();
		Type second = temp.evaluate(parameter);
		return std::make_pair(first, second);
	}
};

/* For all lambda functions below, if you are not clear how they work, please refer to the given example for details. */
namespace polynomial {
	/*
	 * Return a lambda function that takes one parameter (m) and returns a pair<double, double> that:
	 * has (f1+f2)(m) as first, and (d(f1+f2)/dx)(m) as second.
	 */
	auto add = [](auto f1, auto f2) {
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
	auto subtract = [](auto f1, auto f2) {
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
	auto multiply = [](auto f1, auto f2) {
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
	auto divide = [](auto f1, auto f2) {
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
	auto compose = [](auto f1, auto f2) {
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
	auto power = [](auto f, auto exp) {
		return [=](auto m) {
			auto p = f(m);
			using std::pow;
			auto first = pow(p.first, exp);
			auto second = exp * pow(p.first, exp - 1) * p.second;
			return std::make_pair(first, second);
		};
	};
}

#endif
