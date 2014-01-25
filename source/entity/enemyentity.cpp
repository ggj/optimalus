#include "enemyentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"

ENTITY_CREATOR("Enemy", EnemyEntity)

EnemyEntity::EnemyEntity()
	: SpriteEntity("Enemy", "Enemy")
	, pBody(nullptr)
	, fInvicibleTime(0.0f)
{
}

EnemyEntity::~EnemyEntity()
{
}

void EnemyEntity::Load(MetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);
	clSensor.Load(metadata, this);

	b2Vec2 customSize(40, 40);

	pBody = gPhysics->CreateKinematicBody(pSprite, &customSize);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);
}

void EnemyEntity::Update(f32 dt)
{
	b2Vec2 vel = pBody->GetLinearVelocity();
	pBody->SetLinearVelocity(vel);

	if (fInvicibleTime > 0)
	{
		pSprite->SetVisible(!pSprite->IsVisible());

		fInvicibleTime -= dt;
		if (fInvicibleTime <= 0)
		{
			pSprite->SetVisible(true);
			fInvicibleTime = 0;
		}
	}
}

void EnemyEntity::OnCollision(const CollisionEvent &event)
{
	if (event.GetType() == CollisionEventType::OnEnter)
	{
		Log("ENEMY colidiu");

		Entity *other = event.GetOtherEntity();
		if ((other != nullptr && other->GetClassName() == "OptimistPlayer") ||
			(other != nullptr && other->GetClassName() == "RealistPlayer") ||
			(other != nullptr && other->GetClassName() == "PessimistPlayer"))
		{
			PlayerEntity *player = static_cast<PlayerEntity *>(other);

			// Stop player movement
			player->StopPlayerMovement();
			player->SetIsInputEnabled(false);

			// Define a vector to push the player
			b2Vec2 vecToPush = b2Vec2(0, 0);

			// Find where the player comes
			if (gPhysics->RayCast(pBody, b2Vec2(0, -0.32f)) ||
				gPhysics->RayCast(pBody, b2Vec2(0.16f, -0.32f)) ||
				gPhysics->RayCast(pBody, b2Vec2(-0.16f, -0.32f)))
			{
				Log("Push player up");
				vecToPush = b2Vec2(0.0f, -1.0f);
			}
			else if (gPhysics->RayCast(pBody, b2Vec2(0, 0.32f)) ||
					 gPhysics->RayCast(pBody, b2Vec2(0.16f, 0.32f)) ||
					 gPhysics->RayCast(pBody, b2Vec2(-0.16f, 0.32f)))
			{
				Log("Push player down");
				vecToPush = b2Vec2(0.0f, 1.0f);
			}
			else if (gPhysics->RayCast(pBody, b2Vec2(-0.32f, 0.0f)) ||
					 gPhysics->RayCast(pBody, b2Vec2(-0.32f, 0.16f)) ||
					 gPhysics->RayCast(pBody, b2Vec2(-0.32f, -0.16f)))
			{
				Log("Push player left");
				vecToPush = b2Vec2(-1.0f, 0.0f);
			}
			else if (gPhysics->RayCast(pBody, b2Vec2(0.32f, 0.0f)) ||
					 gPhysics->RayCast(pBody, b2Vec2(0.32f, 0.16f)) ||
					 gPhysics->RayCast(pBody, b2Vec2(0.32f, -0.16f)))
			{
				Log("Push player right");
				vecToPush = b2Vec2(1.0f, 0.0f);
			}

			//Do damage to the player
			player->OnDamage(vecToPush);

			//Receive damage
			this->OnDamage();
		}
	}
}

bool EnemyEntity::OnDamage()
{
	// Play damage sound
	gSoundManager->Play(SND_DAMAGE);

	if (fInvicibleTime > 0)
		return false;

	fInvicibleTime = 3;
	return true;
}
