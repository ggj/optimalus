#include "enemyentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"

ENTITY_CREATOR("Enemy", EnemyEntity)

EnemyEntity::EnemyEntity()
	: SpriteEntity("Enemy", "Enemy")
	, pBody(nullptr)
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

	b2Vec2 customSize(32, 32);

	pBody = gPhysics->CreateBody(pSprite, &customSize);

	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);
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

			//Do damage to the player
			player->OnDamage();

			//Receive damage
		}
	}
}
