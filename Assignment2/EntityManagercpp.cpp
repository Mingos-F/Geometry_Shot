#include "EntityManager.h"

EntityManager::EntityManager()
	:m_totalEntities(0) {

}

void EntityManager::update() {
	// add them to vector of all entities
	// add them to the map
	for (auto& e : m_entitiesToAdd) {
		m_entities.push_back(e);

		if (m_entityMap.find(e->m_tag) == m_entityMap.end()) {
			EntityVec vecTag;
			vecTag.push_back(e);
			m_entityMap.insert({ e->m_tag,vecTag });
		}
		else {
			m_entityMap.find(e->m_tag)->second.push_back(e);
		}
	}

	// clear the vector of entities to add
	m_entitiesToAdd.clear();

	//remove dead entities from vector of all entities
	removeDeadEntities(m_entities);

	//remove dead entities from each vector in the map
	for (auto& e : m_entityMap) {
		removeDeadEntities(e.second);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
	// create the iterator
	auto it = vec.begin();

	// loop true vector
	while (it != vec.end()){
		// check if it must me removed
		if (!(*it)->m_active)
		{
			// erase() invalidates the iterator, use returned iterator for next loop
			it = vec.erase(it);
		}
		else {
			// increment iterator if nothing was deleted
			++it;
		}
	}
}

ptr<Entity> EntityManager::addEntity(const std::string& tag) {
	//create the entity shared pointer
	auto entity = ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities() {
	return m_entities;
}

const int EntityManager::verifyTag(const std::string& tag) {
	if (m_entityMap.find(tag) == m_entityMap.end()) {
		return NULL;
	}
	else
	{
		return 1;
	}
}


const EntityVec& EntityManager::getEntities(const std::string& tag) {
		return m_entityMap.find(tag)->second;
}