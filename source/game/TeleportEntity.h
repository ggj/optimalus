#ifndef _TELEPORT_ENTITY_H
#define _TELEPORT_ENTITY_H

#include "entity.h"
#include "collisionsensor.h"

class TeleportEntity: public Entity
{
	public:
		TeleportEntity();
		virtual ~TeleportEntity();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);
		virtual void OnCollision(const CollisionEvent &event);

	private:
		CollisionSensor clSensor;
		bool bDestination;
};

#endif
