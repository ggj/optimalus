#ifndef _TRIGGER_COUNTER_H
#define _TRIGGER_COUNTER_H

#include "entity.h"

class TriggerCounter: public Entity
{
	public:
		TriggerCounter();
		virtual ~TriggerCounter();

		virtual void Load(IMetadataObject &metadata, SceneNode *sprites);
		virtual void Activate();

	private:
		s32 iCounter;
};

#endif
