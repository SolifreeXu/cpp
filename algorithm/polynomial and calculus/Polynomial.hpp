#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <cmath>
#include <utility>
#include <algorithm>

#include <functional>
#include <utility>
#include <cstddef>
#include <initializer_list>
#include <tuple>
#include <vector>

#include <type_traits>

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
		auto&& lcoeffs = this->coefficients;
		auto lsize = lcoeffs.size();
		auto&& rcoeffs = rhs.coefficients;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(lsize, rsize);

		typename std::remove_const<std::remove_reference<decltype(lcoeffs)>::type>::type coefficients;
		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients.push_back(lcoeffs[index] + rcoeffs[index]);
		while (index < lsize)
			coefficients.push_back(lcoeffs[index++]);
		while (index < rsize)
			coefficients.push_back(rcoeffs[index++]);
		return Polynomial(std::move(coefficients));
	}

	Polynomial<Type> operator-(const Polynomial<Type>& rhs) const
	{
		auto&& lcoeffs = this->coefficients;
		auto lsize = lcoeffs.size();
		auto&& rcoeffs = rhs.coefficients;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(lsize, rsize);
		
		typename std::remove_const<std::remove_reference<decltype(lcoeffs)>::type>::type coefficients;
		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients.push_back(lcoeffs[index] - rcoeffs[index]);
		while (index < lsize)
			coefficients.push_back(lcoeffs[index++]);
		while (index < rsize)
			coefficients.push_back(-rcoeffs[index++]);
		return Polynomial(std::move(coefficients));
	}

	Polynomial<Type> operator*(const Polynomial<Type>& rhs) const
	{
		auto&& lcoeffs = this->coefficients;
		auto lsize = lcoeffs.size();
		auto&& rcoeffs = rhs.coefficients;
		auto rsize = rcoeffs.size();
		auto max_size = std::max(lsize + rsize - 1, static_cast<decltype(lsize)>(0));

		std::vector<double> coefficients(max_size);
		for (decltype(lsize) i = 0; i < lsize; ++i)
			for (decltype(rsize) j = 0; j < rsize; ++j)
				coefficients[i + j] += lcoeffs[i] * rcoeffs[j];
		return Polynomial(coefficients);
	}
	
	Polynomial<Type>& operator+=(const Polynomial<Type>& rhs)
	{
		auto&& rcoeffs = rhs.coefficients;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(coefficients.size(), rsize);

		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients[index] += rcoeffs[index];
		while (index < rsize)
			coefficients.push_back(rcoeffs[index++]);
		return *this;
	}

	Polynomial<Type>& operator-=(const Polynomial<Type>& rhs)
	{
		auto&& rcoeffs = rhs.coefficients;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(coefficients.size(), rsize);

		coefficients.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coefficients[index] -= rcoeffs[index];
		while (index < rsize)
			coefficients.push_back(-rcoeffs[index++]);
		return *this;
	}

	Polynomial<Type>& operator*=(const Polynomial<Type>& rhs)
	{
		return *this = *this * rhs;
	}

	/*
	 * This function evaluates the polynomial at "param",
	 * and returns the value of evaluation.
	 * For example, evaluating x+3 at param=7 gives 10.
	 */
	Type evaluate(Type param) const
	{
		Type sum = 0;
		auto size = coefficients.size();
		for (decltype(size) index = 0; index < size; ++index)
			sum += std::pow(param, index)*coefficients[index];
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
		decltype(this->coefficients) coefficients;
		auto size = this->coefficients.size();
		for (decltype(size) index = 1; index < size; ++index)
			coefficients.push_back(index * this->coefficients[index]);
		return Polynomial(std::move(coefficients));
	}

	/*
	* This function returns a pair of:
	* 1. the value (evaluation) of the polynomial at "param".
	* 2. the value of the first-order derivative of the polynomial at "param".
	* For example, calling this operator for x^2-2x at "param"=4
	* will return (8, 6). 返回原函数的值以及一阶导的值
	*/
	std::pair<Type, Type> operator()(Type param) const
	{
		Type left = this->evaluate(param);
		auto temp = this->derivative();
		Type right = temp.evaluate(param);
		return std::make_pair(left, right);
	}
private:
	std::vector<Type> coefficients;
};

#endif
