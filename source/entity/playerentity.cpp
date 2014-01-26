#include <Box2D/Common/b2Math.h>
#include "playerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity()
	: SpriteEntity("Player", "Player")
	, pBody(nullptr)
	, pText(nullptr)
	, vPlayerVectorDirection()
	, eItem(ItemTypes::None)
	, iPreviousState(Idle)
	, iCurrentState(Idle)
	, fVelocity(0.0f)
	, fMove(0.0f)
	, fUpDownMove(0.0f)
	, fInvicibleTime(0.0f)
	, bIsActive(false)
	, bIsInputEnabled(true)
{
}

PlayerEntity::PlayerEntity(const char *className, const char *spriteName, bool bIsActive)
	: SpriteEntity(className, spriteName)
	, pBody(nullptr)
	, pText(nullptr)
	, vPlayerVectorDirection()
	, eItem(ItemTypes::None)
	, iPreviousState(Idle)
	, iCurrentState(Idle)
	, fVelocity(0.0f)
	, fMove(0.0f)
	, fUpDownMove(0.0f)
	, fInvicibleTime(0.0f)
	, bIsActive(bIsActive)
	, bIsInputEnabled(true)
{
}

PlayerEntity::~PlayerEntity()
{
	gScene->Remove(pText);
	sdDelete(pText);

	pInput->RemoveKeyboardListener(this);
	gPhysics->DestroyBody(pBody);
}

void PlayerEntity::Load(MetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);

	pText = sdNew(Sprite(*static_cast<Sprite *>(sprites->GetChildByName("Number1"))));
	pText->SetPosition(0, 0);
	pText->SetVisible(false);
	gScene->Add(pText);

	b2Vec2 customSize(32, 32);

	pBody = gPhysics->CreateBody(pSprite, &customSize);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);

	pInput->AddKeyboardListener(this);
	fVelocity = 2.0f;
	vPlayerVectorDirection = VECTOR_ZERO;
}

Vector3f PlayerEntity::GetPosition()
{
	return pSprite->GetPosition();
}

b2Vec2 PlayerEntity::GetBodyPosition() const
{
	return pBody->GetPosition();
}

Sprite *PlayerEntity::GetSprite() const
{
	return pSprite;
}

void PlayerEntity::Teleport(const b2Vec2 &position)
{
	pBody->SetTransform(position, pBody->GetAngle());

	fMove= 0;
	fUpDownMove= 0;
	this->SetState(Idle);

	gSoundManager->Play(SND_TELEPORT);
}

void PlayerEntity::Update(f32 dt)
{
	pText->SetPosition(pSprite->GetPosition() + Vector3f(0, -40, 0));

	b2Vec2 vel = pBody->GetLinearVelocity();

	if (fInvicibleTime > 0)
	{
		pSprite->SetVisible(!pSprite->IsVisible());

		fInvicibleTime -= dt;
		if (fInvicibleTime <= 0)
		{
			pSprite->SetVisible(true);
			fInvicibleTime = 0;
			if (this->bIsActive)
			{
				this->bIsInputEnabled = true;
				this->StopPlayerMovement();
				SetState(Idle);
				pText->SetVisible(false);
			}
		}
	}

	if (fMove != 0)
	{
		vel.x = fVelocity * fMove;
		pBody->SetLinearVelocity(vel);
	}

	if (fUpDownMove != 0)
	{
		vel.y = fVelocity * fUpDownMove;
		pBody->SetLinearVelocity(vel);
	}

	if (iCurrentState == iPreviousState)
		return;

	if (iCurrentState == Run)
	{
		pSprite->SetAnimation("Run");
	}
	else
	{
		pSprite->SetAnimation("Idle");
	}

	iPreviousState = iCurrentState;
}

bool PlayerEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

		b2Vec2 vel = pBody->GetLinearVelocity();

		if ((k == eKey::Up || k == eKey::W) && iCurrentState != Jump)
		{
			SetState(Run);
			fUpDownMove = -1;
		}

		if (k == eKey::Left || k == eKey::A)
		{
			SetState(Run);
			fMove = -1;
		}

		if (k == eKey::Right || k == eKey::D)
		{
			SetState(Run);
			fMove = 1;
		}

		if (k == eKey::Down || k == eKey::S)
		{
			SetState(Run);
			fUpDownMove = 1;
		}
	}

	return true;
}

