#include "DeathEntity.h"
#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>
#include "GameScene.h"
#include "Sounds.h"

ENTITY_CREATOR("Death", DeathEntity)

#define SLEEP_TIME 10

DeathEntity::DeathEntity():
	SpriteEntity("Death", "Death"),
	fpSleep(SLEEP_TIME),
	pTarget(NULL)
{
	//empty
}

DeathEntity::~DeathEntity()
{
	gPhysics->DestroyBody(pBody);
}

void DeathEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);

	b2Vec2 customSize(50, 40);

	pBody = gPhysics->CreateStaticBody(pSprite, BodyType::SENSOR, true, &customSize);
	pBody->GetFixtureList()->SetUserData(this);	
}

Vector3f DeathEntity::GetPosition()
{
	return pSprite->GetPosition();
}

Sprite *DeathEntity::GetSprite() const
{
	return pSprite;
}

void DeathEntity::Update(f32 dt)
{
	if(fpSleep > 0)
	{
		fpSleep -= dt;
		if(fpSleep < 0)
		{
			this->Activate();
		}
	}
	else
	{
		if(pTarget == NULL)
			pTarget = static_cast<PlayerEntity*>(gWorldManager->FindEntityByClassName("Player"));

		if(pTarget == NULL)
			Log("No player to track");

		b2Vec2 dir = pTarget->GetBodyPosition() - pBody->GetPosition();
		
		if(dir.Normalize() > 0.03f)
		{
			//Go faster down to help player jump
			if(dir.y > 0)
			{
				f32 dy = dt * 2;				
				dir.y = std::min(dy, dir.y);
				dir.x *= dt;
			}
			else
			{
				dir *= dt;
				dir.y /= 2;
			}

			dir += pBody->GetPosition();

			pBody->SetTransform(dir, pBody->GetAngle());
		}
	}
}

void DeathEntity::Activate()
{
	fpSleep = 0;

	gSoundManager->Play(SND_WAKEUP);
}

void DeathEntity::OnCollision(const CollisionEvent &event)
{
	if(event.GetType() == CollisionEventType::ON_ENTER)
	{
		Log("DeathEntity colidiu");
		
		Entity *other = event.GetOtherEntity();
		if(other != NULL && other->GetClassName() == "Player")
		{
			PlayerEntity *player = static_cast<PlayerEntity*>(other);

			if(player->GetItem() == ItemTypes::HEART)
			{
				gSoundManager->Play(SND_POWERUP);
				player->SetItem(ItemTypes::NONE);
				fpSleep = SLEEP_TIME;
			}			
			else if(fpSleep <= 0)
			{
				gSoundManager->Play(SND_DAMAGE);
			}				
		}
	}
}
