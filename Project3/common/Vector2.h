#pragma once
template <class T> class Vector2Temple
{
public:
	Vector2Temple();
	Vector2Temple(T x, T y);
	~Vector2Temple();
	T x;
	T y;
	
	//ë„ì¸ââéZéq
	Vector2Temple& operator = (const Vector2Temple& vec);
	
	// ìYÇ¶éöââéZéq
	T& operator [] (int num);
	
	// íPçÄââéZéq
	Vector2Temple& operator += (const Vector2Temple & vec);
	Vector2Temple& operator -= (const Vector2Temple& vec);
	Vector2Temple& operator *= (T k);
	Vector2Temple& operator /= (T k);
	Vector2Temple operator + ()const;
	Vector2Temple operator - ()const;

	Vector2Temple& operator += (const T u);

	// î‰ärââéZéq
	bool operator == (const Vector2Temple& vec) const;
	bool operator != (const Vector2Temple& vec) const;
	bool operator <= (const Vector2Temple& vec) const;
	bool operator < (const Vector2Temple& vec) const;
	bool operator >= (const Vector2Temple& vec) const;
	bool operator > (const Vector2Temple& vec) const;
};
template <class T>
Vector2Temple<T> operator + (const Vector2Temple<T>& vec, T u);
template <class T>
Vector2Temple<T> operator - (const Vector2Temple<T>& vec, T u);
template <class T>
Vector2Temple<T> operator * (const Vector2Temple<T>& vec, T u);
template <class T>
Vector2Temple<T> operator / (const Vector2Temple<T>& vec, T u);
template <class T>
Vector2Temple<T> operator % (const Vector2Temple<T>& vec, T u);
template <class T>
Vector2Temple<T> operator * (T u, const Vector2Temple<T>& vec);
template <class T>
Vector2Temple<T> operator + (const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb);
template <class T>
Vector2Temple<T> operator - (const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb);
template <class T>
Vector2Temple<T> operator * (const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb);
template <class T>
Vector2Temple<T> operator / (const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb);
template <class T>
Vector2Temple<T> operator % (const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb);

using Vector2 = Vector2Temple<int>;
using Vector2Dbl = Vector2Temple<double>;
using Vector2Flt = Vector2Temple<float>;

#include "details/Vector2.h"