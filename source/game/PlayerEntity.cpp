#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>

#include "../scenes/game_scene.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity():
	pSprite(NULL)
{
	//empty
}

PlayerEntity::~PlayerEntity()
{
	Delete(pSprite);
}

void PlayerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	//FIXME
	pSprite = New(Sprite(*static_cast<Sprite *>(sprites->GetChildByName("Player"))));
    pSprite->SetPosition(metadata.GetPosition());

    strName = "Player";

	gScene->Add(pSprite);
    gPhysics->CreateBody(pSprite);
}

Vector3f PlayerEntity::GetPosition()
{
    Vector3f position = Vector3f(pSprite->GetPosition().getX() - 200, pSprite->GetPosition().getY() - 300, 0);

    return position;
}
