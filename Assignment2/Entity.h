#pragma once

#include "Common.h"

#include "Components.h"

class Entity {
	friend class EntityManager;

	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "delault";

	//constructo and destructor
	//it is private to prevent entities created out of the entity manager aka facory pattern
	Entity(const size_t id, const std::string& tag);

public:

	//Component pointers
	ptr<Ctransform> cTransform;
	ptr<Cshape> cShape;
	ptr<CCollision> cCollision;
	ptr<CInput> cInput;
	ptr<CScore> cScore;
	ptr<CLifespan> cLifespan;

	//private member access functions
	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();


};