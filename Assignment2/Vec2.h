#pragma once

#include "Common.h"

class Vec2 {
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float xin, float yin);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const float rhs) const;
	Vec2 operator * (const float rhs) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator /= (const float val);
	void operator *= (const float val);

	float dist(const Vec2& rhs) const;
	void normalize();
};