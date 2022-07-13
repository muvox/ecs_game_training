#pragma once

#include "Common.h"
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> 	EntityVec;
typedef std::map<std::string, EntityVec> 		EntityMap;

class EntityManager
{
	EntityVec 	m_entities;						// Vector containing all entities
	EntityVec 	m_entitiesToAdd;				// Vector of entities we want to add in the next iteration
	EntityMap 	m_entityMap;
	EntityVec	m_entitiesToRemove;					// Map of tags to entities
	size_t		m_totalEntities = 0;			// Total number of entities to give unique identifiers

	void removeDeadEntities(EntityVec & vec);	//

public:

	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string & tag);

	const EntityVec & getEntities();
	const EntityVec & getEntities(const std::string & tag);
};
