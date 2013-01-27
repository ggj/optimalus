#include "HostageEntity.h"

#include "EntityFactory.h"
#include "GameScene.h"
#include "Sounds.h"

ENTITY_CREATOR("Hostage", HostageEntity)

HostageEntity::HostageEntity():
	SpriteEntity("Hostage")
{
	//empty
}

HostageEntity::~HostageEntity()
{
	gPhysics->DestroyBody(pBody);
}

void HostageEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);

	pBody = gPhysics->CreateStaticBody(&metadata, BodyType::SENSOR);
	pBody->GetFixtureList()->SetUserData(this);	
}

void HostageEntity::OnCollision(const CollisionEvent &event)
{
	if(event.GetType() == CollisionEventType::ON_ENTER)
	{
		Log("HOSTAGE colidiu");

		gSoundManager->Play(HOSTAGE_KILL);		
		pBody->GetFixtureList()->SetUserData(NULL);	
	}
}
