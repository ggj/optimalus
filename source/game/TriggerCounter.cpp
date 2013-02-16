#include "triggercounter.h"
#include "gamescene.h"
#include "entityfactory.h"

ENTITY_CREATOR("TriggerCounter", TriggerCounter)

TriggerCounter::TriggerCounter()
	: Entity("TriggerCounter")
	, iCounter(0)
{
}

TriggerCounter::~TriggerCounter()
{
}

void TriggerCounter::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	Entity::Load(metadata, sprites);

	String counter = metadata.GetProperty("Counter");
	if (counter.empty())
	{
		Log("Counter property not found for TriggerCounter");
	}

	sscanf(counter.c_str(), "%d", &iCounter);
}

void TriggerCounter::Activate()
{
	--iCounter;

	if (iCounter == 0)
	{
		this->DoActivateAll();
	}
}
