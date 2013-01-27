#include "WorldManager.h"
#include "EntityFactory.h"
#include "Entity.h"

WorldManager::~WorldManager()
{
	this->Clear();
}

Entity* WorldManager::BuildEntity(IMetadataObject &metadata, SceneNode *sprites)
{
	const String &className = metadata.GetProperty("Class");

	Log("%s", className.c_str());

	Entity *ent = EntityFactory::CreateEntity(className);
	if (ent == NULL)
		return NULL;

	ent->Load(metadata, sprites);

	vEntities.push_back(ent);

	return ent;
}

void WorldManager::Clear()
{
	for (EntitiesVector_t::iterator it = vEntities.begin(), end = vEntities.end(); it != end; ++it)
	{
		Delete(*it);
	}

	vEntities.clear();
}

void WorldManager::Update(f32 dt)
{
	for (EntitiesVector_t::iterator it = vEntities.begin(), end = vEntities.end(); it != end; ++it)
	{
		(*it)->Update(dt);
	}
}


