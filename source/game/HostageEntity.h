#ifndef _HOSTAGE_H
#define _HOSTAGE_H

#include "spriteentity.h"
#include "collisionsensor.h"

class HostageEntity: public SpriteEntity
{
	public:
		HostageEntity();
		virtual ~HostageEntity();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);
		virtual void OnCollision(const CollisionEvent &event);

	private:
		CollisionSensor clSensor;
};

#endif
