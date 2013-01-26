#include "WorldManager.h"
#include "EntityFactory.h"

#include <map/IMetadataObject.h>

void WorldManager::BuildEntity(Seed::IMetadataObject &metadata)
{
	const std::string &className = metadata.GetProperty("Class");

	Entity *ent = EntityFactory::CreateEntity(className);
	if(ent == NULL)
		return;

	vEntities.push_back(ent);
}


