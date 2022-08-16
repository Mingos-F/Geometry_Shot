#pragma once

#include "Common.h"

class Ctransform {
public:
	Vec2 pos = {0.0f, 0.0f};
	Vec2 speed = { 0.0f,0.0f };
	float angle = 0.0f;

	Ctransform(const Vec2& _pos, const Vec2& _speed, float _angle)
		:pos(_pos), speed(_speed), angle(_angle){}
};

// with sfml evry shape can be drawn from the circle, ence its usage in the shape component
class Cshape {
public:
	sf::CircleShape circle;

	Cshape(float radious, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		:circle(radious, points) {
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		//tell sfml where the origin is, by default it is the left corner of the baunding box, we this line moves it to the center of the bounding box, ence radious
		circle.setOrigin(radious, radious);
	}
};

class CCollision {
public:
	float radious = 0.0f;

	CCollision(float _radious)
		:radious(_radious){}
};

class CScore {
public:
	int score = 0;

	CScore(int _score)
		:score(_score){}
};

class CLifespan{
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int _total)
		:remaining(_total), total(_total){}
};

class CInput {
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;

	CInput() {}
};