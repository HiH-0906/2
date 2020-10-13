
template <class T>
Vector2Temple<T>::Vector2Temple()
{
	x = 0;
	y = 0;
}

template <class T>
Vector2Temple<T>::Vector2Temple(T x, T y)
{
	this->x = x;
	this->y = y;
}

template <class T>
Vector2Temple<T>::~Vector2Temple()
{
}
template <class T>
Vector2Temple<T>&Vector2Temple<T>::operator=(const Vector2Temple<T>& vec)
{
	x = vec.x;
	y = vec.y;
	return (*this);
}
template <class T>
T& Vector2Temple<T>::operator[](int num)
{
	if (num == 0)
	{
		return x;
	}
	else if (num == 1)
	{
		return y;
	}
	else
	{
		// ëzíËÇµÇƒÇ¢Ç»Ç¢ìYÇ¶éöÇ™óàÇΩèÍçáÇ∆ÇËÇ†Ç¶Ç∏xÇÇ©Ç¶Ç∑
		return x;
	}
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator+=(const Vector2Temple<T>& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator-=(const Vector2Temple<T>& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator*=(T k)
{
	x *= k;
	y *= k;
	return *this;
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator/=(T k)
{
	x /= k;
	y /= k;
	return *this;
}

template <class T>
Vector2Temple<T> Vector2Temple<T>::operator+() const
{
	return *this;
}

template <class T>
Vector2Temple<T> Vector2Temple<T>::operator-() const
{
	return Vector2Temple(-this->x, -this->y);
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator+=(const T u)
{
	this->x += u;
	this->y += u;
	return *this;
}
template <class T>
bool Vector2Temple<T>::operator==(const Vector2Temple<T>& vec) const
{
	return ((this->x == vec.x) && (this->y == vec.y));
}
template <class T>
bool Vector2Temple<T>::operator!=(const Vector2Temple<T>& vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
}
template <class T>
bool Vector2Temple<T>::operator<=(const Vector2Temple<T>& vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}
template <class T>
bool Vector2Temple<T>::operator<(const Vector2Temple<T>& vec) const
{
	return ((this->x < vec.x) && (this->y < vec.y));
}
template <class T>
bool Vector2Temple<T>::operator>=(const Vector2Temple<T>& vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));
}
template <class T>
bool Vector2Temple<T>::operator>(const Vector2Temple<T>& vec) const
{
	return ((this->x > vec.x) && (this->y > vec.y));
}
template <class T>
Vector2Temple<T> operator+(const Vector2Temple<T>& vec, T u)
{
	return Vector2Temple<T>(vec.x + u, vec.y + u);
}
template <class T>
Vector2Temple<T> operator-(const Vector2Temple<T>& vec, T u)
{
	return Vector2Temple<T>(vec.x - u, vec.y - u);
}
template <class T>
Vector2Temple<T> operator*(const Vector2Temple<T>& vec, T u)
{
	return Vector2Temple<T>(vec.x * u, vec.y * u);
}
template <class T>
Vector2Temple<T> operator/(const Vector2Temple<T>& vec, T u)
{
	return Vector2Temple<T>(vec.x / u, vec.y / u);
}
template <class T>
Vector2Temple<T> operator%(const Vector2Temple<T>& vec, T u)
{
	return Vector2Temple<T>(vec.x % u, vec.y % u);
}
template <class T>
Vector2Temple<T> operator*(T u, const Vector2Temple<T>& vec)
{
	return Vector2Temple<T>(u * vec.x, u * vec.y);
}
template <class T>
Vector2Temple<T> operator+(const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb)
{
	return Vector2Temple<T>(veca.x + vecb.x, veca.y + vecb.y);
}
template <class T>
Vector2Temple<T> operator-(const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb)
{
	return Vector2Temple<T>(veca.x - vecb.x, veca.y - vecb.y);
}
template <class T>
Vector2Temple<T> operator*(const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb)
{
	return Vector2Temple<T>(veca.x * vecb.x, veca.y * vecb.y);
}
template <class T>
Vector2Temple<T> operator/(const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb)
{
	return Vector2Temple<T>(veca.x / vecb.x, veca.y / vecb.y);
}
template <class T>
Vector2Temple<T> operator%(const Vector2Temple<T>& veca, const Vector2Temple<T>& vecb)
{
	return Vector2Temple<T>(veca.x % vecb.x, veca.y % vecb.y);
}
