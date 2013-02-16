#include "hostageentity.h"
#include "entityfactory.h"
#include "gamescene.h"
#include "sounds.h"

ENTITY_CREATOR("Hostage", HostageEntity)

HostageEntity::HostageEntity()
	: SpriteEntity("Hostage", "Hostage")
{
}

HostageEntity::~HostageEntity()
{
}

void HostageEntity::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	clSensor.Load(metadata, this);
}

void HostageEntity::OnCollision(const CollisionEvent &event)
{
	if (event.GetType() == CollisionEventType::OnEnter)
	{
		Log("HOSTAGE colidiu");

		Entity *other = event.GetOtherEntity();
		if (other != NULL && other->GetClassName() == "Player")
		{
			PlayerEntity *player = static_cast<PlayerEntity *>(other);

			if (player->GetItem() == ItemTypes::None)
			{
				gSoundManager->Play(SND_HOSTAGE_KILL);
				clSensor.Disable();

				player->SetItem(ItemTypes::Heart);
				pSprite->SetAnimation("Death");
			}
		}
	}
}
