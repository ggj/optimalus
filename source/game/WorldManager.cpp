#include "WorldManager.h"
#include "EntityFactory.h"
#include "Entity.h"

#include <map/IMetadataObject.h>

WorldManager::~WorldManager()
{
	for(EntitiesVector_t::iterator it = vEntities.begin(), end = vEntities.end(); it != end; ++it)
	{
		Delete(*it);
	}
}

void WorldManager::BuildEntity(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	const std::string &className = metadata.GetProperty("Class");

	Entity *ent = EntityFactory::CreateEntity(className);
	if(ent == NULL)
		return;

	ent->Load(metadata, sprites);

	vEntities.push_back(ent);
}



