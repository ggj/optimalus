#include "HostageEntity.h"

#include "EntityFactory.h"
#include "GameScene.h"
#include "Sounds.h"

ENTITY_CREATOR("Hostage", HostageEntity)

HostageEntity::HostageEntity():
	SpriteEntity("Hostage", "Hostage")
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

		gSoundManager->Play(SND_HOSTAGE_KILL);		
		pBody->GetFixtureList()->SetUserData(NULL);	

		Entity *other = event.GetOtherEntity();
		if(other != NULL && other->GetClassName() == "Player")
		{
			PlayerEntity *player = static_cast<PlayerEntity*>(other);
			player->SetItem(ItemTypes::HEART);
		}
	}
}
