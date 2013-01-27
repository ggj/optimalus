#include "TriggerEntity.h"

#include "EntityFactory.h"

#include "GameScene.h"

ENTITY_CREATOR("Trigger", TriggerEntity)

TriggerEntity::TriggerEntity():
	Entity("Trigger"),	
	fOnce(true),
	iCount(0)
{
	//empty
}

void TriggerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	Entity::Load(metadata, sprites);

	clSensor.Load(metadata, this);	

	fOnce = metadata.GetProperty("Once").compare("true") == 0;	
}

void TriggerEntity::OnCollision(const CollisionEvent &event)
{
	if(event.GetType() == CollisionEventType::ON_ENTER)
	{
		if(event.GetOtherEntity() && event.GetOtherEntity()->GetClassName().compare("Player") == 0)
		{
			Log("Trigger colidiu");

			if(fOnce && iCount > 0)
			{
				Log("Ignoring, once");
				return;
			}

			++iCount;

			this->DoActivateAll();			
		}
	}		
}
