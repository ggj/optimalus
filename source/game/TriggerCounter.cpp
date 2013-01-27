#include "TriggerCounter.h"

#include "GameScene.h"

#include "EntityFactory.h"

ENTITY_CREATOR("TriggerCounter", TriggerCounter)

TriggerCounter::TriggerCounter():
	Entity("TriggerCounter"),
	iCounter(0)
{
	//empty
}

void TriggerCounter::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	Entity::Load(metadata, sprites);

	String counter = metadata.GetProperty("Counter");
	if(counter.empty())
	{
		Log("Counter property not found for TriggerCounter");
	}

	sscanf(counter.c_str(), "%d", &iCounter);

	strTarget = metadata.GetProperty("Target");
}

void TriggerCounter::Activate()
{
	--iCounter;

	if(iCounter == 0)
	{
		gWorldManager->ActivateAllEntites(strTarget.c_str());
	}
}
