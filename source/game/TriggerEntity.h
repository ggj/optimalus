#ifndef _TRIGGER_ENTITY_H
#define _TRIGGER_ENTITY_H

#include "entity.h"

#include "CollisionSensor.h"

class TriggerEntity: public Entity
{
	public:  
		TriggerEntity();		

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

		virtual void OnCollision(const CollisionEvent &event);

	private:		
		bool		fOnce;
		unsigned	iCount;

		CollisionSensor clSensor;
};

#endif