bool PlayerEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

		b2Vec2 vel = pBody->GetLinearVelocity();
		vel.x = 0;
		vel.y = 0;

		// Remove the directions
		if (k == eKey::Up|| k == eKey::W)
		{
			pBody->SetLinearVelocity(vel);
			fUpDownMove = 0;
		}

		if (k == eKey::Left|| k == eKey::A)
		{
			pBody->SetLinearVelocity(vel);
			fMove = 0;
		}

		if (k == eKey::Right|| k == eKey::D)
		{
			pBody->SetLinearVelocity(vel);
			fMove = 0;
		}

		if (k == eKey::Down|| k == eKey::S)
		{
			pBody->SetLinearVelocity(vel);
			fUpDownMove = 0;
		}

		if (k == eKey::Space)
		{
			gGameScene->ChangePlayer(this->GetClassName());
			return false;
		}

		if (fUpDownMove == 0 && fMove == 0)
		{
			SetState(Idle);
		}
	}

	return true;
}

void PlayerEntity::SetItem(ItemTypes::Enum item)
{
	eItem = item;
	pText->SetVisible(eItem == ItemTypes::Text);
}

ItemTypes::Enum PlayerEntity::GetItem() const
{
	return eItem;
}

void PlayerEntity::SetIsActive(bool isActive)
{
	bIsActive = isActive;
}

void PlayerEntity::SetIsInputEnabled(bool isInputEnabled)
{
	bIsInputEnabled = isInputEnabled;
}

bool PlayerEntity::GetIsInputEnabled() const
{
	return bIsInputEnabled;
}

void PlayerEntity::StopPlayerMovement()
{
	b2Vec2 vel = pBody->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;

	pBody->SetLinearVelocity(vel);
	fUpDownMove = 0;
	fMove = 0;
}

bool PlayerEntity::GetIsActive()
{
	return bIsActive;
}

void PlayerEntity::SetState(int newState)
{
	iPreviousState = iCurrentState;
	iCurrentState = newState;
}

String PlayerEntity::GetDisplayName() const
{
	return sPlayer.displayName;
}

u32 PlayerEntity::GetLevel() const
{
	return sPlayer.iLevel;
}
void PlayerEntity::SetLevel(const u32 level)
{
	sPlayer.iLevel = level;
}

u32 PlayerEntity::GetXP() const
{
	return sPlayer.iXP;
}
void PlayerEntity::SetXP(const u32 xp)
{
	sPlayer.iXP = xp;
}

u32 PlayerEntity::GetAttackPower() const
{
	return sPlayer.iAttackPower;
}
void PlayerEntity::SetAttackPower(const u32 attackPower)
{
	sPlayer.iAttackPower = attackPower;
}

u32 PlayerEntity::GetGold() const
{
	return sPlayer.iGold;
}
void PlayerEntity::SetGold(const u32 gold)
{
	sPlayer.iGold = gold;
	gGui->SetGold(gold);
}

u32 PlayerEntity::GetLife() const
{
	return sPlayer.iLife;
}
void PlayerEntity::SetLife(const u32 life)
{
	sPlayer.iLife = life;
	gGui->SetLife(life);
}
void PlayerEntity::RemoveLife()
{
	sPlayer.iLife--;
}

u32 PlayerEntity::GetStamina() const
{
	return sPlayer.iStamina;
}
void PlayerEntity::SetStamina(const u32 stamina)
{
	sPlayer.iStamina = stamina;
	gGui->SetStamina(stamina);
}
void PlayerEntity::RemoveStamina()
{
	sPlayer.iStamina--;
}

bool PlayerEntity::OnDamage(const b2Vec2 vec2Push/*, EnemyEntity enemy*/)
{
	// Play damage sound
	gSoundManager->Play(SND_DAMAGE);

	// Apply force to player
	pBody->ApplyForce(vec2Push, pBody->GetWorldCenter());

	// Create the ghost effect
	if (fInvicibleTime > 0)
		return false;

	fInvicibleTime = 3;
	pText->SetVisible(true);

	// Receive the damage
	//enemy.

	return true;
}

void PlayerEntity::OnCollect(ItemTypes::Enum item)
{
	// Play damage sound
	gSoundManager->Play(SND_POWERUP);

	if(item == ItemTypes::HealthPotion)
		this->SetLife(this->GetLife() + 10);

	if(item == ItemTypes::StaminaPotion)
		this->SetStamina(this->GetStamina() + 10);

	if(item == ItemTypes::Gold)
		this->SetGold(this->GetGold() + 50);
}

