#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	~Vector2();
	int x;
	int y;
	
	//������Z�q
	Vector2& operator = (const Vector2& vec);
	
	// �Y�������Z�q
	int& operator [] (int num);
	
	// �P�����Z�q
	Vector2& operator += (const Vector2 & vec);
	Vector2& operator -= (const Vector2& vec);
	Vector2& operator *= (int k);
	Vector2& operator /= (int k);
	Vector2 operator + ()const;
	Vector2 operator - ()const;

	Vector2& operator += (const int u);

	// ��r���Z�q
	bool operator == (const Vector2& vec) const;
	bool operator != (const Vector2& vec) const;
	bool operator <= (const Vector2& vec) const;
	bool operator < (const Vector2& vec) const;
	bool operator >= (const Vector2& vec) const;
	bool operator > (const Vector2& vec) const;
};

Vector2 operator + (const Vector2& vec, int u);
Vector2 operator - (const Vector2& vec, int u);
Vector2 operator * (const Vector2& vec, int u);
Vector2 operator / (const Vector2& vec, int u);
Vector2 operator % (const Vector2& vec, int u);

Vector2 operator * (int u, const Vector2& vec);

Vector2 operator + (const Vector2& veca, const Vector2& vecb);
Vector2 operator - (const Vector2& veca, const Vector2& vecb);
Vector2 operator * (const Vector2& veca, const Vector2& vecb);
Vector2 operator / (const Vector2& veca, const Vector2& vecb);
Vector2 operator % (const Vector2& veca, const Vector2& vecb);

