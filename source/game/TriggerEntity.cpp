#include "TriggerEntity.h"

#include "EntityFactory.h"

#include "GameScene.h"

ENTITY_CREATOR("Trigger", TriggerEntity)

TriggerEntity::TriggerEntity():
	Entity("Trigger"),
	pBody(NULL),
	fOnce(false),
	iCount(0)
{
	//empty
}

TriggerEntity::~TriggerEntity()
{
	gPhysics->DestroyBody(pBody);
}

void TriggerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	Entity::Load(metadata, sprites);

	pBody = gPhysics->CreateStaticBody(&metadata, BodyType::SENSOR);
	pBody->GetFixtureList()->SetUserData(this);	

	fOnce = metadata.GetProperty("Once").compare("true") == 0;
	strActivateTarget = metadata.GetProperty("Target");
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

			if(!strActivateTarget.empty())
			{
				Entity *ent = gWorldManager->FindEntityByName(strActivateTarget.c_str());
				if(ent)
					ent->Activate();
			}
		}
	}		
}
