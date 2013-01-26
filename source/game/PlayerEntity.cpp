#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity():
	SpriteEntity("Player")
{
	//empty
}

PlayerEntity::~PlayerEntity()
{
	//empty
}

void PlayerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
}

