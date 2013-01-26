#ifndef _TRIGGER_ENTITY_H
#define _TRIGGER_ENTITY_H

#include "entity.h"

#include <Box2D/Box2D.h>

class TriggerEntity: public Entity
{
	public:  
		TriggerEntity();
		~TriggerEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void OnCollision(const CollisionEvent &event);

	private:
		b2Body		*pBody;
};

#endif
