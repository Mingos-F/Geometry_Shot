#include "Vec2.h"

Vec2::Vec2() {

}

Vec2::Vec2(float xin, float yin)
	:x(xin), y(yin) {

}

bool Vec2::operator == (const Vec2& rhs) const {

	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const {

	return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const {

	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const {

	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float rhs) const {
	if (rhs != 0) {
		return Vec2(x / rhs, y / rhs);
	}
	else {
		std::cerr << "ERROR: trying to diveide by 0";
		return Vec2();
	}
}

Vec2 Vec2::operator * (const float rhs) const {

	return Vec2(x * rhs, y * rhs);
}

void Vec2::operator += (const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator /= (const float val) {
	if (val != 0) {
		x /= val;
		y /= val;
	}
	else {
		std::cerr << "ERROR: trying to diveide by 0";
	}
}

void Vec2::operator *= (const float val) {
	x *= val;
	y *= val;
}

float Vec2::dist(const Vec2& rhs) const {

	return (float)sqrt((rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y));
}

void Vec2::normalize() {
	float length = (float)sqrt(x * x + y * y);
	x /= length;
	y /= length;
}