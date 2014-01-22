#include <Box2D/Common/b2Math.h>
#include "playerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity()
	: SpriteEntity("Player", "Player")
	, pBody(NULL)
	, pIcon(NULL)
	, vPlayerVectorDirection()
	, eItem(ItemTypes::None)
	, iPreviousState(Idle)
	, iCurrentState(Idle)
	, fVelocity(0.0f)
	, fMove(0.0f)
	, fUpDownMove(0.0f)
	, fLandTime(0.0f)
	, fInvicibleTime(0.0f)
	, bIsRunning(false)
{
}

PlayerEntity::~PlayerEntity()
{
	gScene->Remove(pIcon);
	sdDelete(pIcon);

	pInput->RemoveKeyboardListener(this);
	gPhysics->DestroyBody(pBody);
}

void PlayerEntity::Load(MetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);

	pIcon = sdNew(Sprite(*static_cast<Sprite *>(sprites->GetChildByName("Heart"))));
	pIcon->SetPosition(0, 0);
	pIcon->SetVisible(false);
	gScene->Add(pIcon);

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
	pIcon->SetPosition(pSprite->GetPosition() + Vector3f(0, -40, 0));

	b2Vec2 vel = pBody->GetLinearVelocity();

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

void PlayerEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
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

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == eKey::Right || k == eKey::D)
	{
		SetState(Run);

		fMove = 1;

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() < 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == eKey::Down || k == eKey::S)
	{
		fUpDownMove = 1;
	}

	//pSprite->AddPosition(vPlayerVectorDirection * fVelocity);
}

void PlayerEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
}

bool PlayerEntity::CheckGround()
{
	if (gPhysics->RayCast(pBody, b2Vec2(0, 0.32f)))
		return true;
	if (gPhysics->RayCast(pBody, b2Vec2(0.16f, 0.32f)))
		return true;
	if (gPhysics->RayCast(pBody, b2Vec2(-0.16f, 0.32f)))
		return true;

	return false;
}

void PlayerEntity::SetItem(ItemTypes::Enum item)
{
	eItem = item;
	pIcon->SetVisible(eItem == ItemTypes::Heart);
}

ItemTypes::Enum PlayerEntity::GetItem() const
{
	return eItem;
}

void PlayerEntity::SetState(int newState)
{
	iPreviousState = iCurrentState;
	iCurrentState = newState;
}

bool PlayerEntity::OnDamage()
{
	if (fInvicibleTime > 0)
		return false;

	fInvicibleTime = 3;
	return true;
}
