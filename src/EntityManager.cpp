#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
	for (auto& e: m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();

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
	auto itr = std::remove_if(
		vec.begin(),
		vec.end(),
		[&](std::shared_ptr<Entity> e) { return !e->isActive();}
	);

	vec.erase(itr, vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
	std::cout << "Added an entity: " << tag << std::endl;
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

// Get all entities
const EntityVec & EntityManager::getEntities()
{
	// std::cout << "Returning all entities, all :" << m_entities.size() << std::endl;
	return m_entities;
}

// Get all entities by tag
const EntityVec & EntityManager::getEntities(const std::string & tag)
{
	return m_entityMap[tag];
}