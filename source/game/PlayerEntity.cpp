#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>
#include "GameScene.h"

#include <Box2D/Common/b2Math.h>

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity():
	SpriteEntity("Player", "Player"),
	eItem(ItemTypes::NONE)
{
	//empty
}

PlayerEntity::~PlayerEntity()
{
	pInput->RemoveKeyboardListener(this);
	gPhysics->DestroyBody(pBody);
}

void PlayerEntity::Load(Seed::IMetadataObject &metadata, Seed::SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);

	pBody = gPhysics->CreateBody(pSprite);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);

	pInput->AddKeyboardListener(this);
	fVelocity = 5.0f;
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

void PlayerEntity::Update(f32 dt)
{
	if (iPreviousState ==JUMP && CheckGround())
	{
		SetState(LAND);
	}

	if (iCurrentState == iPreviousState)
		return;

	if (iCurrentState == JUMP)
	{
		pSprite->SetAnimation("Jump");
	}
	else if (iCurrentState == LAND)
	{
		pSprite->SetAnimation("Land");
	}
	else if (iCurrentState == RUN)
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

	if ((k == Seed::KeyUp || k == Seed::KeyW) && iCurrentState != JUMP)
	{
		SetState(JUMP);
		pBody->ApplyForce(b2Vec2(0,650), pBody->GetWorldCenter());
	}

	if (k == Seed::KeyLeft || k == Seed::KeyA)
	{
		SetState(RUN);

		vel.x = -5;
		pBody->SetLinearVelocity(vel);

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight || k == Seed::KeyD)
	{
		SetState(RUN);

		vel.x = 5;
		pBody->SetLinearVelocity(vel);

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() < 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyDown || k == Seed::KeyS)
	{
		// Sum the normalized vector down with the current vector
		// Maybe later, maybe if the player can duck
	}

	pSprite->AddPosition(vPlayerVectorDirection * fVelocity);
}

void PlayerEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	b2Vec2 vel = pBody->GetLinearVelocity();
	vel.x = 0;

	// Remove the directions
	if (k == Seed::KeyUp|| k == Seed::KeyW)
	{

	}

	if (k == Seed::KeyLeft|| k == Seed::KeyA)
	{
		pBody->SetLinearVelocity(vel);

		if (CheckGround())
			SetState(IDLE);
		else
			SetState(JUMP);
	}

	if (k == Seed::KeyRight|| k == Seed::KeyD)
	{
		pBody->SetLinearVelocity(vel);

		if (CheckGround())
			SetState(IDLE);
		else
			SetState(JUMP);
	}

	if (k == Seed::KeyDown|| k == Seed::KeyS)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}
}

bool PlayerEntity::CheckGround()
{
	return gPhysics->RayCast(pBody, b2Vec2(0, 0.32f));
}


void PlayerEntity::SetItem(ItemTypes::Enum item)
{
	eItem = item;
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

