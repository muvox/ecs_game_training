#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
	// TODO: add entities from m_entitiesToAdd to the proper location
	//		-	add them to the vector of all entities
	//		-	add them to the vector inside the map wth the tag as key

	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	// C17 way
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
	// TODO: 	check entities isActive and remove from vector
	//			remember iteration invalidation
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

// Get all entities
const EntityVec & EntityManager::getEntities()
{
	return m_entities;
}

// Get all entities by tag
const EntityVec & EntityManager::getEntities(const std::string & tag)
{
	EntityVec foundEntities;

	// TODO: look for entities with tag and return that EntityVec

	return foundEntities;
}