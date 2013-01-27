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


void HostageEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);

	clSensor.Load(metadata, this);	
}

void HostageEntity::OnCollision(const CollisionEvent &event)
{
	if(event.GetType() == CollisionEventType::ON_ENTER)
	{
		Log("HOSTAGE colidiu");
		
		Entity *other = event.GetOtherEntity();
		if(other != NULL && other->GetClassName() == "Player")
		{
			PlayerEntity *player = static_cast<PlayerEntity*>(other);

			if(player->GetItem() == ItemTypes::NONE)
			{
				gSoundManager->Play(SND_HOSTAGE_KILL);		
				clSensor.Disable();				

				player->SetItem(ItemTypes::HEART);

				pSprite->SetAnimation("Death");
			}
		}
	}
}
