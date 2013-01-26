#include "WorldManager.h"
#include "EntityFactory.h"
#include "Entity.h"

WorldManager::~WorldManager()
{
	for (EntitiesVector_t::iterator it = vEntities.begin(), end = vEntities.end(); it != end; ++it)
	{
		Delete(*it);
	}
}

void WorldManager::BuildEntity(IMetadataObject &metadata, SceneNode *sprites)
{
	const String &className = metadata.GetProperty("Class");

	Entity *ent = EntityFactory::CreateEntity(className);
	if (ent == NULL)
		return;

	ent->Load(metadata, sprites);

	vEntities.push_back(ent);
}



