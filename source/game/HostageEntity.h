#ifndef _HOSTAGE_H
#define _HOSTAGE_H

#include "SpriteEntity.h"

#include <Box2D/Box2D.h>

class HostageEntity: public SpriteEntity
{
	public:
		HostageEntity();	
		~HostageEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void OnCollision(const CollisionEvent &event);

	private:
		b2Body		*pBody;
};

#endif