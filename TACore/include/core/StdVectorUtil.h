#ifndef STDVECTOR_UTIL_H
#define STDVECTOR_UTIL_H

#include <algorithm>
#include <numeric>

template <class T>
std::ostream& operator<< (std::ostream& ofs, const std::vector<T>& vec) {
	ofs << vec.size() << "\n";
	for (size_t i = 0; i < vec.size(); i++)
	{
		ofs << vec[i] << ", ";
	}
	ofs << "\n";
	return ofs;
}

template <class T>
std::vector<T> operator+(const std::vector<T>& v1, const std::vector<T>& v2)
{
	size_t minSize = std::min(v1.size(), v2.size());
	std::vector<T> ret(minSize);
	for (size_t i = 0; i < minSize; i++)
	{
		ret[i] = v1[i] + v2[i];
	}
	return ret;
}

template <class T>
std::vector<T> operator-(const std::vector<T>& v1, const std::vector<T>& v2)
{
	size_t minSize = std::min(v1.size(), v2.size());
	std::vector<T> ret(minSize);
	for (size_t i = 0; i < minSize; i++)
	{
		ret[i] = v1[i] - v2[i];
	}
	return ret;
}

template <class T>
std::vector<T> operator*(const std::vector<T>& v1, const std::vector<T>& v2)
{
	size_t minSize = std::min(v1.size(), v2.size());
	std::vector<T> ret(minSize);
	for (size_t i = 0; i < minSize; i++)
	{
		ret[i] = v1[i] * v2[i];
	}
	return ret;
}

template <class T>
std::vector<T> operator/(const std::vector<T>& v1, const std::vector<T>& v2)
{
	size_t minSize = std::min(v1.size(), v2.size());
	std::vector<T> ret(minSize);
	for (size_t i = 0; i < minSize; i++)
	{
		ret[i] = v1[i] / v2[i];
	}
	return ret;
}

template <class T>
std::vector<T> operator+(const std::vector<T>& v, const T& val)
{
	std::vector<T> ret(v.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		ret[i] = v[i] + val;
	}
	return ret;
}

template <class T>
std::vector<T> operator-(const std::vector<T>& v, const T& val)
{
	std::vector<T> ret(v.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		ret[i] = v[i] - val;
	}
	return ret;
}

template <class T>
std::vector<T> operator*(const std::vector<T>& v, const T& val)
{
	std::vector<T> ret(v.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		ret[i] = v[i] * val;
	}
	return ret;
}

template <class T>
std::vector<T> operator/(const std::vector<T>& v, const T& val)
{
	std::vector<T> ret(v.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		ret[i] = v[i] / val;
	}
	return ret;
}

template <class T> 
T StdVecSumElems(const std::vector<T>& v)
{
	return std::accumulate(v.begin(), v.end(), T(0));
}

template <class T>
T StdVecL2Norm(const std::vector<T>& v1, const std::vector<T>& v2)
{
	std::vector<T> diff = v1 - v2;
	std::vector<T> diffSquare = diff * diff;
	return sqrt(StdVecSumElems(diffSquare));
}

#endif