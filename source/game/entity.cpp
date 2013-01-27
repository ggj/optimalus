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

void Entity::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	UNUSED(sprites)
	strName = metadata.sName;
}
