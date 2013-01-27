#include "Entity.h"

Entity::Entity(const String &className):
	strClassName(className)
{
	//empty
}

Entity::~Entity()
{
	//empty
}

void Entity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	strName = metadata.sName;
}
