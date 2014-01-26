#include "enemyentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"

ENTITY_CREATOR("Enemy", EnemyEntity)

EnemyEntity::EnemyEntity()
	: SpriteEntity("Enemy", "Enemy")
	, pBody(nullptr)
	, fInvicibleTime(0.0f)
	, pTarget(nullptr)
	, bPlayerLock(false)
{
	sEnemy.displayName = "Enemy";
	sEnemy.iEnemyId = 0;
	sEnemy.iLevel = 1;
	sEnemy.iAttackPower = 3;
	sEnemy.iDefensePower = 2;
	sEnemy.iLife = 5;
	sEnemy.iLifeTotal = 5;
}

EnemyEntity::~EnemyEntity()
{
	gPhysics->DestroyBody(pBody);
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

	// Search a nerby player
	if (pTarget == nullptr || (pTarget != nullptr && !pTarget->GetIsActive()))
		pTarget = static_cast<OptimistPlayerEntity *>(gWorldManager->FindEntityByClassName("OptimistPlayer"));

	if (pTarget == nullptr || (pTarget != nullptr &&!pTarget->GetIsActive()))
		pTarget = static_cast<RealistPlayerEntity *>(gWorldManager->FindEntityByClassName("RealistPlayerEntity"));

	if (pTarget == nullptr || (pTarget != nullptr && !pTarget->GetIsActive()))
		pTarget = static_cast<PessimistPlayerEntity *>(gWorldManager->FindEntityByClassName("PessimistPlayerEntity"));

	if (pTarget != nullptr && pTarget->GetIsActive())
	{
		b2Vec2 dir = pTarget->GetBodyPosition() - pBody->GetPosition();

		f32 distance = dir.Normalize();

		if (distance <= 1.0f)
		{
			bPlayerLock = true;
			this->SetDisplayName(this->GetDisplayName());
			this->SetLevel(this->GetLevel());
			this->SetLife(this->GetLife());
			gGui->SelectEnemy(pTarget->GetDisplayName(), this->sEnemy.iEnemyId);
		}

		if(bPlayerLock && distance >= 1.0f)
		{
			bPlayerLock = false;
			gGui->SelectEnemy("", 0);
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

			u32 damageToPlayer = (player->GetDefensePower() - sEnemy.iAttackPower) + (rand() % 3 + 1);

			//Do damage to the player
			player->OnDamage(vecToPush, damageToPlayer);

			//Receive damage
			this->OnDamage(player->sPlayer.iAttackPower);
		}
	}
}

bool EnemyEntity::OnDamage(u32 amount)
{
	// Play damage sound
	gSoundManager->Play(SND_DAMAGE);

	if (fInvicibleTime > 0)
		return false;

	// Receive the damage
	this->SetLife(this->GetLife() - amount);

	if((int)this->GetLife() <= 0)
	{
		// Disable item
		this->pSprite->SetVisible(false);

		// Add body to a list to remove
		//gPhysics->lstBodiesForRemove.push_back(pBody);
	}
	else
		fInvicibleTime = 3;

	return true;
}

String EnemyEntity::GetDisplayName() const
{
	return sEnemy.displayName;
}

void EnemyEntity::SetDisplayName(String displayName)
{
	sEnemy.displayName = displayName;
	gGui->SetEnemyName(displayName);
}

u32 EnemyEntity::GetLevel() const
{
	return sEnemy.iLevel;
}

void EnemyEntity::SetLevel(u32 level)
{
	sEnemy.iLevel = level;
	gGui->SetEnemyLevel(level);
}

u32 EnemyEntity::GetLife() const
{
	return sEnemy.iLife;
}

void EnemyEntity::SetLife(u32 life)
{
	sEnemy.iLife = life;
	gGui->SetEnemyLife(life, this->sEnemy.iLifeTotal);
}
