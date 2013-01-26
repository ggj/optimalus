#include "TriggerEntity.h"

#include "EntityFactory.h"

#include "GameScene.h"

ENTITY_CREATOR("Trigger", TriggerEntity)

TriggerEntity::TriggerEntity():
	pBody(NULL)
{
	//empty
}

TriggerEntity::~TriggerEntity()
{
	gPhysics->DestroyBody(pBody);
}

void TriggerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{

}