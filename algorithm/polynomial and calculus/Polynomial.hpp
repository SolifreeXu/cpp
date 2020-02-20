#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>
#include <tuple>
#include <type_traits>

template<typename T>
class Polynomial
{
	static inline auto sort = [](auto left, auto right)
	{
		if (left < right)
			return std::make_tuple(left, right);
		else
			return std::make_tuple(right, left);
	};
public:
	/* Default constructor */
	Polynomial()
	{

	}

	/* Constructor for a polynomial that's a constant number */
	Polynomial(T constant)
	{
		coeffs.push_back(constant);
	}

	/* Copying constructor with lvalue reference */
	Polynomial(const Polynomial<T>& another)
	{
		coeffs = another.coeffs;
	}

	/* Moving constructor with rvalue reference */
	Polynomial(Polynomial<T>&& another)
	{
		coeffs = std::move(another.coeffs);
	}

	/* Given an lvalue reference of vector of coefficients */
	Polynomial(const std::vector<T>& coeffs)
	{
		this->coeffs = coeffs;
	}

	/* Given an rvalue reference of a vector of coefficients */
	Polynomial(std::vector<T>&& coeffs)
	{
		this->coeffs = std::move(coeffs);
	}

	/* Given an array of coefficients with size of the array */
	Polynomial(const T *coeffs, size_t num)
	{
		this->coeffs.reserve(num);
		for (decltype(num) index = 0; index < num; ++index)
			this->coeffs.push_back(coeffs[index]);
	}

	/* Given an initializer_list */
	Polynomial(const std::initializer_list<T>& coeffs)
	{
		this->coeffs = coeffs;
	}

	/* Virtual destructor */
	virtual ~Polynomial()
	{
		
	}

	/* Copying assignment operator with lvalue reference */
	Polynomial<T>& operator=(const Polynomial<T>& another)
	{
		coeffs = another.coeffs;
		return *this;
	}

	/* Moving assignment operator with rvalue reference */
	Polynomial<T>& operator=(Polynomial<T>&& another)
	{
		coeffs = std::move(another.coeffs);
		return *this;
	}

	/* Assignment operator with initializer_list */
	Polynomial<T>& operator=(const std::initializer_list<T>& coeffs)
	{
		this->coeffs = coeffs;
		return *this;
	}

	/*
	 * Below are operations of polynomials.
	 * They are quite self-explanatory.
	 */

	Polynomial<T> operator+(const Polynomial<T>& rhs) const
	{
		auto&& lcoeffs = this->coeffs;
		auto lsize = lcoeffs.size();
		auto&& rcoeffs = rhs.coeffs;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(lsize, rsize);

		typename std::remove_const<std::remove_reference<decltype(lcoeffs)>::type>::type coeffs;
		coeffs.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coeffs.push_back(lcoeffs[index] + rcoeffs[index]);
		while (index < lsize)
			coeffs.push_back(lcoeffs[index++]);
		while (index < rsize)
			coeffs.push_back(rcoeffs[index++]);
		return Polynomial(std::move(coeffs));
	}

	Polynomial<T> operator-(const Polynomial<T>& rhs) const
	{
		auto&& lcoeffs = this->coeffs;
		auto lsize = lcoeffs.size();
		auto&& rcoeffs = rhs.coeffs;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(lsize, rsize);
		
		typename std::remove_const<std::remove_reference<decltype(lcoeffs)>::type>::type coeffs;
		coeffs.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coeffs.push_back(lcoeffs[index] - rcoeffs[index]);
		while (index < lsize)
			coeffs.push_back(lcoeffs[index++]);
		while (index < rsize)
			coeffs.push_back(-rcoeffs[index++]);
		return Polynomial(std::move(coeffs));
	}

	Polynomial<T> operator*(const Polynomial<T>& rhs) const
	{
		auto&& lcoeffs = this->coeffs;
		auto lsize = lcoeffs.size();
		auto&& rcoeffs = rhs.coeffs;
		auto rsize = rcoeffs.size();
		auto max_size = std::max(lsize + rsize - 1, static_cast<decltype(lsize)>(0));

		std::vector<double> coeffs(max_size);
		for (decltype(lsize) i = 0; i < lsize; ++i)
			for (decltype(rsize) j = 0; j < rsize; ++j)
				coeffs[i + j] += lcoeffs[i] * rcoeffs[j];
		return Polynomial(coeffs);
	}
	
	Polynomial<T>& operator+=(const Polynomial<T>& rhs)
	{
		auto&& rcoeffs = rhs.coeffs;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(coeffs.size(), rsize);

		coeffs.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coeffs[index] += rcoeffs[index];
		while (index < rsize)
			coeffs.push_back(rcoeffs[index++]);
		return *this;
	}

	Polynomial<T>& operator-=(const Polynomial<T>& rhs)
	{
		auto&& rcoeffs = rhs.coeffs;
		auto rsize = rcoeffs.size();
		auto [min_size, max_size] = sort(coeffs.size(), rsize);

		coeffs.reserve(max_size);
		decltype(min_size) index = 0;
		for (; index < min_size; ++index)
			coeffs[index] -= rcoeffs[index];
		while (index < rsize)
			coeffs.push_back(-rcoeffs[index++]);
		return *this;
	}

	Polynomial<T>& operator*=(const Polynomial<T>& rhs)
	{
		return *this = *this * rhs;
	}

	/*
	 * This function evaluates the polynomial at "param",
	 * and returns the value of evaluation.
	 * For example, evaluating x+3 at param=7 gives 10.
	 */
	T evaluate(T param) const
	{
		T sum = 0;
		auto size = coeffs.size();
		for (decltype(size) index = 0; index < size; ++index)
			sum += std::pow(param, index)*coeffs[index];
		return sum;
	}

	/*
	 * This function takes the derivative of the polynomial,
	 * and returns a new polynomial of its derivative.
	 * For example, calling this function for x^2
	 * will return a polynomial 2x. 求导
	 */
	Polynomial<T> derivative() const
	{
		decltype(this->coeffs) coeffs;
		auto size = this->coeffs.size();
		for (decltype(size) index = 1; index < size; ++index)
			coeffs.push_back(index * this->coeffs[index]);
		return Polynomial(std::move(coeffs));
	}

	/*
	* This function returns a pair of:
	* 1. the value (evaluation) of the polynomial at "param".
	* 2. the value of the first-order derivative of the polynomial at "param".
	* For example, calling this operator for x^2-2x at "param"=4
	* will return (8, 6). 返回原函数的值以及一阶导的值
	*/
	std::pair<T, T> operator()(T param) const
	{
		T left = this->evaluate(param);
		auto temp = this->derivative();
		T right = temp.evaluate(param);
		return std::make_pair(left, right);
	}
private:
	std::vector<T> coeffs;
};

#endif
