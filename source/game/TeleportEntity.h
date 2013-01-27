#ifndef _TELEPORT_ENTITY_H
#define _TELEPORT_ENTITY_H

#include "entity.h"

#include "CollisionSensor.h"

class TeleportEntity: public Entity
{
	public:
		TeleportEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void OnCollision(const CollisionEvent &event);

	private:
		CollisionSensor clSensor;

		bool fDestination;

};

#endif