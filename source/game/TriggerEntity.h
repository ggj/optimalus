#ifndef _TRIGGER_ENTITY_H
#define _TRIGGER_ENTITY_H

#include "entity.h"

class TriggerEntity: public Entity
{
	public:  
		TriggerEntity();

		virtual void Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites);

	private:

};

#endif
