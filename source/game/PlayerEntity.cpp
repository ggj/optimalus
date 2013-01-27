#include "PlayerEntity.h"

#include "EntityFactory.h"

#include <SceneNode.h>
#include <LeakReport.h>
#include <Sprite.h>
#include "GameScene.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity():
	SpriteEntity("Player")
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

	pInput->AddKeyboardListener(this);
	fVelocity = 5.0f;
	vPlayerVectorDirection = VECTOR_ZERO;
}

Vector3f PlayerEntity::GetPosition()
{
	return pSprite->GetPosition();
}

Sprite *PlayerEntity::GetSprite() const
{
	return pSprite;
}

void PlayerEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	b2Vec2 vel = pBody->GetLinearVelocity();

	if (k == Seed::KeyUp || k == Seed::KeyW)
	{
		pSprite->SetAnimation("Jump");

		pBody->ApplyForce(b2Vec2(0,300), pBody->GetWorldCenter());
	}

	if (k == Seed::KeyLeft || k == Seed::KeyA)
	{
		vel.x = -5;
		pBody->SetLinearVelocity(vel);

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);

		// Play the animation
		pSprite->SetAnimation("Run");
	}

	if (k == Seed::KeyRight || k == Seed::KeyD)
	{
		vel.x = 5;
		pBody->SetLinearVelocity(vel);

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() < 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);

		// Play the animation
		pSprite->SetAnimation("Run");
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
		vPlayerVectorDirection -= VECTOR_UP;
	}

	if (k == Seed::KeyLeft|| k == Seed::KeyA)
	{
		pSprite->SetAnimation("Idle");
		pBody->SetLinearVelocity(vel);
	}

	if (k == Seed::KeyRight|| k == Seed::KeyD)
	{
		pSprite->SetAnimation("Idle");
		pBody->SetLinearVelocity(vel);
	}

	if (k == Seed::KeyDown|| k == Seed::KeyS)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}
}
