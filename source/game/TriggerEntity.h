#ifndef _TRIGGER_ENTITY_H
#define _TRIGGER_ENTITY_H

#include "entity.h"
#include "collisionsensor.h"

class TriggerEntity: public Entity
{
	public:
		TriggerEntity();
		virtual ~TriggerEntity();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);
		virtual void OnCollision(const CollisionEvent &event);

	private:
		bool bOnce;
		s32 iCount;

		CollisionSensor clSensor;
};

#endif
