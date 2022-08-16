#pragma once

#include "Common.h"
#include "Entity.h"

typedef std::vector<ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {

	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities;

	void removeDeadEntities(EntityVec& vec);
	
public:
	EntityManager();

	void update();

	ptr<Entity> addEntity(const std::string& tag);

	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);
	const int verifyTag(const std::string& tag);
};