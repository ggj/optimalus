#include "DeathEntity.h"
#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>
#include "GameScene.h"

ENTITY_CREATOR("Death", DeathEntity)

DeathEntity::DeathEntity():
	SpriteEntity("Death")
{
	//empty
}

DeathEntity::~DeathEntity()
{
}

void DeathEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
}

Vector3f DeathEntity::GetPosition()
{
	return pSprite->GetPosition();
}

Sprite *DeathEntity::GetSprite() const
{
	return pSprite;
}

void DeathEntity::Update(f32 dt)
{

}
