#include "Geometry.h"

Vector2 operator+(const Vector2& lvel, const Vector2& levl2)
{
	return Vector2(lvel.x + levl2.x, lvel.y + levl2.y);
}

Vector2 operator-(const Vector2& lvel, const Vector2& levl2)
{
	return Vector2(lvel.x - levl2.x, lvel.y - levl2.y);
}
