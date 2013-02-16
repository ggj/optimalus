#include "teleportentity.h"
#include "gamescene.h"
#include "entityfactory.h"

ENTITY_CREATOR("Teleport", TeleportEntity)

TeleportEntity::TeleportEntity()
	: Entity("Teleport")
	, fDestination(false)
{
}

TeleportEntity::~TeleportEntity()
{
}

void TeleportEntity::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	Entity::Load(metadata, sprites);

	String destination = metadata.GetProperty("Destination");
	if (!destination.empty())
	{
		fDestination = destination.compare("true") == 0;
	}

	clSensor.Load(metadata, this);
	if (fDestination)
		clSensor.Disable();
}

void TeleportEntity::OnCollision(const CollisionEvent &event)
{
	if (event.GetType() == CollisionEventType::OnEnter)
	{
		if (event.GetOtherEntity() && event.GetOtherEntity()->GetClassName().compare("Player") == 0)
		{
			Log("Teleport colidiu");

			PlayerEntity *player = static_cast<PlayerEntity*>(event.GetOtherEntity());
			Entity *target = gWorldManager->FindEntityByName(this->GetTarget().c_str());
			if (!target)
				Log("Teleport %s without destination %s", this->GetName().c_str(), this->GetTarget().c_str());

			TeleportEntity *destination = static_cast<TeleportEntity *>(target);
			player->Teleport(destination->clSensor.GetBodyPosition());
		}
	}
}
