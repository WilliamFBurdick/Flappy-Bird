#pragma once

class Vector2
{
public:
	float x, y;
	Vector2(float x = 0, float y = 0)
		:x(x), y(y) {}

	friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
};