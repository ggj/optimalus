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

	if (k == Seed::KeyUp || k == Seed::KeyW)
	{
		// Sum the normalized vector up with the current vector
		pBody->ApplyForce( b2Vec2(0,50), pBody->GetWorldCenter());
	}

	if (k == Seed::KeyLeft || k == Seed::KeyA)
	{
		// Sum the normalized vector left with the current vector
		pBody->ApplyForce( b2Vec2(-25,0), pBody->GetWorldCenter());

		// Change the scale to turn the player sprite
		if (pSprite->GetScaleX() > 0)
			pSprite->SetScaleX(pSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight || k == Seed::KeyD)
	{
		// Sum the normalized vector right with the current vector
		pBody->ApplyForce( b2Vec2(25,0), pBody->GetWorldCenter());

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

	// Remove the directions
	if (k == Seed::KeyUp)
	{
		vPlayerVectorDirection -= VECTOR_UP;
	}

	if (k == Seed::KeyLeft)
	{
		vPlayerVectorDirection -= VECTOR_LEFT;
	}

	if (k == Seed::KeyRight)
	{
		vPlayerVectorDirection -= VECTOR_RIGHT;
	}

	if (k == Seed::KeyDown)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}
}
