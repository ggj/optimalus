#include "deathentity.h"
#include "entityfactory.h"
#include "gamescene.h"
#include "sounds.h"
#include "gamescene.h"

ENTITY_CREATOR("Death", DeathEntity)

#define SLEEP_TIME 10

DeathEntity::DeathEntity()
	: SpriteEntity("Death", "Death")
	, pTarget(NULL)
	, fpSleepConfig(SLEEP_TIME)
	, fpSleep(SLEEP_TIME)
	, fpSpeedFactor(1)
{
}

DeathEntity::~DeathEntity()
{
}

void DeathEntity::Load(IMetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	b2Vec2 customSize(50, 40);

	clSensor.Load(*pSprite, true, &customSize, this);

	String sleep = metadata.GetProperty("Sleep");
	if (!sleep.empty())
	{
		sscanf(sleep.c_str(), "%f", &fpSleep);
		fpSleepConfig = fpSleep;
	}

	String speedFactor = metadata.GetProperty("SpeedFactor");
	if (!speedFactor.empty())
	{
		sscanf(speedFactor.c_str(), "%f", &fpSpeedFactor);
	}
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
	if (fpSleep > 0)
	{
		fpSleep -= dt;
		if (fpSleep < 0)
		{
			this->Activate();
		}
	}
	else
	{
		if (pTarget == NULL)
			pTarget = static_cast<PlayerEntity *>(gWorldManager->FindEntityByClassName("Player"));

		if (pTarget == NULL)
			Log("No player to track");

		b2Vec2 dir = pTarget->GetBodyPosition() - clSensor.GetBodyPosition();

		f32 distance = dir.Normalize();
		if (distance > 0.03f)
		{
			//Go faster down to help player jump
			if (dir.y > 0)
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

			distance = distance / 1.5f;
			if (distance < 1)
				distance = 1;

			dir *= distance;
			dir *= fpSpeedFactor;
			dir += clSensor.GetBodyPosition();

			clSensor.SetBodyPosition(dir);
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
	if (event.GetType() == CollisionEventType::OnEnter)
	{
		Log("DeathEntity colidiu");

		Entity *other = event.GetOtherEntity();
		if (other != NULL && other->GetClassName() == "Player")
		{
			PlayerEntity *player = static_cast<PlayerEntity *>(other);
			if (player->GetItem() == ItemTypes::Heart)
			{
				gSoundManager->Play(SND_POWERUP);
				player->SetItem(ItemTypes::None);
				fpSleep = fpSleepConfig;

				gGameScene->RemoveHostage();
			}
			else if (fpSleep <= 0)
			{
				if (player->OnDamage())
				{
					gSoundManager->Play(SND_DAMAGE);
					gGameScene->RemoveLife();
				}
			}
		}
	}
}
