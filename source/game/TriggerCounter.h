#ifndef _TRIGGER_COUNTER_H
#define _TRIGGER_COUNTER_H

#include "entity.h"

class TriggerCounter: public Entity
{
	public:
		TriggerCounter();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);

		virtual void Activate();

	private:
		int iCounter;
};

#endif
