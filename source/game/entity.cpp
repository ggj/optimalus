#include "entity.h"

#include "GameScene.h"

Entity::Entity(const String &className):
	strClassName(className)
{
	//empty
}

Entity::~Entity()
{
	//empty
}

void Entity::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	UNUSED(sprites)
	strName = metadata.sName;

	strTarget = metadata.GetProperty("Target");
}

void Entity::DoActivateAll()
{
	if(!strTarget.empty())
		gWorldManager->ActivateAllEntites(strTarget.c_str());
}
