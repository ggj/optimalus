#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>
#include "../scenes/game_scene.h"

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

    gPhysics->CreateBody(pSprite);

}

Vector3f PlayerEntity::GetPosition()
{
    Vector3f position = Vector3f(pSprite->GetPosition().getX() - 200, pSprite->GetPosition().getY() - 300, 0);

    return position;
}

