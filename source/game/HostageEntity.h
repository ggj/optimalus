#ifndef _HOSTAGE_H
#define _HOSTAGE_H

#include "SpriteEntity.h"

#include "CollisionSensor.h"

class HostageEntity: public SpriteEntity
{
	public:
		HostageEntity();			

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void OnCollision(const CollisionEvent &event);

	private:
		CollisionSensor clSensor;
};

#endif
